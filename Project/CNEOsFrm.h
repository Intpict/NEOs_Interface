// CNEOsFrm.h : CNEOsFrame 类的接口
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

// 属性
public:

// 操作
public:

// 重写
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void ActivateFrame(int nCmdShow = -1);
// 实现
public:
	virtual ~CNEOsFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#endif 