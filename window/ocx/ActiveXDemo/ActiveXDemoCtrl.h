#pragma once

// ActiveXDemoCtrl.h : CActiveXDemoCtrl ActiveX �ؼ����������


// CActiveXDemoCtrl : �й�ʵ�ֵ���Ϣ������� ActiveXDemoCtrl.cpp��

class CActiveXDemoCtrl : public COleControl
{
	DECLARE_DYNCREATE(CActiveXDemoCtrl)

// ���캯��
public:
	CActiveXDemoCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CActiveXDemoCtrl();

	DECLARE_OLECREATE_EX(CActiveXDemoCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CActiveXDemoCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CActiveXDemoCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CActiveXDemoCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
		dispidcb_add = 1L,
		dispidshowString = 2L,
		dispidAdd = 1L
	};
protected:
	LONG Add(LONG a1, LONG a2);
	BSTR showString(LPCTSTR msg);
	void cb_add(DOUBLE a1, DOUBLE a2);
};

