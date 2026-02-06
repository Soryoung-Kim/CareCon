// CregistryDlg.cpp: 구현 파일
//

#include "pch.h"
#include "CareConn.h"
#include "afxdialogex.h"
//#include "CregistryDlg.h"
#include "ReceptionDlg.h"
#include "mysql.h"
#include "../../../Program Files/MySQL/MySQL Server 8.0/include/mysql.h"

// CregistryDlg 대화 상자

IMPLEMENT_DYNAMIC(CregistryDlg, CDialogEx)

extern CCareConnApp* m_App;
CregistryDlg::CregistryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RegistryDlg, pParent)
{

}

CregistryDlg::~CregistryDlg()
{
}

void CregistryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_lstComp, m_lstComp);
	DDX_Control(pDX, IDC_edtName, m_edtName);
	DDX_Control(pDX, IDC_txtComp_name, m_txtComp_name);
	DDX_Control(pDX, IDC_txtSc_name, m_txtSc_name);
	DDX_Control(pDX, IDC_txtTel_num, m_txtTel_num);
	DDX_Control(pDX, IDC_txtComp_code, m_txtComp_code);
	DDX_Control(pDX, IDC_edtUser, m_edtUser);
	DDX_Control(pDX, IDC_edtPlace, m_edtPlace);
}


BEGIN_MESSAGE_MAP(CregistryDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CregistryDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_btnExit, &CregistryDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_btnSearch, &CregistryDlg::OnBnClickedbtnsearch)
	//ON_BN_CLICKED(IDC_COMPANYLOGO, &CregistryDlg::OnBnClickedCompanyLogo)
	ON_NOTIFY(LVN_KEYDOWN, IDC_lstComp, &CregistryDlg::OnKeydownLstcomp)
	ON_NOTIFY(NM_CLICK, IDC_lstComp, &CregistryDlg::OnClickLstcomp)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_lstComp, &CregistryDlg::OnCustomdrawLstcomp)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE, &CregistryDlg::OnDownloadComplete)
	ON_MESSAGE(WM_DOWNLOAD_FAILED, &CregistryDlg::OnDownloadFailed)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CregistryDlg 메시지 처리기


BOOL CregistryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CreateControl();
	DrawBackground();

	DWORD dwStyle = m_lstComp.GetExtendedStyle();
	dwStyle &= ~LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT; // LVS_EX_GRIDLINES를 포함하지 않음
	m_lstComp.SetExtendedStyle(dwStyle);

	m_lstComp.ModifyStyle(0, LVS_NOCOLUMNHEADER);
	m_lstComp.InsertColumn(0, NULL, LVCFMT_LEFT, 120);
	m_lstComp.InsertColumn(1, NULL, LVCFMT_LEFT, 140);
	m_lstComp.InsertColumn(2, NULL, LVCFMT_LEFT, 100);
	m_lstComp.InsertColumn(3, L"", LVCFMT_LEFT, 0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CregistryDlg::OnPaint()
{
	CPaintDC dc(this); // 화면에 그릴 DC

	CRect rc;
	GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = memDC.SelectObject(&m_BkgndBmp);

	// 배경 이미지 그리기
	memDC.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	// 최종 이미지 화면에 복사
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
}


HBRUSH CregistryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	COLORREF bgColor;

	if (pWnd->GetDlgCtrlID() == IDC_txtSc_name || 
		pWnd->GetDlgCtrlID() == IDC_txtComp_name || 
		pWnd->GetDlgCtrlID() == IDC_txtTel_num )		 
	{
		// 배경색 설정 (예: 연한 회색)
		if (m_App->UI_Type == L"A")
		{
			bgColor = RGB(249, 249, 249);
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (m_App->UI_Type == L"B")
		{
			bgColor = RGB(49, 48, 48);
			pDC->SetTextColor(RGB(250, 250, 250));
		}
		pDC->SetBkColor(bgColor);

		// 배경 브러시 반환
		static CBrush brush(bgColor);
		hbr = brush;
	}
	else if (pWnd->GetDlgCtrlID() == IDC_edtName ||
			pWnd->GetDlgCtrlID() == IDC_edtUser ||
			pWnd->GetDlgCtrlID() == IDC_edtPlace)
	{
		if (m_App->UI_Type == L"A")
		{
			bgColor = RGB(255, 255, 255);
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (m_App->UI_Type == L"B")
		{
			bgColor = RGB(53, 51, 51);
			pDC->SetTextColor(RGB(250, 250, 250));
		}
		pDC->SetBkColor(bgColor);

		// 배경 브러시 반환
		static CBrush brush(bgColor);
		hbr = brush;
	}
	else if (pWnd->GetDlgCtrlID() == IDC_txtVersion)
	{
		if (m_App->UI_Type == L"A")
		{
			bgColor = RGB(249, 249, 249);
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


void CregistryDlg::CreateControl()
{
	// LOGFONT 구조체 설정
	int nFontSize = 12; // 20pt
	CString strFontName = L"맑은고딕";

	CDC* pDC = GetDC();
	int nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);
	int nFontHeight = -MulDiv(nFontSize, nLogPixelsY, 72);
	ReleaseDC(pDC);

	LOGFONT lf = { 0 };
	lf.lfHeight = nFontHeight;
	lf.lfWeight = FW_NORMAL; // 일반 굵기
	_tcscpy_s(lf.lfFaceName, strFontName);

	m_edtFont.CreateFontIndirect(&lf);

	// edit control 속성 설정
	m_edtName.ModifyStyle(WS_BORDER, 0);
	m_edtName.SetFont(&m_edtFont);

	m_edtUser.ModifyStyle(WS_BORDER, 0);
	m_edtUser.SetFont(&m_edtFont);

	m_edtPlace.ModifyStyle(WS_BORDER, 0);
	m_edtPlace.SetFont(&m_edtFont);

	memset(&m_font, 0x00, sizeof(m_font));
	_tcscpy_s(m_font.lfFaceName, _countof(m_font.lfFaceName), _T("굴림체"));

	m_btnExit.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(390, 6, 414, 30), this, IDC_btnExit);
	if (m_App->UI_Type == L"A")
		m_btnExit.SetNormalImage(IDB_btnExitA, true);
	else if (m_App->UI_Type == L"B")
		m_btnExit.SetNormalImage(IDB_btnExitB, true);

	if (m_App->UI_Company == L"CARECON")
	{
		mbtnLogo.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(15, 15, 161, 57), this, IDC_COMPANYLOGO);
		mbtnLogo.SetNormalImage(IDB_CareCon, true);
		m_txtVer.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(170, 29, 250, 44), this, IDC_txtVersion);
	}
	else if (m_App->UI_Company == L"INET")
	{
		mbtnLogo.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(15, 15, 161, 57), this, IDC_COMPANYLOGO);
		mbtnLogo.SetNormalImage(IDB_INET, true);
		m_txtVer.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(170, 29, 250, 44), this, IDC_txtVersion);
	}
	else if (m_App->UI_Company == L"LUCOMS")
	{
		mbtnLogo.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(15, 15, 109, 43), this, IDC_COMPANYLOGO);
		mbtnLogo.SetNormalImage(IDB_LUCOMS, true);
		m_txtVer.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(114, 29, 190, 44), this, IDC_txtVersion);
	}
	mbtnLogo.ShowWindow(true);

	if (m_App->UI_Type == L"A")
	{
		m_txtVer.SetTextColor(RGB(10, 12, 10));
		m_txtVer.SetBkgnd(RGB(249, 249, 249));
	}
	else if (m_App->UI_Type == L"B")
	{
		m_txtVer.SetTextColor(RGB(215, 235, 215));
		m_txtVer.SetBkgnd(RGB(32, 38, 50));
	}
	m_font.lfHeight = 11;
	m_txtVer.SetDefaultFont(m_font);
	m_txtVer.SetWindowText(L"v1.1.4");


	m_btnSearch.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(330, 100, 406, 140), this, IDC_btnSearch);
	m_btnSearch.SetNormalImage(IDB_btnSearch, true);
	m_btnSearch.ShowWindow(true);

	m_btnRegistry.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, CRect(149, 479, 269, 527), this, IDOK);
	m_btnRegistry.SetNormalImage(IDB_btnRegistry, true);
	m_btnRegistry.ShowWindow(true);

}


void CregistryDlg::DrawBackground()
{
	m_BkgndBmp.DeleteObject();

	CDC* pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	if (m_App->UI_Type == L"A")
		m_bgRegistry.Load(MAKEINTRESOURCE(IDB_bgRegistryA), _T("PNG"));
	else if (m_App->UI_Type == L"B")
		m_bgRegistry.Load(MAKEINTRESOURCE(IDB_bgRegistryB), _T("PNG"));
	

	m_BkgndBmp.CreateCompatibleBitmap(pDC, m_bgRegistry.GetWidth(), m_bgRegistry.GetHeight());
	CBitmap* pOldBmp = memDC.SelectObject(&m_BkgndBmp);

	Graphics graphics(memDC);

	graphics.DrawImage(&m_bgRegistry, 0, 0, m_bgRegistry.GetWidth(), m_bgRegistry.GetHeight());

	memDC.SelectObject(pOldBmp);
	ReleaseDC(pDC);
}


LRESULT CregistryDlg::OnNcHitTest(CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint ptClient = point;
	ScreenToClient(&ptClient);

	CRect dragArea(0, 0, 419, 546); // 예: 좌측 상단의 200x50 영역

	if (dragArea.PtInRect(ptClient))
	{
		return HTCAPTION;
	}
	else
	{
		return CDialogEx::OnNcHitTest(point);
	}
}


// 사용자 검색
void CregistryDlg::OnBnClickedbtnsearch()
{
	CString mClass, mName;

	m_lstComp.DeleteAllItems();

	m_edtName.GetWindowText(mName);

	if (mName == _T(""))
	{
		AfxMessageBox(L"검색어를 입력해주시기 바랍니다.");
		return;
	}
	LoadCompanyInfo('1', mName);

}


void CregistryDlg::OnKeydownLstcomp(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


// mySQL DB Connect
BOOL CregistryDlg::ConnectDB()
{
	mysql_init(&conn);

	CT2CA strHost(m_App->DBHost);
	const char* host = strHost;
	mysqlConn = mysql_real_connect(&conn, host, DBUser, DBPass, DBName, m_App->DBPort, (char*)NULL, 0);
	if (mysqlConn == NULL)
	{
		AfxMessageBox(L"Database Server에 연결되지 않았습니다");
		LogWrite(L"MySQL", L"Database Server에 연결되지 않았습니다");
		return false;
	}

	mysql_query(&conn, "set Names euckr");

	LogWrite(L"MySQL", L"Database Server 접속 성공!");

	return true;
}


void CregistryDlg::DisconnectDB(void)
{
	mysql_close(mysqlConn);
	mysqlConn = NULL;
	LogWrite(L"MySQL", L"Database Server disconnected");

}


void CregistryDlg::LoadCompanyInfo(char cls, CString companyName)
{
	CString strQuery;
	char* strquery;
	int nCnt = 0, resultMsg;

	if (!ConnectDB())
	{
		AfxMessageBox(L"Database가 연결되지 않았습니다.");
		LogWrite(L"MySQL", L"Database에서 제품 정보를 가져오지 못했습니다");
		return;
	}

	companyName = _T("%") + companyName + _T("%");
	// Loading 시도교육청 이름(sc_name), 기업/학교명(comp_name),  ----------------------------------------------------------------------------------------------------------
	if (cls == '\0')
		strQuery.Format(L"SELECT sc_name, comp_name, tel_num, comp_code FROM %s.bizstory.company_info", m_App->dbName);
	else if (cls == '\0' && companyName != "")
		strQuery.Format(L"SELECT sc_name, comp_name, tel_num, comp_code FROM %s.company_info where comp_name like '%s'", m_App->dbName, companyName);
	else
		strQuery.Format(L"SELECT sc_name, comp_name, tel_num, comp_code FROM %s.company_info where comp_class = '%c' and comp_name like '%s'", m_App->dbName, cls, companyName);

	USES_CONVERSION;

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		return;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : Company_info mysql_store_result");
		return;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		int nDx = m_lstComp.GetItemCount();
		ListAddItem(nDx, A2W(sql_row[0]), A2W(sql_row[1]), A2W(sql_row[2]), A2W(sql_row[3]));
		//nCnt++;
	}

	mysql_free_result(sql_result);
}


BOOL CregistryDlg::CheckMacAddr()
{
	CString strQuery, macAddr1, macAddr2;
	char* strquery;
	int nCnt = 0, resultMsg;

	strQuery.Format(L"SELECT macaddress1, macaddress2 FROM %s.agent_info where agent_comp_code='%s'", m_App->dbName, m_App->agent_compCode);

	USES_CONVERSION;

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 CareConn(agent_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 CareConn 정보(agent_info)를 가져오지 못했습니다");
		return false;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 CareConn(agent_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : agent_info mysql_store_result");
		return false;
	}

	while (sql_row = mysql_fetch_row(sql_result))
	{
		macAddr1 = sql_row[0];
		macAddr2 = sql_row[1];
	}

	mysql_free_result(sql_result);

	if (m_App->activeMacCode == macAddr1 || m_App->activeMacCode == macAddr2)
		return true;
	else
	{
		if (macAddr2 == "" || sql_row[1] == NULL)
		{
			if (!Update_MacAddr2(macAddr2))
				AfxMessageBox(L"오류 : 맥어드레스 2nd 데이타를 등록하지 못했습니다!");
		}
		return false;
	}
}


void CregistryDlg::ListAddItem(int nDx, CString scName, CString compName, CString telNum, CString code)
{
	LVITEM lvitem;

	ZeroMemory(&lvitem, sizeof(LVITEM));

	lvitem.iItem = nDx;
	lvitem.mask = LVIF_TEXT;

	lvitem.iSubItem = 0;
	lvitem.pszText = scName.GetBuffer();
	m_lstComp.InsertItem(&lvitem);

	lvitem.iSubItem = 1;
	lvitem.pszText = compName.GetBuffer();
	m_lstComp.SetItem(&lvitem);

	lvitem.iSubItem = 2;
	lvitem.pszText = telNum.GetBuffer();
	m_lstComp.SetItem(&lvitem);

	lvitem.iSubItem = 3;
	lvitem.pszText = code.GetBuffer();
	m_lstComp.SetItem(&lvitem);
}


void CregistryDlg::OnClickLstcomp(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nSelectedCel = pNMListView->iItem;

	if (nSelectedCel != -1)
	{
		sc_name = m_lstComp.GetItemText(nSelectedCel, 0);
		m_App->companyName = m_lstComp.GetItemText(nSelectedCel, 1);
		tel_num = m_lstComp.GetItemText(nSelectedCel, 2);
		comp_code = m_lstComp.GetItemText(nSelectedCel, 3);

		m_txtSc_name.SetWindowText(sc_name);
		m_txtComp_name.SetWindowText(m_App->companyName);
		m_txtTel_num.SetWindowText(tel_num);
		m_txtComp_code.SetWindowText(comp_code);
	}

	*pResult = 0;
}


void CregistryDlg::OnCustomdrawLstcomp(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// 글자색 설정.
		CDC* pDC = CDC::FromHandle(pNMCD->hdc);
		pDC->SetTextColor(RGB(255, 0, 0));
		*pResult = CDRF_NEWFONT;
	}
}

/*
void CregistryDlg::OnNMCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		pLVCD->clrText = RGB(255, 255, 255);     // 텍스트 색상 (흰색)
		pLVCD->clrTextBk = RGB(0, 0, 0);         // 배경 색상 (검정색)
		*pResult = CDRF_DODEFAULT;
	}
	else
	{
		*pResult = CDRF_DODEFAULT;
	}
}*/


// 사용자 등록
void CregistryDlg::OnBnClickedOk()
{
	CString strQuery, strQuery1, strQuery2;
	CString mName, mUser, mPlace;
	char* strquery;
	int resultMsg;

	CTime today = CTime::GetCurrentTime();

	m_edtUser.GetWindowText(mUser);
	m_App->manager = mUser;
	m_edtPlace.GetWindowText(mPlace);
	m_App->place = mPlace;

	//if (mUser == L"" || mPlace == L"")
	//{
	//	AfxMessageBox(L"담당자명 또는 설치장소 입력이 되지 않았습니다.");
	//	m_edtUser.SetFocus();
	//	return;
	//}

	m_edtName.GetWindowText(mName);
	if (mName == _T(""))
	{
		AfxMessageBox(L"학교명이 입력되지 되지 않았습니다.");
		m_edtName.SetFocus();
		return;
	}

	int itemCount = m_lstComp.GetItemCount();
	if (itemCount <= 0)
	{
		OnBnClickedbtnsearch();
		return;
	}

	CString EduOffice, SchoolName;
	m_txtSc_name.GetWindowText(EduOffice);
	m_txtComp_name.GetWindowText(SchoolName);
	if (EduOffice == _T("") && SchoolName == _T(""))
	{
		AfxMessageBox(L"학교 정보가 선택되지 않았습니다.");
		m_lstComp.SetFocus();
		return;
	}

	int nResp = MessageBox(L"선택하신 사용자 정보를 등록하시겠습니까?", _T("CareConn"), MB_ICONWARNING | MB_YESNO);
	if (nResp == IDYES)
	{
		USES_CONVERSION;

		// comp_code와 mac-addr이 동일한 정보가 있는 경우 pass ---------------------------------------------------------------------------------------------
		strQuery.Format(L"SELECT agent_comp_code FROM %s.agent_info where comp_code='%s' and (macaddress1='%s' or macaddress2='%s')",
			m_App->dbName, comp_code, m_App->activeMacCode, m_App->activeMacCode);
		strquery = W2A(strQuery);
		resultMsg = mysql_query(&conn, strquery);

		if (resultMsg != 0)
		{
			AfxMessageBox(L"Database에서 CareConn(agent_info)를 가져오지 못했습니다");
			LogWrite(L"MySQL Error", L"Database에서 CareConn 정보(agent_info)를 가져오지 못했습니다");
			return;
		}

		if ((sql_result = mysql_store_result(&conn)) == NULL)
		{
			AfxMessageBox(L"Database에서 CareConn(agent_info)를 가져오지 못했습니다");
			LogWrite(L"MySQL Error", L"Database Error : agent_info mysql_store_result");
			return;
		}

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row != NULL)
		{
			//AfxMessageBox(L"동일한 고객 정보가 등록되어 있습니다! 관리자에게 문의주시기 바랍니다!");
			mysql_free_result(sql_result);

			// update del_yn -> y 
			strQuery.Format(L"UPDATE %s.agent_info SET del_yn = 'Y', ver_info = '1.1.4' WHERE macaddress1='%s' or macaddress2='%s';",
				m_App->dbName, m_App->activeMacCode, m_App->activeMacCode);

			if (!WriteDBQuery(strQuery)) {
				AfxMessageBox(L"Failed to DB Update : sales");
				return;
			}
		}

		mysql_free_result(sql_result);
		// comp_code와 mac-addr이 동일한 정보가 있는 경우 pass ---------------------------------------------------------------------------------------------


		m_App->agent_compCode.Format(L"%s-%04d%02d%02d%02d%02d%02d", comp_code,
			today.GetYear(), today.GetMonth(), today.GetDay(), today.GetHour(), today.GetMinute(), today.GetSecond());

		regDate.Format(L"%04d-%02d-%02d %02d:%02d:%02d",
			today.GetYear(), today.GetMonth(), today.GetDay(),
			today.GetHour(), today.GetMinute(), today.GetSecond());
		lastDate = regDate;

		// CareCon_Version
		strQuery1.Format(L"INSERT INTO %s.agent_info(comp_code, ver_info, agent_comp_code, macaddress1, reg_date, last_date)", m_App->dbName); // , user, install_place)", m_App->dbName);
		strQuery2.Format(L" VALUES('%s','%s','%s','%s','%s','%s');", //, '%s', '%s');",
			comp_code, m_App->strVersion, m_App->agent_compCode, m_App->activeMacCode, regDate, lastDate);// , mUser, mPlace);
		strQuery = strQuery1 + strQuery2;

		if (!WriteDBQuery(strQuery)) {
			AfxMessageBox(L"Failed to DB write : 사용자등록");
			return;
		}

		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"comp_code", comp_code);
		m_App->comp_code = comp_code;
		SetRegistryIntValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Count", 1);
		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"agent_comp_code", m_App->agent_compCode);
		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"version", m_App->strVersion);
		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"MacAddr1", m_App->activeMacCode);
		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"Company", m_App->companyName);
		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"manager", m_App->manager);
		SetRegistryStrValue(REG_USER, L"Software\\METASTORY\\CareCon", L"place", m_App->place);
		AfxMessageBox(L"사용자 등록이 완료되었습니다!");

		EndDialog(IDOK);

		ReceptionDlg recepDlg;
		if (LoadConfigure())
		{
			//CString tst;
			//tst.Format(L"%s %s %s", comp_code, m_App->agent_compCode, m_App->companyName);
			//AfxMessageBox(tst);
			INT_PTR nResponse = recepDlg.DoModal();
			if (nResponse == IDOK)
			{
			}
		}
	}
	else
		return;

	CDialogEx::OnOK();
}


//void CregistryDlg::OnBnClickedCompanyLogo()
//{}


BOOL CregistryDlg::Update_MacAddr2(CString macAddr)
{
	CString	strQuery;

	strQuery.Format(L"UPDATE %s.agent_info SET macaddress2 = '%s' WHERE agent_comp_code = '%s';", m_App->dbName, macAddr, m_App->agent_compCode);

	if (!WriteDBQuery(strQuery)) {
		AfxMessageBox(L"Failed to DB Update : sales");
		return false;
	}

	return true;
}

CString CregistryDlg::parseVersion(CString versionStr) {

	int versionParts[3] = { 0, 0, 0 };
	int start = 0;
	int index = 0;
	int dotPos;
	while ((dotPos = versionStr.Find(_T("."), start)) != -1 && index < 2) {
		versionParts[index] = _ttoi(versionStr.Mid(start, dotPos - start));
		start = dotPos + 1;
		index++;
	}
	if (start < versionStr.GetLength()) {
		versionParts[index] = _ttoi(versionStr.Mid(start));
	}
	CString resultStr;
	resultStr.Format(_T("%d.%d.%d"), versionParts[0], versionParts[1], versionParts[2]);
	return resultStr;

}


BOOL CregistryDlg::checkVersion(CString userVersion)
{
	CString strQuery;
	char* strquery;
	int nCnt = 0, resultMsg;

	if (!ConnectDB())
	{
		AfxMessageBox(L"Database가 연결되지 않았습니다.");
		LogWrite(L"MySQL", L"Database에서 제품 정보를 가져오지 못했습니다");
		return true;
	}

	// get version info
	strQuery.Format(L"SELECT ver_info FROM %s.agent_product where end_date IS NULL", m_App->dbName);

	USES_CONVERSION;

	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 버전정보(agent_product)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 버전정보(agent_product)를 가져오지 못했습니다");
		return true;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 버전정보(agent_product)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : agent_product mysql_store_result");
		return true;
	}
	else
	{
		sql_row = mysql_fetch_row(sql_result);

		CString dbVersion, parDbVer, parUserVer, token;

		dbVersion = A2W(sql_row[0]);
	
		parDbVer = parseVersion(dbVersion);
		parUserVer = parseVersion(userVersion);

		int pos, idx;
		
		pos = 0;
		idx = 0;

		int reUserVersion[3] = { 0, 0, 0 };
		token = parUserVer.Tokenize(_T("."), pos);

		while (!token.IsEmpty() && idx < 3) {
			idx++;
			reUserVersion[idx-1] = _ttoi(token);
			token = parUserVer.Tokenize(_T("."), pos);
		}

		pos = 0;
		idx = 0;

		int reDbVersion[3] = { 0, 0, 0 };
		token = parDbVer.Tokenize(_T("."), pos);

		while (!token.IsEmpty() && idx < 3) {
			idx++;
			reDbVersion[idx-1] = _ttoi(token);
			token = parDbVer.Tokenize(_T("."), pos);
		}

		for (int i = 0; i < 3; ++i) {
			if (reDbVersion[i] > reUserVersion[i]) {				
				return false;
			}
		}

		return true;
		
			
	}

}


BOOL CregistryDlg::WriteDBQuery(CString strQuery)
{
	char* strquery;
	int resultMsg;

	USES_CONVERSION;
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Failed MySQL query.\n");
		return false;
	}

	sql_result = mysql_store_result(&conn);
	mysql_free_result(sql_result);

	return true;
}


BOOL CregistryDlg::LoadConfigure()
{
	CString strQuery;
	char* strquery;
	int nCnt = 0, resultMsg;

	if (!ConnectDB())
	{
		AfxMessageBox(L"Database가 연결되지 않았습니다.");
		LogWrite(L"MySQL", L"Database에서 제품 정보를 가져오지 못했습니다");
		return false;
	}

	USES_CONVERSION;

	// get 학교/기업명 정보 -------------------------------------------------------------------------
	strQuery.Format(L"SELECT comp_idx, home_page FROM %s.company_info WHERE del_yn = 'N' and comp_code = '%s'", m_App->dbName, m_App->comp_code);
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		return false;
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 고객정보(Company_info)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : Company_info mysql_store_result");
		return false;
	}

	sql_row = mysql_fetch_row(sql_result);

	if (sql_row != NULL)
	{
		m_App->comp_idx = A2W(sql_row[0]);
		m_App->homepage = A2W(sql_row[1]);
		mysql_free_result(sql_result);
		sql_row = NULL;
	}
	else
	{
		AfxMessageBox(L"고객 정보가 없습니다!");
		mysql_free_result(sql_result);
		return false;
	}
	// get 학교/기업명 정보 -------------------------------------------------------------------------


	// get 학교/기업 로고 이미지 -------------------------------------------------------------------------
	strQuery.Format(L"SELECT img_sname FROM %s.company_file WHERE comp_idx = '%s'", m_App->dbName, m_App->comp_idx);
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 고객파일(company_file)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 고객파일(company_file)를 가져오지 못했습니다");
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 고객파일(company_file)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : company_file mysql_store_result");
	}

	sql_row = mysql_fetch_row(sql_result);

	if (sql_row != NULL)
	{
		if (sql_row[0] != NULL)
		{
			m_App->LogoImage = A2W(sql_row[0]);
			m_App->bCompanyLogo = true;
		}

		ThreadParams* pParams = new ThreadParams;
		pParams->strType = "logo";
		pParams->number = 0;
		AfxBeginThread(DownloadThread, pParams);	// Download Logo Image
	}

	mysql_free_result(sql_result);
	sql_row = NULL;
	// get 학교/기업 로고 이미지 -------------------------------------------------------------------------


	// get 배너 이미지 -------------------------------------------------------------------------
	strQuery.Format(L"SELECT img_sname, link_url, comp_idx FROM %s.carecon_banner WHERE del_yn = 'N' and view_yn = 'Y' and (comp_mult = 'All' or concat(comp_mult, ',') LIKE '%%,%s,%%')", m_App->dbName, m_App->comp_idx);
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 고객배너(carecon_banner)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 고객배너(carecon_banner)를 가져오지 못했습니다");
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 고객배너(carecon_banner)를 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : carecon_banner mysql_store_result");
	}

	int idx = 0;
	while (sql_row = mysql_fetch_row(sql_result))
	{
		idx++;
		m_App->bannerImg[idx-1] = A2W(sql_row[0]);	// image
		m_App->bannerURL[idx-1] = A2W(sql_row[1]);	// url
		m_App->bannerIDX[idx - 1] = A2W(sql_row[2]);	// comp_idx

		if (idx == 2)
			break;
	}

	if (idx > 0)
	{
		ThreadParams* pParams = new ThreadParams;
		pParams->strType = "banner";
		pParams->number = idx;
		m_App->bannerNum = idx;
		AfxBeginThread(DownloadThread, pParams);	// Download Banner Image
	}

	mysql_free_result(sql_result);
	sql_row = NULL;
	// get 배너 이미지 -------------------------------------------------------------------------


	// Page 설정
	strQuery.Format(L"SELECT btn_type, btn_name, link_url FROM %s.carecon_button WHERE del_yn = 'N' and comp_idx = '%s' ORDER BY sort ASC", m_App->dbName, m_App->comp_idx);
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 carecon_button을 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 carecon_button을 가져오지 못했습니다");
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 carecon_button을 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : carecon_button mysql_store_result");
	}

	idx = 0;
	while (sql_row = mysql_fetch_row(sql_result))
	{
		m_App->buttonType[idx] = A2W(sql_row[0]);
		m_App->buttonName[idx] = A2W(sql_row[1]);	
		m_App->buttonURL[idx] = A2W(sql_row[2]);

		if (idx == 3)
			break;

		idx++;
	}

	if (idx == 0)
	{
		//m_App->buttonName[0] = L"접수내역";
		//m_App->buttonName[1] = L"접수하기";
		m_App->buttonName[0] = L"알림게시판";
		m_App->buttonName[1] = L"누리집보기";
	}

	mysql_free_result(sql_result);
	sql_row = NULL;
	// get Page 설정 -------------------------------------------------------------------------


	// 공지사항
	strQuery.Format(L"SELECT import_type, content, link_url FROM %s.carecon_notice WHERE del_yn = 'N' and view_yn = 'Y' ORDER BY sort ASC LIMIT 2", m_App->dbName, m_App->comp_idx);
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 carecon_notice을 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 carecon_notice을 가져오지 못했습니다");
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 carecon_notice을 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : carecon_notice mysql_store_result");
	}

	idx = 0;
	while (sql_row = mysql_fetch_row(sql_result))
	{
		idx++;
		m_App->noticeType[idx-1] = A2W(sql_row[0]);
		m_App->noticeContent[idx-1] = A2W(sql_row[1]);
		m_App->noticeURL[idx-1] = A2W(sql_row[2]);

		if (idx == 2)
			break;
	}

	if (idx > 0)
		m_App->noticeNum = idx;

	mysql_free_result(sql_result);
	sql_row = NULL;
	// 공지사항 -------------------------------------------------------------------------

	// 최근 접수
	strQuery.Format(L"SELECT ri.ri_idx, ri.subject, crs.code_name, ri.writer, ri.reg_date "
					 "FROM %s.receipt_info ri LEFT JOIN %s.code_receipt_status crs "
					 "ON ri.comp_idx = crs.comp_idx AND ri.receipt_status = crs.code_value "
					 "WHERE ri.del_yn = 'N' AND ri.comp_idx = '%s' ORDER BY ri.reg_date DESC LIMIT 4", m_App->dbName, m_App->dbName, m_App->comp_idx);
	strquery = W2A(strQuery);
	resultMsg = mysql_query(&conn, strquery);

	if (resultMsg != 0)
	{
		AfxMessageBox(L"Database에서 receipt_info을 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database에서 receipt_info을 가져오지 못했습니다");
	}

	if ((sql_result = mysql_store_result(&conn)) == NULL)
	{
		AfxMessageBox(L"Database에서 receipt_info을 가져오지 못했습니다");
		LogWrite(L"MySQL Error", L"Database Error : receipt_info mysql_store_result");
	}

	idx = 0;
	while (sql_row = mysql_fetch_row(sql_result))
	{
		idx++;
		m_App->receiptIdx[idx - 1] = A2W(sql_row[0]);
		m_App->receiptSubject[idx - 1] = A2W(sql_row[1]);
		m_App->receiptCodeName[idx - 1] = A2W(sql_row[2]);
		m_App->receiptWriter[idx - 1] = A2W(sql_row[3]);
		CString rawDate, mDate;
		rawDate = A2W(sql_row[4]);
		COleDateTime oleDateTime;
		if (oleDateTime.ParseDateTime(rawDate)) {
			// 년.월.일 형식으로 변환
			mDate.Format(_T("%04d.%02d.%02d"), oleDateTime.GetYear(), oleDateTime.GetMonth(), oleDateTime.GetDay());
		}
		else {
			// 변환 실패 시 기본 값 설정
			mDate = _T("Invalid Date");
		}
		m_App->receiptDate[idx - 1] = mDate;

		if (idx == 4)
			break;
	}

	if (idx > 0)
		m_App->receiptNo = idx;

	mysql_free_result(sql_result);
	sql_row = NULL;
	// 최근 접수 -------------------------------------------------------------------------

	return true;
}


UINT DownloadThread(LPVOID pParam) 
{
	CregistryDlg* pDialog = reinterpret_cast<CregistryDlg*>(pParam);
	ThreadParams* pParams = (ThreadParams*)pParam;

	CString strURL, strFileName;
	CString strURL1, strURL2, strURL3, strFileName1, strFileName2, strFileName3;

	HRESULT hr = NULL;

	// http://218.145.31.94 : /www/data/bizstory

	if (pParams->strType == "logo")
	{
		strURL.Format(L"https://new.bizstory.co.kr/data/company/%s/company/%s", m_App->comp_idx, m_App->LogoImage);
		strFileName.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->LogoImage);

		hr = URLDownloadToFile(NULL, strURL, strFileName, 0, 0);
		//AfxMessageBox(strURL);
		if (SUCCEEDED(hr)) {
			pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
		}
		else {
			pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
		}
	}
	else if (pParams->strType == "banner")
	{
		if (pParams->number == 1)
		{
			strURL.Format(L"https://new.bizstory.co.kr/data/company/%s/banner/%s", m_App->bannerIDX[0], m_App->bannerImg[0]);
			strFileName.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[0]);
			
			hr = URLDownloadToFile(NULL, strURL, strFileName, 0, 0);
			if (SUCCEEDED(hr)) {
				pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
			}
			else {
				pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
			}
		}
		else if (pParams->number == 2)
		{
			strURL1.Format(L"https://new.bizstory.co.kr/data/company/%s/banner/%s", m_App->bannerIDX[0], m_App->bannerImg[0]);
			strURL2.Format(L"https://new.bizstory.co.kr/data/company/%s/banner/%s", m_App->bannerIDX[1], m_App->bannerImg[1]);
			strFileName1.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[0]);
			strFileName2.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[1]);

			hr = URLDownloadToFile(NULL, strURL1, strFileName1, 0, 0);
			if (SUCCEEDED(hr)) {
				pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
				hr = URLDownloadToFile(NULL, strURL2, strFileName2, 0, 0);
				if (SUCCEEDED(hr))
					pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
				else
					pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
			}
			else {
				pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
			}
		}
		else if (pParams->number == 3)
		{
			strURL1.Format(L"https://new.bizstory.co.kr/data/company/%s/banner/%s", m_App->comp_idx, m_App->bannerImg[0]);
			strURL2.Format(L"https://new.bizstory.co.kr/data/company/%s/banner/%s", m_App->comp_idx, m_App->bannerImg[1]);
			strURL3.Format(L"https://new.bizstory.co.kr/data/company/%s/banner/%s", m_App->comp_idx, m_App->bannerImg[2]);
			strFileName1.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[0]);
			strFileName2.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[1]);
			strFileName3.Format(L"C:\\Program Files (x86)\\METASTORY\\CareCon\\%s", m_App->bannerImg[2]);

			URLDownloadToFile(NULL, strURL1, strFileName1, 0, 0);
			if (SUCCEEDED(hr)) {
				pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
				hr = URLDownloadToFile(NULL, strURL2, strFileName2, 0, 0);
				if (SUCCEEDED(hr))
				{
					pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
					hr = URLDownloadToFile(NULL, strURL3, strFileName3, 0, 0);
					if (SUCCEEDED(hr))
						pDialog->PostMessage(WM_DOWNLOAD_COMPLETE, 0, 0);
					else
						pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
				}
				else
					pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
			}
			else {
				pDialog->PostMessage(WM_DOWNLOAD_FAILED, 0, 0);
			}
		}
	}

	delete pParams;

	return 0;
}


LRESULT CregistryDlg::OnDownloadComplete(WPARAM wParam, LPARAM lParam) 
{

	return 0;
}


LRESULT CregistryDlg::OnDownloadFailed(WPARAM wParam, LPARAM lParam) 
{
	AfxMessageBox(L"Image Download Failed!");
	return 0;
}


void CregistryDlg::OnBnClickedExit()
{
	exit(1);
}