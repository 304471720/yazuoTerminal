#pragma once

#include "resource.h"
// CGetCard 对话框

class CGetCard : public CDialogEx
{
	DECLARE_DYNAMIC(CGetCard)

public:
	CGetCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGetCard();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_No;
};
