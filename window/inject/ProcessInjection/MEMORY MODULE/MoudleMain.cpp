#include <iostream>
#include "Loader.h"
using namespace std;
/*
�ڷ���ʽ����ע���У������Ѿ�ʵ����һ�ζ�DLL�Ľ����������ڴ��д��ڴ�Ƭ��RWX����
��GitHub�ϵ���ΪMemory Mouldle��Ŀ�У�������һ�ֿ��Խ��ڴ���������Ϊ����������
��Ϊ���Ƶķ����������Ŀ���Ƕ����ַ�����ѧϰ��

*/
int main()
{
	do
	{
		//�Ѿ��������dll����ʵ��LoadLibraryA�����׶Σ�����������DLL
		char *dllFile = "D:\\users\\sudo\\Documents\\GitHub\\ProcessInjection\\x64\\Debug\\MemroyInjectionDLL.dll";
		HANDLE hFile = CreateFileA(dllFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			cout << "File Path is Wrong!" << endl;
			break;
		}
		else
		{
			cout << "Get File Success." << endl;
		}
		DWORD dwLength = GetFileSize(hFile, NULL);
		if (dwLength == INVALID_FILE_SIZE || dwLength == 0)
		{
			cout << "Failed to get the Dll file size." << endl;
			break;
		}
		else
		{
			cout << "File size is :" << dwLength << endl;
		}
		//dll���ڴ��еĵ�ַ��δ���أ�
		LPVOID hBaseAddress = VirtualAlloc(NULL, dwLength, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!hBaseAddress)
		{
			cout << "Failed to Alloc Memory." << endl;
			break;
		}
		else
		{
			cout << "BaseAddress is :" << hBaseAddress << endl;
		}
		DWORD dwBytesRead;
		if (ReadFile(hFile, hBaseAddress, dwLength, &dwBytesRead, NULL) == false)
			cout << "Failed to Read File!" << endl;
		Loader((ULONG_PTR)hBaseAddress);

	} while (0);
	system("parse");
	return 0;
}