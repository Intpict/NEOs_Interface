#include "stdafx.h"
#include "NEOs_Interface.h"
#include "MainFrm.h"

#include "CNEOsFrm.h"
#include "NEOs_InterfaceDoc.h"
#include "NEO_InterfaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CNEOs_InterfaceApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CNEOs_InterfaceApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_CreateIntFace, &CNEOs_InterfaceApp::OnCreateintface)
END_MESSAGE_MAP()


// CNEOs_InterfaceApp 构造

CNEOs_InterfaceApp::CNEOs_InterfaceApp()
{
}

// 唯一的一个 CNEOs_InterfaceApp 对象

CNEOs_InterfaceApp theApp;

// CNEOs_InterfaceApp 初始化

BOOL CNEOs_InterfaceApp::InitInstance()
{
	//**************************************************************************************
	HANDLE hDeviceTemp[TotelDeviceNum]; 
	LONG DeviceLgcID[TotelDeviceNum], DevicePhysID[TotelDeviceNum];
	for(int i=0;i<TotelDeviceNum;i++){
		hDeviceTemp[i]=INVALID_HANDLE_VALUE;	//初始化设备句柄，对应逻辑ID号
		m_hDeviceApp[i]=INVALID_HANDLE_VALUE;  //初始化设备句柄，对应物理ID号
		Device_link_state[i]=FALSE;  //初始化设备状态标志，flase代表未连接
	}
	hDeviceTemp[0]=USB1020_CreateDevice(0);
	Device_Num=USB1020_GetDeviceCount(hDeviceTemp[0]);

	USB1020_GetDeviceCurrentID(hDeviceTemp[0], &DeviceLgcID[0], &DevicePhysID[0]);
	
	//获取有效的设备句柄
	for (int i=1;i<Device_Num;i++)
	{
		hDeviceTemp[i]=USB1020_CreateDevice(i);
		USB1020_GetDeviceCurrentID(hDeviceTemp[i], &DeviceLgcID[i], &DevicePhysID[i]);
	}

	for (int i=0;i<Device_Num;i++)
	{
		switch (DevicePhysID[i]){
			case USB1020_1:                
				m_hDeviceApp[0]=hDeviceTemp[i];
				Device_link_state[0]=TRUE;
				break;
			case USB1020_2:
				m_hDeviceApp[1]=hDeviceTemp[i];
				Device_link_state[1]=TRUE;
				break;
			case USB1020_3:
				m_hDeviceApp[2]=hDeviceTemp[i];
				Device_link_state[2]=TRUE;
				break;
			default:
				break;
		}
	}

	for(int k=0; k<TotelDeviceNum; k++){
		if(INVALID_HANDLE_VALUE != m_hDeviceApp[k]){
			USB1020_SetLP(m_hDeviceApp[k], USB1020_ALLAXIS, 0); // 设置逻辑位置计数器
			USB1020_SetEP(m_hDeviceApp[k], USB1020_ALLAXIS,	0);	// 设置实位计数器 		 	
			USB1020_SetAccofst(m_hDeviceApp[k], USB1020_ALLAXIS, 0);	// 设置加速计数器偏移
		}
	}

	//**************************************************************************************
	int Len = GetSystemMetrics(SM_CXSCREEN);  // 取得屏幕宽度
	if(Len<1024) // 如果屏幕宽度大小小于1024，则**
	{
		if(AfxMessageBox("请最好使用1024*768或以上的显示器分辨率，继续吗？",MB_ICONWARNING|MB_YESNO,0)==IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}

	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	SetRegistryKey(_T("小行星采样器附着力控制应用程序"));
	LoadStdProfileSettings();  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	
	pDocTemplate = new CMultiDocTemplate(IDR_NEOs_InterfaceTYPE,
		RUNTIME_CLASS(CNEOs_InterfaceDoc),
		RUNTIME_CLASS(CNEOsFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CNEO_InterfaceView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	m_pMainWnd->DragAcceptFiles();
    ::SetProp(m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);
	
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CString MainFrmName; WCHAR str[100];
	swprintf_s(str, L"小行星取样器系统-关节腿-连接/总(%d/%d)", Device_Num, TotelDeviceNum);
	MainFrmName = str;
	pMainFrame->SetTitle(MainFrmName);

	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
   OnCreateintface();
	return TRUE;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CNEOs_InterfaceApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

int CNEOs_InterfaceApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	for(int i=0; i<TotelDeviceNum; i++){
		if(m_hDeviceApp[i] != INVALID_HANDLE_VALUE) 
			USB1020_ReleaseDevice(m_hDeviceApp);	
	}
	return CWinApp::ExitInstance();
}

void CNEOs_InterfaceApp::OnCreateintface()
{
	// TODO: 在此添加命令处理程序代码
	CDocument *pDoc;
	pDoc =  pDocTemplate->CreateNewDocument();
	m_NIfDoc = (CNEOs_InterfaceDoc*)pDoc;
	m_NIfFrm = (CNEOsFrame*)pDocTemplate->CreateNewFrame(m_NIfDoc, NULL);
	pDocTemplate->InitialUpdateFrame(m_NIfFrm, m_NIfDoc, TRUE);
	pDoc->SetTitle("系统控制界面");
}