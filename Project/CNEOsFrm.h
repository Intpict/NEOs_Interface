// CNEOsFrm.h : CNEOsFrame ��Ľӿ�
//

#if !defined(AFX_ADFRM_H__D99D7F2C_3AEC_4CEC_81A5_F0231D95D059__INCLUDED_)
#define AFX_ADFRM_H__D99D7F2C_3AEC_4CEC_81A5_F0231D95D059__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNEOsFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CNEOsFrame)
public:
	CNEOsFrame();

// ����
public:

// ����
public:

// ��д
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void ActivateFrame(int nCmdShow = -1);
// ʵ��
public:
	virtual ~CNEOsFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#endif 