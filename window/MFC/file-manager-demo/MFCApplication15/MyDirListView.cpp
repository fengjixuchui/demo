// MyDirListView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication15.h"
#include "MyDirListView.h"


// CMyDirListView

IMPLEMENT_DYNCREATE(CMyDirListView, CListView)

CMyDirListView::CMyDirListView()
{

}

CMyDirListView::~CMyDirListView()
{
}

BEGIN_MESSAGE_MAP(CMyDirListView, CListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_LISTINSERTDATA, &CMyDirListView::OnListinsertdata)
END_MESSAGE_MAP()


// CMyDirListView ���

#ifdef _DEBUG
void CMyDirListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyDirListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

#endif
#endif //_DEBUG



void CMyDirListView::SelectListHeader(int a)
{
	GetListCtrl().DeleteAllItems();
	while (GetListCtrl().GetHeaderCtrl() && GetListCtrl().GetHeaderCtrl()->GetItemCount() > 0) {
		GetListCtrl().DeleteColumn(0);
	}
		
	if (a == 1)
	{
		GetListCtrl().InsertColumn(0, "����", LVCFMT_LEFT, 140);
		GetListCtrl().InsertColumn(1, "����", LVCFMT_LEFT, 100);
		GetListCtrl().InsertColumn(2, "��С", LVCFMT_RIGHT, 100);
		GetListCtrl().InsertColumn(3, "���ÿռ�", LVCFMT_RIGHT, 100);
	}
	else
	{
		GetListCtrl().InsertColumn(0, "����", LVCFMT_LEFT, 150);
		GetListCtrl().InsertColumn(1, "����", LVCFMT_LEFT, 100);
		GetListCtrl().InsertColumn(2, "��С", LVCFMT_RIGHT, 100);
		GetListCtrl().InsertColumn(3, "�޸�ʱ��", LVCFMT_RIGHT, 200);
		GetListCtrl().InsertColumn(4, "����Ŀ¼", LVCFMT_LEFT, 250);
	}


}

int CMyDirListView::InsertListData(int type, char * path)
{
	SelectListHeader(type);
	int nItem = GetListCtrl().InsertItem(GetListCtrl().GetItemCount(),path);
	if (nItem == -1)	return FALSE;

	GetListCtrl().SetItemText(nItem, 0, "0");
	if (type == 1)
	{
		GetListCtrl().SetItemText(nItem, 1, "1");
		GetListCtrl().SetItemText(nItem, 2, "2");
		GetListCtrl().SetItemText(nItem, 3, "3");
	}
	else
	{
		
		GetListCtrl().SetItemText(nItem, 1, "1");
		GetListCtrl().SetItemText(nItem, 2, "2");
		GetListCtrl().SetItemText(nItem, 3, "3");
		GetListCtrl().SetItemText(nItem, 4, "4");
	}
	return 0;
}

// CMyDirListView ��Ϣ�������


int CMyDirListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	int nImage = 0;
	m_TreeImageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 6, 6);
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_COMPUTER));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_CLOSE));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_OPEN));

	
	GetListCtrl().ModifyStyle(0, LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SORTASCENDING | LVS_EDITLABELS);
	//GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	GetListCtrl().SetImageList(&m_TreeImageList, LVSIL_SMALL);

	//GetListCtrl().InsertColumn(0, "����", LVCFMT_LEFT, 140);
	//GetListCtrl().InsertColumn(1, "����", LVCFMT_LEFT, 170);
	//GetListCtrl().InsertColumn(2, "��С", LVCFMT_RIGHT, 100);
	//GetListCtrl().InsertColumn(3, "���ÿռ�", LVCFMT_RIGHT, 100);

	//InsertListData(0, "");
	SelectListHeader(1);
	return 0;
}


void CMyDirListView::OnSize(UINT nType, int cx, int cy)
{
	CListView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}





afx_msg LRESULT CMyDirListView::OnListinsertdata(WPARAM wParam, LPARAM lParam)
{
	char * path = (char*)wParam;

	return 0;
}
