
// CareConn.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "CareConn.h"
#include "CareConnDlg.h"
#include <UIAutomation.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCareConnApp

BEGIN_MESSAGE_MAP(CCareConnApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCareConnApp 생성

CCareConnApp::CCareConnApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CCareConnApp 개체입니다.

CCareConnApp theApp;


// CCareConnApp 초기화

BOOL CCareConnApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));

	/////////////////////////////////////////////////////////////////////////////////////////////////////
		// 중복 실행 방지 코드
	HANDLE hMutexOneInstance = ::CreateMutex(NULL, TRUE, _T("Unique Name of CareCon v1.00"));

	BOOL bFound = FALSE;
	bCompanyLogo = false;

	dbName = L"new_bizstory";

	// 만약 이미 만들어져 있다면 Instance가 이미 존재함
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
		bFound = TRUE;

	if (hMutexOneInstance)
		::ReleaseMutex(hMutexOneInstance);

	// 이미 하나의 Instance가 존재하면 프로그램 종료
	if (bFound) {
		//AfxMessageBox(_T("CareConn 프로그램은 중복 사용할 수 없습니다."), MB_ICONINFORMATION);
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	if (Gdiplus::Ok != Gdiplus::GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL))	// Initialize GDI+
	{
		AfxMessageBox(_T("Error Initialize GDI+"));
		return FALSE;
	}

	// product version
	strVersion = L"1.1.4";
	
	TCHAR dbHost[15], ui_company[255], ui_type[2];
	CString Path;
	Path.Format(_T("%s\\CareCon.ini"), L"C:\\Program Files (x86)\\METASTORY\\CareCon");
	::GetPrivateProfileString(_T("Info"), _T("UI_Type"), 0, ui_type, sizeof(ui_type), Path);
	::GetPrivateProfileString(_T("Info"), _T("UI_Company"), 0, ui_company, sizeof(ui_company), Path);
	::GetPrivateProfileString(_T("DB"), _T("HOST"), 0, dbHost, sizeof(dbHost), Path);
	UI_Type.Format(L"%s", ui_type);
	UI_Company.Format(L"%s", ui_company);
	DBPort = ::GetPrivateProfileInt(_T("DB"), _T("DBPort"), 0, Path);
	DBHost.Format(L"%s", dbHost);

	CCareConnDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}


int CCareConnApp::ExitInstance()
{
	GdiplusShutdown(m_GdiplusToken);
	
	return CWinApp::ExitInstance();
}


void CCareConnApp::linkURL(CString url)
{
	if (url == L"")
		return;

	// check 필요
	//if (IsURLAlreadyOpened(url))
	//{
	//	AfxMessageBox(L"선택한 항목이 이미 열려 있습니다.");
	//	return;
	//}

	// Edge가 기본 브라우저인지 확인
	CString browserPath = GetRegistryStrValue(REG_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msedge.exe", L"Path");
	if (!browserPath.IsEmpty())
	{
		CString pathURL;
		pathURL.Format(L"%s\\msedge.exe", browserPath);

		SHELLEXECUTEINFO sei = { 0 };
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.hwnd = NULL;
		sei.lpVerb = _T("open");
		sei.lpFile = pathURL;
		sei.lpParameters = url;
		sei.nShow = SW_SHOWNORMAL;
		sei.fMask = SEE_MASK_NO_CONSOLE;

		ShellExecuteEx(&sei);
	}
	else
	{
		// Chrome이 설치되어 있는지 확인
		browserPath = GetRegistryStrValue(REG_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe", L"Path");
		if (!browserPath.IsEmpty())
		{
			CString pathURL;
			pathURL.Format(L"%s\\chrome.exe", browserPath);
			ShellExecute(NULL, _T("open"), pathURL, url, NULL, SW_HIDE);
		}
		else
		{
			AfxMessageBox(L"Edge 또는 Chrome 브라우저를 찾을 수 없습니다. 관리자에게 문의하세요.");
		}
	}
}



bool CCareConnApp::IsURLAlreadyOpened(CString url)
{
	CoInitialize(NULL);
	IUIAutomation* pAutomation = NULL;
	IUIAutomationElement* pRootElement = NULL;

	// Create UIAutomation object
	HRESULT hr = CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, IID_IUIAutomation, (void**)&pAutomation);
	if (FAILED(hr) || pAutomation == NULL) {
		CoUninitialize();
		return false;
	}

	// Get the root element
	hr = pAutomation->GetRootElement(&pRootElement);
	if (FAILED(hr) || pRootElement == NULL) {
		pAutomation->Release();
		CoUninitialize();
		return false;
	}

	// Find all Edge windows
	IUIAutomationCondition* pCondition = NULL;
	hr = pAutomation->CreatePropertyCondition(UIA_ClassNamePropertyId, _variant_t(L"Chrome_WidgetWin_1"), &pCondition);
	if (FAILED(hr) || pCondition == NULL) {
		pRootElement->Release();
		pAutomation->Release();
		CoUninitialize();
		return false;
	}

	IUIAutomationElementArray* pFound = NULL;
	hr = pRootElement->FindAll(TreeScope_Children, pCondition, &pFound);
	if (SUCCEEDED(hr) && pFound != NULL) {
		int length = 0;
		pFound->get_Length(&length);
		for (int i = 0; i < length; i++) {
			IUIAutomationElement* pElement = NULL;
			pFound->GetElement(i, &pElement);
			if (pElement) {
				// Find the address bar element
				IUIAutomationCondition* pEditCondition = NULL;
				hr = pAutomation->CreatePropertyCondition(UIA_ControlTypePropertyId, _variant_t(UIA_EditControlTypeId), &pEditCondition);
				if (SUCCEEDED(hr) && pEditCondition != NULL) {
					IUIAutomationElement* pAddressElement = NULL;
					hr = pElement->FindFirst(TreeScope_Descendants, pEditCondition, &pAddressElement);
					if (SUCCEEDED(hr) && pAddressElement) {
						// Get the URL from the address bar
						BSTR bstrValue;
						pAddressElement->get_CurrentName(&bstrValue);
						CString windowTitle(bstrValue);
						SysFreeString(bstrValue);

						if (windowTitle.Find(url) != -1) {
							pAddressElement->Release();
							pEditCondition->Release();
							pElement->Release();
							pFound->Release();
							pCondition->Release();
							pRootElement->Release();
							pAutomation->Release();
							CoUninitialize();
							return true;
						}

						pAddressElement->Release();
					}
					pEditCondition->Release();
				}
				pElement->Release();
			}
		}
		pFound->Release();
	}

	pCondition->Release();
	pRootElement->Release();
	pAutomation->Release();
	CoUninitialize();
	return false;
}
