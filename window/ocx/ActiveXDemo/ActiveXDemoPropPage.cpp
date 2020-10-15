// ActiveXDemoPropPage.cpp : CActiveXDemoPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "ActiveXDemo.h"
#include "ActiveXDemoPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CActiveXDemoPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CActiveXDemoPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CActiveXDemoPropPage, "ACTIVEXDEMO.ActiveXDemoPropPage.1",
	0x2ee91c54, 0x8004, 0x4238, 0xa8, 0x12, 0x85, 0xe4, 0x8c, 0x37, 0x96, 0xcb)



// CActiveXDemoPropPage::CActiveXDemoPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CActiveXDemoPropPage ��ϵͳע�����

BOOL CActiveXDemoPropPage::CActiveXDemoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACTIVEXDEMO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CActiveXDemoPropPage::CActiveXDemoPropPage - ���캯��

CActiveXDemoPropPage::CActiveXDemoPropPage() :
	COlePropertyPage(IDD, IDS_ACTIVEXDEMO_PPG_CAPTION)
{
}



// CActiveXDemoPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CActiveXDemoPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CActiveXDemoPropPage ��Ϣ�������
