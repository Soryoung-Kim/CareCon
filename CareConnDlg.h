
// CareConnDlg.h: 헤더 파일
//

#pragma once

//#include "CMacIpProcess.h"
#include <atlbase.h>
#include <atlcom.h>
#include <shellapi.h>
#include "CregistryDlg.h"
#include "ReceptionDlg.h"


#pragma comment(lib, "WS2_32")
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi")

// CCareConnDlg 대화 상자
class CCareConnDlg : public CDialogEx
{
// 생성입니다.
public:
	CCareConnDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CregistryDlg registryDlg;
	ReceptionDlg receptionDlg;

	typedef DWORD(WINAPI* lpfnGetAdaptersInfo)(
		PIP_ADAPTER_INFO pAdapterInfo,
		PULONG pOutBufLen
		);

	lpfnGetAdaptersInfo m_pGetAdaptersInfo;
	HMODULE hIpHlpApi;
	CString strMac, macName;

	CString GetMACAddress();
	CString GetIP();
	CString strVersion;

	IWebBrowser2* pBrowser = NULL;


	void print_ip(DWORD nAddr);

	void callBizstory();
	BOOL isAuthentication();

	void SetBrowserSize(int width, int height);
	void LoadURL(CString url);

	void callReception();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARECONN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedbtnmac();
	CLinkCtrl m_sysLink;
	afx_msg void OnClickSyslink1(NMHDR* pNMHDR, LRESULT* pResult);
};
