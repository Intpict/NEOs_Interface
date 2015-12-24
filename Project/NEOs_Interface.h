// NEOs_Interface.h : NEOs_Interface 应用程序的主头文件
//

#if !defined(AFX_SYS_H__366C9F68_5419_4B22_8BE0_56282ECDCEE7__INCLUDED_)
#define AFX_SYS_H__366C9F68_5419_4B22_8BE0_56282ECDCEE7__INCLUDED_

#if _MSC_VER > 1000      
#pragma once
#endif // _MSC_VER > 1000  定义编译器版本

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"      
#include "NEOs_InterfaceDoc.h"
#include "CNEOsFrm.h"
#include "MainFrm.h"

class CNEOs_InterfaceApp : public CWinApp
{
public:
	CNEOs_InterfaceApp();
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
    //关节腿控制卡设备句柄
	HANDLE m_hDeviceApp[3];
	//设备连接状态
    bool Device_link_state[3];  
	CNEOs_InterfaceDoc* m_NIfDoc;
	CNEOsFrame* m_NIfFrm;
	CMultiDocTemplate* pDocTemplate;
    int Device_Num;     //实际连接的设备数
	CMainFrame* pMainFrame;
public:
	afx_msg void OnCreateintface();
};

#endif 