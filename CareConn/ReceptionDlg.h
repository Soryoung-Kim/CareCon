#pragma once
#include "afxdialogex.h"
#include "PGTransparentStatic.h"
#include "PGBitmapButton.h"
#include "CregistryDlg.h"

// ReceptionDlg 대화 상자

class ReceptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ReceptionDlg)

public:
	ReceptionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ReceptionDlg();

	CregistryDlg registryDlg;

	UINT nBannerNo, nNoticeNo;
	BOOL bLoadLogo;

	void loadLogo();
	void drawBanner();
	void loadNotice();

	void buttonURL(int ndx);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_receptionDlg };
#endif

private:
	LOGFONT m_font;
	CBitmap	m_BkgndBmp;
	CPGImage m_bgReception;
	CPGBitmapButton mbtnLogo;
	CPGTransparentStatic m_txtCompanyName, m_txtManagerPlace;
	CPGTransparentStatic m_txtButton1, m_txtButton2, m_txtButton3, m_txtButton4;
	CPGTransparentStatic m_NoticeType, m_NoticeContent;
	CPGTransparentStatic m_receiptSubject[4], m_receiptWriter[4], m_receiptDate[4];
	CPGTransparentStatic m_ASType[4];
	CPGTransparentStatic m_txtVer;
	CPGBitmapButton m_bannerBtn, m_btnReception, m_btnExit;

	void DrawBackground();
	void CreateControl();
	
	CImage m_LogoImage;
	
	CStatic m_ctrlBanner;
	CImage m_bannerImage;

	CRect m_rcPictureControl;
	CRect m_rcButton1, m_rcButton2, m_rcButton3, m_rcButton4;
	CRect m_rcNotice;
	CRect m_rcMore;
	CRect m_rcReceiptSubject1, m_rcReceiptSubject2, m_rcReceiptSubject3, m_rcReceiptSubject4;
	CRect m_rReset;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedReception();
	afx_msg void OnBnClickedExit();

	afx_msg LRESULT OnNcHitTest(CPoint point);

	CStatic m_ctrlSchool;
	afx_msg void OnStnClickedSchool();
};
