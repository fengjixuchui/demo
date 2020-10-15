// ActiveXDemoCtrl.cpp : CActiveXDemoCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "ActiveXDemo.h"
#include "ActiveXDemoCtrl.h"
#include "ActiveXDemoPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CActiveXDemoCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CActiveXDemoCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CActiveXDemoCtrl, COleControl)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "Add", dispidAdd, Add, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "showString", dispidshowString, showString, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "cb_add", dispidcb_add, cb_add, VT_EMPTY, VTS_R8 VTS_R8)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CActiveXDemoCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CActiveXDemoCtrl, 1)
	PROPPAGEID(CActiveXDemoPropPage::guid)
END_PROPPAGEIDS(CActiveXDemoCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CActiveXDemoCtrl, "ACTIVEXDEMO.ActiveXDemoCtrl.1",
	0x153809b, 0x60ed, 0x402b, 0x9e, 0xf2, 0x8f, 0x46, 0xe2, 0x17, 0x6c, 0x6d)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CActiveXDemoCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DActiveXDemo = { 0x7FA6A843, 0x6D85, 0x4825, { 0xBB, 0x0, 0xE0, 0xEF, 0x3F, 0x48, 0xBE, 0xFB } };
const IID IID_DActiveXDemoEvents = { 0x2B04342B, 0xBC5E, 0x442B, { 0x9E, 0xC2, 0x84, 0x7, 0x6F, 0x8A, 0x2C, 0xE5 } };


// �ؼ�������Ϣ

static const DWORD _dwActiveXDemoOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CActiveXDemoCtrl, IDS_ACTIVEXDEMO, _dwActiveXDemoOleMisc)



// CActiveXDemoCtrl::CActiveXDemoCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CActiveXDemoCtrl ��ϵͳע�����

BOOL CActiveXDemoCtrl::CActiveXDemoCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ACTIVEXDEMO,
			IDB_ACTIVEXDEMO,
			afxRegApartmentThreading,
			_dwActiveXDemoOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CActiveXDemoCtrl::CActiveXDemoCtrl - ���캯��

CActiveXDemoCtrl::CActiveXDemoCtrl()
{
	InitializeIIDs(&IID_DActiveXDemo, &IID_DActiveXDemoEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}



// CActiveXDemoCtrl::~CActiveXDemoCtrl - ��������

CActiveXDemoCtrl::~CActiveXDemoCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CActiveXDemoCtrl::OnDraw - ��ͼ����

void CActiveXDemoCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CActiveXDemoCtrl::DoPropExchange - �־���֧��

void CActiveXDemoCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CActiveXDemoCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CActiveXDemoCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CActiveXDemoCtrl::AboutBox - ���û���ʾ�����ڡ���

void CActiveXDemoCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_ACTIVEXDEMO);
	dlgAbout.DoModal();
}



// CActiveXDemoCtrl ��Ϣ�������


LONG CActiveXDemoCtrl::Add(LONG a1, LONG a2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	this->cb_add(a1,a2);
	return a1 + a2;
}


BSTR CActiveXDemoCtrl::showString(LPCTSTR msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(msg);

	// TODO: �ڴ���ӵ��ȴ���������
	MessageBox(msg, msg, MB_OK);
	return strResult.AllocSysString();
}


void CActiveXDemoCtrl::cb_add(DOUBLE a1, DOUBLE a2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	CString str;
	str.Format(TEXT(" %f + %f = %f"),a1,a2,a1+a2);
	MessageBox(str, str, MB_OK);
	FireEvent(dispidcb_add, EVENT_PARAM(VTS_R8 VTS_R8), a1, a2);
	


}
