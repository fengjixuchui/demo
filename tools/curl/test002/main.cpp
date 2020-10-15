#include "main.h"
#include "resource.h"


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//�����������Ի���Ĵ��ھ������Ϊ����Ҫ��������ڷ���Ϣ������֪���䴰�ھ��
HWND	g_hDlgWnd = NULL;

#define TT_HANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
      HANDLE_##message((hWnd), (wParam), (lParam), (fn))))


enum {
	HANDLE_WM_COMMAND
};
/*
WM_COMMAND ��Ϣ��Դ	WPARAM ��λ	WPARAM ��λ	LPARAM
�˵� 				0 		�˵� ID 	0
��ݼ� 				1 		��ݼ���Ӧ�˵� ID 	0
�ؼ� 				��Ӧ Code����BN_CLICKED�� 	�ؼ� ID 	�ؼ����
*/
static void OnCommand(HWND hwnd,UINT uNotifyCode, int nID, HWND wndCtl){
	if (uNotifyCode == 0){ 
		//menu
		

	}
	if (uNotifyCode == 1){
		//accelerator
	}
	if (uNotifyCode == BN_CLICKED){
		switch (nID){
		case ID_BTN_DOWNLOAD:{
			HANDLE hThread = CreateThread(NULL, 0, showDownloadDlg, 0, 0, NULL);
			CloseHandle(hThread);
			break;
		}
		case ID_BTN_UPLOAD:{

			break;
		}
		}
	}
}

// �Ի�����̺���
static INT_PTR WINAPI MainDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		OnCommand(hwnd,(UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);
		break;
	}

	case WM_INITDIALOG:
	{
		CenterWindow(hwnd);
		break;
	}
	
	case WM_SHOWWINDOW:
	{
		break;
	}
	case WM_ERASEBKGND:
		return TRUE;
	case WM_CTLCOLORSTATIC:
		return (INT_PTR)(HBRUSH)::GetStockObject(WHITE_BRUSH);
	
	case WM_NCACTIVATE:
	{
		break;
	}
	
	case WM_NCPAINT:
	{
		break;
	}
	
	case WM_CTLCOLORBTN:
	{

		break;
	}
	
	case WM_PAINT:
	{


		break;
	}
	
	case WM_CREATE:
	{
		break;
	}
	
	
	
	case WM_DESTROY:
	{
		PostQuitMessage(NULL);
		break;
	}
	
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	//�����Ի���֪���Ի���رղŻ�ִ���˳�
	//DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, MainDlg_Proc, 0);
	DialogBox(NULL, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, MainDlg_Proc);
	return 0;

}







void CenterWindow(HWND hwnd){
	int scrWidth, scrHeight;
	RECT rect;
	//�����Ļ�ߴ�
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//ȡ�ô��ڳߴ�
	GetWindowRect(hwnd, &rect);
	//��������rect���ֵ
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//�ƶ����ڵ�ָ����λ��
	SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

}

