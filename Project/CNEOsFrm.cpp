// CNEOsFrm.cpp : CNEOsFrame ���ʵ��
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


// CNEOsFrame ����/����

CNEOsFrame::CNEOsFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CNEOsFrame::~CNEOsFrame()
{
}


BOOL CNEOsFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CNEOsFrame ���

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
// CNEOsFrame ��Ϣ�������
