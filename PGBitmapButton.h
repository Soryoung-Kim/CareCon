#pragma once

// CPGBitmapButton

class CPGBitmapButton : public CButton
{
	DECLARE_DYNAMIC(CPGBitmapButton)

public:
	CPGBitmapButton();
	virtual ~CPGBitmapButton();

public:
	BOOL SetAllImage(UINT nNormalID, UINT nSelectedID, UINT nDisableID, BOOL bDrawBack = FALSE);
	BOOL SetSelectedImage(UINT nResourceID, BOOL bDrawBack = FALSE);
	BOOL SetNormalImage(UINT nResourceID, BOOL bDrawBack = FALSE);
	BOOL SetDisableImage(UINT nResourceID, BOOL bDrawBack = FALSE);

	void SetTransparentBkgnd(CDC* pDC);

	void SetToolTipText(int nID, BOOL bActivate = TRUE);
	void SetToolTipText(LPCTSTR szText, BOOL bActivate = TRUE);
	void SetToolTipActive(BOOL bActive);

	void SetDrawText(BOOL bDrawText);
	int SetTextColor(COLORREF color, BOOL bTransparent = TRUE);
	int SetDefaultFont(LOGFONT& lf);

private:
	BOOL m_bTrack;
	BOOL m_bHover;

	BOOL m_bDrawBkgnd;

	UINT m_nIDNormal;
	UINT m_nIDSelected;
	UINT m_nIDDisable;

	CDC m_dcBk;

	CToolTipCtrl m_Tooltip;
	CString m_szToolTip;

	BOOL m_bDrawText;
	COLORREF m_TextColor;
	BOOL m_bTransparentBk;
	LOGFONT m_logFont;

private:
	void DrawBackground(CDC* pDC);
	void DrawBtnImage(CDC* pDestDC, CDC* pSrcDC);
	void InitToolTip();
	void SetToolTipRect();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
};