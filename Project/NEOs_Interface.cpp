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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_CreateIntFace, &CNEOs_InterfaceApp::OnCreateintface)
END_MESSAGE_MAP()


// CNEOs_InterfaceApp ����

CNEOs_InterfaceApp::CNEOs_InterfaceApp()
{
}

// Ψһ��һ�� CNEOs_InterfaceApp ����

CNEOs_InterfaceApp theApp;

// CNEOs_InterfaceApp ��ʼ��

BOOL CNEOs_InterfaceApp::InitInstance()
{
	//**************************************************************************************
	HANDLE hDeviceTemp[TotelDeviceNum]; 
	LONG DeviceLgcID[TotelDeviceNum], DevicePhysID[TotelDeviceNum];
	for(int i=0;i<TotelDeviceNum;i++){
		hDeviceTemp[i]=INVALID_HANDLE_VALUE;	//��ʼ���豸�������Ӧ�߼�ID��
		m_hDeviceApp[i]=INVALID_HANDLE_VALUE;  //��ʼ���豸�������Ӧ����ID��
		Device_link_state[i]=FALSE;  //��ʼ���豸״̬��־��flase����δ����
	}
	hDeviceTemp[0]=USB1020_CreateDevice(0);
	Device_Num=USB1020_GetDeviceCount(hDeviceTemp[0]);

	USB1020_GetDeviceCurrentID(hDeviceTemp[0], &DeviceLgcID[0], &DevicePhysID[0]);
	
	//��ȡ��Ч���豸���
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
			USB1020_SetLP(m_hDeviceApp[k], USB1020_ALLAXIS, 0); // �����߼�λ�ü�����
			USB1020_SetEP(m_hDeviceApp[k], USB1020_ALLAXIS,	0);	// ����ʵλ������ 		 	
			USB1020_SetAccofst(m_hDeviceApp[k], USB1020_ALLAXIS, 0);	// ���ü��ټ�����ƫ��
		}
	}

	//**************************************************************************************
	int Len = GetSystemMetrics(SM_CXSCREEN);  // ȡ����Ļ���
	if(Len<1024) // �����Ļ��ȴ�СС��1024����**
	{
		if(AfxMessageBox("�����ʹ��1024*768�����ϵ���ʾ���ֱ��ʣ�������",MB_ICONWARNING|MB_YESNO,0)==IDNO)	
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
	
	SetRegistryKey(_T("С���ǲ���������������Ӧ�ó���"));
	LoadStdProfileSettings();  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	
	pDocTemplate = new CMultiDocTemplate(IDR_NEOs_InterfaceTYPE,
		RUNTIME_CLASS(CNEOs_InterfaceDoc),
		RUNTIME_CLASS(CNEOsFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CNEO_InterfaceView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	m_pMainWnd->DragAcceptFiles();
    ::SetProp(m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);
	
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CString MainFrmName; WCHAR str[100];
	swprintf_s(str, L"С����ȡ����ϵͳ-�ؽ���-����/��(%d/%d)", Device_Num, TotelDeviceNum);
	MainFrmName = str;
	pMainFrame->SetTitle(MainFrmName);

	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
   OnCreateintface();
	return TRUE;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CNEOs_InterfaceApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

int CNEOs_InterfaceApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	for(int i=0; i<TotelDeviceNum; i++){
		if(m_hDeviceApp[i] != INVALID_HANDLE_VALUE) 
			USB1020_ReleaseDevice(m_hDeviceApp);	
	}
	return CWinApp::ExitInstance();
}

void CNEOs_InterfaceApp::OnCreateintface()
{
	// TODO: �ڴ���������������
	CDocument *pDoc;
	pDoc =  pDocTemplate->CreateNewDocument();
	m_NIfDoc = (CNEOs_InterfaceDoc*)pDoc;
	m_NIfFrm = (CNEOsFrame*)pDocTemplate->CreateNewFrame(m_NIfDoc, NULL);
	pDocTemplate->InitialUpdateFrame(m_NIfFrm, m_NIfDoc, TRUE);
	pDoc->SetTitle("ϵͳ���ƽ���");
}