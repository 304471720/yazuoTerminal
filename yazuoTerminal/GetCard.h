#pragma once

#include "resource.h"
// CGetCard �Ի���

class CGetCard : public CDialogEx
{
	DECLARE_DYNAMIC(CGetCard)

public:
	CGetCard(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGetCard();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_No;
};