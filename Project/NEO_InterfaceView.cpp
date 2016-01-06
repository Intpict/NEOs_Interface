#include "stdafx.h"
#include "stdio.h"
#include "func.h"
#include "NEOs_Interface.h"
#include "NEO_InterfaceView.h"
#include "NEOs_InterfaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CNEO_InterfaceView, CFormView)

CNEOs_InterfaceApp* m_theApp;   //指向App的全局指针
double x_angle[Max_length],y_angle[Max_length],z_angle[Max_length];
int x_pluse[Max_length-1],y_pluse[Max_length-1],z_pluse[Max_length-1];
int x_angle_length = 0, x_pluse_length = 0;
int y_angle_length = 0, y_pluse_length = 0;
int z_angle_length = 0, z_pluse_length = 0;	

UINT ShiftThread(LPVOID lpParam){
	CNEO_InterfaceView* ptr = (CNEO_InterfaceView*)lpParam;
	char ch[10];
	ptr->ShiftSpeedMuti.GetWindowText(ch, 10);
	int SpeedRatio = atoi(ch);
	Work_State1_Set(m_theApp->m_hDeviceApp, ptr->x_realangle, ptr->y_realangle, ptr->z_realangle, x_angle[0],y_angle[0],z_angle[0], SpeedRatio);
	ptr->ShiftFlag = FALSE;
	return 0;
}

UINT CutThread(LPVOID lpParam){
	CNEO_InterfaceView* ptr = (CNEO_InterfaceView*)lpParam;
	char ch[10];
	ptr->CutSpeedMuti.GetWindowText(ch, 10);
	int SpeedRatio = atoi(ch);
	Work_State2_Set(m_theApp->m_hDeviceApp, ptr->x_realangle, ptr->y_realangle, ptr->z_realangle, x_pluse, y_pluse, z_pluse, x_pluse_length, SpeedRatio, ptr->CutFlag);
	ptr->CutFlag = FALSE;
	return 0;
}

UINT ResetThread(LPVOID lpParam){
	CNEO_InterfaceView* ptr = (CNEO_InterfaceView*)lpParam;
	char ch[10];

	ptr->CutSpeedMuti.GetWindowText(ch, 10);
	int SpeedRatio = atoi(ch);
	Work_State2_Reset(m_theApp->m_hDeviceApp, x_pluse, y_pluse, z_pluse, x_pluse_length, SpeedRatio, ptr->ResetFlag);

	ptr->ShiftSpeedMuti.GetWindowText(ch, 10);
	SpeedRatio = atoi(ch);
	Work_State1_Reset(m_theApp->m_hDeviceApp, x_angle[0], y_angle[0],z_angle[0], SpeedRatio, ptr->ResetFlag);
	ptr->ResetFlag = FALSE;
	return 0;
}


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
	DDX_Control(pDX, IDC_SHIFT_SWITCH, ShiftButton);
	DDX_Control(pDX, IDC_CUT_SWITCH, CutButton);
	DDX_Control(pDX, IDC_RESET_SWITCH, ResetButton);
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
	DDX_Control(pDX, IDC_SHIFT_SPEED_MUTI, ShiftSpeedMuti);
	DDX_Control(pDX, IDC_CUT_SPEED_MUTI, CutSpeedMuti);
	DDX_Control(pDX, IDC_CUT_DEEPTH, CutDeepth);
	DDX_Control(pDX, IDC_SHOW_ANGLE11, Gj11_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE12, Gj12_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE13, Gj13_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE21, Gj21_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE22, Gj22_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE23, Gj23_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE31, Gj31_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE32, Gj32_AngleShow);
	DDX_Control(pDX, IDC_SHOW_ANGLE33, Gj33_AngleShow);
}

BEGIN_MESSAGE_MAP(CNEO_InterfaceView, CFormView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_STEP_DRIVER, &CNEO_InterfaceView::OnCbnSelchangeComboStepDriver)
	ON_BN_CLICKED(IDC_CLEAR, &CNEO_InterfaceView::OnBnClickedClear)
END_MESSAGE_MAP()

// CNEO_InterfaceView 诊断

#ifdef _DEBUG
void CNEO_InterfaceView::AssertValid() const{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNEO_InterfaceView::Dump(CDumpContext& dc) const{
	CFormView::Dump(dc);
}
#endif
#endif


int CNEO_InterfaceView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_theApp = (CNEOs_InterfaceApp*)AfxGetApp();    //得到当前应用进程的指针
	return 0;
}

void CNEO_InterfaceView::OnTimer(UINT_PTR nIDEvent){
	switch(nIDEvent) {
		case 1:
			for (int DeviceIndex = 0; DeviceIndex<TotelDeviceNum; DeviceIndex++)
			{
				if(INVALID_HANDLE_VALUE == m_theApp->m_hDeviceApp[DeviceIndex])
					continue;
				UpdateGJState(DeviceIndex);
				UpdatePos(DeviceIndex);
			}
			RefreshPosShow();
			RefreshAutoModeLight();
			RefreshAutoModeCtrlButton();
			break;
		default:
			break;
	}
	CFormView::OnTimer(nIDEvent);
}

void CNEO_InterfaceView::OnClose(){
	KillTimer(1);
	m_theApp = NULL;
	CFormView::OnClose();
}

void CNEO_InterfaceView::OnInitialUpdate(){
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
	ShowStateMode.SetCurSel(PULSENUMMODE);
	StepDvPulseNum.SetValue(1000);
	StepSpeedBase.SetValue(100);
	StepSpeedMulti.SetValue(1);
	StepSpeedAcc.SetValue(150);
	StepSpeedDec.SetValue(150);
	CutAngle.SetCurSel(ANGLE60);
	CString str;
	str.Format("%d",  5);
	ShiftSpeedMuti.SetWindowText(str);
	str.Format("%d",  3);
	CutSpeedMuti.SetWindowText(str);
	str.Format("%d",  20);
	CutDeepth.SetWindowText(str);
	CutDeepth.EnableWindow(FALSE);

	x_outfile.open("OutFiles/x_pluse.txt");
	y_outfile.open("OutFiles/y_pluse.txt");
	z_outfile.open("OutFiles/z_pluse.txt");

	x_realangle.open("OutFiles/x_realangle.txt");
	y_realangle.open("OutFiles/y_realangle.txt");
	z_realangle.open("OutFiles/z_realangle.txt");

	ShiftFlag = FALSE;
	CutFlag = FALSE;
	ResetFlag = FALSE;
	Mutex = CreateMutex(NULL,  false, "NEOs Lock");

	for(int i=0; i<TotelDeviceNum; i++){
		for(int j=0; j<AxisNum; j++){
			m_count[i][j] = 0;    //初始化矫正计数器
			PosInfo[i][j] = 0;
		}
	}

	for (int i = 0; i<3; i++)
		auto_count[i] = 0;


	//初始化控制板卡参数信息
	for(int i=0; i<TotelDeviceNum; i++){
		Para[i].OUT0 = 0;
		Para[i].OUT1 = 0;
		Para[i].OUT2 = 0;
		Para[i].OUT3 = 0;
		Para[i].OUT4 = 0;
		Para[i].OUT5 = 0;
		Para[i].OUT6 = 0;
		Para[i].OUT7 = 0;
		if (INVALID_HANDLE_VALUE != m_theApp->m_hDeviceApp[i])
			USB1020_SetDeviceDO(m_theApp->m_hDeviceApp[i], USB1020_XAXIS, &Para[i]);
		for (int j=0; j<AxisNum; j++)
		{
			LC[i][j].AxisNum = j;						// 轴号(USB1020_XAXIS:X轴; USB1020_YAXIS:Y轴;USB1020_ZAXIS:Z轴; 
			LC[i][j].LV_DV= StepMode_Driver.GetCurSel();	  // 驱动方式 USB1020_DV:定长驱动 USB1020_LV: 连续驱动
			LC[i][j].PulseMode = USB1020_CPDIR;		// 模式0：CW/CCW方式，1：CP/DIR方式 
			LC[i][j].Line_Curve = USB1020_LINE;		// 直线曲线(0:直线加/减速; 1:S曲线加/减速)
			LC[i][j].Direction = StepMode_Dir.GetCurSel();     //方向
			LC[i][j].DecMode = 0;
			LC[i][j].PLSLogLever = 0;
			LC[i][j].DIRLogLever = 0;
			LC[i][j].nPulseNum = StepDvPulseNum.Value;
			DL[i][j].Multiple = LONG(StepSpeedMulti.Value);       //倍率
			DL[i][j].Acceleration = LONG(StepSpeedAcc.Value);				// 加速度(125~1000,000)(直线加减速驱动中加速度一直不变）
			DL[i][j].Deceleration = LONG(StepSpeedDec.Value);				// 减速度(125~1000000)
			DL[i][j].StartSpeed = 300;                  // 初始速度
			DL[i][j].DriveSpeed = LONG(StepSpeedBase.Value);      //驱动速度
			DL[i][j].DecIncRate = 1000;
			DL[i][j].AccIncRate = 1000;
		}
	}
	SetTimer(1, 20 , NULL);  // 启动定时器1，用来刷新计数器和各关节状态
}

BEGIN_EVENTSINK_MAP(CNEO_InterfaceView, CFormView)
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
	ON_EVENT(CNEO_InterfaceView, IDC_SHIFT_SWITCH, DISPID_CLICK, CNEO_InterfaceView::ClickShiftSwitch, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_CUT_SWITCH, DISPID_CLICK, CNEO_InterfaceView::ClickCutSwitch, VTS_NONE)
	ON_EVENT(CNEO_InterfaceView, IDC_RESET_SWITCH, DISPID_CLICK, CNEO_InterfaceView::ClickResetSwitch, VTS_NONE)
END_EVENTSINK_MAP()

void CNEO_InterfaceView::ClickGj11Control(){
	int DeviceIndex = 0, AxisIndex = 0;
	GJ_Ctrl_Event(Gj11_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj21Control(){
	int DeviceIndex = 1, AxisIndex = 0;
	GJ_Ctrl_Event(Gj21_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj31Control(){
	int DeviceIndex = 2, AxisIndex = 0;
	GJ_Ctrl_Event(Gj31_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj12Control(){
	int DeviceIndex = 0, AxisIndex = 1;
	GJ_Ctrl_Event(Gj12_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj22Control(){
	int DeviceIndex = 1, AxisIndex = 1;
	GJ_Ctrl_Event(Gj22_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj32Control(){
	int DeviceIndex = 2, AxisIndex = 1;
	GJ_Ctrl_Event(Gj32_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj13Control(){
	int DeviceIndex = 0, AxisIndex = 2;
	GJ_Ctrl_Event(Gj13_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj23Control(){
	int DeviceIndex = 1, AxisIndex = 2;
	GJ_Ctrl_Event(Gj23_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj33Control(){
	int DeviceIndex = 2, AxisIndex = 2;
	GJ_Ctrl_Event(Gj33_Control, DeviceIndex, AxisIndex);
}

void CNEO_InterfaceView::ClickGj14Control(){
	int DeviceIndex = 0;
	Cut_Ctrl_Event(Gj14_Control, DeviceIndex);
}

void CNEO_InterfaceView::ClickGj24Control(){
	int DeviceIndex = 1;
	Cut_Ctrl_Event(Gj24_Control, DeviceIndex);
}

void CNEO_InterfaceView::ClickGj34Control(){
	int DeviceIndex = 2;
	Cut_Ctrl_Event(Gj34_Control, DeviceIndex);
}


void CNEO_InterfaceView::OnCbnSelchangeComboStepDriver(){
	if (StepMode_Driver.GetCurSel() == USB1020_LV)
		StepDvPulseNum.EnableWindow(FALSE);
	else
        StepDvPulseNum.EnableWindow(TRUE);
}

void CNEO_InterfaceView::GJ_Ctrl_Event(NI::CNiButton &Gj_Ctrl, int DeviceIndex , int AxisIndex){
	if(INVALID_HANDLE_VALUE == m_theApp->m_hDeviceApp[DeviceIndex]){
		std::string msg = "关节腿";
		char index = '1' + DeviceIndex;
		msg.push_back(index);
		msg.append("连接失败!");
		AfxMessageBox(msg.c_str());
		Gj_Ctrl.Value ^= 0x1;
		return;
	}

	if(IsRunAuto()){
		AfxMessageBox("自动控制模式运行中!");
		Gj_Ctrl.Value ^= 0x1;
		return;
	}

	if(Gj_Ctrl.Value){
		if(!Drive_GJ_Move(DeviceIndex, AxisIndex))
			AfxMessageBox("启动失败!");
	}else{
		if(!USB1020_InstStop(m_theApp->m_hDeviceApp[DeviceIndex], LC[DeviceIndex][AxisIndex].AxisNum))
			AfxMessageBox("停止失败!");
	}
}

void CNEO_InterfaceView::Cut_Ctrl_Event(NI::CNiButton &Cut_Ctrl, int DeviceIndex){
	if (INVALID_HANDLE_VALUE == m_theApp->m_hDeviceApp[DeviceIndex]){
		std::string msg = "关节腿";
		char index = '1' + DeviceIndex;
		msg.push_back(index);
		msg.append("连接失败!");
		AfxMessageBox(msg.c_str());
		Cut_Ctrl.Value ^= 0x1;
		return;
	}
	if (Cut_Ctrl.Value)
	{
		Para[DeviceIndex].OUT0 = 1;
		if(!USB1020_SetDeviceDO(m_theApp->m_hDeviceApp[DeviceIndex], USB1020_XAXIS, &Para[DeviceIndex]))
			AfxMessageBox("启动失败!");
	}else{
		Para[DeviceIndex].OUT0 = 0;
		if(!USB1020_SetDeviceDO(m_theApp->m_hDeviceApp[DeviceIndex], USB1020_XAXIS, &Para[DeviceIndex]))
			AfxMessageBox("停止失败!");
	}
}

bool CNEO_InterfaceView::Drive_GJ_Move(int DeviceIndex , int AxisIndex){
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

void CNEO_InterfaceView::ClickShiftSwitch()
{
	MakeInputFiles();
	initial_data(x_angle, x_pluse, x_angle_length, x_pluse_length, x_infile, x_outfile);
	initial_data(y_angle, y_pluse, y_angle_length, y_pluse_length, y_infile, y_outfile);
	initial_data(z_angle, z_pluse, z_angle_length, z_pluse_length, z_infile, z_outfile);

	if(ShiftButton.Value){
		if(CutButton.Value || ResetButton.Value || IsRunSingle()){
			AfxMessageBox("有其他任务正在运行，请关闭重试!");
			ShiftButton.Value ^= 0x1;
		}
		else{
			ShiftFlag = TRUE;
			AfxBeginThread(ShiftThread, LPVOID(this));
		}
	}else{
		ShiftFlag = FALSE;
		StopAllAxis();
	}
}

void CNEO_InterfaceView::ClickCutSwitch()
{
	MakeInputFiles();
	initial_data(x_angle, x_pluse, x_angle_length, x_pluse_length, x_infile, x_outfile);
	initial_data(y_angle, y_pluse, y_angle_length, y_pluse_length, y_infile, y_outfile);
	initial_data(z_angle, z_pluse, z_angle_length, z_pluse_length, z_infile, z_outfile);

	if(CutButton.Value){
		if(ShiftButton.Value || ResetButton.Value || IsRunSingle()){
			AfxMessageBox("有其他任务正在运行，请关闭重试!");
			CutButton.Value ^= 0x1;
		}
		else{
			CutFlag = TRUE;
			AfxBeginThread(CutThread, LPVOID(this));
		}
	}else{
		CutFlag = FALSE;
		StopAllAxis();
	}
}


//[注]：仅用于正确运行完移位和切割过程后的复位操作
//.................否则需要进行手动复位操作
void CNEO_InterfaceView::ClickResetSwitch()
{
	MakeInputFiles();
	initial_data(x_angle, x_pluse, x_angle_length, x_pluse_length, x_infile, x_outfile);
	initial_data(y_angle, y_pluse, y_angle_length, y_pluse_length, y_infile, y_outfile);
	initial_data(z_angle, z_pluse, z_angle_length, z_pluse_length, z_infile, z_outfile);

	if(ResetButton.Value){
		if(ShiftButton.Value || CutButton.Value || IsRunSingle()){
			AfxMessageBox("有其他任务正在运行，请关闭重试!");
			ResetButton.Value ^= 0x1;
		}
		else{
			ResetFlag = TRUE;
			AfxBeginThread(ResetThread, LPVOID(this));
		}
	}else{
		ResetFlag = FALSE;
		StopAllAxis();
	}
}

void CNEO_InterfaceView::OnBnClickedClear()
{
	for(int k=0; k<TotelDeviceNum; k++){
		if(INVALID_HANDLE_VALUE != m_theApp->m_hDeviceApp[k]){
			USB1020_SetLP(m_theApp->m_hDeviceApp[k], USB1020_ALLAXIS, 0); // 设置逻辑位置计数器
			USB1020_SetEP(m_theApp->m_hDeviceApp[k], USB1020_ALLAXIS,	0);	// 设置实位计数器 		 	
			USB1020_SetAccofst(m_theApp->m_hDeviceApp[k], USB1020_ALLAXIS, 0);	// 设置加速计数器偏移
		}
	}
}

bool CNEO_InterfaceView::IsRunAuto(){
	if(ShiftButton.Value || CutButton.Value || ResetButton.Value)
		return TRUE;
	return FALSE;
}

bool CNEO_InterfaceView::IsRunSingle(){
	if(Gj11_Control.Value || Gj12_Control.Value || Gj13_Control.Value || Gj14_Control.Value
		|| Gj21_Control.Value || Gj22_Control.Value || Gj23_Control.Value || Gj24_Control.Value
		|| Gj31_Control.Value || Gj32_Control.Value || Gj33_Control.Value || Gj34_Control.Value)
		return TRUE;
	return FALSE;
}

void CNEO_InterfaceView::UpdateGJState(int DeviceIndex){
	USB1020_GetRR0Status(m_theApp->m_hDeviceApp[DeviceIndex], &(RR0[DeviceIndex]));
	switch(DeviceIndex){
		case 0:
			Gj11_State.Value = bool(RR0[DeviceIndex].XDRV);
			ReviseCtrlButtonValue(Gj11_Control, DeviceIndex, 0, Gj11_State.Value);
			Gj12_State.Value = bool(RR0[DeviceIndex].YDRV);
			ReviseCtrlButtonValue(Gj12_Control, DeviceIndex, 1, Gj12_State.Value);
			Gj13_State.Value = bool(RR0[DeviceIndex].ZDRV);
			ReviseCtrlButtonValue(Gj13_Control, DeviceIndex, 2, Gj13_State.Value);
			Gj14_State.Value = bool(Para[DeviceIndex].OUT0);
			break;
		case 1:
			Gj21_State.Value = bool(RR0[DeviceIndex].XDRV);
			ReviseCtrlButtonValue(Gj21_Control, DeviceIndex, 0, Gj21_State.Value);
			Gj22_State.Value = bool(RR0[DeviceIndex].YDRV);
			ReviseCtrlButtonValue(Gj22_Control, DeviceIndex, 1, Gj22_State.Value);
			Gj23_State.Value = bool(RR0[DeviceIndex].ZDRV);
			ReviseCtrlButtonValue(Gj23_Control, DeviceIndex, 2, Gj23_State.Value);
			Gj24_State.Value = bool(Para[DeviceIndex].OUT0);
			break;
		case 2:
			Gj31_State.Value = bool(RR0[DeviceIndex].XDRV);
			ReviseCtrlButtonValue(Gj31_Control, DeviceIndex, 0, Gj31_State.Value);
			Gj32_State.Value = bool(RR0[DeviceIndex].YDRV);
			ReviseCtrlButtonValue(Gj32_Control, DeviceIndex, 1, Gj32_State.Value);
			Gj33_State.Value = bool(RR0[DeviceIndex].ZDRV);
			ReviseCtrlButtonValue(Gj33_Control, DeviceIndex, 2, Gj33_State.Value);
			Gj34_State.Value = bool(Para[DeviceIndex].OUT0);
			break;
		default:
			break;
	}
}

void CNEO_InterfaceView::ReviseCtrlButtonValue(NI::CNiButton &Gj_Ctrl, int DeviceIndex , int AxisIndex, bool value){
	if(value != Gj_Ctrl.Value){
		m_count[DeviceIndex][AxisIndex]++;
		if(ReviseCount == m_count[DeviceIndex][AxisIndex]){
			//保证控制按钮不会自动变为运行状态
			if(TRUE == value){
				m_count[DeviceIndex][AxisIndex] = 0;
			}else{
				Gj_Ctrl.Value = value;
			}
		}
	}else{
		m_count[DeviceIndex][AxisIndex] = 0;
	}
}

void CNEO_InterfaceView::UpdatePos(int DeviceIndex){
	for (int AxisIndex = 0; AxisIndex<AxisNum; AxisIndex++)
		PosInfo[DeviceIndex][AxisIndex] = USB1020_ReadLP(m_theApp->m_hDeviceApp[DeviceIndex], AxisIndex);
}

void CNEO_InterfaceView::RefreshAutoModeLight(){
	CutState_Shift.SetValue(ShiftButton.Value);
	CutState_Cutting.SetValue(CutButton.Value);
	CutState_Reset.SetValue(ResetButton.Value);
}

void CNEO_InterfaceView::RefreshPosShow(){
	CString str;
	if(PULSENUMMODE == ShowStateMode.GetCurSel()){
		str.Format("%d", PosInfo[0][0]);
		Gj11_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[0][1]);
		Gj12_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[0][2]);
		Gj13_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[1][0]);
		Gj21_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[1][1]);
		Gj22_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[1][2]);
		Gj23_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[2][0]);
		Gj31_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[2][1]);
		Gj32_AngleShow.SetWindowText(str);
		str.Format("%d", PosInfo[2][2]);
		Gj33_AngleShow.SetWindowText(str);
	}
	if(ANGLEMODE == ShowStateMode.GetCurSel()){
		str.Format("%0.2f", ratio_k * PosInfo[0][0]);
		Gj11_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[0][1]);
		Gj12_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[0][2]);
		Gj13_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[1][0]);
		Gj21_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[1][1]);
		Gj22_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[1][2]);
		Gj23_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[2][0]);
		Gj31_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[2][1]);
		Gj32_AngleShow.SetWindowText(str);
		str.Format("%0.2f", ratio_k * PosInfo[2][2]);
		Gj33_AngleShow.SetWindowText(str);
	}
}

void CNEO_InterfaceView::StopAllAxis(){
	for(int k=0; k<TotelDeviceNum; k++){
		if(INVALID_HANDLE_VALUE != m_theApp->m_hDeviceApp[k]){
			USB1020_InstStop(m_theApp->m_hDeviceApp[k], USB1020_ALLAXIS);
		}
	}
}

void CNEO_InterfaceView::MakeInputFiles(){
	switch(CutAngle.GetCurSel()){
		case ANGLE45:
			x_infile.open("InFiles/data45_2cm/x_in.txt");
			y_infile.open("InFiles/data45_2cm/y_in.txt");
			z_infile.open("InFiles/data45_2cm/z_in.txt");
			break;
		case ANGLE60:
			x_infile.open("InFiles/data60_2cm/x_in.txt");
			y_infile.open("InFiles/data60_2cm/y_in.txt");
			z_infile.open("InFiles/data60_2cm/z_in.txt");
			break;
		case ANGLE75:
			x_infile.open("InFiles/data75_2cm/x_in.txt");
			y_infile.open("InFiles/data75_2cm/y_in.txt");
			z_infile.open("InFiles/data75_2cm/z_in.txt");
			break;
		default:
			break;
	}
}

void CNEO_InterfaceView::RefreshAutoModeCtrlButton(){
	if(ShiftFlag != ShiftButton.Value){
		auto_count[0]++;
		if(ReviseCount == auto_count[0]){
			ShiftButton.Value = ShiftFlag;
		}
	}else{
		auto_count[0] = 0;
	}
	if(CutFlag != CutButton.Value){
		auto_count[1]++;
		if(ReviseCount * 2 == auto_count[1]){
			CutButton.Value = CutFlag;
		}
	}else{
		auto_count[1] = 0;
	}
	if(ResetFlag != ResetButton.Value){
		auto_count[2]++;
		if(ReviseCount * 2 == auto_count[2]){
			ResetButton.Value = ResetFlag;
		}
	}else{
		auto_count[2] = 0;
	}
}