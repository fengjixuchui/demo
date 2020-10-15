#pragma once



// CMyDirTreeView ��ͼ

class CMyDirTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyDirTreeView)

protected:
	CMyDirTreeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyDirTreeView();
	
	CImageList		m_TreeImageList;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	CWnd *m_container;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	void DeleteTreeChild(HTREEITEM hItem);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


