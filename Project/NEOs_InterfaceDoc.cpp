// NEOs_InterfaceDoc.cpp : CNEOs_InterfaceDoc ���ʵ��
//

#include "stdafx.h"
#include "NEOs_Interface.h"

#include "NEOs_InterfaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CNEOs_InterfaceDoc

IMPLEMENT_DYNCREATE(CNEOs_InterfaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CNEOs_InterfaceDoc, CDocument)
END_MESSAGE_MAP()


// CNEOs_InterfaceDoc ����/����

CNEOs_InterfaceDoc::CNEOs_InterfaceDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CNEOs_InterfaceDoc::~CNEOs_InterfaceDoc()
{
}

BOOL CNEOs_InterfaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CNEOs_InterfaceDoc ���л�

void CNEOs_InterfaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CNEOs_InterfaceDoc ���

#ifdef _DEBUG
void CNEOs_InterfaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNEOs_InterfaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNEOs_InterfaceDoc ����
