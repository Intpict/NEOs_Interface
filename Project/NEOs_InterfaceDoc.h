// NEOs_InterfaceDoc.h : CNEOs_InterfaceDoc ��Ľӿ�
//


#pragma once


class CNEOs_InterfaceDoc : public CDocument
{
protected: // �������л�����
	CNEOs_InterfaceDoc();
	DECLARE_DYNCREATE(CNEOs_InterfaceDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CNEOs_InterfaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


