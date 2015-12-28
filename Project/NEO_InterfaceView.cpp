#include "stdafx.h"
#include "NEOs_Interface.h"
#include "NEO_InterfaceView.h"
#include "NEOs_InterfaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CNEO_InterfaceView, CFormView)

CNEOs_InterfaceApp* m_theApp;   //指向App的全局指针

CNEO_InterfaceView::CNEO_InterfaceView()
	: CFormView(CNEO_InterfaceView::IDD)
{
}

CNEO_InterfaceView::~CNEO_InterfaceView()
{
}

void CNEO_InterfaceView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOWGJ1, state_Gj1);
	DDX_Control(pDX, IDC_SHOWGJ2, state_Gj2);
	DDX_Control(pDX, IDC_SHOWGJ3, state_Gj3);
	DDX_Control(pDX, IDC_WORK1_START, CutStart);
	DDX_Control(pDX, IDC_WORK1_STOP, CutStop);
	DDX_Control(pDX, IDC_WORK1_CONTINUE, CutContinue);
	DDX_Control(pDX, IDC_WORK1_MOVSPEED_MULTI, CutMovSpeedMulti);
	DDX_Control(pDX, IDC_WORK1_CUTSPEED_MULTI, CutSpeedMulti);
	DDX_Control(pDX, IDC_WORK1_DEEP, CutDeepth);
	DDX_Control(pDX, IDC_CUTSTATE_RESET, CutState_Reset);
	DDX_Control(pDX, IDC_CUTSTATE_SHIFT, CutState_Shift);
	DDX_Control(pDX, IDC_CUTSTATE_CUTTING, CutState_Cutting);
	DDX_Control(pDX, IDC_STEP_SPEEDBASE, StepSpeedBase);
	DDX_Control(pDX, IDC_STEP_SPEEDMULTI, StepSpeedMulti);
	DDX_Control(pDX, IDC_STEP_SPEEDACC, StepSpeedAcc);
	DDX_Control(pDX, IDC_STEP_SPEEDDEC, StepSpeedDec);
	DDX_Control(pDX, IDC_STEP_DVPLUSENUM, StepDvPulseNum);
	DDX_Control(pDX, IDC_COMBO_STEP_DIR, StepMode_Dir);
	DDX_Control(pDX, IDC_COMBO_STEP_DRIVER, StepMode_Driver);
	DDX_Control(pDX, IDC_COMBO_CUTMOTOR,CutMotorDir);
	DDX_Control(pDX, IDC_GJ11_CONTROL,Gj11_Control);
	DDX_Control(pDX, IDC_GJ12_CONTROL,Gj12_Control);
	DDX_Control(pDX, IDC_GJ13_CONTROL,Gj13_Control);
	DDX_Control(pDX, IDC_GJ14_CONTROL,Gj14_Control);
	DDX_Control(pDX, IDC_GJ21_CONTROL,Gj21_Control);
	DDX_Control(pDX, IDC_GJ22_CONTROL,Gj22_Control);
	DDX_Control(pDX, IDC_GJ23_CONTROL,Gj23_Control);
	DDX_Control(pDX, IDC_GJ24_CONTROL,Gj24_Control);
	DDX_Control(pDX, IDC_GJ31_CONTROL,Gj31_Control);
	DDX_Control(pDX, IDC_GJ32_CONTROL,Gj32_Control);
	DDX_Control(pDX, IDC_GJ33_CONTROL,Gj33_Control);
	DDX_Control(pDX, IDC_GJ34_CONTROL,Gj34_Control);
	DDX_Control(pDX, IDC_GJ11_ANGLE, Gj11_AngleShow);
	DDX_Control(pDX, IDC_GJ12_ANGLE, Gj12_AngleShow);
	DDX_Control(pDX, IDC_GJ13_ANGLE, Gj13_AngleShow);
	DDX_Control(pDX, IDC_GJ21_ANGLE, Gj21_AngleShow);
	DDX_Control(pDX, IDC_GJ22_ANGLE, Gj22_AngleShow);
	DDX_Control(pDX, IDC_GJ23_ANGLE, Gj23_AngleShow);
	DDX_Control(pDX, IDC_GJ31_ANGLE, Gj31_AngleShow);
	DDX_Control(pDX, IDC_GJ32_ANGLE, Gj32_AngleShow);
	DDX_Control(pDX, IDC_GJ33_ANGLE, Gj33_AngleShow);
	DDX_Control(pDX, IDC_GJ11_STATE, Gj11_State);
	DDX_Control(pDX, IDC_GJ12_STATE, Gj12_State);
	DDX_Control(pDX, IDC_GJ13_STATE, Gj13_State);
	DDX_Control(pDX, IDC_GJ14_STATE, Gj14_State);
	DDX_Control(pDX, IDC_GJ21_STATE, Gj21_State);
	DDX_Control(pDX, IDC_GJ22_STATE, Gj22_State);
	DDX_Control(pDX, IDC_GJ23_STATE, Gj23_State);
	DDX_Control(pDX, IDC_GJ24_STATE, Gj24_State);
	DDX_Control(pDX, IDC_GJ31_STATE, Gj31_State);
	DDX_Control(pDX, IDC_GJ32_STATE, Gj32_State);
	DDX_Control(pDX, IDC_GJ33_STATE, Gj33_State);
	DDX_Control(pDX, IDC_GJ34_STATE, Gj34_State);
	DDX_Control(pDX, IDC_COMBO_STATEMODE, ShowStateMode);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_logo);
	DDX_Control(pDX, IDC_COMBO_CUT_ANGLE, CutAngle);
}

BEGIN_MESSAGE_MAP(CNEO_InterfaceView, CFormView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_STEP_DRIVER, &CNEO_InterfaceView::OnCbnSelchangeComboStepDriver)
END_MESSAGE_MAP()


// CNEO_InterfaceView 诊断

#ifdef _DEBUG
void CNEO_InterfaceView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNEO_InterfaceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNEO_InterfaceView 消息处理程序
int CNEO_InterfaceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	m_theApp = (CNEOs_InterfaceApp*)AfxGetApp();    //得到当前应用进程的指针
	return 0;
}

void CNEO_InterfaceView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent) {
		case 1:
        break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CNEO_InterfaceView::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFormView::OnClose();
}

void CNEO_InterfaceView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	//关节连接指示灯
	(m_theApp->Device_link_state[0])?(state_Gj1.Value=TRUE):(state_Gj1.Value=FALSE);
	(m_theApp->Device_link_state[1])?(state_Gj2.Value=TRUE):(state_Gj2.Value=FALSE);
	(m_theApp->Device_link_state[2])?(state_Gj3.Value=TRUE):(state_Gj3.Value=FALSE);
	
	//系统名称显示
	static CFont font1; 
	font1.CreatePointFont(300, "隶书"); 
	GetDlgItem(IDC_STATIC_TXT)->SetFont(&font1);  
	
	//logo显示
	static CBitmap logo; 
	logo.LoadBitmap(IDB_LOGO);
    m_logo.SetBitmap(logo);
   
	//初始化配置窗口
	StepMode_Dir.SetCurSel(USB1020_PDIRECTION);
	StepMode_Driver.SetCurSel(USB1020_LV); 
	StepDvPulseNum.EnableWindow(FALSE);
	CutMotorDir.SetCurSel(USB1020_PDIRECTION);
	ShowStateMode.SetCurSel(PULSENUMMODE);
	StepDvPulseNum.SetValue(1000);
	StepSpeedBase.SetValue(100);
	StepSpeedMulti.SetValue(1);
	StepSpeedAcc.SetValue(150);
	StepSpeedDec.SetValue(150);

	//初始化控制板卡参数信息
	for(int i=0; i<TotelDeviceNum; i++)
		for (int j=0; j<AxisNum; j++)
		{
			LC[i][j].AxisNum = j;						// 轴号(USB1020_XAXIS:X轴; USB1020_YAXIS:Y轴;USB1020_ZAXIS:Z轴; 
			LC[i][j].LV_DV= StepMode_Driver.GetCurSel();				// 驱动方式 USB1020_DV:定长驱动 USB1020_LV: 连续驱动
			LC[i][j].PulseMode = USB1020_CPDIR;		// 模式0：CW/CCW方式，1：CP/DIR方式 
			LC[i][j].Line_Curve = USB1020_LINE;		// 直线曲线(0:直线加/减速; 1:S曲线加/减速)
			LC[i][j].Direction = StepMode_Dir.GetCurSel();     //方向
			LC[i][j].PLSLogLever =  0;
			LC[i][j].DIRLogLever = 0;
			LC[i][j].DecMode = 0;
			LC[i][j].nPulseNum = StepDvPulseNum.Value;
			DL[i][j].Multiple = StepSpeedMulti.Value;       //倍率
			DL[i][j].Acceleration = StepSpeedAcc.Value;				// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
			DL[i][j].Deceleration = StepSpeedDec.Value;				// 减速度(125~1000000)
			DL[i][j].StartSpeed = 300;                  // 初始速度
			DL[i][j].DriveSpeed = StepSpeedBase.Value;      //驱动速度
			DL[i][j].DecIncRate = 1000;
			DL[i][j].AccIncRate = 1000;
		}
}


void CNEO_InterfaceView::RefreshStateGJ1(void)
{
}

void CNEO_InterfaceView::RefreshStateGJ2(void)
{
}

void CNEO_InterfaceView::RefreshStateGJ3(void)
{
}

BEGIN_EVENTSINK_MAP(CNEO_InterfaceView, CFormView)
	ON_EVENT(CNEO_InterfaceView, IDC_WORK1_START, DISPID_CLICK, CNEO_InterfaceView::ClickWork1Start, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_WORK1_STOP, DISPID_CLICK, CNEO_InterfaceView::ClickWork1Stop, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_WORK1_CONTINUE, DISPID_CLICK, CNEO_InterfaceView::ClickWork1Continue, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ11_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj11Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ21_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj21Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ31_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj31Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ12_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj12Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ22_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj22Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ32_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj32Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ13_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj13Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ23_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj23Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ33_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj33Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ14_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj14Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ24_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj24Control, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_GJ34_CONTROL, DISPID_CLICK, CNEO_InterfaceView::ClickGj34Control, VTS_NONE)
END_EVENTSINK_MAP()

void CNEO_InterfaceView::ClickWork1Start()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickWork1Stop()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickWork1Continue()
{
	// TODO: 在此处添加消息处理程序代码
}


void CNEO_InterfaceView::ClickGj11Control()
{
}

void CNEO_InterfaceView::ClickGj21Control()
{
	int DeviceIndex = 1, AxisIndex = 0;
	if(INVALID_HANDLE_VALUE == m_theApp->m_hDeviceApp[DeviceIndex]){
		AfxMessageBox("关节驱动连接失败！");
		return;
	}
	if(Gj21_Control.Value){
		if(!Drive_GJ_Move(DeviceIndex, AxisIndex))
			AfxMessageBox("关节启动失败！");
	}else{
		if(!USB1020_InstStop(m_theApp->m_hDeviceApp[DeviceIndex], LC[DeviceIndex][AxisIndex].AxisNum))
			AfxMessageBox("关节停止失败！");
	}
}

void CNEO_InterfaceView::ClickGj31Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj12Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj22Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj32Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj13Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj23Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj33Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj14Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj24Control()
{
	// TODO: 在此处添加消息处理程序代码
}

void CNEO_InterfaceView::ClickGj34Control()
{
	// TODO: 在此处添加消息处理程序代码
}


void CNEO_InterfaceView::OnCbnSelchangeComboStepDriver()
{
	// TODO: 在此添加控件通知处理程序代码
	if (StepMode_Driver.GetCurSel() == USB1020_LV)
		StepDvPulseNum.EnableWindow(FALSE);
	else
        StepDvPulseNum.EnableWindow(TRUE);
}

bool CNEO_InterfaceView::Drive_GJ_Move(int DeviceIndex , int AxisIndex)
{
	//基本参数设置
	DL[DeviceIndex][AxisIndex].DriveSpeed = StepSpeedBase.Value;
	DL[DeviceIndex][AxisIndex].Multiple = LONG(StepSpeedMulti.Value);
	LC[DeviceIndex][AxisIndex].Direction =  StepMode_Dir.GetCurSel();
	//驱动模式设置
	LC[DeviceIndex][AxisIndex].LV_DV = StepMode_Driver.GetCurSel();
	if (USB1020_DV == LC[DeviceIndex][AxisIndex].LV_DV)
		LC[DeviceIndex][AxisIndex].nPulseNum = LONG(StepDvPulseNum.Value);
	//初始化运动板卡
	if(!USB1020_SetEncoderSignalType(m_theApp->m_hDeviceApp[DeviceIndex], LC[DeviceIndex][AxisIndex].AxisNum, 0, 0))
		return FALSE;
	if(!USB1020_PulseInputMode(m_theApp->m_hDeviceApp[DeviceIndex], LC[DeviceIndex][AxisIndex].AxisNum, 0))
		return FALSE;
	if(!USB1020_InitLVDV(m_theApp->m_hDeviceApp[DeviceIndex], &DL[DeviceIndex][AxisIndex],  &LC[DeviceIndex][AxisIndex]))
		return FALSE;
	//驱动该轴运动
	if(!USB1020_StartLVDV(m_theApp->m_hDeviceApp[DeviceIndex], LC[DeviceIndex][AxisIndex].AxisNum))
		return FALSE;
	return TRUE;
}
