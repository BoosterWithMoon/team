
// PyroSoftM.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//
//#define _WIN32_DCOM

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "PyroSoftM.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "PyroSoftMDoc.h"
#include "GlassFlowView.h"
#include "ResultView.h"
#include "ROIGridView.h"
#include "ScaleView.h"

#include "DataMsgThread.h"
#include "DataSocket.h"

#include "DlgAddNewDevice.h"

//#include "mmsystem.h"
#include <dshow.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ICOUNT	22


//CPyroSoftMApp

BEGIN_MESSAGE_MAP(CPyroSoftMApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CPyroSoftMApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CPyroSoftMApp::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_DEVICE_DO_OPENSIMULATION, &CPyroSoftMApp::OnDeviceDoOpensimulation)
	ON_COMMAND(ID_DEVICE_DO_OPEN, OnDeviceDoOpen)
	//ON_WM_TIMER()
	ON_COMMAND(ID_DEVICE_DO_SCAN, &CPyroSoftMApp::OnDeviceDoScan)
END_MESSAGE_MAP()


// CPyroSoftMApp ����

CPyroSoftMApp::CPyroSoftMApp()
{
	TimerEventID = 1024;

	m_bHiColorIcons = TRUE;

	IRDX_Mode = 1;

	m_ClientTimerID = NULL;
	m_dwTransactionID = 0;
	m_strSingleValue = _T("");
	m_uReadPeriod = 500;          // default value : 500 ms
	m_uReadMode = EventMode;      // default mode : event
	m_strImpExpFileName.Empty();
	theApp.m_bLoggingRunning = false;

	m_Laser_Distance = 0;
	DAQ_TaskHandle = 0;

	nResultDlgWidth = 0;
	nStatusDlgHeight = 0;
	nScaleDlgWidth = 0;
	adjust_count= 0;

	m_childFrmWidth = 0;
	m_childFrmHeight = 0;

	m_StatusInfFlag = false;
	m_isSamsung_dev = false;
	m_FocusFlag = false;
	m_ForcedFocusFlg = false;
	m_TchartFlag = false;
	m_TimerFlag = false;
	m_bFileOpen = false;

	m_RHK_name = "";

	m_bCameraConnected = false;

	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("PyroSoftM.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.	

}

// ������ CPyroSoftMApp ��ü�Դϴ�.

CPyroSoftMApp theApp;

CCriticalSection m_cs;

// CPyroSoftMApp �ʱ�ȭ

BOOL CPyroSoftMApp::InitInstance()
{
	::GetModuleFileName(::AfxGetInstanceHandle(), AppPath, _MAX_PATH);
	PathRemoveFileSpec(AppPath);

	//*******************************************************************	
	m_nNumDetectDevices = 0;

	m_bOpenViaBuffer = FALSE;

	///< clear all use of devices
	for (unsigned long i = 0; i<DDAQ_MAX_DEVICES; i++)
		m_DocWithDevice[i] = NULL;

	// no messaging
	//m_MessagingType = Msg_No;		// default
	//m_MessagingType = Msg_Window;
	m_MessagingType = Msg_Thread;
	//m_MessagingType = Msg_Event;

	
	//*******************************************************************

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.

	SetRegistryKey(_T("PyroSoftM"));

	//CleanState();


	//*******************************************************************	
	/*
	char buf[_MAX_PATH];
	char drive[_MAX_DRIVE]; char dir[_MAX_DIR];
	char fname[_MAX_FNAME]; char ext[_MAX_EXT];
	GetModuleFileName(m_hInstance, buf, _MAX_PATH);
	_splitpath_s(buf, drive, dir, fname, ext);

	free((void*)m_pszProfileName);
	BOOL bEnable = AfxEnableMemoryTracking(FALSE);
	//_makepath(buf, drive, dir, fname, "ini");
	_makepath_s(buf, drive, dir, fname, _T("ini"));
	m_pszProfileName = _tcsdup(buf);
	AfxEnableMemoryTracking(bEnable);

	//*******************************************************************
	*/
	LoadStdProfileSettings(6);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	InitContextMenuManager();

	//InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_PyroLineTYPE,
		RUNTIME_CLASS(CPyroSoftMDoc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CGlassFlowView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �� MDI ������ â�� ����ϴ�.
	//*******************************************************************
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	// ��� ���⿡ ���� ���⸦ Ȱ��ȭ�մϴ�.
	m_pMainWnd->DragAcceptFiles();

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;

	// disabel FileNew
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	ParseCommandLine(cmdInfo);

	// DDE Execute ���⸦ Ȱ��ȭ�մϴ�.
	EnableShellOpen();
	//*******************************************************************

	RegisterShellFileTypes(TRUE);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	pMainFrame->SetWindowText("RHK Thermal Imaging System");

	m_pDataMsgThread = (CDataMsgThread*)AfxBeginThread(RUNTIME_CLASS(CDataMsgThread), THREAD_PRIORITY_NORMAL);

	m_pDataSocket = NULL;

	ConnectionLaserDist();

	Devicedetection();

	return TRUE;
}

void CPyroSoftMApp::OnFileOpen()
{
	// TODO: Add your command handler code here
	// ���� ���� �޺��ڽ��� ����� ���͸� �����Ѵ�. (*.*, *.cpp, *.txt)
	if (m_bCameraConnected || m_bFileOpen) return;

	char name_filter[] = "IRDX Files (*.irdx)|*.irdx||";

	// TRUE -> �����ȭ����, "cpp" -> ����ڰ� Ȯ���� ���� ���ϸ� �Է������� �ڵ����� �߰��� Ȯ���ڸ�
	// ��, stdafx ������ �Է��ϸ� stdafx.cpp��� �Է��ѰͰ� �����ϰ� �ϰ� ������ ���
	// "*.cpp" �����̸� ����Ʈ�� ��µ� �⺻ ���ڿ�
	// OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT -> ���� ��ȭ���ڿ� �߰������� ����� �Ӽ�
	// name_filter -> ���� ���� �޺��ڽ��� ����� ���������� ����ִ� �޸��� �ּ�
	CFileDialog ins_dlg(TRUE, "irdx", "*.irdx",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);

	// ���� ���� �޺��ڽ��� ������ ���͵� �߿��� 2��° �׸�(*.cpp)�� ����
	ins_dlg.m_ofn.nFilterIndex = 2;

	if (ins_dlg.DoModal() == IDOK) {
		// ���õ� ������ ��θ��� �̿��Ͽ� ��ť��Ʈ ������ �籸��

		adjust_count = 0;		// �߰������� ������ Child Frame�� ȭ�� adj�� ����
		OpenDocumentFile(ins_dlg.GetPathName());
	}
	else {
		// ���� "���� ����" ��ɿ����� ���� ���⸦ ������� ��, Ư���� �޽����� ������ ������
		// �������� ȿ���� ��Ÿ���� ���ؼ� ���� ���⸦ ���������, ����ߴٴ� �޽����� ��������
		// �籸�� 
		::MessageBox(NULL, "���� ���⸦ ����Ͽ����ϴ�.", "�˸�", MB_ICONINFORMATION);
	}
}

void CPyroSoftMApp::ConnectionLaserDist()
{
	int error;
	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	error = DAQmxCreateTask("", &DAQ_TaskHandle);

	DAQmxCreateAICurrentChan(DAQ_TaskHandle, "cDAQ1Mod1/ai0", "RHK Laser In", DAQmx_Val_RSE, -0.02, 0.02, DAQmx_Val_Amps,
		DAQmx_Val_Internal, 95.5764, "");
}

int CPyroSoftMApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	DAQ_LASER_DISTANCE_DAQmxClearTask(DAQ_TaskHandle);

	AfxOleTerm(FALSE);
	
	return CWinAppEx::ExitInstance();
}

// CPyroSoftMApp �޽��� ó����

/////////////////////////////////////////////////////////////////////////////
void CPyroSoftMApp::EndDataMsgThread()
{
	if (m_pDataMsgThread)
	{
		m_pDataMsgThread->PostThreadMessage(WM_EndDataMsgThread, 0, 0);
		Sleep(10);
	}
}
/////////////////////////////////////////////////////////////////////////////

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

														// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CPyroSoftMApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CPyroSoftMApp ����� ���� �ε�/���� �޼���

void CPyroSoftMApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CPyroSoftMApp::LoadCustomState()
{
}

void CPyroSoftMApp::SaveCustomState()
{
}

// CPyroSoftMApp �޽��� ó����

BOOL CPyroSoftMApp::OnIdle(LONG lCount)
{

	DWORD ret = CWinApp::OnIdle(lCount);

	// if polling or event signal, proceed update data in idle!
	if ((m_MessagingType == Msg_No) || (m_MessagingType == Msg_Event))
	{
		// call doc-template idle hook
		POSITION pos = NULL;
		if (m_pDocManager != NULL)
			pos = m_pDocManager->GetFirstDocTemplatePosition();

		while (pos != NULL)
		{
			CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
			ASSERT_KINDOF(CDocTemplate, pTemplate);
			pTemplate->OnIdle();
		}

		// prevent high CPU load
		Sleep(10);

		// need idle time again!
		return TRUE;
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////////////

void CPyroSoftMApp::DoErrorMessage(UINT ids, unsigned long nErrorCode, BOOL with_box)
{
	CString s;
	s.LoadString(ids);
	if (nErrorCode != 0)
	{
		CString s2;
		s2.Format(IDS_ERROR_NO, nErrorCode);
		s += '\n';
		s += s2;
	}
	TRACE0(s + "\r\n");

	if (with_box)
		AfxMessageBox(s);
}

/////////////////////////////////////////////////////////////////////////////

CPyroSoftMDoc* CPyroSoftMApp::GetDocumentFromDevice(unsigned short DevNo)
{
	DevNo--;	// Index from 0

	if (DevNo >= DDAQ_MAX_DEVICES)
		return NULL;

	return m_DocWithDevice[DevNo];
}

/////////////////////////////////////////////////////////////////////////////

TMessagingType CPyroSoftMApp::GetMessagingType()
{
	return m_MessagingType;
};

/////////////////////////////////////////////////////////////////////////////

DWORD CPyroSoftMApp::GetMessageThreadID()
{
	if (m_pDataMsgThread == NULL)
		return 0;

	return m_pDataMsgThread->m_nThreadID;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// DDAQ_DEVICE functions
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CPyroSoftMApp::DDAQ_DEVICE_DO_Open(unsigned long nDevNo, const char* pFileName)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_OPEN(nDevNo, (char*)pFileName);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_OPEN, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_OpenSimulation(unsigned long nDevNo, const char* pFileName)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_OPENSIMULATION(nDevNo, (char*)pFileName);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_OPENSIMULATION, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_Close(unsigned long nDevNo)
{
	// document for device 
	m_DocWithDevice[nDevNo - 1] = NULL;

	DDAQ_U32 ret = DDAQ_DEVICE_DO_CLOSE(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_CLOSE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_Start(unsigned long nDevNo)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_START(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_START, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_StartSingleShot(unsigned long nDevNo)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_STARTSINGLESHOT(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_START, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_Stop(unsigned long nDevNo)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_STOP(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_STOP, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_UpdateData(unsigned long nDevNo)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_UPDATEDATA(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_UPDATEDATA, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_DownloadImage(unsigned long nDevNo, unsigned short ImageIdx, const char* pFileName)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_DOWNLOADIMAGE(nDevNo, ImageIdx, (char*)pFileName);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_DOWNLOADIMAGE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_DeleteImages(unsigned long nDevNo)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_DELETEIMAGES(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_DELETEIMAGES, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_DO_MotorFocus(unsigned long nDevNo, unsigned short Cmd)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_MOTORFOCUS(nDevNo, Cmd);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_DO_MOTORFOCUS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_Id(unsigned long nDevNo, unsigned long* pID, unsigned long* pType)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_ID(nDevNo, pID, pType);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_ID, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_IDString(unsigned long nDevNo, CString& s)
{
	s.Empty();
	DDAQ_U32	ret = DDAQ_DEVICE_GET_IDSTRING(nDevNo, s.GetBuffer(64), 64);
	s.ReleaseBuffer();
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_IDSTRING, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_InterfaceString(unsigned long nDevNo, CString& s)
{
	s.Empty();
	DDAQ_U32	ret = DDAQ_DEVICE_GET_INTERFACESTRING(nDevNo, s.GetBuffer(1024), 1024);
	s.ReleaseBuffer();
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_INTERFACESTRING, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_IRDXHandle(unsigned long nDevNo, HANDLE* pH)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_IRDX(nDevNo, pH);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_IRDX, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_NewDataReady(unsigned long nDevNo, bool* pbool)
{
	DDAQ_U32 ret = DDAQ_DEVICE_GET_NEWDATAREADY(nDevNo, pbool);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_NEWDATAREADY, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_UpdateFrequency(unsigned long nDevNo, float* pFrequ)
{
	DDAQ_U32 ret = DDAQ_DEVICE_GET_UPDATEFREQUENCY(nDevNo, pFrequ);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_UPDATEFREQUENCY, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_NumMRanges(unsigned long nDevNo, unsigned short* pNum)
{
	DDAQ_U32 ret = DDAQ_DEVICE_GET_NUMMRANGES(nDevNo, pNum);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_NUMMRANGES, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_MRInfoString(unsigned long nDevNo, unsigned short mr, CString& s)
{
	DDAQ_U32 ret = DDAQ_DEVICE_GET_MRANGESTRING(nDevNo, mr, s.GetBuffer(128), 128);
	s.ReleaseBuffer();
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_MRINFOSTRING, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_ErrorState(unsigned long nDevNo, unsigned long* pBuf, unsigned long nBufSize)
{
	DDAQ_U32 ret = DDAQ_DEVICE_GET_ERRORS(nDevNo, pBuf, nBufSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_ERRORS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_Din(unsigned long nDevNo, unsigned long* pDIN)
{
	DDAQ_U32 ret = DDAQ_DEVICE_GET_DINOUT(nDevNo, pDIN);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_DIN, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_CameraMode(unsigned long nDevNo, bool* pOK)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_CAMERAMODEOK(nDevNo, pOK);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_OPERATIONMODE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_DetectorTemp(unsigned long nDevNo, float* pTemp, bool* pOK)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_DETECTORTEMP(nDevNo, pTemp, pOK);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_DETECTORTEMP, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_CameraTemp(unsigned long nDevNo, float* pTemp, bool* pOK)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_CAMERATEMP(nDevNo, pTemp, pOK);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_CAMERATEMP, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_GET_NumImages(unsigned long nDevNo, unsigned short* pNumImages)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_NUMIMAGES(nDevNo, pNumImages);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_NUMIMAGES, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_SET_Dout(unsigned long nDevNo, unsigned long DOUT)
{
	DDAQ_U32 ret = DDAQ_DEVICE_SET_DOUT(nDevNo, DOUT);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_SET_DOUT, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_SET_MsgHWND(unsigned long nDevNo, HWND hWnd)
{
	DDAQ_U32 ret = DDAQ_DEVICE_SET_MSGHWND(nDevNo, hWnd);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_SET_MSGHWND, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_SET_MsgThreadID(unsigned long nDevNo, unsigned long nThreadID)
{
	DDAQ_U32 ret = DDAQ_DEVICE_SET_MSGTHREAD(nDevNo, nThreadID);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_SET_MSGTHREADID, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_DEVICE_SET_MsgHEvent(unsigned long nDevNo, HANDLE hEvent)
{
	DDAQ_U32 ret = DDAQ_DEVICE_SET_MSGHEVENT(nDevNo, hEvent);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_SET_MSGHEVENT, ret);
		return false;
	}
	return true;
}

bool CPyroSoftMApp::DDAQ_DEVICE_DO_ENABLE_NextMsg(unsigned long nDevNo)
{
	DDAQ_U32 ret = DDAQ_DEVICE_DO_ENABLE_NEXTMSG(nDevNo);
	if (ret != DDAQ_NO_ERROR)
	{
		//		DoErrorMessage(IDS_ERROR_DEVICE_SET_MSGHEVENT, ret);
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// DDAQ_IRDX functions
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CPyroSoftMApp::DDAQ_IRDX_FILE_OpenMem(HANDLE* phIRDX)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_OPEN_MEM(phIRDX);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_MEM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_OpenRead(const char* pFileName, HANDLE* phIRDX)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_OPEN_READ((char*)pFileName, true, phIRDX);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_OPENREAD, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_OpenReadWrite(const char* pFileName, bool append, HANDLE* phIRDX)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_OPEN_READWRITE((char*)pFileName, append, phIRDX);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_OPENREAD, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_OpenWrite(const char* pFileName, bool append, HANDLE* phIRDX)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_OPEN_WRITE((char*)pFileName, append, phIRDX);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_OPENWRITE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_Close(HANDLE hIRDX)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_CLOSE(hIRDX);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_CLOSE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_Save(HANDLE hIRDX, const char* pFileName)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_SAVE(hIRDX, (char*)pFileName);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_SAVE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_AddIRDX(HANDLE hIRDX, HANDLE hIRDX_add)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_ADD_IRDX(hIRDX, hIRDX_add);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_ADDIRDX, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_GetNumDataSets(HANDLE hIRDX, unsigned long* pNumDS)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_GET_NUMDATASETS(hIRDX, pNumDS);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_GET_NUMDS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_GetCurDataSet(HANDLE hIRDX, unsigned long* pDS)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_GET_CURDATASET(hIRDX, pDS);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_GET_CURDS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_SetCurDataSet(HANDLE hIRDX, unsigned long DS)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_SET_CURDATASET(hIRDX, DS);
	if (ret != DDAQ_NO_ERROR)
	{
		//DoErrorMessage(IDS_ERROR_IRDX_FILE_SET_CURDS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

bool CPyroSoftMApp::DDAQ_IRDX_FILE_GetModified(HANDLE hIRDX, bool* pModif)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_GET_MODIFIED(hIRDX, pModif);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_GET_MODIFIED, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_FILE_RemoveDataSet(HANDLE hIRDX, unsigned long DS)
{
	DDAQ_U32	ret = DDAQ_IRDX_FILE_REMOVE_DATASET(hIRDX, DS);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_FILE_REMOVE_DATASET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetID(HANDLE hIRDX, unsigned long* pID, unsigned long* pType)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_ID(hIRDX, pID, pType);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_ID, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetIDString(HANDLE hIRDX, CString& s)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_IDSTRING(hIRDX, s.GetBuffer(64), 64);
	s.ReleaseBuffer();
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_IDSTRING, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetRangeMinMax(HANDLE hIRDX, float* pMin, float* pMax)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_MRANGEMINMAX(hIRDX, pMin, pMax);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_DEVICE_GET_RANGEMINMAX, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetInternalFPS(HANDLE hIRDX, float* pFps)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_INTERNALFPS(hIRDX, pFps);
	if (ret != DDAQ_NO_ERROR)
	{
		// without message box!
		DoErrorMessage(IDS_ERROR_DEVICE_GET_INTERNALFPS, ret, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetCameraMode(HANDLE hIRDX, bool* pOK)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_CAMERAMODEOK(hIRDX, pOK);
	if (ret != DDAQ_NO_ERROR)
	{
		// without message box!
		DoErrorMessage(IDS_ERROR_DEVICE_GET_OPERATIONMODE, ret, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetDetectorTemp(HANDLE hIRDX, float* pTemp, bool* pOK)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_DETECTORTEMP(hIRDX, pTemp, pOK);
	if (ret != DDAQ_NO_ERROR)
	{
		// without message box!
		DoErrorMessage(IDS_ERROR_DEVICE_GET_DETECTORTEMP, ret, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_DEVICE_GetCameraTemp(HANDLE hIRDX, float* pTemp, bool* pOK)
{
	DDAQ_U32	ret = DDAQ_IRDX_DEVICE_GET_CAMERATEMP(hIRDX, pTemp, pOK);
	if (ret != DDAQ_NO_ERROR)
	{
		// without message box!
		DoErrorMessage(IDS_ERROR_DEVICE_GET_CAMERATEMP, ret, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_GetEmissivity(HANDLE hIRDX, float* pEmiss)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_GET_EMISSIVITY(hIRDX, pEmiss);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_GET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_SetEmissivity(HANDLE hIRDX, float Emiss)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_SET_EMISSIVITY(hIRDX, Emiss);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_SET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_GetTransmission(HANDLE hIRDX, float* pTrans)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_GET_TRANSMISSION(hIRDX, pTrans);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_GET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_SetTransmission(HANDLE hIRDX, float Trans)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_SET_TRANSMISSION(hIRDX, Trans);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_SET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_GetAmbientTemp(HANDLE hIRDX, float* pAmbientTemp)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_GET_AMBIENTTEMP(hIRDX, pAmbientTemp);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_GET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_SetAmbientTemp(HANDLE hIRDX, float AmbientTemp)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_SET_AMBIENTTEMP(hIRDX, AmbientTemp);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_SET, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_GetAmbientTempCorr(HANDLE hIRDX, unsigned short* pmode, float* poffs)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_GET_AMBIENTTEMP_CORR(hIRDX, pmode, poffs);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_GET_AMBIENTCORR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_SetAmbientTempCorr(HANDLE hIRDX, unsigned short mode, float offs)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_SET_AMBIENTTEMP_CORR(hIRDX, mode, offs);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_SET_AMBIENTCORR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_GetTransmissionCorr(HANDLE hIRDX, unsigned short* pCorrMode, float* pRefTemp, float* pRefEmmis, unsigned short* pLeft, unsigned short* pTop, unsigned short* pRight, unsigned short* pBottom, float* pWeight, float* pMinTransm)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_GET_TRANSMISSION_CORR(hIRDX, pCorrMode, pRefTemp, pRefEmmis, pLeft, pTop, pRight, pBottom, pWeight, pMinTransm);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_GET_TRANSMISSION_CORR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_SetTransmissionCorr(HANDLE hIRDX, unsigned short CorrMode, float RefTemp, float RefEmmis, unsigned short Left, unsigned short Top, unsigned short Right, unsigned short Bottom, float Weight, float MinTransm)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_SET_TRANSMISSION_CORR(hIRDX, CorrMode, RefTemp, RefEmmis, Left, Top, Right, Bottom, Weight, MinTransm);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_SET_TRANSMISSION_CORR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_DoTransmissionCorr(HANDLE hIRDX, float* pCalcTransm, unsigned short* pValidity)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_DO_TRANSMISSION_CORR(hIRDX, pCalcTransm, pValidity);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_DO_TRANSMISSION_CORR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_OBJECT_GetTransmissionCorrState(HANDLE hIRDX, float* pCalcTransm, unsigned short* pValidity)
{
	DDAQ_U32	ret = DDAQ_IRDX_OBJECT_GET_TRANSMISSION_CORR_STATE(hIRDX, pCalcTransm, pValidity);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_OBJECT_GET_TRANSMISSION_CORR_STATE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetTimeStampString(HANDLE hIRDX, CString& date, CString& time)
{
	long	year, month, day, hour, min, sec, msec;
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_TIMESTAMP(hIRDX, &year, &month, &day, &hour, &min, &sec, &msec);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_TIMESTAMP, ret);
		return false;
	}
	//s.Format("%.2i/%.2i/%i %.2i:%.2i:%.2i.%.3i", month, day, year, hour, min, sec, msec);	
	date.Format("%i-%.2i-%.2i", year, month, day);
	time.Format("%.2i:%.2i:%.2i.%.3i", hour, min, sec, msec);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetAveraging(HANDLE hIRDX, unsigned short* pav)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_AVERAGING(hIRDX, pav);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_AVERAGING, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetAveraging(HANDLE hIRDX, unsigned short av)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_AVERAGING(hIRDX, av);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_AVERAGING, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetMeasurementRange(HANDLE hIRDX, unsigned short* pmr)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_MRANGE(hIRDX, pmr);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_MEASUREMENTRANGE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetMeasurementRange(HANDLE hIRDX, unsigned short mr)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_MRANGE(hIRDX, mr);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_MEASUREMENTRANGE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetMRInfoString(HANDLE hIRDX, CString& s)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_MRANGESTRING(hIRDX, s.GetBuffer(64), 64);
	s.ReleaseBuffer();
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_MRString, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetNumFrames(HANDLE hIRDX, unsigned short* pNumFrames)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_NUMFRAMES(hIRDX, pNumFrames);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_NUMFRAMES, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetNumFrames(HANDLE hIRDX, unsigned short NumFrames)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_NUMFRAMES(hIRDX, NumFrames);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_NUMFRAMES, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetValidFrames(HANDLE hIRDX, unsigned short* pValidFrames)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_VALIDFRAMES(hIRDX, pValidFrames);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_VALIDFRAMES, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetSingleTrigger(HANDLE hIRDX, unsigned short* ptrig)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_SINGLETRIGGER(hIRDX, ptrig);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_SINGLETRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetSingleTrigger(HANDLE hIRDX, unsigned short trig)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_SINGLETRIGGER(hIRDX, trig);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_SINGLETRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetSequenceTrigger(HANDLE hIRDX, unsigned short* ptrig)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_SEQUENCETRIGGER(hIRDX, ptrig);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_SEQUENCETRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetSequenceTrigger(HANDLE hIRDX, unsigned short trig)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_SEQUENCETRIGGER(hIRDX, trig);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_SEQUENCETRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetTempSequenceTrigger(DDAQ_HANDLE hIRDX, DDAQ_P_F32 pThreshold, DDAQ_P_U16 pMode)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_TEMPSEQUENCETRIGGER(hIRDX, pThreshold, pMode);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_TEMPSEQUENCETRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetTempSequenceTrigger(DDAQ_HANDLE hIRDX, DDAQ_F32 Threshold, DDAQ_U16 Mode)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_TEMPSEQUENCETRIGGER(hIRDX, Threshold, Mode);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_TEMPSEQUENCETRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetShutterTrigger(HANDLE hIRDX, unsigned short* ptrig)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_SHUTTERTRIGGER(hIRDX, ptrig);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_GET_SHUTTERTRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetShutterTrigger(HANDLE hIRDX, unsigned short trig)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_SHUTTERTRIGGER(hIRDX, trig);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_ACQUISITION_SET_SHUTTERTRIGGER, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_SCALE_GetMinMax(HANDLE hIRDX, float* pMin, float* pMax)
{
	DDAQ_U32	ret = DDAQ_IRDX_SCALE_GET_MINMAX(hIRDX, pMin, pMax);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_SCALE_GET_MINMAX, ret);
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_SCALE_SetMinMax(HANDLE hIRDX, float Min, float Max)
{
	DDAQ_U32	ret = DDAQ_IRDX_SCALE_SET_MINMAX(hIRDX, Min, Max);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_SCALE_SET_MINMAX, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_SCALE_GetAutoMode(HANDLE hIRDX, unsigned short* pMode, float* pAVPlus, float* pAVMinus)
{
	DDAQ_U32	ret = DDAQ_IRDX_SCALE_GET_AUTOMODE(hIRDX, pMode, pAVPlus, pAVMinus);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_SCALE_GET_AUTOMODE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_SCALE_SetAutoMode(HANDLE hIRDX, unsigned short Mode, float AVPlus, float AVMinus)
{
	DDAQ_U32	ret = DDAQ_IRDX_SCALE_SET_AUTOMODE(hIRDX, Mode, AVPlus, AVMinus);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_SCALE_SET_AUTOMODE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_SCALE_GetStep(HANDLE hIRDX, float* pStep)
{
	DDAQ_U32	ret = DDAQ_IRDX_SCALE_GET_STEP(hIRDX, pStep);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_SCALE_GET_STEP, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_SCALE_SetStep(HANDLE hIRDX, float Step)
{
	DDAQ_U32	ret = DDAQ_IRDX_SCALE_SET_STEP(hIRDX, Step);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_SCALE_SET_STEP, ret);
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetBar(HANDLE hIRDX, unsigned short* pBar, unsigned short* pNumCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_BAR(hIRDX, pBar, pNumCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_BAR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_SetBar(HANDLE hIRDX, unsigned short Bar, unsigned short NumCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_SET_BAR(hIRDX, Bar, NumCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_SET_BAR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetBackgroundColor(HANDLE hIRDX, unsigned long* pBkCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_BACKGROUNDCOL(hIRDX, pBkCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_BACKGROUNDCOL, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_SetBackgroundColor(HANDLE hIRDX, unsigned long BkCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_SET_BACKGROUNDCOL(hIRDX, BkCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_SET_BACKGROUNDCOL, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetTextColor(HANDLE hIRDX, unsigned long* pTxtCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_TEXTCOL(hIRDX, pTxtCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_TEXTCOL, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_SetTextColor(HANDLE hIRDX, unsigned long TxtCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_SET_TEXTCOL(hIRDX, TxtCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_SET_TEXTCOL, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetNumIsotherms(HANDLE hIRDX, unsigned short* pNumIsoth)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_NUMISOTHERMS(hIRDX, pNumIsoth);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_NUMISOTHERMS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_SetNumIsotherms(HANDLE hIRDX, unsigned short NumIsoth)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_SET_NUMISOTHERMS(hIRDX, NumIsoth);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_SET_NUMISOTHERMS, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetIsotherm(HANDLE hIRDX, unsigned short nNo, float* pMin, float* pMax, unsigned long* pCol, unsigned short* pSat, bool* pTransp)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_ISOTHERM(hIRDX, nNo, pMin, pMax, pCol, pSat, pTransp);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_ISOTHERM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_SetIsotherm(HANDLE hIRDX, unsigned short nNo, float Min, float Max, unsigned long Col, unsigned short Sat, bool Transp)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_SET_ISOTHERM(hIRDX, nNo, Min, Max, Col, Sat, Transp);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_SET_ISOTHERM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetBitmap(HANDLE hIRDX, unsigned short Width, unsigned short Height, void** ppBits, BITMAPINFO** ppBitmapInfo)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_BITMAP(hIRDX, Width, Height, ppBits, ppBitmapInfo);
	if (ret != DDAQ_NO_ERROR)
	{
		//		No error message box!
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_BITMAP, ret, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_SetTempPrecision(unsigned short precision)
{
	DDAQ_U32   ret = DDAQ_SET_TEMPPRECISION(precision);
	if (ret != DDAQ_NO_ERROR)
	{
		//DoErrorMessage(IDS_ERROR_IRDX_PALLET_SET_BAR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetBitmapScale(HANDLE hIRDX, unsigned short Width, unsigned short Height, void** ppBits, BITMAPINFO** ppBitmapInfo)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_BITMAPSCALE(hIRDX, Width, Height, ppBits, ppBitmapInfo);
	if (ret != DDAQ_NO_ERROR)
	{
		//		No error message box!
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_BITMAPSCALE, ret, FALSE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetBitmapScaleEx(HANDLE hIRDX, unsigned short Width, unsigned short Height, void** ppBits, BITMAPINFO** ppBitmapInfo, long* pparam, unsigned long size)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_BITMAPSCALEEX(hIRDX, Width, Height, ppBits, ppBitmapInfo, pparam, size);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_BITMAPSCALE, ret, TRUE);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PALLET_GetBarColor(HANDLE hIRDX, unsigned short Idx, unsigned long* pCol)
{
	DDAQ_U32	ret = DDAQ_IRDX_PALLET_GET_BARCOL(hIRDX, Idx, pCol);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PALLET_GET_COLOR, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_IMAGE_GetSize(HANDLE hIRDX, unsigned short* pSizeX, unsigned short* pSizeY)
{
	DDAQ_U32	ret = DDAQ_IRDX_IMAGE_GET_SIZE(hIRDX, pSizeX, pSizeY);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_IMAGE_GET_SIZE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_IMAGE_GetZoom(HANDLE hIRDX, unsigned short* pZMode, float* pZoom)
{
	DDAQ_U32	ret = DDAQ_IRDX_IMAGE_GET_ZOOM(hIRDX, pZMode, pZoom);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_IMAGE_GET_ZOOM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_IMAGE_SetZoom(HANDLE hIRDX, unsigned short ZMode, float Zoom)
{
	DDAQ_U32	ret = DDAQ_IRDX_IMAGE_SET_ZOOM(hIRDX, ZMode, Zoom);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_IMAGE_SET_ZOOM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_IMAGE_GetDrawMode(HANDLE hIRDX, unsigned short* pDrawMode)
{
	DDAQ_U32	ret = DDAQ_IRDX_IMAGE_GET_DRAWMODE(hIRDX, pDrawMode);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_IMAGE_GET_DRAWMODE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_IMAGE_SetDrawMode(HANDLE hIRDX, unsigned short DrawMode)
{
	DDAQ_U32	ret = DDAQ_IRDX_IMAGE_SET_DRAWMODE(hIRDX, DrawMode);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_IMAGE_SET_ZOOM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_IMAGE_GetBitmap(HANDLE hIRDX, unsigned short* pWidth, unsigned short* pHeight, void** ppBits, BITMAPINFO** ppBitmapInfo)
{
	DDAQ_U32	ret = DDAQ_IRDX_IMAGE_GET_BITMAP(hIRDX, pWidth, pHeight, ppBits, ppBitmapInfo);
	if (ret != DDAQ_NO_ERROR)
	{
		//		No error message while drawing!!
		MessageBeep(-1);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PIXEL_GetSize(HANDLE hIRDX, unsigned short* pSizeX, unsigned short* pSizeY)
{
	DDAQ_U32	ret = DDAQ_IRDX_PIXEL_GET_SIZE(hIRDX, pSizeX, pSizeY);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PIXEL_GET_SIZE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PIXEL_GetMinMax(HANDLE hIRDX, float* pMin, float* pMax)
{
	DDAQ_U32	ret = DDAQ_IRDX_PIXEL_GET_MINMAX(hIRDX, pMin, pMax);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PIXEL_GET_MINMAX, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PIXEL_GetAverage(HANDLE hIRDX, float* pAV)
{
	DDAQ_U32	ret = DDAQ_IRDX_PIXEL_GET_AVERAGE(hIRDX, pAV);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PIXEL_GET_AVERAGE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PIXEL_GetAVStdDev(HANDLE hIRDX, float* pAV, float* pStdDev)
{
	DDAQ_U32	ret = DDAQ_IRDX_PIXEL_GET_AVSTDDEV(hIRDX, pAV, pStdDev);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PIXEL_GET_AVSTDDEV, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PIXEL_GetData(HANDLE hIRDX, float* pData, unsigned long BufSize)
{
	DDAQ_U32	ret = DDAQ_IRDX_PIXEL_GET_DATA(hIRDX, pData, BufSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PIXEL_GET_DATA, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_PIXEL_GetDataPoint(HANDLE hIRDX, unsigned short x, unsigned short y, float* pData)
{
	DDAQ_U32	ret = DDAQ_IRDX_PIXEL_GET_DATA_POINT(hIRDX, x, y, pData);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_PIXEL_GET_DATA_POINT, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_USERDATA_GetSize(HANDLE hIRDX, unsigned short no, unsigned long* pSize)
{
	DDAQ_U32	ret = DDAQ_IRDX_USERDATA_GET_SIZE(hIRDX, no, pSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_USERDATA_GET_SIZE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_USERDATA_GetData(HANDLE hIRDX, unsigned short no, unsigned long Size, void* pData)
{
	DDAQ_U32	ret = DDAQ_IRDX_USERDATA_GET_DATA(hIRDX, no, Size, pData);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_USERDATA_GET_DATA, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_USERDATA_SetData(HANDLE hIRDX, unsigned short no, unsigned long Size, void* pData)
{
	DDAQ_U32	ret = DDAQ_IRDX_USERDATA_SET_DATA(hIRDX, no, Size, pData);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_USERDATA_SET_DATA, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_BUFFER_GetSize(HANDLE hIRDX, DWORD* pBufSize)
{
	DDAQ_U32	ret = DDAQ_IRDX_BUFFER_GET_SIZE(hIRDX, pBufSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_GET_SIZE, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_BUFFER_WriteTo(HANDLE hIRDX, BYTE* pBuf, DWORD BufSize, DWORD* pUsedBufSize)
{
	DDAQ_U32	ret = DDAQ_IRDX_BUFFER_WRITE_TO(hIRDX, pBuf, BufSize, pUsedBufSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_WRITE_TO, ret);
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_BUFFER_OpenFrom(BYTE* pBuf, DWORD BufSize, HANDLE* pIRDX)
{
	DDAQ_U32	ret = DDAQ_IRDX_BUFFER_OPEN_FROM(pBuf, BufSize, pIRDX);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_OPEN_FROM, ret);
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_BUFFER_WriteToEx(HANDLE hIRDX, BYTE* pBuf, DWORD BufSize, DWORD* pUsedBufSize, DWORD* pRawDataOfs, DWORD* pRawDataSize)
{
	DDAQ_U32	ret = DDAQ_IRDX_BUFFER_WRITE_TO_EX(hIRDX, pBuf, BufSize, pUsedBufSize, pRawDataOfs, pRawDataSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_WRITE_TO, ret);
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CPyroSoftMApp::DDAQ_IRDX_BUFFER_OpenFromEx(BYTE* pBuf, DWORD BufSize, HANDLE* pIRDX, DWORD* pRawDataOfs, DWORD* pRawDataSize)
{
	DDAQ_U32	ret = DDAQ_IRDX_BUFFER_OPEN_FROM_EX(pBuf, BufSize, pIRDX, pRawDataOfs, pRawDataSize);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_OPEN_FROM, ret);
		return false;
	}
	return true;
}

bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_GetMotorFocusPos(HANDLE hIRDX, unsigned short* pPos, unsigned short* pState)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_GET_MOTOR_FOCUS_POSITION(hIRDX, pPos, pState);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_OPEN_FROM, ret);
		return false;
	}
	return true;
}

bool CPyroSoftMApp::DDAQ_IRDX_ACQUISITION_SetMotorFocusPos(HANDLE hIRDX, unsigned short	Pos)
{
	DDAQ_U32	ret = DDAQ_IRDX_ACQUISITION_SET_MOTOR_FOCUS_POSITION(hIRDX, Pos);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_OPEN_FROM, ret);
		return false;
	}
	return true;
}

bool CPyroSoftMApp::DDAQ_DEVICE_GET_MOTOR_FocusVersion(unsigned long nDaQDevNo, unsigned long* pVersion)
{
	DDAQ_U32	ret = DDAQ_DEVICE_GET_MOTOR_FOCUS_VERSION(nDaQDevNo, pVersion);
	if (ret != DDAQ_NO_ERROR)
	{
		DoErrorMessage(IDS_ERROR_IRDX_BUFFER_OPEN_FROM, ret);
		return false;
	}
	return true;
}

bool CPyroSoftMApp::DAQ_LASER_DISTANCE_DAQmxStartTask(TaskHandle THandle)
{
	int error = DAQmxStartTask(DAQ_TaskHandle);
	
	if (error == error)		// ������ ���� ���� ����
		return true;

	return true;
}

bool CPyroSoftMApp::DAQ_LASER_DISTANCE_DAQmxClearTask(TaskHandle THandle)
{
	int error;

	if (DAQ_TaskHandle != 0)
	{
		error = DAQmxStopTask(DAQ_TaskHandle);
		if (error == error)		// ������ ���� ���� ����
			error = error;
		error = DAQmxClearTask(DAQ_TaskHandle);
		if (error == error)		// ������ ���� ���� ����
			return true;
	}
	return true;
}

bool CPyroSoftMApp::DAQ_LASER_DAQmxReadAnalogScalarF64(TaskHandle THandle, float64 timeout, float64 *value, bool32 *reserved)
{
	int error;
	double fA;
	double rate;

	if (DAQ_TaskHandle != 0)
	{
		error = DAQmxReadAnalogScalarF64(DAQ_TaskHandle, 0, &fA, NULL);

		m_Laser_mA = roundXL(fA*1000.0f, 2);

		rate = (fA - 0.004f) / (0.02f - 0.004f);

		m_Laser_Distance = roundXL(5000.0f*rate, 0);

		if (error == error)
			return true;
	}
	return true;
}

double CPyroSoftMApp::roundXL(double x, int digit)
{
	return (floor((x)* pow(double(10), digit) + 0.5f) / pow(double(10), digit));
}

/////////////////////////////////////////////////////////////////////////////
void CPyroSoftMApp::OnDeviceDoOpensimulation()
{
	// get not used devices 
	unsigned long numNotUsedDevices = 0;
	unsigned long DDAQDevicNo[DDAQ_MAX_DEVICES];

	unsigned long i;

	for (i = 0; i<DDAQ_MAX_DEVICES; i++)
	{
		if (m_DocWithDevice[i] == NULL)
		{
			DDAQDevicNo[numNotUsedDevices] = i + 1;
			numNotUsedDevices++;
		}
	}

	// simulation camera free
	bool bFreeSimuDevice = false;

	// Add the device strings for free devices

	for (i = 0; i<numNotUsedDevices; i++)
	{
		// get device string
		CString s;
		if (!DDAQ_DEVICE_GET_IDString(DDAQDevicNo[i], s))
			return;

		// if s is empty, device is not a real camera
		if (s.IsEmpty())
		{
			bFreeSimuDevice = true;
			break;
		}
	}

	// available?
	if (!bFreeSimuDevice)
	{
		DoErrorMessage(IDS_ERROR_FREE_DEVNO, 0);
		return;
	}

	// File open dialog for simulation file
	LPCTSTR szFilter = _T("IRD file (*.ird) | *.ird");
	CFileDialog dlg(TRUE, _T("ird"), NULL, OFN_HIDEREADONLY, szFilter);

	CString intiDir = theApp.AppPath + CString("\\SumulationData");
	dlg.m_ofn.lpstrInitialDir = (LPSTR)(LPCTSTR)intiDir;

	if (dlg.DoModal() != IDOK)
		return;

	// open the simulation device 
	CString SimuName = dlg.GetPathName();
	if (!DDAQ_DEVICE_DO_OpenSimulation(DDAQDevicNo[i], SimuName))
		return;

	theApp.IRDX_Mode = 2;

	// get the template
	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate*	pTemplate = GetNextDocTemplate(pos);

	ASSERT(pTemplate != NULL);

	// create the document
	CPyroSoftMDoc* pDoc = (CPyroSoftMDoc*)pTemplate->OpenDocumentFile(NULL);
	ASSERT(pDoc != NULL);

	pDoc->m_OpenMode = 2;

	pos = pDoc->GetFirstViewPosition();
	CView* pView;
	CView* pPView;
	pView = pDoc->GetNextView(pos);
	pView = pDoc->GetNextView(pos);
	((CScaleView *)pView)->pDoc = pDoc;
	((CScaleView *)pView)->m_ScaleDlg.pDoc = pDoc;

	pView = pDoc->GetNextView(pos);
	pPView = pView;
	((CROIGridView *)pView)->pDoc = pDoc;
	((CROIGridView *)pView)->m_ROIGridDlg.pDoc = pDoc;

	pView = pDoc->GetNextView(pos);
	((CResultView *)pView)->pDoc = pDoc;

	//***********************************************************

	// device no
	unsigned long	dev_no = DDAQDevicNo[i];

	// set document for messaging
	m_DocWithDevice[dev_no - 1] = pDoc;

	// set dev no to document
	pDoc->SetDDAQDeviveNo(dev_no);
	pDoc->GetCamNBufferSize();
	//////////////////////////////////////////////////////////////////////////

	// update all view
	pDoc->UpdateAllViews(NULL, 2);

	//***********************************************************
	//theApp.IRDX_Mode = 1;

	// Load Config & Set Params
	pDoc->LoadConfig();
	// Load Recipe Config & Set Params
	pDoc->LoadRecipeConfig();
	// Load Emissivity Config & Set Params
	pDoc->LoadETPerDistance();

	((CResultView *)pView)->m_ResultDlg.pDoc = pDoc;
	((CResultView *)pView)->m_MaxTabDlg.pDoc = pDoc;
	((CResultView *)pView)->m_MinTabDlg.pDoc = pDoc;
	((CResultView *)pView)->m_AvgTabDlg.pDoc = pDoc;
	((CResultView *)pView)->InitROIData();
	((CROIGridView *)pPView)->InitStatusData();

	//***********************************************************

	this->m_pPropertyWnd->ChangeProperties(pDoc);

	m_FocusFlag = true;
	
	pDoc->OnDeviceDoStart();
}

/////////////////////////////////////////////////////////////////////////////
void CPyroSoftMApp::Devicedetection()
{
	//AfxMessageBox(IDS_DETECT_WAIT);

	BeginWaitCursor();
	m_nNumDetectDevices = DDAQ_DEVICE_DO_DETECTION();
	EndWaitCursor();

	CString		s;
	s.Format(IDS_DETECT_NUMDEVICES, m_nNumDetectDevices);
	//AfxMessageBox(s);
	
	// clear use of all devices
	for (unsigned long i = 0; i<DDAQ_MAX_DEVICES; i++)
		m_DocWithDevice[i] = NULL;

	if (m_nNumDetectDevices != 0)
		OnDeviceDoOpen();
	else
	{
		//
	}
}
/////////////////////////////////////////////////////////////////////////////

void CPyroSoftMApp::OnDeviceDoOpen()
{
	if (m_bCameraConnected || m_bFileOpen) return;

	// get not used devices 
	unsigned long	numNotUsedDevices = 0;
	unsigned long	DDAQDevicNo[DDAQ_MAX_DEVICES];

	unsigned long i;

	for (i = 0; i<m_nNumDetectDevices; i++)
	{
		if (m_DocWithDevice[i] == NULL)
		{
			DDAQDevicNo[numNotUsedDevices] = i + 1;	// for DDAQ is 0 invalid!
			numNotUsedDevices++;
		}
	}

	CDlgAddNewDevice	dlg;

	// really connected cameras
	unsigned long numRealDevices = 0;
	CString sd;
	CString isVaildSerialNum;
	// Add the device strings for free devices
	for (i = 0; i<numNotUsedDevices; i++)
	{
		// get device string
		//CString sd;
		if (!DDAQ_DEVICE_GET_IDString(DDAQDevicNo[i], sd))
			break;

		// get interface string
		CString si;
		if (!DDAQ_DEVICE_GET_InterfaceString(DDAQDevicNo[i], si))
			break;

		// if s is empty, device is not a real camera
		if (sd.IsEmpty())
		{
			break;
		}
		else
		{
			// add strings to device list
			dlg.m_DeviceStrings.Add(sd);
			dlg.m_InterfaceStrings.Add(si);
			numRealDevices++;
		}
	}

	isVaildSerialNum = sd.Right(8);

	// available?
	if (numRealDevices == 0)
	{
		DoErrorMessage(IDS_ERROR_FREE_DEVNO, 0);
		return;
	}

	if (dlg.DoModal() != IDOK)
		return;

	const char* pTmpl = NULL;
	if (dlg.m_UseTemplate)
		pTmpl = dlg.m_TemplateFile;

	// open the device 
	BeginWaitCursor();

	unsigned long kk = DDAQDevicNo[dlg.m_SelDevice];

	if (!DDAQ_DEVICE_DO_Open(DDAQDevicNo[dlg.m_SelDevice], pTmpl))
	{
		EndWaitCursor();
		return;
	}
	EndWaitCursor();

	theApp.IRDX_Mode = 2;
	// get the template

	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate*	pTemplate = GetNextDocTemplate(pos);

	ASSERT(pTemplate != NULL);

	// create the document
	CPyroSoftMDoc* pDoc = (CPyroSoftMDoc*)pTemplate->OpenDocumentFile(NULL);

	ASSERT(pDoc != NULL);

	//***********************************************************
	pDoc->m_OpenMode = 3;

	pos = pDoc->GetFirstViewPosition();
	CView* pView;
	CView* pPView;
	pView = pDoc->GetNextView(pos);
	pView = pDoc->GetNextView(pos);
	((CScaleView *)pView)->pDoc = pDoc;
	((CScaleView *)pView)->m_ScaleDlg.pDoc = pDoc;

	pView = pDoc->GetNextView(pos);
	pPView = pView;
	((CROIGridView *)pView)->pDoc = pDoc;
	((CROIGridView *)pView)->m_ROIGridDlg.pDoc = pDoc;

	pView = pDoc->GetNextView(pos);
	((CResultView *)pView)->pDoc = pDoc;

	//pView->UpdateWindow();

	//***********************************************************

	// devive no
	unsigned long	dev_no = DDAQDevicNo[dlg.m_SelDevice];

	// set document for messaging
	m_DocWithDevice[dev_no - 1] = pDoc;

	// set dev no to document
	pDoc->SetDDAQDeviveNo(dev_no);
	pDoc->GetCamNBufferSize();

	// update all view
	pDoc->UpdateAllViews(NULL, 2);

	//theApp.IRDX_Mode = 1;

	// Load Config & Set Params
	pDoc->LoadConfig();
	// Load Recipe Config & Set Params
	pDoc->LoadRecipeConfig();
	// Load Emissivity Config & Set Params
	pDoc->LoadETPerDistance();

	((CResultView *)pView)->m_ResultDlg.pDoc = pDoc;
	((CResultView *)pView)->m_MaxTabDlg.pDoc = pDoc;
	((CResultView *)pView)->m_MinTabDlg.pDoc = pDoc;
	((CResultView *)pView)->m_AvgTabDlg.pDoc = pDoc;
	((CResultView *)pView)->InitROIData();
	((CROIGridView *)pPView)->InitStatusData();

	m_pPropertyWnd->ChangeProperties(pDoc);

	DDAQ_DEVICE_GET_MOTOR_FocusVersion(DDAQDevicNo[dlg.m_SelDevice], &m_ForcedFocusFlg);
	
	if (m_ForcedFocusFlg == 3)
	{
		unsigned short Pos;
		unsigned short Status;
		m_FocusFlag = true;

		BeginWaitCursor();

		DDAQ_IRDX_ACQUISITION_GetMotorFocusPos(pDoc->m_hIRDX_Doc, &Pos, &Status);
		DDAQ_IRDX_ACQUISITION_SetMotorFocusPos(pDoc->m_hIRDX_Doc, 0);
		Sleep(Pos * 2 / 3);
		DDAQ_IRDX_ACQUISITION_GetMotorFocusPos(pDoc->m_hIRDX_Doc, &Pos, &Status);

		EndWaitCursor();
	}

	m_bCameraConnected = true;

	pDoc->OnDeviceDoStart();
}



void CPyroSoftMApp::OnDeviceDoScan()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	Devicedetection();
}
