// yazuoTerminal.h : yazuoTerminal DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CyazuoTerminalApp
// �йش���ʵ�ֵ���Ϣ������� yazuoTerminal.cpp
//

class CyazuoTerminalApp : public CWinApp
{
public:
	CyazuoTerminalApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
