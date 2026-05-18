// PGBitmapButton.cpp : implementation file
//
#include "pch.h"
#include "PGImage.h"
#include "PGBitmapButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPGBitmapButton

IMPLEMENT_DYNAMIC(CPGBitmapButton, CButton)

	CPGBitmapButton::CPGBitmapButton() : 
m_bTrack(FALSE), m_bHover(FALSE), m_bDrawBkgnd(TRUE), m_nIDNormal(0), m_nIDSelected(0), m_nIDDisable(0), m_bDrawText(FALSE),
	m_TextColor(RGB(0x00, 0x00, 0x00)), m_bTransparentBk(TRUE)
{
	::ZeroMemory(&m_logFont, sizeof(m_logFont));
	m_logFont.lfHeight = 18;
	m_logFont.lfWeight = FW_BOLD;
	_stprintf_s(m_logFont.lfFaceName, _T("%s"), _T("¸¼Àº °íµñ"));
}

CPGBitmapButton::~CPGBitmapButton()
{
	m_dcBk.DeleteDC();
}

BEGIN_MESSAGE_MAP(CPGBitmapButton, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_SIZE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

// CPGBitmapButton message handlers

void CPGBitmapButton::PreSubclassWindow()
{
	SetButtonStyle(GetButtonStyle() | BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

BOOL CPGBitmapButton::PreTranslateMessage(MSG* pMsg)
{
	if (WM_MOUSEFIRST <= pMsg->message && WM_MOUSELAST >= pMsg->message && NULL != m_Tooltip.GetSafeHwnd())
	{
		m_Tooltip.RelayEvent(pMsg);
	}

	return CButton::PreTranslateMessage(pMsg);
}

void CPGBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Add your code to draw the specified item

	CRect rc(lpDrawItemStruct->rcItem);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SetStretchBltMode(HALFTONE);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp, *pOldBmp;
	bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	pOldBmp = memDC.SelectObject(&bmp);

	DrawBackground(&memDC);

	if (0 != m_nIDNormal)
	{
		Graphics graphics(memDC);
		CPGImage backImage;

		if (lpDrawItemStruct->itemState & ODS_DISABLED)
		{
			UINT nResourceID = (0 == m_nIDDisable) ? m_nIDNormal : m_nIDDisable;
			backImage.Load(MAKEINTRESOURCE(nResourceID), _T("PNG"));
			graphics.DrawImage(&backImage, 0, 0, rc.Width(), rc.Height());
		}
		else if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			UINT nResourceID = (0 == m_nIDSelected) ? m_nIDNormal : m_nIDSelected;
			backImage.Load(MAKEINTRESOURCE(nResourceID), _T("PNG"));
			graphics.DrawImage(&backImage, 1, 1, rc.Width() - 2, rc.Height() - 2);
		}
		else
		{
			if (TRUE == m_bHover)
			{
				UINT nResourceID = (0 == m_nIDSelected) ? m_nIDNormal : m_nIDSelected;
				backImage.Load(MAKEINTRESOURCE(nResourceID), _T("PNG"));
				graphics.DrawImage(&backImage, 0, 0, rc.Width(), rc.Height());
			}
			else
			{
				backImage.Load(MAKEINTRESOURCE(m_nIDNormal), _T("PNG"));
				graphics.DrawImage(&backImage, 0, 0, rc.Width(), rc.Height());
			}
		}

		if (TRUE == m_bDrawText)
		{
			SolidBrush brush(Color(GetRValue(m_TextColor), GetGValue(m_TextColor), GetBValue(m_TextColor)));
			StringFormat sf;
			DWORD dwStyle = 0;
			dwStyle |= (FW_BOLD == m_logFont.lfWeight) ? FontStyleBold : FontStyleRegular;
			if (TRUE == m_logFont.lfItalic)
				dwStyle |= FontStyleItalic;

			if (TRUE == m_logFont.lfUnderline)
				dwStyle |= FontStyleUnderline;

			if (TRUE == m_logFont.lfStrikeOut)
				dwStyle |= FontStyleStrikeout;

			sf.SetAlignment(StringAlignmentCenter);
			sf.SetLineAlignment(StringAlignmentCenter);
			CStringW szText;
			GetWindowTextW(szText);

			if (lpDrawItemStruct->itemState & ODS_SELECTED)
			{

				Gdiplus::Font font(m_logFont.lfFaceName, (Gdiplus::REAL)m_logFont.lfHeight - 1, dwStyle, UnitPixel);
				RectF rcText(1.0f, 1.0f, (Gdiplus::REAL)rc.Width() - 2, (Gdiplus::REAL)rc.Height() - 2);
				graphics.DrawString(szText, szText.GetLength(), &font, rcText, &sf, &brush);
			}
			else
			{
				Gdiplus::Font font(m_logFont.lfFaceName, (Gdiplus::REAL)m_logFont.lfHeight, dwStyle, UnitPixel);
				RectF rcText(0.0, 0.0, (Gdiplus::REAL)rc.Width(), (Gdiplus::REAL)rc.Height());
				graphics.DrawString(szText, szText.GetLength(), &font, rcText, &sf, &brush);
			}
		}
	}

	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBmp);
	bmp.DeleteObject();

	memDC.DeleteDC();
}

HBRUSH CPGBitmapButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

BOOL CPGBitmapButton::OnEraseBkgnd(CDC* /*pDC*/)
{
	if (FALSE != m_bDrawBkgnd)
	{
		m_dcBk.DeleteDC();
		CDC* pParentDC = GetParent()->GetDC();
		SetTransparentBkgnd(pParentDC);
		ReleaseDC(pParentDC);
	}

	m_bDrawBkgnd = TRUE;

	return FALSE;
}

void CPGBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (FALSE == m_bTrack)
	{
		TRACKMOUSEEVENT MouseEvent;
		::ZeroMemory(&MouseEvent, sizeof(MouseEvent));

		MouseEvent.cbSize = sizeof(MouseEvent);
		MouseEvent.dwFlags = TME_LEAVE | TME_HOVER;
		MouseEvent.hwndTrack = this->m_hWnd;
		MouseEvent.dwHoverTime = 1;

		m_bTrack = ::_TrackMouseEvent(&MouseEvent);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CPGBitmapButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bHover = TRUE;
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	CButton::OnMouseHover(nFlags, point);
}

void CPGBitmapButton::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	TRACKMOUSEEVENT MouseEvent;
	::ZeroMemory(&MouseEvent, sizeof(MouseEvent));

	MouseEvent.cbSize = sizeof(MouseEvent);
	MouseEvent.dwFlags = TME_CANCEL;
	MouseEvent.hwndTrack = this->m_hWnd;

	::_TrackMouseEvent(&MouseEvent);

	m_bTrack = FALSE;
	m_bHover = FALSE;
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

	CButton::OnMouseLeave();
}

void CPGBitmapButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	SetToolTipRect();
}

void CPGBitmapButton::OnMove(int x, int y)
{
	CButton::OnMove(x, y);

	SetToolTipRect();
}

BOOL CPGBitmapButton::SetAllImage(UINT nNormalID, UINT nSelectedID, UINT nDisableID, BOOL bDrawBack)
{
	m_nIDNormal = nNormalID;
	m_nIDSelected = nSelectedID;
	m_nIDDisable = nDisableID;
	m_bDrawBkgnd = bDrawBack;
	Invalidate();
	return 0;
}

BOOL CPGBitmapButton::SetNormalImage(UINT nResourceID, BOOL bDrawBack)
{
	m_nIDNormal = nResourceID;
	m_bDrawBkgnd = bDrawBack;
	Invalidate();
	return 0;
}
BOOL CPGBitmapButton::SetSelectedImage(UINT nResourceID, BOOL bDrawBack)
{
	m_nIDSelected = nResourceID;
	m_bDrawBkgnd = bDrawBack;
	Invalidate();
	return 0;
}

BOOL CPGBitmapButton::SetDisableImage(UINT nResourceID, BOOL bDrawBack)
{
	m_nIDDisable = nResourceID;
	m_bDrawBkgnd = bDrawBack;
	Invalidate();
	return 0;
}

void CPGBitmapButton::SetTransparentBkgnd(CDC* pDC)
{
	CRect rcWin, rcClient;
	CBitmap bmp, *pOldBmp;

	GetClientRect(&rcClient);
	GetWindowRect(&rcWin);
	GetParent()->ScreenToClient(&rcWin);

	m_dcBk.DeleteDC();

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

	pOldBmp = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pDC, rcWin.left, rcWin.top, SRCCOPY);
	bmp.DeleteObject();
}

void CPGBitmapButton::SetToolTipText(int nID, BOOL bActivate)
{
	if (NULL == GetSafeHwnd())
		return ;

	InitToolTip();

	m_szToolTip.LoadString(nID);
	if (FALSE != m_szToolTip.IsEmpty() && NULL != m_Tooltip.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);

		m_Tooltip.AddTool(this, m_szToolTip, &rc, m_Tooltip.GetToolCount() + 1);
		m_Tooltip.Activate(bActivate);
	}
}

void CPGBitmapButton::SetToolTipText(LPCTSTR szText, BOOL bActivate)
{
	if (NULL == GetSafeHwnd())
		return ;

	if (NULL != szText)
	{
		m_szToolTip.Format(_T("%s"), szText);

		InitToolTip();
		if (NULL != m_Tooltip.GetSafeHwnd())
		{
			CRect rc;
			GetClientRect(&rc);

			m_Tooltip.AddTool(this, m_szToolTip, &rc, m_Tooltip.GetToolCount() + 1);
			m_Tooltip.Activate(bActivate);
		}
	}
}

void CPGBitmapButton::SetToolTipActive(BOOL bActive)
{
	if (NULL != m_Tooltip.GetSafeHwnd())
	{
		m_Tooltip.Activate(bActive);
	}
}

void CPGBitmapButton::SetDrawText(BOOL bDrawText)
{
	m_bDrawText = bDrawText;
}

int CPGBitmapButton::SetTextColor(COLORREF color, BOOL bTransparent/* = TRUE*/)
{
	m_TextColor = color;
	m_bTransparentBk = bTransparent;
	return 0;
}

int CPGBitmapButton::SetDefaultFont(LOGFONT& lf)
{
	memcpy_s(&m_logFont, sizeof(lf), &lf, sizeof(lf));

	return 0;
}

void CPGBitmapButton::DrawBackground(CDC* pDC)
{
	if (NULL == m_dcBk.GetSafeHdc())
	{
		CDC* pParentDC = GetParent()->GetDC();
		SetTransparentBkgnd(pParentDC);
		ReleaseDC(pParentDC);
	}

	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}

void CPGBitmapButton::DrawBtnImage(CDC* pDestDC, CDC* pSrcDC)
{
	CRect rect;
	GetClientRect(rect);
	pDestDC->BitBlt(0, 0, rect.Width(), rect.Height(), pSrcDC, 0, 0, SRCCOPY);
}

void CPGBitmapButton::InitToolTip()
{
	if (NULL == m_Tooltip.GetSafeHwnd())
	{
		m_Tooltip.Create(this);
		m_Tooltip.Activate(FALSE);
		m_Tooltip.SetMaxTipWidth(500);
		m_Tooltip.SetDelayTime(TTDT_INITIAL, 150);
	}
	for (int i = m_Tooltip.GetToolCount() ; i >= 1  ; i--)
	{
		m_Tooltip.DelTool(this, i);
	}

	ASSERT(m_Tooltip.GetSafeHwnd());
}

void CPGBitmapButton::SetToolTipRect()
{
	if (NULL == GetSafeHwnd() || NULL == m_Tooltip.GetSafeHwnd())
		return ;

	CRect rc;
	GetClientRect(&rc);

	for (int i = 0 ; i < m_Tooltip.GetToolCount() ; i++)
	{
		m_Tooltip.SetToolRect(this, i + 1, &rc);
	}
}