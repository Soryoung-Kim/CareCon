// ReceptionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CareConn.h"
#include "afxdialogex.h"
#include "ReceptionDlg.h"
#include "CregistryDlg.h"


// ReceptionDlg 대화 상자

IMPLEMENT_DYNAMIC(ReceptionDlg, CDialogEx)

extern CCareConnApp* m_App;
ReceptionDlg::ReceptionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_receptionDlg, pParent)
{
	nBannerNo = 0;
	nNoticeNo = 0;
	bLoadLogo = false;
	memset(&m_font, 0, sizeof(m_font));
}

ReceptionDlg::~ReceptionDlg()
{
}

void ReceptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BANNER, m_ctrlBanner);
	DDX_Control(pDX, IDC_SCHOOL, m_ctrlSchool);
}


BEGIN_MESSAGE_MAP(ReceptionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_btnExit, &ReceptionDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_btnReception, &ReceptionDlg::OnBnClickedReception)
	ON_WM_NCHITTEST()
	ON_STN_CLICKED(IDC_SCHOOL, &ReceptionDlg::OnStnClickedSchool)
END_MESSAGE_MAP()


// ReceptionDlg 메시지 처리기


BOOL ReceptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyle(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_CAPTION, WS_BORDER);
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

	// ForegroundLockTimeout을 일시적으로 0으로 설정하여 포그라운드 강제 획득
	DWORD dwTimeout = 0;
	::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &dwTimeout, 0);
	::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)0, 0);

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	BringWindowToTop();
	SetForegroundWindow();
	SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)(ULONG_PTR)dwTimeout, 0);

	CreateControl();
	DrawBackground();

	// loading banner image
	SetTimer(tBannerSec, 1000, NULL);
	SetTimer(tNoticeSec, 2000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


HBRUSH ReceptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	COLORREF bgColor = RGB(0, 0, 0);

	if (pWnd->GetDlgCtrlID() == IDC_txtButton1 || pWnd->GetDlgCtrlID() == IDC_txtButton2 ||
		pWnd->GetDlgCtrlID() == IDC_txtButton3 || pWnd->GetDlgCtrlID() == IDC_txtButton4)
	{
		COLORREF bgColor = RGB(242, 242, 242);
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(bgColor);
		static CBrush brush(bgColor);
		hbr = brush;
	}
	else if (pWnd->GetDlgCtrlID() == IDC_txtVersion)
	{
		if (m_App->UI_Type == L"A")
		{
			bgColor = RGB(242, 242, 242);
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (m_App->UI_Type == L"B")
		{
			bgColor = RGB(38, 38, 38);
			pDC->SetTextColor(RGB(250, 250, 250));
		}
		pDC->SetBkColor(bgColor);

		// 배경 브러시 반환
		static CBrush brush(bgColor);
		hbr = brush;
	}
	return hbr;
}


LRESULT ReceptionDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint ptClient = point;
	ScreenToClient(&ptClient);

	CRect dragArea(0, 0, 419, 129); // 예: 좌측 상단의 200x50 영역

	if (dragArea.PtInRect(ptClient))
	{
		return HTCAPTION;
	}
	else
	{
		return CDialogEx::OnNcHitTest(point);
	}
}


void ReceptionDlg::DrawBackground()
{
	m_BkgndBmp.DeleteObject();

	CDC* pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	if (m_App->UI_Type == L"A")
		m_bgReception.Load(MAKEINTRESOURCE(IDB_bgReceptionA), _T("PNG"));
	else if (m_App->UI_Type == L"B")
		m_bgReception.Load(MAKEINTRESOURCE(IDB_bgReceptionB), _T("PNG"));

	m_BkgndBmp.CreateCompatibleBitmap(pDC, m_bgReception.GetWidth(), m_bgReception.GetHeight());
	CBitmap* pOldBmp = memDC.SelectObject(&m_BkgndBmp);

	Graphics graphics(memDC);

	graphics.DrawImage(&m_bgReception, 0, 0, m_bgReception.GetWidth(), m_bgReception.GetHeight());

	memDC.SelectObject(pOldBmp);
	ReleaseDC(pDC);
}


// C:\\Program Files (x86)\\METASTORY\\CareCon\\customerLogo.png
void ReceptionDlg::OnPaint()
{
	CPaintDC dc(this); // 화면에 그릴 DC

	CRect rc;
	GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = memDC.SelectObject(&m_BkgndBmp);

	// 배경 이미지 그리기
	memDC.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	if (m_BkgndBmp.GetSafeHandle() == NULL)
		return;

	// 로고 그리기
	if (m_LogoImage) {
		HDC hImageDC = m_LogoImage.GetDC();
		if (hImageDC) { // HDC 유효성 검사
			//BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, 190, AC_SRC_ALPHA }; // 투명도 설정
			//memDC.AlphaBlend(8, 8, m_LogoImage.GetWidth(), m_LogoImage.GetHeight(),
			//	CDC::FromHandle(hImageDC), 0, 0,
			//	m_LogoImage.GetWidth(), m_LogoImage.GetHeight(), blendFunc);
			m_LogoImage.ReleaseDC();

			HBITMAP hBmp = (HBITMAP)m_LogoImage.Detach();
			m_ctrlSchool.SetBitmap(hBmp);
			m_ctrlSchool.Invalidate();
		}
	}
	else {
		m_txtCompanyName.ShowWindow(true);
	}

	// 최종 이미지 화면에 복사
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
}


void ReceptionDlg::CreateControl()
{
	memset(&m_font, 0x00, sizeof(m_font));
	_tcscpy_s(m_font.lfFaceName, _countof(m_font.lfFaceName), _T("맑은고딕"));

	if (m_App->UI_Company == L"CARECON")
	{
		mbtnLogo.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(15, 15, 161, 57), this, IDC_COMPANYLOGO);
		mbtnLogo.SetNormalImage(IDB_CareCon, true);
		m_txtVer.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(168, 27, 200, 44), this, IDC_txtVersion);
	}
	else if (m_App->UI_Company == L"INET")
	{
		mbtnLogo.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(15, 15, 161, 57), this, IDC_COMPANYLOGO);
		mbtnLogo.SetNormalImage(IDB_INET, true);
		m_txtVer.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(168, 27, 200, 44), this, IDC_txtVersion);
	}
	else if (m_App->UI_Company == L"LUCOMS")
	{
		mbtnLogo.SetNormalImage(IDB_LUCOMS, true);
		mbtnLogo.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(15, 15, 109, 43), this, IDC_COMPANYLOGO);
		m_txtVer.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(116, 24, 148, 40), this, IDC_txtVersion);
	}
	//mbtnLogo.ShowWindow(true);

	if (m_App->bCompanyLogo)	// 학교 로고가 있는 경우
	{
		//m_txtCompanyName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(85, 80, 200, 98), this, IDC_txtCompanyName);
		//m_txtManagerPlace.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(85, 98, 300, 113), this, IDC_txtManagerPlace);
		//m_ctrlSchool.ShowWindow(true);

	}
	else  // 학교 로고가 없는 경우
	{
		m_ctrlSchool.ShowWindow(false);
		m_txtCompanyName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(25, 80, 197, 127), this, IDC_txtCompanyName);
		//m_txtManagerPlace.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(25, 98, 300, 113), this, IDC_txtManagerPlace);
	}

	if (m_App->UI_Type == L"A")
	{
		m_txtVer.SetTextColor(RGB(10, 12, 10));
		m_txtVer.SetBkgnd(RGB(242, 242, 242));
		m_txtCompanyName.SetTextColor(RGB(10, 12, 10));
		m_txtCompanyName.SetBkgnd(RGB(255, 255, 255));
		//m_txtManagerPlace.SetTextColor(RGB(10, 12, 10));
		//m_txtManagerPlace.SetBkgnd(RGB(255, 255, 255));
	}
	else if (m_App->UI_Type == L"B")
	{
		m_txtVer.SetTextColor(RGB(215, 235, 215));
		m_txtVer.SetBkgnd(RGB(38, 38, 38));
		m_txtCompanyName.SetTextColor(RGB(215, 235, 215));
		m_txtCompanyName.SetBkgnd(RGB(38, 38, 38));
		//m_txtManagerPlace.SetTextColor(RGB(215, 235, 215));
		//m_txtManagerPlace.SetBkgnd(RGB(38, 38, 38));
	}
	m_font.lfHeight = 14;
	m_txtVer.SetDefaultFont(m_font);

	CString displayVersion;
	displayVersion.Format(L"v%s", m_App->strVersion.GetString());
	m_txtVer.SetWindowText(displayVersion);

	m_font.lfHeight = 20;
	m_txtCompanyName.SetDefaultFont(m_font);
	
	if (!m_App->bCompanyLogo)
		m_txtCompanyName.SetWindowText(m_App->companyName);

	//m_txtManagerPlace.SetDefaultFont(m_font);
	
	//CString strManager;
	//strManager.Format(L"%s / %s", m_App->manager, m_App->place);
	//m_txtManagerPlace.SetWindowText(strManager);


	// 공지사항
	m_rcNotice.SetRect(23, 139, 397, 186);

	// 더보기
	m_rcMore.SetRect(330, 205, 388, 232);

	// AS 항목 영역
	m_rcReceiptSubject1.SetRect(30, 242, 386, 276);
	m_rcReceiptSubject2.SetRect(30, 271, 386, 295);
	m_rcReceiptSubject3.SetRect(30, 301, 386, 325);
	m_rcReceiptSubject4.SetRect(30, 330, 386, 354);

	// 메뉴 영역
	m_rcButton1.SetRect(20, 360, 100, 396);
	m_rcButton2.SetRect(120, 360, 200, 396);
	m_rcButton3.SetRect(220, 360, 300, 396);
	m_rcButton4.SetRect(320, 360, 400, 396);

	// 배너 영역
	m_rcPictureControl.SetRect(21, 402, 395, 490);

	// 등록 초기화 영역
	m_rReset.SetRect(20, 504, 113, 532);

	m_btnExit.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(390, 6, 414, 30), this, IDC_btnExit);
	if (m_App->UI_Type == L"A")
		m_btnExit.SetNormalImage(IDB_btnExitA, true);
	else if (m_App->UI_Type == L"B")
		m_btnExit.SetNormalImage(IDB_btnExitB, true);

	m_btnReception.ShowWindow(true);

	m_btnReception.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(270, 70, 390, 119), this, IDC_btnReception);
	m_btnReception.SetNormalImage(IDB_btnReception, true);
	CClientDC dcParent(this->GetParent());
	m_btnReception.SetTransparentBkgnd(&dcParent);
	m_btnReception.ShowWindow(true);

	m_font.lfHeight = 20;
	m_font.lfWeight = FW_BOLD;
	//m_font.lfUnderline = true;
	m_txtButton1.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(32, 372, 100, 400), this, IDC_txtButton1);
	m_txtButton1.SetDefaultFont(m_font);
	m_txtButton1.SetWindowText(m_App->buttonName[0]);

	m_txtButton2.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(122, 372, 200, 400), this, IDC_txtButton2);
	m_txtButton2.SetDefaultFont(m_font);
	m_txtButton2.SetWindowText(m_App->buttonName[1]);

	m_txtButton3.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(222, 372, 300, 400), this, IDC_txtButton3);
	m_txtButton3.SetDefaultFont(m_font);
	m_txtButton3.SetWindowText(m_App->buttonName[2]);

	m_txtButton4.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(322, 372, 400, 400), this, IDC_txtButton4);
	m_txtButton4.SetDefaultFont(m_font);
	m_txtButton4.SetWindowText(m_App->buttonName[3]);
	//m_font.lfUnderline = false;

	m_NoticeType.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY | BS_OWNERDRAW, CRect(30, 152, 54, 176), this, IDC_noticeType);

	m_NoticeContent.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(63, 156, 380, 178), this, IDC_noticeContent);
	m_font.lfHeight = 18;
	m_NoticeContent.SetDefaultFont(m_font);
	if (m_App->UI_Type == L"A")
	{
		m_txtButton1.SetBkgnd(RGB(242, 242, 242));
		m_txtButton1.SetTextColor(RGB(104, 84, 152));
		m_txtButton2.SetBkgnd(RGB(242, 242, 242));
		m_txtButton2.SetTextColor(RGB(104, 84, 152));
		m_txtButton3.SetBkgnd(RGB(242, 242, 242));
		m_txtButton3.SetTextColor(RGB(104, 84, 152));
		m_txtButton4.SetBkgnd(RGB(242, 242, 242));
		m_txtButton4.SetTextColor(RGB(104, 84, 152));
		m_NoticeType.SetBkgnd(RGB(255, 255, 255));
		m_NoticeContent.SetTextColor(RGB(104, 84, 152));
		m_NoticeContent.SetBkgnd(RGB(255, 255, 255));
	}
	else if (m_App->UI_Type == L"B")
	{
		m_txtButton1.SetBkgnd(RGB(38, 38, 38));
		m_txtButton1.SetTextColor(RGB(250, 250, 250));
		m_txtButton2.SetBkgnd(RGB(38, 38, 38));
		m_txtButton2.SetTextColor(RGB(250, 250, 250));
		m_txtButton3.SetBkgnd(RGB(38, 38, 38));
		m_txtButton3.SetTextColor(RGB(250, 250, 250));
		m_txtButton4.SetBkgnd(RGB(38, 38, 38));
		m_txtButton4.SetTextColor(RGB(250, 250, 250));
		m_NoticeType.SetBkgnd(RGB(53, 51, 51));
		m_NoticeContent.SetTextColor(RGB(210, 210, 210));
		m_NoticeContent.SetBkgnd(RGB(53, 51, 51));
	}


	m_ASType[0].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY | BS_OWNERDRAW, CRect(30, 240, 60, 259), this, IDC_ASType1);
	m_ASType[1].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY | BS_OWNERDRAW, CRect(30, 270, 60, 289), this, IDC_ASType2);
	m_ASType[2].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY | BS_OWNERDRAW, CRect(30, 300, 60, 319), this, IDC_ASType3);
	m_ASType[3].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY | BS_OWNERDRAW, CRect(30, 330, 60, 349), this, IDC_ASType4);
	if (m_App->UI_Type == L"A")
	{
		for (int i = 0; i <= 3; i++)
			m_ASType[i].SetBkgnd(RGB(255, 255, 255));
	}
	else if (m_App->UI_Type == L"B")
	{
		for (int i = 0; i <= 3; i++)
			m_ASType[i].SetBkgnd(RGB(53, 51, 51));
	}

	m_receiptWriter[0].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(70, 243, 110, 260), this, IDC_receiptWriter1);
	m_receiptWriter[1].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(70, 273, 110, 290), this, IDC_receiptWriter2);
	m_receiptWriter[2].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(70, 303, 110, 320), this, IDC_receiptWriter3);
	m_receiptWriter[3].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(70, 333, 110, 350), this, IDC_receiptWriter4);

	m_receiptSubject[0].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(120, 243, 300, 260), this, IDC_receiptSubject1);
	m_receiptSubject[1].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(120, 273, 300, 290), this, IDC_receiptSubject2);
	m_receiptSubject[2].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(120, 303, 300, 320), this, IDC_receiptSubject3);
	m_receiptSubject[3].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(120, 333, 300, 350), this, IDC_receiptSubject4);

	m_receiptDate[0].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(320, 243, 389, 260), this, IDC_receiptDate1);
	m_receiptDate[1].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(320, 273, 389, 290), this, IDC_receiptDate2);
	m_receiptDate[2].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(320, 303, 389, 320), this, IDC_receiptDate3);
	m_receiptDate[3].Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(320, 333, 389, 350), this, IDC_receiptDate4);

	m_font.lfHeight = 15;
	if (m_App->UI_Type == L"A")
	{
		for (int i = 0; i <= 3; i++)
		{
			m_receiptWriter[i].SetDefaultFont(m_font);
			m_receiptWriter[i].SetTextColor(RGB(120, 120, 120));
			m_receiptWriter[i].SetBkgnd(RGB(255, 255, 255));

			m_receiptSubject[i].SetDefaultFont(m_font);
			m_receiptSubject[i].SetTextColor(RGB(30, 30, 30));
			m_receiptSubject[i].SetBkgnd(RGB(255, 255, 255));

			m_receiptDate[i].SetDefaultFont(m_font);
			m_receiptDate[i].SetTextColor(RGB(140, 140, 140));
			m_receiptDate[i].SetBkgnd(RGB(255, 255, 255));
		}
	}
	else if (m_App->UI_Type == L"B")
	{
		for (int i = 0; i <= 3; i++)
		{
			m_receiptWriter[i].SetDefaultFont(m_font);
			m_receiptWriter[i].SetTextColor(RGB(171, 171, 171));
			m_receiptWriter[i].SetBkgnd(RGB(53, 51, 51));

			m_receiptSubject[i].SetDefaultFont(m_font);
			m_receiptSubject[i].SetTextColor(RGB(240, 240, 240));
			m_receiptSubject[i].SetBkgnd(RGB(53, 51, 51));

			m_receiptDate[i].SetDefaultFont(m_font);
			m_receiptDate[i].SetTextColor(RGB(150, 150, 150));
			m_receiptDate[i].SetBkgnd(RGB(53, 51, 51));
		}
	}

	int receiptCount = min(m_App->receiptNo, 4); // 최대 4개의 항목만 생성
	for (int i = 0; i < receiptCount; i++) {
		if (m_App->receiptCodeName[i].Find(_T("등록")) != -1) 
			m_ASType[i].SetBkgndImage(MAKEINTRESOURCE(IDB_btnAS1), _T("PNG"));
		else if (m_App->receiptCodeName[i].Find(_T("승인")) != -1)
			m_ASType[i].SetBkgndImage(MAKEINTRESOURCE(IDB_btnAS2), _T("PNG"));
		else if (m_App->receiptCodeName[i].Find(_T("진행")) != -1)
			m_ASType[i].SetBkgndImage(MAKEINTRESOURCE(IDB_btnAS3), _T("PNG"));
		else if (m_App->receiptCodeName[i].Find(_T("완료")) != -1)
			m_ASType[i].SetBkgndImage(MAKEINTRESOURCE(IDB_btnAS4), _T("PNG"));

		CString strSubject;
		int strLen = m_App->receiptSubject[i].GetLength();
		if (strLen > 15)
			strSubject = m_App->receiptSubject[i].Left(15) + L"...";
		else
			strSubject = m_App->receiptSubject[i];

		m_receiptWriter[i].SetWindowText(m_App->receiptWriter[i]);
		m_receiptSubject[i].SetWindowText(strSubject);
		m_receiptDate[i].SetWindowText(m_App->receiptDate[i]);
	}

	if (m_App->receiptNo == 0) {
		m_receiptSubject[0].SetDefaultFont(m_font);
		m_receiptSubject[0].SetWindowText(L"접수된 내용이 없습니다.");
	}
}


void ReceptionDlg::loadLogo()
{
	CString imgPath = L"C:\\Program Files (x86)\\METASTORY\\CareCon\\" + m_App->LogoImage;
	
	if (FAILED(m_LogoImage.Load(imgPath))) {
		//AfxMessageBox(_T("고객 로고 이미지 불러오기 실패!"));
		bLoadLogo = false;
		return;
	}
	else {
		bLoadLogo = true;
		if (m_App->bCompanyLogo)	// 학교 로고가 있는 경우
			m_ctrlSchool.ShowWindow(true);
		//Invalidate(); 
	}
}


void ReceptionDlg::drawBanner()
{
	CString bannerImg;

	m_ctrlBanner.ShowWindow(true);

	if (m_App->bannerNum >= 1 && nBannerNo == 0)
	{
		bannerImg.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[0].GetString());
		if (m_App->bannerNum > 1)
			nBannerNo = 1;
	}
	else if (m_App->bannerNum >= 1 && nBannerNo == 1)
	{
		bannerImg.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[1].GetString());
		if (m_App->bannerNum > 2)
			nBannerNo = 2;
		else
			nBannerNo = 0;
	}
	else if (m_App->bannerNum >= 1 && nBannerNo == 2)
	{
		bannerImg.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[2].GetString());
		nBannerNo = 0;
	}
	else if (m_App->bannerNum <= 0)  // default banner
	{
		bannerImg = L"C:\\Program Files (x86)\\METASTORY\\CareCon\\banner.png";
		nBannerNo = 3;
	}

	HRESULT hr = m_bannerImage.Load(bannerImg);

	if (FAILED(hr))
	{
		//AfxMessageBox(_T("배너 이미지 로딩 실패"));
		return;
	}

	// Picture Control에 이미지 설정
	m_ctrlBanner.ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);

	int x = 22, y = 402;
	int width = m_bannerImage.GetWidth();
	int height = m_bannerImage.GetHeight();
	m_ctrlBanner.MoveWindow(x, y, width, height, TRUE);

	HBITMAP hBmp = (HBITMAP)m_bannerImage.Detach();
	m_ctrlBanner.SetBitmap(hBmp);

	SetTimer(tBannerSec, 3000, NULL);
}


void ReceptionDlg::loadNotice()
{
	if (m_App->noticeNum >= 2)
	{
		if (nNoticeNo == 0)
		{
			if (m_App->noticeType[0] == L"0")
				m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeLow), _T("PNG"));
			else if (m_App->noticeType[0] == L"1")
				m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeMiddle), _T("PNG"));
			else if (m_App->noticeType[0] == L"2")
				m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeHigh), _T("PNG"));
			m_NoticeType.ShowWindow(true);

			m_NoticeContent.SetWindowText(m_App->noticeContent[0]);
			nNoticeNo = 1;
		}
		else if (nNoticeNo == 1)
		{
			if (m_App->noticeType[1] == L"0")
				m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeLow), _T("PNG"));
			else if (m_App->noticeType[1] == L"1")
				m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeMiddle), _T("PNG"));
			else if (m_App->noticeType[1] == L"2")
				m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeHigh), _T("PNG"));
			m_NoticeType.ShowWindow(true);

			m_NoticeContent.SetWindowText(m_App->noticeContent[1]);
			nNoticeNo = 0;
		}
		SetTimer(tNoticeSec, 3000, NULL);
	}
	else if (m_App->noticeNum == 1)  // 공지사항이 1개만 있는 경우
	{
		if (m_App->noticeType[0] == L"0")
			m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeLow), _T("PNG"));
		else if (m_App->noticeType[0] == L"1")
			m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeMiddle), _T("PNG"));
		else if (m_App->noticeType[0] == L"2")
			m_NoticeType.SetBkgndImage(MAKEINTRESOURCE(IDB_typeHigh), _T("PNG"));

		m_NoticeContent.SetWindowText(m_App->noticeContent[0]);

	}
	else
	{
		m_NoticeContent.SetWindowText(L"등록된 공지 내용이 없습니다.");
		m_NoticeType.ShowWindow(false);
	}

	CRect Rect;
	m_NoticeType.GetWindowRect(&Rect);
	ScreenToClient(&Rect);
	InvalidateRect(Rect);

}


LRESULT ReceptionDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DefWindowProc(message, wParam, lParam);
}


BOOL ReceptionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}


void ReceptionDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CString url;

	// 배너 영역
	if (m_rcPictureControl.PtInRect(point))		
	{
		if (nBannerNo == 1)
		{
			m_App->linkURL(m_App->bannerURL[0]);
		}
		else
		{
			m_App->linkURL(m_App->bannerURL[1]);
		}
	}

	// 버튼 영역
	else if (m_rcButton1.PtInRect(point))		
	{
		buttonURL(0);
	}
	else if (m_rcButton2.PtInRect(point))
	{
		buttonURL(1);
	}
	else if (m_rcButton3.PtInRect(point))
	{
		buttonURL(2);
	}
	else if (m_rcButton4.PtInRect(point))
	{
		buttonURL(3);
	}

	// 공지사항 영역
	else if (m_rcNotice.PtInRect(point))	
	{
		if (nNoticeNo == 0 && m_App->noticeNum >= 2)
			m_App->linkURL(m_App->noticeURL[0]);
		else if (nNoticeNo == 1 && m_App->noticeNum >= 2)
			m_App->linkURL(m_App->noticeURL[1]);
		else
			m_App->linkURL(m_App->noticeURL[0]);
	}

	// 더보기 영역
	else if (m_rcMore.PtInRect(point))	
	{
		url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}

	// 처리내역 영역
	else if (m_rcReceiptSubject1.PtInRect(point))
	{
		if (m_App->receiptSubject[0] != L"")
		{
			url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s&receipt_idx=%s&sub_type=viewform",
				m_App->comp_idx.GetString(), m_App->activeMacCode.GetString(), m_App->receiptIdx[0].GetString());
			m_App->linkURL(url);
		}
	}
	else if (m_rcReceiptSubject2.PtInRect(point))
	{
		if (m_App->receiptSubject[1] != L"")
		{
			url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s&receipt_idx=%s&sub_type=viewform",
				m_App->comp_idx.GetString(), m_App->activeMacCode.GetString(), m_App->receiptIdx[1].GetString());
			m_App->linkURL(url);
		}
	}
	else if (m_rcReceiptSubject3.PtInRect(point))
	{
		if (m_App->receiptSubject[2] != L"")
		{
			url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s&receipt_idx=%s&sub_type=viewform",
				m_App->comp_idx.GetString(), m_App->activeMacCode.GetString(), m_App->receiptIdx[2].GetString());
			m_App->linkURL(url);
		}
	}
	else if (m_rcReceiptSubject4.PtInRect(point))
	{
		if (m_App->receiptSubject[3] != L"")
		{
			url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s&receipt_idx=%s&sub_type=viewform",
				m_App->comp_idx.GetString(), m_App->activeMacCode.GetString(), m_App->receiptIdx[3].GetString());
			m_App->linkURL(url);
		}
	}
	else if (m_rReset.PtInRect(point))
	{
		int nResp = MessageBox(L"정말 학교등록 정보를 초기화하시겟습니까?", _T("CareConn"), MB_ICONWARNING | MB_YESNO);
		if (nResp == IDYES)
		{
			// 키 삭제 대신 값만 초기화 (키 삭제 시 Windows Installer 셀프 힐링 발동)
			SetRegistryIntValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Count", 0);
			SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"carecon_comp_code", L"");
			SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"comp_code", L"");
			SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Company", L"");
			SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"manager", L"");
			SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"place", L"");
			SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"MacAddr1", L"");

			EndDialog(IDOK);

			CregistryDlg regDlg;
			regDlg.DoModal();
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void ReceptionDlg::buttonURL(int ndx)
{
	CString url;

	if (m_App->buttonType[ndx] == L"1")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/cboard.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (m_App->buttonType[ndx] == L"2")
	{
		m_App->linkURL(m_App->buttonURL[ndx]);
	}
	else if (m_App->buttonType[ndx] == L"3")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/bnotice.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (m_App->buttonType[ndx] == L"4")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/consult.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (ndx == 0 && m_App->buttonName[0] == L"접수하기")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s&sub_type=postform", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (ndx == 1 && m_App->buttonName[1] == L"알림게시판")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/bnotice.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (ndx == 2 && m_App->buttonName[2] == L"상담게시판")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/consult.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (ndx == 3 && m_App->buttonName[3] == L"누리집보기")
	{
		m_App->linkURL(m_App->homepage);
	}
	else if (ndx == 0 && m_App->buttonName[0] == L"알림게시판")
	{
		url.Format(L"https://new.bizstory.co.kr/agent/bnotice.php?comp_idx=%s&macaddress=%s", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
		m_App->linkURL(url);
	}
	else if (ndx == 1 && m_App->buttonName[1] == L"누리집보기")
	{
		m_App->linkURL(m_App->homepage);
	}

}


void ReceptionDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case tBannerSec:
		KillTimer(tBannerSec);

		if (!bLoadLogo) {
			// loading Logo Image
			loadLogo();
		}
		drawBanner();
		break;

	case tNoticeSec:
		KillTimer(tNoticeSec);
		loadNotice();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void ReceptionDlg::OnBnClickedReception()
{
	CString url;

	url.Format(L"https://new.bizstory.co.kr/agent/receipt.php?comp_idx=%s&macaddress=%s&sub_type=postform", m_App->comp_idx.GetString(), m_App->activeMacCode.GetString());
	m_App->linkURL(url);
}


void ReceptionDlg::OnBnClickedExit()
{
	exit(1);
}


void ReceptionDlg::OnStnClickedSchool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
