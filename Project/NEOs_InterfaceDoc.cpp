// NEOs_InterfaceDoc.cpp : CNEOs_InterfaceDoc 类的实现
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


// CNEOs_InterfaceDoc 构造/析构

CNEOs_InterfaceDoc::CNEOs_InterfaceDoc()
{
	// TODO: 在此添加一次性构造代码

}

CNEOs_InterfaceDoc::~CNEOs_InterfaceDoc()
{
}

BOOL CNEOs_InterfaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CNEOs_InterfaceDoc 序列化

void CNEOs_InterfaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CNEOs_InterfaceDoc 诊断

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


// CNEOs_InterfaceDoc 命令
