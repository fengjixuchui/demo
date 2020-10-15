#include "Loader.h"
HINSTANCE hAppInstance = NULL;

// Protection flags for memory pages (Executable, Readable, Writeable)
static int ProtectionFlags[2][2][2] = {
	{
		// not executable
		{ PAGE_NOACCESS, PAGE_WRITECOPY },
		{ PAGE_READONLY, PAGE_READWRITE },
	},{
		// executable
		{ PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY },
		{ PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE },
	},
};
//���ϲ�1�ֽ�һҳ�󣬼�Ϊ����һ��ҳ����ʼ��ַ
DWORD AlignValueUp(DWORD value, DWORD alignment)
{
	return (value + alignment - 1) & ~(alignment - 1);
}
//0x1000-1 = 0xfff ȡ���󣬺���λΪ0���������Ľ��Ϊ����λ��Ϊ0��ֵ����Ϊ����ȡ��
uintptr_t
AlignValueDown(uintptr_t value, uintptr_t alignment) {
	return value & ~(alignment - 1);
}

LPVOID
AlignAddressDown(LPVOID address, uintptr_t alignment) {
	return (LPVOID)AlignValueDown((uintptr_t)address, alignment);
}


SIZE_T
GetRealSectionSize(PMemoryModule module, PIMAGE_SECTION_HEADER section,PIMAGE_NT_HEADERS header) {
	DWORD size = section->SizeOfRawData;
	if (size == 0) {
		//�жϽ�����
		if (section->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) {
			size = header->OptionalHeader.SizeOfInitializedData;
		}
		else if (section->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) {
			size = header->OptionalHeader.SizeOfUninitializedData;//���к�δ��ʼ�����ݵĽڵĴ�С
		}
	}
	return (SIZE_T)size;
}

BOOL CopySections(PMemoryModule mModule)
{
	int i, section_size;
	ULONG_PTR dest;
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(mModule->header); 
	ULONG_PTR ValueA;
	ULONG_PTR ValueB;
	ULONG_PTR ValueC;
	for (i = 0; i < mModule->header->FileHeader.NumberOfSections; i++, section++)
	{
		//��DLL�У���ǰ�ڲ��������ݣ����ǿ��ܶ���δ��ʼ��������
		if (section->SizeOfRawData == 0)
		{
			//�ڴ��нڵĶ�������
			section_size = mModule->old_header->OptionalHeader.SectionAlignment;
			if (section_size > 0)
			{
				dest = mModule->mVirutalAlloc(mModule->baseAddress+ section->VirtualAddress, section_size, MEM_COMMIT, PAGE_READWRITE, NULL);//mModule->flProtect);
				if (dest == NULL)
					return FALSE;
				//ʼ�ձ���ҳ���룬���Ϸ�����ڴ棬����Ϊһҳ
				dest = mModule->baseAddress + section->VirtualAddress;
				//64λģʽ�£�����ضϳ�32λģʽ
				section->Misc.PhysicalAddress = (DWORD)((uintptr_t)dest & 0xffffffff);
			}
			//section Ϊ��
			continue;
		}

		//���к�������
		dest = mModule->mVirutalAlloc(mModule->baseAddress + section->VirtualAddress, section->SizeOfRawData, MEM_COMMIT, PAGE_READWRITE, NULL);
		//
		ValueA = section->SizeOfRawData;//�ڵĴ�С
		ValueB = mModule->bufferAddress + section->PointerToRawData;//���ݵ���ʼ��ַ
		ValueC = dest;//���ݽ������������ĵ�ַ
								 //����ͷ�ͽڱ�����ݵ��¿��ٵĻ�����
		while (ValueA--)
			*(BYTE *)ValueC++ = *(BYTE *)ValueB++;

		if (dest == NULL)
			return FALSE;
		dest = mModule->baseAddress + section->VirtualAddress;
		
		section->Misc.PhysicalAddress = (DWORD)((uintptr_t)dest & 0xffffffff);
	}//end for
	return TRUE;
}
ULONG_PTR WINAPI Loader(ULONG_PTR callAddress)
{
	LOADLIBRARYA pLoadLibraryA = NULL;
	GETPROCADDRESS pGetProcAddress = NULL;
	VIRTUALALLOC pVirtualAlloc = NULL;
	NTFLUSHINSTRUCTIONCACHE pNtFlushInstructionCache = NULL;
	VIRTUALPROTECT pVirtualProtect = NULL;
	VIRTUALFREE pVirtualFree = NULL;
	USHORT usCounter;
	ULONG_PTR uiLibraryAddress;
	ULONG_PTR uiBaseAddress;
	// variables for processing the kernels export table
	ULONG_PTR uiAddressArray;
	ULONG_PTR uiNameArray;
	ULONG_PTR uiExportDir;
	ULONG_PTR uiNameOrdinals;
	DWORD dwHashValue;

	// variables for loading this image
	ULONG_PTR uiHeaderValue;
	ULONG_PTR uiValueA;
	ULONG_PTR uiValueB;
	ULONG_PTR uiValueC;
	ULONG_PTR uiValueD;
	ULONG_PTR uiValueE;

	//�ڴ�ӳ��׶��õ�
	PIMAGE_SECTION_HEADER section;
	DWORD optionalSectionSize;
	DWORD lastSectionEnd = 0;
	SYSTEM_INFO sysInfo;//��ȡҳ���С��
	DWORD alignedImageSize;
	ULONG_PTR code;//address
	ULONG_PTR header;

	uiLibraryAddress = callAddress + 10;
	//ͨ�������ڴ��ַ���ҵ��ļ����ڴ��е���ʼ��ַ
	while (TRUE)
	{
		if (((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_magic == IMAGE_DOS_SIGNATURE)
		{
			//peͷƫ��RVA
			uiHeaderValue = ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;
			// some x64 dll's can trigger a bogus signature (IMAGE_DOS_SIGNATURE == 'POP r10'),
			// we sanity check the e_lfanew with an upper threshold value of 1024 to avoid problems.
			//�ж�PEͷ����ȷ��
			if (uiHeaderValue >= sizeof(IMAGE_DOS_HEADER) && uiHeaderValue < 1024)
			{
				//peͷ���ڴ��е�λ��
				uiHeaderValue += uiLibraryAddress;
				// break if we have found a valid MZ/PE header
				//����ҵ��ļ�ͷ���˳�ѭ��
				if (((PIMAGE_NT_HEADERS)uiHeaderValue)->Signature == IMAGE_NT_SIGNATURE)
					break;
			}
		}
		uiLibraryAddress--;
	}
	//��������loader��Ҫ���ں˵�����
	// STEP 1: process the kernels exports for the functions our loader needs...
	//���PEB,64λ�£�GSָ����ڴ��0x60ƫ�ƴ�����peb�ṹ��λ�ã�32λ��λFS:[0x30]
	// get the Process Enviroment Block
	uiBaseAddress = __readgsqword(0x60);//uiBaseAddress -> peb
										//��ȡ���̼���ģ��
										// get the processes loaded modules. ref: http://msdn.microsoft.com/en-us/library/aa813708(VS.85).aspx
	uiBaseAddress = (ULONG_PTR)((_PPEB)uiBaseAddress)->pLdr;//uiBaseAddress->pLdr
															//��ȡ��һ�����ڴ�˳��ģ���б����
															// get the first entry of the InMemoryOrder module list
	uiValueA = (ULONG_PTR)((PPEB_LDR_DATA)uiBaseAddress)->InMemoryOrderModuleList.Flink;//uiBaseAddress->PLDR_DATA_TABLE_ENTRY
	while (uiValueA)
	{
		// get pointer to current modules name (unicode string)
		//��ǰģ������ַ
		uiValueB = (ULONG_PTR)((PLDR_DATA_TABLE_ENTRY)uiValueA)->BaseDllName.pBuffer;
		// set bCounter to the length for the loop
		usCounter = ((PLDR_DATA_TABLE_ENTRY)uiValueA)->BaseDllName.Length;
		// clear uiValueC which will store the hash of the module name
		uiValueC = 0;
		//����ģ������hash
		// compute the hash of the module name...
		do
		{
			uiValueC = ror((DWORD)uiValueC);
			// normalize to uppercase if the madule name is in lowercase
			if (*((BYTE *)uiValueB) >= 'a')
				uiValueC += *((BYTE *)uiValueB) - 0x20;
			else
				uiValueC += *((BYTE *)uiValueB);
			uiValueB++;
		} while (--usCounter);
		//��ȡkerlnel32�еĺ�����ַ
		// compare the hash with that of kernel32.dll
		if ((DWORD)uiValueC == KERNEL32DLL_HASH)
		{
			// get this modules base address
			uiBaseAddress = (ULONG_PTR)((PLDR_DATA_TABLE_ENTRY)uiValueA)->DllBase;

			// get the VA of the modules NT Header
			uiExportDir = uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew;

			// uiNameArray = the address of the modules export directory entry
			uiNameArray = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

			// get the VA of the export directory
			uiExportDir = (uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress);

			// get the VA for the array of name pointers
			uiNameArray = (uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNames);

			// get the VA for the array of name ordinals
			uiNameOrdinals = (uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNameOrdinals);

			usCounter = 5;

			// loop while we still have imports to find
			while (usCounter > 0)
			{
				// compute the hash values for this function name
				dwHashValue = hash((char *)(uiBaseAddress + DEREF_32(uiNameArray)));

				// if we have found a function we want we get its virtual address
				if (dwHashValue == LOADLIBRARYA_HASH || dwHashValue == GETPROCADDRESS_HASH || dwHashValue == VIRTUALALLOC_HASH || dwHashValue == VIRTUALFREE_HASH || dwHashValue == VIRTUALPROCT_HASH)
				{
					// get the VA for the array of addresses
					uiAddressArray = (uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfFunctions);

					// use this functions name ordinal as an index into the array of name pointers
					uiAddressArray += (DEREF_16(uiNameOrdinals) * sizeof(DWORD));
					//��ȡ������ַ
					// store this functions VA
					if (dwHashValue == LOADLIBRARYA_HASH)
						pLoadLibraryA = (LOADLIBRARYA)(uiBaseAddress + DEREF_32(uiAddressArray));
					else if (dwHashValue == GETPROCADDRESS_HASH)
						pGetProcAddress = (GETPROCADDRESS)(uiBaseAddress + DEREF_32(uiAddressArray));
					else if (dwHashValue == VIRTUALALLOC_HASH)
						pVirtualAlloc = (VIRTUALALLOC)(uiBaseAddress + DEREF_32(uiAddressArray));
					else if (dwHashValue == VIRTUALPROCT_HASH)
						pVirtualProtect = (VIRTUALPROTECT)(uiBaseAddress + DEREF_32(uiAddressArray));
					else if (dwHashValue == VIRTUALFREE_HASH)
						pVirtualFree = (VIRTUALFREE)(uiBaseAddress + DEREF_32(uiAddressArray));
					// decrement our counter
					usCounter--;
				}

				// get the next exported function name
				uiNameArray += sizeof(DWORD);

				// get the next exported function name ordinal
				uiNameOrdinals += sizeof(WORD);
			}
		}
		else if ((DWORD)uiValueC == NTDLLDLL_HASH)
		{
			//��֮ǰ����PE�ṹ��ͬ
			// get this modules base address
			uiBaseAddress = (ULONG_PTR)((PLDR_DATA_TABLE_ENTRY)uiValueA)->DllBase;

			// get the VA of the modules NT Header
			uiExportDir = uiBaseAddress + ((PIMAGE_DOS_HEADER)uiBaseAddress)->e_lfanew;

			// uiNameArray = the address of the modules export directory entry
			uiNameArray = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

			// get the VA of the export directory
			uiExportDir = (uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress);

			// get the VA for the array of name pointers
			uiNameArray = (uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNames);

			// get the VA for the array of name ordinals
			uiNameOrdinals = (uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfNameOrdinals);

			usCounter = 1;

			// loop while we still have imports to find
			while (usCounter > 0)
			{
				// compute the hash values for this function name
				dwHashValue = hash((char *)(uiBaseAddress + DEREF_32(uiNameArray)));

				// if we have found a function we want we get its virtual address
				if (dwHashValue == NTFLUSHINSTRUCTIONCACHE_HASH)
				{
					// get the VA for the array of addresses
					uiAddressArray = (uiBaseAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfFunctions);

					// use this functions name ordinal as an index into the array of name pointers
					uiAddressArray += (DEREF_16(uiNameOrdinals) * sizeof(DWORD));

					// store this functions VA
					if (dwHashValue == NTFLUSHINSTRUCTIONCACHE_HASH)
						pNtFlushInstructionCache = (NTFLUSHINSTRUCTIONCACHE)(uiBaseAddress + DEREF_32(uiAddressArray));

					// decrement our counter
					usCounter--;
				}

				// get the next exported function name
				uiNameArray += sizeof(DWORD);

				// get the next exported function name ordinal
				uiNameOrdinals += sizeof(WORD);
			}
		}

		// we stop searching when we have found everything we need.
		if (pLoadLibraryA && pGetProcAddress && pVirtualAlloc && pNtFlushInstructionCache)
			break;

		// get the next entry
		uiValueA = DEREF(uiValueA);
	}//end while
	 //NTͷ�������ַ��ԭDLL�ļ���
	uiHeaderValue = uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;
	//����ַ
	//uiBaseAddress = (ULONG_PTR)pVirtualAlloc(NULL, ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//���ӵ�һЩ�ж�
	if (((PIMAGE_NT_HEADERS)uiHeaderValue)->Signature != IMAGE_NT_SIGNATURE)
	{
		return NULL;
	}
	/*
		���������һЩУ�飬�ü��ر�ø�׼ȷ���������
	*/
	//�������ȣ�Ӧ����2�ı���
	if (((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SectionAlignment & 1)
	{
		return NULL;
	}
	//��һ���ڵĵ�ַ
	section = IMAGE_FIRST_SECTION((PIMAGE_NT_HEADERS)uiHeaderValue);
	//�ڴ��еĽڵĶ�������
	optionalSectionSize = ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SectionAlignment;
	//��������ĵ�ַ
	for (int i = 0; i < (((PIMAGE_NT_HEADERS)uiHeaderValue)->FileHeader.NumberOfSections); i++, section++)
	{
		DWORD endOfSection;
		if (section->SizeOfRawData == 0)
		{
			//�������û�����ݣ���Ĭ�ϰ������ȷ���һ��
			endOfSection = section->VirtualAddress + optionalSectionSize;
		}
		else
		{
			//�����ݣ���������������ݳ���
			endOfSection = section->VirtualAddress + (section->SizeOfRawData);
		}

		if (endOfSection > lastSectionEnd)
		{
			lastSectionEnd = endOfSection;
		}
	}
	//sysInfo->dwPageSize=0x1000 ------4K
	GetNativeSystemInfo(&sysInfo);
	//Git�ϵ�MemoryModule�����еĶ��뺯����Ϊ����ģʽ�������ڵ��ԣ��������޸ĳ������ĺ�������
	//param1:�ڴ��е�����PEӳ���С	param2��ҳ���С
	//���㾵�����Ĵ�С
	alignedImageSize = AlignValueUp(((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SizeOfImage, sysInfo.dwPageSize);
	//���нڼ����������ĵ�ַ�����һ����ʹ��SizeOfImage����Ĵ�С����ͬ��
	if (alignedImageSize != AlignValueUp(lastSectionEnd, sysInfo.dwPageSize))
	{
		return NULL;
	}
	//�Ȱ��վ�����Ļ�ַ���пռ���䱣���ĵ��ڴ档(��Git�ϵ���Ŀ�У���MEM_RESERVE|MEM_COMMIT)
	code = pVirtualAlloc((LPVOID)(((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.ImageBase),
		alignedImageSize,
		MEM_RESERVE ,//| MEM_COMMIT,
		PAGE_READWRITE,
		NULL);
	//�������ռ�õ�λ�ñ�ռ�ã���ѡ������λ�á�
	if (code == NULL)
	{
		code = pVirtualAlloc(NULL,
			alignedImageSize,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE,
			NULL);
		if (code == NULL)
		{
			return;
		}
	}
	uiBaseAddress = code;
	//�ύ�ڴ�
	uiHeaderValue = uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;
	header = pVirtualAlloc(code, ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SizeOfHeaders,
		MEM_COMMIT,
		PAGE_READWRITE,
		NULL);
	uiValueA = ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.SizeOfHeaders;//����ͷ+�ڱ�Ĵ�С
	uiValueB = uiLibraryAddress;//DLL����ʼ��ַ��������������ʼ��ַ
	uiValueC = code;//dll�������صĵ�ַ����ʼ��ַ
	//����ͷ�ͽڱ�����ݵ��¿��ٵĻ�����
	while (uiValueA--)
		*(BYTE *)uiValueC++ = *(BYTE *)uiValueB++;
	PMemoryModule module = (PMemoryModule)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MemoryModule));
	module->mVirutalAlloc = pVirtualAlloc;
	module->mVirtualProtect = pVirtualProtect;
	module->mVirtualFree = pVirtualFree;
	module->bufferAddress = uiLibraryAddress;
	module->baseAddress = uiBaseAddress;
	module->header = code + ((PIMAGE_DOS_HEADER)header)->e_lfanew;
	module->old_header = uiHeaderValue;
	module->sysInfo = sysInfo;


	//������ļ��ص�ַд��PEͷ�������Ƿ��ڽ���ĵ�ַ����
	module->header->OptionalHeader.ImageBase = code;


	//CopySections(PIMAGE_NT_HEADERS srcHeader, PIMAGE_NT_HEADERS targetHeader,ULONG_PTR srcAddress,ULONG_PTR targetAddress,PMemoryModule mModule)
	if (!CopySections(module))
	{
		return NULL;
	}

	// uiValueB = the address of the import directory
	uiValueB = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	// we assume their is an import table to process
	// uiValueC is the first entry in the import table
	//����ַ+RVA��������������ĵ�ַVA
	uiValueC = (uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiValueB)->VirtualAddress);

	// itterate through all imports
	//���ӿ�����
	while (((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->Name)
	{
		// use LoadLibraryA to load the imported module into memory
		//ʹ��LoadLibraryA����Ҫ��ģ����ص��ڴ�
		uiLibraryAddress = (ULONG_PTR)pLoadLibraryA((LPCSTR)(uiBaseAddress + ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->Name));

		// uiValueD = VA of the OriginalFirstThunk
		//ָ��INT��IMAGE_THUNK_DATA��VA
		uiValueD = (uiBaseAddress + ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->OriginalFirstThunk);

		// uiValueA = VA of the IAT (via first thunk not origionalfirstthunk)
		//Ҫ����IAT��IMAGE_THUNK_DATA�ṹ��
		uiValueA = (uiBaseAddress + ((PIMAGE_IMPORT_DESCRIPTOR)uiValueC)->FirstThunk);

		// itterate through all imported functions, importing by ordinal if no name present
		while (DEREF(uiValueA))
		{
			// sanity check uiValueD as some compilers only import by FirstThunk
			if (uiValueD && ((PIMAGE_THUNK_DATA)uiValueD)->u1.Ordinal & IMAGE_ORDINAL_FLAG)
			{
				// get the VA of the modules NT Header
				uiExportDir = uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew;

				// uiNameArray = the address of the modules export directory entry
				uiNameArray = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiExportDir)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

				// get the VA of the export directory
				uiExportDir = (uiLibraryAddress + ((PIMAGE_DATA_DIRECTORY)uiNameArray)->VirtualAddress);

				// get the VA for the array of addresses
				uiAddressArray = (uiLibraryAddress + ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->AddressOfFunctions);

				// use the import ordinal (- export ordinal base) as an index into the array of addresses
				uiAddressArray += ((IMAGE_ORDINAL(((PIMAGE_THUNK_DATA)uiValueD)->u1.Ordinal) - ((PIMAGE_EXPORT_DIRECTORY)uiExportDir)->Base) * sizeof(DWORD));

				// patch in the address for this imported function
				DEREF(uiValueA) = (uiLibraryAddress + DEREF_32(uiAddressArray));
			}
			else
			{
				// get the VA of this functions import by name struct
				uiValueB = (uiBaseAddress + DEREF(uiValueA));

				// use GetProcAddress and patch in the address for this imported function
				DEREF(uiValueA) = (ULONG_PTR)pGetProcAddress((HMODULE)uiLibraryAddress, (LPCSTR)((PIMAGE_IMPORT_BY_NAME)uiValueB)->Name);
			}
			// get the next imported function
			uiValueA += sizeof(ULONG_PTR);
			if (uiValueD)//INT
				uiValueD += sizeof(ULONG_PTR);
		}

		// get the next import
		uiValueC += sizeof(IMAGE_IMPORT_DESCRIPTOR);
	}
	//�������װ�ص�ַ��ʵ��װ�ص�ַ�Ĳ�
	uiLibraryAddress = uiBaseAddress - ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.ImageBase;
	//�ض����ĵ�ַ
	uiValueB = (ULONG_PTR)&((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

	if (((PIMAGE_DATA_DIRECTORY)uiValueB)->Size)//�ض�λ���С
	{
		// uiValueC is now the first entry (IMAGE_BASE_RELOCATION)
		//�ض�λ��ĵ�ַ
		uiValueC = (uiBaseAddress + ((PIMAGE_DATA_DIRECTORY)uiValueB)->VirtualAddress);

		// and we itterate through all entries...
		while (((PIMAGE_BASE_RELOCATION)uiValueC)->SizeOfBlock)//�ض�λ��Ĵ�С
		{
			// uiValueA = the VA for this relocation block
			uiValueA = (uiBaseAddress + ((PIMAGE_BASE_RELOCATION)uiValueC)->VirtualAddress);//�ض�λ�ڴ�ҳ����ʼRVA
																							//�ض�λ���е�����(������Ĵ�С��ȥ�ṹ��Ĵ�С���õ��ض�λ����ܴ�С������ÿ���ض�λ��Ĵ�С)
																							// uiValueB = number of entries in this relocation block
			uiValueB = (((PIMAGE_BASE_RELOCATION)uiValueC)->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_RELOC);
			//�ض�λ��ĵ�һ��
			// uiValueD is now the first entry in the current relocation block
			uiValueD = uiValueC + sizeof(IMAGE_BASE_RELOCATION);
			//�����ض�λ��
			// we itterate through all the entries in the current block...
			while (uiValueB--)
			{
				// perform the relocation, skipping IMAGE_REL_BASED_ABSOLUTE as required.
				// we dont use a switch statement to avoid the compiler building a jump table
				// which would not be very position independent!
				//�ض�λ��ĸ���λ������ض�λ�������
				if (((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_DIR64)
					*(ULONG_PTR *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += uiLibraryAddress;
				else if (((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_HIGHLOW)
					*(DWORD *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += (DWORD)uiLibraryAddress;

				else if (((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_HIGH)
					*(WORD *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += HIWORD(uiLibraryAddress);
				else if (((PIMAGE_RELOC)uiValueD)->type == IMAGE_REL_BASED_LOW)
					*(WORD *)(uiValueA + ((PIMAGE_RELOC)uiValueD)->offset) += LOWORD(uiLibraryAddress);
				//��һ���ض�λ��
				// get the next entry in the current relocation block
				uiValueD += sizeof(IMAGE_RELOC);
			}
			//��һ���ض�λ��
			// get the next entry in the relocation directory
			uiValueC = uiValueC + ((PIMAGE_BASE_RELOCATION)uiValueC)->SizeOfBlock;
		}
	}
	////�ͷű�Ϊ�����������ڴ棬���ս�ͷ�޸��ڴ�����
	if (!FinalizeSections(module))
	{
		return NULL;
	}

	// uiValueA = the VA of our newly loaded DLL/EXE's entry point
	uiValueA = (uiBaseAddress + ((PIMAGE_NT_HEADERS)uiHeaderValue)->OptionalHeader.AddressOfEntryPoint);

	// We must flush the instruction cache to avoid stale code being used which was updated by our relocation processing.
	pNtFlushInstructionCache((HANDLE)-1, NULL, 0);
	BOOL RES = ((DLLMAIN)uiValueA)((HINSTANCE)uiBaseAddress, DLL_PROCESS_ATTACH, NULL);
	if (RES);
	// STEP 8: return our new entry point address so whatever called us can call DllMain() if needed.
	//���ѭ�������ã���֤���̲��˳������ﲻ�����õ���������ͣ���������������ʱ��-_-!��
	while (1);
}

BOOL FinalizeSections(PMemoryModule mMemory)
{
	int i;
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(mMemory->header);
#ifdef _WIN64
	// "PhysicalAddress" might have been truncated to 32bit above, expand to
	// 64bits again.
	//imageOffset��ֵ������PhysicalAddress��ֵ���ضϳ�32λ����Ҫ�ָ�Ϊ64λ�������ֵ���Ǿ�����ص��ڴ��ǰ8λ�����ضϵ���8λ��
	uintptr_t imageOffset = ((uintptr_t)mMemory->header->OptionalHeader.ImageBase & 0xffffffff00000000);
#else
	static const uintptr_t imageOffset = 0;
#endif
	SECTIONFINALIZEDATA sectionData;
	//�ָ�Ϊ64λ
	sectionData.address = (LPVOID)((uintptr_t)section->Misc.PhysicalAddress | imageOffset);
	//���׵�ַ
	sectionData.alignedAddress = AlignAddressDown(sectionData.address, mMemory->sysInfo.dwPageSize);
	//�ڴ�С
	sectionData.size = GetRealSectionSize(mMemory, section, mMemory->header);
	//����
	sectionData.characteristics = section->Characteristics;
	sectionData.last = FALSE;
	section++;

	for (i = 1; i<mMemory->header->FileHeader.NumberOfSections; i++, section++) {
		LPVOID sectionAddress = (LPVOID)((uintptr_t)section->Misc.PhysicalAddress | imageOffset);
		LPVOID alignedAddress = AlignAddressDown(sectionAddress, mMemory->sysInfo.dwPageSize);
		SIZE_T sectionSize = GetRealSectionSize(mMemory, section, mMemory->header);
		// Combine access flags of all sections that share a page
		// TODO(fancycode): We currently share flags of a trailing large section
		//   with the page of a first small section. This should be optimized.
		//��ǰ��һ����Ե�һ���ڵ�Ȩ��Ϊ׼����Ҫ�Ż���
		//ȷ���ڵ�ǰҳ��
		if (sectionData.alignedAddress == alignedAddress || (uintptr_t)sectionData.address + sectionData.size >(uintptr_t) alignedAddress) {
			// Section shares page with previous
			//�����Ҿ�����һ���жϾͿ���-_-!
			if ((section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE) == 0 || (sectionData.characteristics & IMAGE_SCN_MEM_DISCARDABLE) == 0) {
				sectionData.characteristics = (sectionData.characteristics | section->Characteristics) & ~IMAGE_SCN_MEM_DISCARDABLE;
			}
			else {
				sectionData.characteristics |= section->Characteristics;
			}
			sectionData.size = (((uintptr_t)sectionAddress) + ((uintptr_t)sectionSize)) - (uintptr_t)sectionData.address;
			continue;
		}

		if (!FinalizeSection(mMemory, &sectionData)) {
			return FALSE;
		}
		sectionData.address = sectionAddress;
		sectionData.alignedAddress = alignedAddress;
		sectionData.size = sectionSize;
		sectionData.characteristics = section->Characteristics;
	}
	sectionData.last = TRUE;
	if (!FinalizeSection(mMemory, &sectionData)) {
		return FALSE;
	}
	return TRUE;
}



//�Խ����Ե��޸�
static BOOL
FinalizeSection(PMemoryModule module, PSECTIONFINALIZEDATA sectionData) {
	DWORD protect, oldProtect;
	BOOL executable;
	BOOL readable;
	BOOL writeable;

	if (sectionData->size == 0) {
		return TRUE;
	}
	//IMAGE_SCN_MEM_DISCARDABLE:���Ը�����Ҫ���������ٱ�ʹ�ã����԰�ȫ�ͷŵ�
	if (sectionData->characteristics & IMAGE_SCN_MEM_DISCARDABLE) {
		//ȷ���ͷŵ�ǰҳû������
		if (sectionData->address == sectionData->alignedAddress &&
			(sectionData->last ||
				module->header->OptionalHeader.SectionAlignment == module->sysInfo.dwPageSize ||
				(sectionData->size % module->sysInfo.dwPageSize) == 0)
			) {
			//�ͷ�
			module->mVirtualFree(sectionData->address, sectionData->size, MEM_DECOMMIT, NULL);
		
		}
		return TRUE;
	}

	// determine protection flags based on characteristics
	executable = (sectionData->characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
	readable = (sectionData->characteristics & IMAGE_SCN_MEM_READ) != 0;
	writeable = (sectionData->characteristics & IMAGE_SCN_MEM_WRITE) != 0;
	protect = ProtectionFlags[executable][readable][writeable];
	if (sectionData->characteristics & IMAGE_SCN_MEM_NOT_CACHED) {
		protect |= PAGE_NOCACHE;
	}

	//�޸����η���Ȩ��
	if (module->mVirtualProtect(sectionData->address, sectionData->size, protect, &oldProtect) == 0) {
		return FALSE;
	}

	return TRUE;
}