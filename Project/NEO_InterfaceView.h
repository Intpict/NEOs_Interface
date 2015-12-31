#include "afxext.h"
#include "afxwin.h"
#include "nibutton.h"
#include "NiNumEdit.h"

#if !defined(AFX_ADVIEW_H__890AFD7C_4E65_4B40_B646_D1E42C19FBC2__INCLUDED_)
#define AFX_ADVIEW_H__890AFD7C_4E65_4B40_B646_D1E42C19FBC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CNEO_InterfaceView : public CFormView
{
	DECLARE_DYNCREATE(CNEO_InterfaceView)

protected:
	CNEO_InterfaceView();           // 动态创建所使用的受保护的构造函数
	virtual ~CNEO_InterfaceView();

public:
	enum { IDD = IDD_NEO_INTERFACEVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	virtual void OnInitialUpdate();
	void UpdateState(int DeviceIndex);
	void UpdatePos(int DeviceIndex);
	bool Drive_GJ_Move(int DeviceIndex , int AxisIndex);
	void GJ_Ctrl_Event(NI::CNiButton &Gj_Ctrl, int DeviceIndex , int AxisIndex);
	void Cut_Ctrl_Event(NI::CNiButton &Cut_Ctrl, int DeviceIndex);
	bool IsRunAuto();
	bool IsRunSingle();
	void ReviseCtrlButtonValue(NI::CNiButton &Gj_Ctrl, int DeviceIndex , int AxisIndex, bool value);

public:
	NI::CNiButton state_Gj1;
	NI::CNiButton state_Gj2;
	NI::CNiButton state_Gj3;
    NI::CNiButton ShiftButton;
	NI::CNiButton CutButton;
	NI::CNiButton ResetButton;
	NI::CNiButton CutState_Reset;
	NI::CNiButton CutState_Shift;
	NI::CNiButton CutState_Cutting;

	NI::CNiButton Gj11_Control;
	NI::CNiButton Gj12_Control;
	NI::CNiButton Gj13_Control;
	NI::CNiButton Gj14_Control;

	NI::CNiButton Gj21_Control;
	NI::CNiButton Gj22_Control;
	NI::CNiButton Gj23_Control;
	NI::CNiButton Gj24_Control;

	NI::CNiButton Gj31_Control;
	NI::CNiButton Gj32_Control;
	NI::CNiButton Gj33_Control;
	NI::CNiButton Gj34_Control;

	NI::CNiNumEdit CutMovSpeedMulti;
	NI::CNiNumEdit CutSpeedMulti;
	NI::CNiNumEdit CutDeepth;
	NI::CNiNumEdit StepSpeedBase;
	NI::CNiNumEdit StepSpeedMulti;
	NI::CNiNumEdit StepSpeedAcc;
	NI::CNiNumEdit StepSpeedDec; 
	NI::CNiNumEdit StepDvPulseNum;  

    //状态窗口显示变量
    NI::CNiNumEdit Gj11_AngleShow;
	NI::CNiNumEdit Gj12_AngleShow;
	NI::CNiNumEdit Gj13_AngleShow;
	NI::CNiNumEdit Gj21_AngleShow;
	NI::CNiNumEdit Gj22_AngleShow;
	NI::CNiNumEdit Gj23_AngleShow;
	NI::CNiNumEdit Gj31_AngleShow;
	NI::CNiNumEdit Gj32_AngleShow;
	NI::CNiNumEdit Gj33_AngleShow;
	NI::CNiButton Gj11_State;
	NI::CNiButton Gj12_State;
	NI::CNiButton Gj13_State;
	NI::CNiButton Gj14_State;
	NI::CNiButton Gj21_State;
	NI::CNiButton Gj22_State;
	NI::CNiButton Gj23_State;
	NI::CNiButton Gj24_State;
	NI::CNiButton Gj31_State;
	NI::CNiButton Gj32_State;
	NI::CNiButton Gj33_State;
	NI::CNiButton Gj34_State;
public:
	DECLARE_EVENTSINK_MAP()

public:
	CComboBox StepMode_Dir;
	CComboBox StepMode_Driver;

public:
	void ClickGj11Control();
	void ClickGj21Control();
	void ClickGj31Control();
	void ClickGj12Control();
	void ClickGj22Control();
	void ClickGj32Control();
	void ClickGj13Control();
	void ClickGj23Control();
	void ClickGj33Control();
	void ClickGj14Control();
	void ClickGj24Control();
	void ClickGj34Control();
	void ClickShiftSwitch();
	void ClickCutSwitch();
	void ClickResetSwitch();

public:
	CComboBox ShowStateMode;
	CStatic m_logo;
	CComboBox CutAngle;

public:
	afx_msg void OnCbnSelchangeComboStepDriver();

private:
	USB1020_PARA_DataList DL[3][3];		//公用配置参数，分别对应三关节的X，Y，Z轴     
	USB1020_PARA_LCData LC[3][3];		// 直线运动配置参数，分别对应三关节的X，Y，Z轴
	USB1020_PARA_DO Para[3];     //IO输出，用于控制切割电机运动
	USB1020_PARA_RR0 RR0[3];	   //状态寄存器RR0
	LONG PosInfo[3][3];     //存储各关节位置信息
	LONG m_count[3][3];    //用于矫正定长模式下的关节启动按钮状态
	afx_msg void OnBnClickedClear();
};

#endif