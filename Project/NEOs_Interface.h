// NEOs_Interface.h : NEOs_Interface Ӧ�ó������ͷ�ļ�
//

#if !defined(AFX_SYS_H__366C9F68_5419_4B22_8BE0_56282ECDCEE7__INCLUDED_)
#define AFX_SYS_H__366C9F68_5419_4B22_8BE0_56282ECDCEE7__INCLUDED_

#if _MSC_VER > 1000      
#pragma once
#endif // _MSC_VER > 1000  ����������汾

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"      
#include "NEOs_InterfaceDoc.h"
#include "CNEOsFrm.h"
#include "MainFrm.h"

class CNEOs_InterfaceApp : public CWinApp
{
public:
	CNEOs_InterfaceApp();
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
    //�ؽ��ȿ��ƿ��豸���
	HANDLE m_hDeviceApp[3];
	//�豸����״̬
    bool Device_link_state[3];  
	CNEOs_InterfaceDoc* m_NIfDoc;
	CNEOsFrame* m_NIfFrm;
	CMultiDocTemplate* pDocTemplate;
    int Device_Num;     //ʵ�����ӵ��豸��
	CMainFrame* pMainFrame;
public:
	afx_msg void OnCreateintface();
};

#endif 