
// CareConnDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CareConn.h"
#include "CareConnDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCareConnApp* m_App = (CCareConnApp*)AfxGetApp();

// CCareConnDlg 대화 상자

CCareConnDlg::CCareConnDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CARECONN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCareConnDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SYSLINK1, m_sysLink);
}

BEGIN_MESSAGE_MAP(CCareConnDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnMac, &CCareConnDlg::OnBnClickedbtnmac)
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CCareConnDlg::OnClickSyslink1)
END_MESSAGE_MAP()


// CCareConnDlg 메시지 처리기

BOOL CCareConnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

    m_sysLink.ShowWindow(false);

	// Check bizStory Registry 
	
	//HKEY hKey;
	int nRetCode = 0, nCount = 0;
	CString activeIP, browserPath;

	m_App->activeMacCode = GetMACAddress();

	nRetCode = GetRegistryIntValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Count");
	if (nRetCode <= 0)
	{
        // 초기 사용자 등록
        INT_PTR nResponse = registryDlg.DoModal();
        if (nResponse == IDOK)
        {
            SetRegistryIntValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Count", 1);
            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"agent_comp_code", m_App->agent_compCode);
            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"version", m_App->strVersion);
            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"MacAddr1", m_App->activeMacCode);
            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Company", m_App->companyName);
            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"manager", m_App->manager);
            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"place", m_App->place);

            m_App->comp_code = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"comp_code");

            if (registryDlg.LoadConfigure())
                callReception();
        }
        else if (nResponse == IDCANCEL)
        {
            AfxMessageBox(L"사용자 등록이 취소되었습니다!");
        }        
        

        exit(1);
    }
	else
	{
        strVersion = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"version");
        m_App->agent_compCode = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"agent_comp_code");
        m_App->comp_code = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"comp_code");
        m_App->companyName = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Company");// 학교/회사
        m_App->manager = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"manager");    // 담당자
        m_App->place = GetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"place");        // 설치 장소

        

        // Compare to agent version
       
        if (!registryDlg.checkVersion(strVersion))
        {
            //AfxMessageBox(L"db up");
            //m_sysLink.ShowWindow(true);
            //SetDlgItemText(IDC_SYSLINK1, _T("상위 버전의 제품이 있습니다. CareCon 설치 페이지로 이동합니다 : <a href=\"https://new.bizstory.co.kr\">비즈스토리 케어콘</a>"));
            ShellExecute(nullptr, _T("open"), L"C:\\Program Files (x86)\\METASTORY\\CareCon\\CareConUpdate.exe", nullptr, nullptr, SW_SHOWNORMAL);

            SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"version", strVersion);

            exit(1);
        }
        else
        {
            nCount = nRetCode + 1;
            SetRegistryIntValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Count", nCount);

            // Authentification : MacAddr Check
            if (isAuthentication())
            {
                if (registryDlg.LoadConfigure())
                    callReception();
                //AfxMessageBox()
            }
            else
            {
                AfxMessageBox(L"등록되지 않은 사용자입니다. 관리자에게 문의해주세요!");
            }
            exit(1);
        }
       
    }


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCareConnDlg::OnPaint()
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
HCURSOR CCareConnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCareConnDlg::OnBnClickedbtnmac()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CCareConnDlg::OnClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult)
{
    PNMLINK pNMLink = reinterpret_cast<PNMLINK>(pNMHDR);
    LITEM item = pNMLink->item;

    if (item.szUrl != nullptr)
    {
        ShellExecute(nullptr, _T("open"), item.szUrl, nullptr, nullptr, SW_SHOWNORMAL);
        exit(1);
    }

    *pResult = 0;
}

// MacAddr Authentication
BOOL CCareConnDlg::isAuthentication()
{
    if (registryDlg.CheckMacAddr())
        return true;
    else
        return false;
}


// 접수창 다이얼로그 오픈 
void CCareConnDlg::callReception()
{
    INT_PTR nResponse = receptionDlg.DoModal();
    if (nResponse == IDOK)
    {

    }
}


void CCareConnDlg::callBizstory()
{
    CString browserPath;
    CString url, baseUrl, pathURL;

    baseUrl = L"https://new.bizstory.co.kr/agent/agent_verification.php?agent_client_code=";
    url.Format(L"%s%s&macaddress=%s&agent_version=%s", baseUrl, m_App->agent_compCode, m_App->activeMacCode, m_App->strVersion);

    // Edge를 기본으로 열 때
    browserPath = GetRegistryStrValue(REG_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msedge.exe", L"Path");
    if (browserPath != "")
    {
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
        // Chrome으로 열 때
        browserPath = GetRegistryStrValue(REG_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe", L"Path");
        if (browserPath != "")
        {
            pathURL.Format(L"%s\\chrome.exe", browserPath);
            ShellExecute(NULL, _T("open"), pathURL, url, NULL, SW_HIDE);
        }
        else
            AfxMessageBox(L"크롬 브라우저를 열지 못합니다. 관리자에게 문의해주세요!");
    }
}


void CCareConnDlg::SetBrowserSize(int width, int height)
{
    CRect rect(0, 0, width, height);
    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

// URL 로드
void CCareConnDlg::LoadURL(CString url)
{
    BSTR bstrURL = SysAllocString(url.GetBuffer());
    pBrowser->Navigate(bstrURL, NULL, NULL, NULL, NULL);

    SysFreeString(bstrURL);
    pBrowser->Release();
}


CString CCareConnDlg::GetMACAddress()
{
    CString strMac = _T("");
    CString str;
    CString strDevice = _T("");
    CString strDesc = _T("");
    CString strIP = _T("");

    hIpHlpApi = LoadLibrary(_T("iphlpapi.dll"));
    if (hIpHlpApi == NULL) {
        AfxMessageBox(_T("Can't load iphlpapi.dll"));
        return L"";
    }

    //Finding function
    m_pGetAdaptersInfo =
        (lpfnGetAdaptersInfo)GetProcAddress(hIpHlpApi, "GetAdaptersInfo");

    if (!m_pGetAdaptersInfo) {
        AfxMessageBox(_T("GetProcAddress(GetAdaptersInfo) failed."));
        FreeLibrary(hIpHlpApi);
        return L"";
    }

    IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information			(*)
    // for up to 16 NICs			(*)

    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer	(*)

    DWORD dwStatus = m_pGetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS) {
        AfxMessageBox(_T("GetAdaptersInfo failed."));
        FreeLibrary(hIpHlpApi);
        return L"";
    }

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;

    if (!pAdapterInfo)
    {
        AfxMessageBox(_T("pAdapterInfo Error"));
        printf("pAdapterInfo error\n");
    }

    while (pAdapterInfo) {
        // (1) 이더넷 타입인지 확인
        if (pAdapterInfo->Type != MIB_IF_TYPE_ETHERNET) {
            pAdapterInfo = pAdapterInfo->Next;
            continue;
        }

        // (2) 어댑터 설명(Description) 확인
        strDesc = pAdapterInfo->Description;

        // 필요에 따라 가상 어댑터(VMware, Virtual 등) 제외
        if (strDesc.Find(L"VMware") >= 0 || strDesc.Find(L"Virtual") >= 0) {
            pAdapterInfo = pAdapterInfo->Next;
            continue;
        }

        // 필요에 따라 블루투스 어댑터 제외
        if (strDesc.Find(L"Bluetooth") >= 0) {
            pAdapterInfo = pAdapterInfo->Next;
            continue;
        }

        // (3) 유효 IP인지 확인 (0.0.0.0 등 제외)
        strIP.Format(L"%s", pAdapterInfo->IpAddressList.IpAddress.String);
        char mIP = (char)strIP.GetAt(0);
        // 첫 글자가 '0'이거나 숫자가 아니면 제외
        if (mIP == '0' || !isdigit(mIP)) {
            pAdapterInfo = pAdapterInfo->Next;
            continue;
        }

        // (4) MAC 주소 문자열 만들기 (XX-XX-XX-XX-XX-XX)
        strMac.Empty();
        for (int i = 0; i < pAdapterInfo->AddressLength; i++) {
            // 보통 6바이트(0~5)
            str.Format(_T("%02X"), (unsigned char)pAdapterInfo->Address[i]);
            // 마지막 바이트 전이라면 '-' 추가
            if (i < (pAdapterInfo->AddressLength - 1)) {
                str += _T("-");
            }
            strMac += str;
        }

        // 첫 번째로 찾은 이더넷 어댑터의 MAC만 반환
        // 만약 여러 이더넷 어댑터가 있을 때, 계속해서 다음 어댑터까지 검사하고 싶다면
        // break 대신 pAdapterInfo = pAdapterInfo->Next;로 계속 탐색
        // pAdapterInfo = pAdapterInfo->Next;
        break;
    }

    FreeLibrary(hIpHlpApi);
    return strMac;
}


CString CCareConnDlg::GetIP()
{
    CString strIP = L"";
    CStringArray strIPArray;

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD retVal = 0;

    ULONG bufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = new IP_ADAPTER_INFO[bufLen];

    USES_CONVERSION;

    if (pAdapterInfo == NULL)
        return NULL;

    if (GetAdaptersInfo(pAdapterInfo, &bufLen) == ERROR_BUFFER_OVERFLOW)
    {
        delete pAdapterInfo;
        pAdapterInfo = new IP_ADAPTER_INFO[bufLen];
        if (pAdapterInfo == NULL)
            return NULL;
    }

    if ((retVal = GetAdaptersInfo(pAdapterInfo, &bufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            strIP.Format(L"%s", pAdapter->IpAddressList.IpAddress.String);
            strIPArray.Add(strIP);
            pAdapter = pAdapter->Next;
        }
    }

    delete pAdapterInfo;
    pAdapterInfo = NULL;

    return strIP;
}


void CCareConnDlg::print_ip(DWORD nAddr)
{
    printf("%d.%d.%d.%d\n", nAddr & 0x000000FF, (nAddr & 0x0000FF00) >> 8, (nAddr & 0x00FF0000) >> 16, (nAddr & 0xFF000000) >> 24);
}


