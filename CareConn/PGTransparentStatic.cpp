// PGTransparentStatic.cpp : implementation file
//
#include "pch.h"
#include "PGTransparentStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPGTransparentStatic

IMPLEMENT_DYNAMIC(CPGTransparentStatic, CStatic)

	CPGTransparentStatic::CPGTransparentStatic() : m_TextColor(RGB(0x00, 0x00, 0x00)), m_rcMargin(0, 0, 0, 0), m_bDrawBkgnd(FALSE), m_bDrawImage(FALSE)
{
	m_BkgndBmp.DeleteObject();

	::ZeroMemory(&m_logFont, sizeof(m_logFont));
	m_logFont.lfHeight = 20;
	m_logFont.lfWeight = FW_BOLD;
	_stprintf_s(m_logFont.lfFaceName, _T("%s"), _T("Arial"));
}

CPGTransparentStatic::~CPGTransparentStatic()
{
	m_BkgndBmp.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPGTransparentStatic, CStatic)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CPGTransparentStatic message handlers

LRESULT CPGTransparentStatic::OnSetText(WPARAM /*wParam*/,LPARAM /*lParam*/)
{
	LRESULT Result = Default();

	Invalidate();
	UpdateWindow();

	return Result;
}

HBRUSH CPGTransparentStatic::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	//pDC->SetBkMode(TRANSPARENT);
	//return (HBRUSH)GetStockObject(NULL_BRUSH);

	m_Brush.DeleteObject();

	if (m_BackColor == TRANS_BACK) {
		m_Brush.CreateStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}
	else {
		m_Brush.CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
	}

	pDC->SetTextColor(m_TextColor);

	return (HBRUSH)m_Brush;
}

BOOL CPGTransparentStatic::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CPGTransparentStatic::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SetTextColor(m_TextColor);
	memDC.SetBkMode(TRANSPARENT);

	CBitmap bmp, *pOldBmp;
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	pOldBmp = memDC.SelectObject(&bmp);

	DrawBkgnd(&memDC);
	DrawBkgndImage(&memDC);
	DrawText(&memDC);

	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	bmp.DeleteObject();

	memDC.DeleteDC();
}

int CPGTransparentStatic::SetBkgnd(COLORREF bkColor, CDC* pParentDC/* = NULL*/, BOOL bDraw/* = TRUE*/)
{
	if (NULL == GetSafeHwnd())
		return -1;

	m_bDrawBkgnd = bDraw;
	m_BkgndBmp.DeleteObject();

	CRect rc;
	GetClientRect(&rc);

	CDC* pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	m_BkgndBmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&m_BkgndBmp);

	ReleaseDC(pDC);

	if (NULL == pParentDC)
	{
		memDC.FillSolidRect(&rc, bkColor);
	}
	else
	{
		CRect rcTemp(rc);
		ClientToScreen(&rcTemp);
		GetParent()->ScreenToClient(&rcTemp);

		memDC.BitBlt(0, 0, rc.Width(), rc.Height(), pParentDC, rcTemp.left, rcTemp.top, SRCCOPY);	
	}

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();

	return 0;
}

int CPGTransparentStatic::SetBkgndImage(LPCTSTR szName, LPCTSTR szType, BOOL bDraw/* = TRUE*/)
{
	if (NULL == GetSafeHwnd())
		return -1;

	m_bDrawImage = bDraw;
	m_Image.Load(szName, szType);

	return 0;
}

int CPGTransparentStatic::SetDefaultFont(LOGFONT& lf)
{
	memcpy_s(&m_logFont, sizeof(lf), &lf, sizeof(lf));

	return 0;
}

int CPGTransparentStatic::SetTextColor(COLORREF color)
{
	m_TextColor = color;

	return 0;
}

int CPGTransparentStatic::SetTextMargin(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcMargin.SetRect(nLeft, nTop, nRight, nBottom);

	return 0;
}

int CPGTransparentStatic::DrawBkgnd(CDC* pDC)
{
	if (NULL == pDC)
		return -1;

	if (FALSE == m_bDrawBkgnd)
		return -2;

	CRect rc;
	GetClientRect(&rc);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap* pOldBmp = memDC.SelectObject(&m_BkgndBmp);
	BITMAP bmp;
	m_BkgndBmp.GetBitmap(&bmp);

	pDC->StretchBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, rc.Width(), rc.Height(), SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();

	return 0;
}

int CPGTransparentStatic::DrawBkgndImage(CDC* pDC)
{
	if (NULL == pDC)
		return -1;

	if (FALSE == m_bDrawImage)
		return -2;

	CRect rc;
	GetClientRect(&rc);

	Graphics graphics(*pDC);

	graphics.DrawImage(&m_Image, 0, 0, rc.Width(), rc.Height());

	return 0;
}

int CPGTransparentStatic::DrawText(CDC* pDC)
{
	if (NULL == pDC)
		return -1;

	CRect rc;
	GetClientRect(&rc);

	rc.DeflateRect(m_rcMargin.left, m_rcMargin.top, m_rcMargin.right, m_rcMargin.bottom);

	CString szText;
	GetWindowText(szText);

	CFont font;
	CFont *pOldFont;
	font.CreateFontIndirect(&m_logFont);
	pOldFont = pDC->SelectObject(&font);

	// Draw Code Start
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
	UINT nAlign;
	if (lStyle & SS_CENTER)
	nAlign = DT_CENTER;
	else if (lStyle & SS_RIGHT)
	nAlign = DT_RIGHT;
	else
	nAlign = DT_LEFT;

	pDC->DrawText(szText, &rc, nAlign | DT_WORDBREAK | DT_EXPANDTABS | DT_EDITCONTROL | DT_END_ELLIPSIS);

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
	pOldFont->DeleteObject();

	return 0;
}