#include "global.h"



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
	SetWindowPos(hwnd, HWND_TOP, rect.left+10, rect.top+20, rect.right, rect.bottom, SWP_SHOWWINDOW);

}
