// CNEOsFrm.cpp : CNEOsFrame 类的实现
//
#include "stdafx.h"
#include "NEOs_Interface.h"

#include "CNEOsFrm.h"
//#include "NEO_InterfaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// CNEOsFrame

IMPLEMENT_DYNCREATE(CNEOsFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CNEOsFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CNEOsFrame 构造/析构

CNEOsFrame::CNEOsFrame()
{
	// TODO: 在此添加成员初始化代码
}

CNEOsFrame::~CNEOsFrame()
{
}


BOOL CNEOsFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CNEOsFrame 诊断

#ifdef _DEBUG
void CNEOsFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CNEOsFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

void CNEOsFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
// CNEOsFrame 消息处理程序
