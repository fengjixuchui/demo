
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MFCApplication15.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_MESSAGE(WM_TREESELCHANGED, &CMainFrame::OnTreeselchanged)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("δ�ܴ�����ͼ����\n");
	//	return -1;
	//}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	//m_wndToolBar.SetButtonStyle(0, TBBS_AUTOSIZE);
	//m_wndToolBar.SetButtonText(0, _T("����1"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//OnCreateClient�����ĵ�����OnCreate����֮���ڹ�����ͼ����Ͳ�����ͼ����֮ǰ
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (!m_WndSplitter.CreateStatic(this, 2, 1))
		return FALSE;
	//Ϊ��һ�����������ͼ
	m_WndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CMyDirView),
		CSize(0, 0),
		pContext);
	m_WndSplitter.CreateView(1, 0, // ������С�������
		RUNTIME_CLASS(CMyDirView),//��ͼ��
		CSize(0, 0),//��ʼ����С
		pContext);//�����ڵĴ�������

	CMyDirView* pDirView = (CMyDirView*)m_WndSplitter.GetPane(0, 0);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_WndSplitter.GetSafeHwnd())
	{
		m_WndSplitter.MoveWindow(-2, -2, cx + 2, cy + 2); // ȥ���ָ����ı߿�
														  //ָ��һ���ָ��������С�
														  //ָ���ָ��������е������ر�ʾ�������ȡ�
														  // 	ָ���ָ��������������ر�ʾ����С��ȡ�
		//m_WndSplitter.SetRowInfo(0, 200, 0); //ִ�е�һ�п��
		m_WndSplitter.RecalcLayout();//���²��ô��ڡ�
	}
}


afx_msg LRESULT CMainFrame::OnTreeselchanged(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hItem = (HTREEITEM)wParam;
	

	return 0;
}
