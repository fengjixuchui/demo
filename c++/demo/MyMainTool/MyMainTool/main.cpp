#include "main.h"
#include "tab_mac.h"
#include "tab_other.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define TAB_NUM  2
HWND hDlg_tab[TAB_NUM];	

LPSTR tabname[TAB_NUM] = { "�޸�mac", "other" }; //����һ����ά���� ���tab��ǩ����

DLGPROC dlg_tab_proc[TAB_NUM] = { tabMAC_Proc, tabOther_Proc };

DWORD dlg_id[TAB_NUM] = { IDD_DLG_TAB_MAC, IDD_DLG_TAB_OTHER };

HWND htabctrl;	

HINSTANCE g_hInstance;

static void initTabCtrl(HWND hwnd){
	htabctrl = GetDlgItem(hwnd, IDC_TAB_MAIN);
	RECT rectClient;
	GetClientRect(hwnd, &rectClient);
	MoveWindow(htabctrl, rectClient.left, rectClient.top, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, TRUE);

	TCITEM tie;//����tab��ǩ������

	//���忪ʼ���� tie���ֶ� Mask psztext,ccxtextmax,image,lparam
	tie.mask = TCIF_TEXT | TCIF_IMAGE;//psztext�ֶ���Ч
	tie.iImage = -1;
	for (INT i = 0; i < 2; i++)
	{
		tie.pszText = tabname[i];
		TabCtrl_InsertItem(htabctrl, i, &tie);
	}

	RECT rect;
	GetClientRect(htabctrl, &rect);

	for (int i = 0; i < TAB_NUM; i++){
		hDlg_tab[i] = CreateDialogA(GetModuleHandle(0), MAKEINTRESOURCE(dlg_id[i]), htabctrl, dlg_tab_proc[i]);
		MoveWindow(hDlg_tab[i], rect.left, rect.top + 20, rect.right - rect.left, rect.bottom - rect.top, FALSE);
	}


	ShowWindow(hDlg_tab[0], TRUE);

}

static void initMenu(HINSTANCE hInstance,HWND hwnd, int nCmdShow){
	HMENU hMenu = LoadMenu(0, MAKEINTRESOURCE(IDR_MENU_MAIN));//���ز˵���Դ�����ز˵����

	SetMenu(hwnd, hMenu);
	
}

/*
WM_COMMAND ��Ϣ��Դ	WPARAM ��λ	WPARAM ��λ	LPARAM
�˵� 				0 		�˵� ID 	0
��ݼ� 				1 		��ݼ���Ӧ�˵� ID 	0
�ؼ� 				��Ӧ Code����BN_CLICKED�� 	�ؼ� ID 	�ؼ����
*/
static void OnCommand(HWND hwnd, UINT uNotifyCode, int nID, HWND wndCtl){
	if (uNotifyCode == 0){
		//menu


	}
	if (uNotifyCode == 1){
		//accelerator
	}
	if (uNotifyCode == BN_CLICKED){
		switch (nID){
		case 0:{
			//HANDLE hThread = CreateThread(NULL, 0, showDownloadDlg, 0, 0, NULL);
			//CloseHandle(hThread);
			break;
		}
		case 1:{

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
		OnCommand(hwnd, (UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);
		break;
	}

	case WM_INITDIALOG:
	{
		CenterWindow(hwnd);
		//initMenu(g_hInstance, hwnd,SW_SHOW);
		initTabCtrl(hwnd);


		
		break;
	}
	case WM_NOTIFY:{
		if ((INT)wParam == IDC_TAB_MAIN){
			LPNMHDR pnmr = (LPNMHDR)lParam;
			if ( pnmr->code == TCN_SELCHANGE){
				int sel = TabCtrl_GetCurSel(htabctrl);
				for (int i = 0; i < TAB_NUM; i++){
					if (i == sel){
						ShowWindow(hDlg_tab[i], TRUE);
					}
					else{
						ShowWindow(hDlg_tab[i], FALSE);
					}
				}
			
			}
		}

		break;
	}
	case WM_SIZE:{
		RECT rect;

		GetClientRect(hwnd, &rect);
		MoveWindow(htabctrl, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		break;
	}

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam; // item drawing information
		if (htabctrl == lpdis->hwndItem){
			// which tab? first, second...fifth
			switch (lpdis->itemID)
			{
			case 0:
				
				break;
			case 1:
				;
				break;
			
			}
			char szTabText[1024];
			memset(szTabText, '\0', sizeof(szTabText));

			HBRUSH hbr = (HBRUSH)::GetStockObject(WHITE_BRUSH);

			TCITEM tci = {0};
			tci.mask = TCIF_TEXT;
			tci.pszText = szTabText;
			tci.cchTextMax = sizeof(szTabText) - 1;

			TabCtrl_GetItem(htabctrl, lpdis->itemID, &tci);

			FillRect(lpdis->hDC, &lpdis->rcItem, hbr);
			SetBkColor(lpdis->hDC, RGB(0xff,0,0));

			TextOut(lpdis->hDC,
				lpdis->rcItem.left,
				lpdis->rcItem.top,
				tci.pszText,
				lstrlen(tci.pszText));
		}
		break;
	}
	case WM_ERASEBKGND:{
		//HDC hdc = (HDC)wParam;
		//HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH); //��ʾ��ȡϵͳ��ɫ��ˢ
		//SelectObject(hdc, hBrush);

		return TRUE;
	}
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
		HDC hdc = (HDC)wParam;
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
	g_hInstance = hInstance;
	//�����Ի���֪���Ի���رղŻ�ִ���˳�
	//DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, MainDlg_Proc, 0);
	DialogBox(NULL, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, MainDlg_Proc);
	return 0;

}