#pragma once

#include <Windows.h>
#include <atltypes.h>
#include <GdiPlus.h>

#include "PGImage.h"

#pragma comment(lib, "Gdiplus.lib")
#define TRANS_BACK -1

// CPGTransparentStatic

class CPGTransparentStatic : public CStatic
{
	DECLARE_DYNAMIC(CPGTransparentStatic)

public:
	CPGTransparentStatic();
	virtual ~CPGTransparentStatic();

public:
	int SetBkgnd(COLORREF bkColor, CDC* pParentDC = NULL, BOOL bDraw = TRUE);
	int SetBkgndImage(LPCTSTR szName, LPCTSTR szType, BOOL bDraw = TRUE);
	int SetDefaultFont(LOGFONT& lf);
	int SetTextColor(COLORREF txtColor);
	int SetTextMargin(int nLeft, int nTop, int nRight, int nBottom);

private:
	// Draw order : DrawBkgnd -> DrawBkgndImage -> DrawText
	int DrawBkgnd(CDC* pDC);
	int DrawBkgndImage(CDC* pDC);
	int DrawText(CDC* pDC);

private:
	CBitmap m_BkgndBmp;
	CPGImage m_Image;
	
	COLORREF m_TextColor;
	LOGFONT m_logFont;
	CRect m_rcMargin;
	
	BOOL m_bDrawBkgnd;
	BOOL m_bDrawImage;
	CBrush   m_Brush;
	COLORREF m_BackColor;

protected:
	LRESULT OnSetText(WPARAM wParam,LPARAM lParam);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


