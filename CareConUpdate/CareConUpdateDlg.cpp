
// CareConUpdateDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CareConUpdate.h"
#include "CareConUpdateDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {
	const wchar_t* kCareConExePath = L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.exe";

	BOOL TerminateCareConIfRunning()
	{
		BOOL terminated = FALSE;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return FALSE;

		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnapshot, &pe))
		{
			do
			{
				if (_wcsicmp(pe.szExeFile, L"CareCon.exe") != 0)
					continue;

				HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pe.th32ProcessID);
				if (!hProcess)
					continue;

				wchar_t processPath[MAX_PATH] = { 0 };
				DWORD processPathLen = _countof(processPath);
				if (QueryFullProcessImageName(hProcess, 0, processPath, &processPathLen) && _wcsicmp(processPath, kCareConExePath) == 0)
				{
					if (TerminateProcess(hProcess, 0))
					{
						WaitForSingleObject(hProcess, 5000);
						terminated = TRUE;
					}
				}

				CloseHandle(hProcess);
			} while (Process32Next(hSnapshot, &pe));
		}

		CloseHandle(hSnapshot);
		return terminated;
	}
}

// CCareConUpdateDlg 대화 상자

CCareConUpdateDlg::CCareConUpdateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CARECONUPDATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCareConUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CCareConUpdateDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE, &CCareConUpdateDlg::OnDownloadComplete)
	ON_MESSAGE(WM_DOWNLOAD_FAILED, &CCareConUpdateDlg::OnDownloadFailed)
END_MESSAGE_MAP()


// CCareConUpdateDlg 메시지 처리기
void CCareConUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCareConUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CCareConUpdateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_ProgressCtrl.SetRange(0, 100);
	m_ProgressCtrl.SetPos(0);
	AfxBeginThread(DownloadThread, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


UINT DownloadThread(LPVOID pParam) {
	CCareConUpdateDlg* pDialog = reinterpret_cast<CCareConUpdateDlg*>(pParam);

	CString strURL1 = L"https://new.bizstory.co.kr/data/CareCon/CareCon.in_";
	CString strURL2 = L"https://new.bizstory.co.kr/data/CareCon/CareCon.ex_";

	CString strFileName1 = L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.in_";
	CString strFileName2 = L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.ex_";	


	pDialog->m_Callback.m_pProgressCtrl = &pDialog->m_ProgressCtrl;

	URLDownloadToFile(NULL, strURL1, strFileName1, 0, &pDialog->m_Callback);
	HRESULT hr = URLDownloadToFile(NULL, strURL2, strFileName2, 0, &pDialog->m_Callback);
	if (SUCCEEDED(hr)) {
		pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
	}
	else {
		pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
	}

	return 0;
}


LRESULT CCareConUpdateDlg::OnDownloadComplete(WPARAM wParam, LPARAM lParam) {

	TerminateCareConIfRunning();

	// 압축파일 삭제
	CFile::Remove(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.ini");
	CFile::Rename(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.in_", L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.ini");

	CFile::Remove(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.exe");
	CFile::Rename(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.ex_", L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.exe");

	// carecon.ini의 버전 정보를 레지스트리에 기록
	TCHAR strVer[32];
	CString Path;
	Path.Format(_T("%s\\CareCon.ini"), L"C:\\Program Files (x86)\\METASTORY\\CareCon");
	::GetPrivateProfileString(_T("Version"), _T("Ver"), 0, strVer, _countof(strVer), Path);
	SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"version", strVer);

	AfxMessageBox(L"CareCon 업데이트가 완료되었습니다! CareCon을 재시작합니다.");

	ShellExecute(nullptr, _T("runas"), L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareCon.exe", nullptr, nullptr, SW_SHOWNORMAL);
	exit(1);
	return 0;
}


LRESULT CCareConUpdateDlg::OnDownloadFailed(WPARAM wParam, LPARAM lParam) {
	AfxMessageBox(L"CareCon Download Failed!");
	exit(1);
	return 0;
}


HRESULT CDownloadCallback::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText) {
	if (m_pProgressCtrl) {
		int nPos = (int)(100.0 * ulProgress / ulProgressMax);
		m_pProgressCtrl->SetPos(nPos);
	}
	return S_OK;
}

