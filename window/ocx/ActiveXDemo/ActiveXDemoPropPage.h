#pragma once

// ActiveXDemoPropPage.h : CActiveXDemoPropPage ����ҳ���������


// CActiveXDemoPropPage : �й�ʵ�ֵ���Ϣ������� ActiveXDemoPropPage.cpp��

class CActiveXDemoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CActiveXDemoPropPage)
	DECLARE_OLECREATE_EX(CActiveXDemoPropPage)

// ���캯��
public:
	CActiveXDemoPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_ACTIVEXDEMO };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

