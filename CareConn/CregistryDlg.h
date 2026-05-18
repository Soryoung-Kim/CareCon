#pragma once
#include "afxdialogex.h"
#include "PGTransparentStatic.h"
#include "PGBitmapButton.h"

UINT DownloadThread(LPVOID pParam);

struct ThreadParams {
	CString strType;
	int number = 0;
};

// 다운로드 콜백 클래스 정의
class CDownloadCallback : public IBindStatusCallback {
public:
	ULONG STDMETHODCALLTYPE AddRef() { return 1; }
	ULONG STDMETHODCALLTYPE Release() { return 1; }
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv) {
		if (!ppv) return E_INVALIDARG;
		*ppv = nullptr;
		if (riid == IID_IUnknown || riid == IID_IBindStatusCallback) {
			*ppv = static_cast<IBindStatusCallback*>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, IBinding* pib) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE GetPriority(LONG* pnPriority) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, IUnknown* punk) { return E_NOTIMPL; }
};

// CregistryDlg 대화 상자

class CregistryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CregistryDlg)

private:
	LOGFONT m_font;
	CFont m_edtFont;
	CBitmap	m_BkgndBmp;
	CPGImage m_bgRegistry;
	CPGBitmapButton m_btnSearch, m_btnRegistry, mbtnLogo, m_btnExit;
	CPGTransparentStatic m_txtVer;

	//CImage m_LogoImage;
		
	void DrawBackground();
	void CreateControl();

public:
	CregistryDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CregistryDlg();

	BOOL ConnectDB();
	void DisconnectDB(void);

	void LoadCompanyInfo(char cls, CString companyName);
	BOOL WriteDBQuery(CString strquery);

	void ListAddItem(int nDx, CString scName, CString compName, CString telNum, CString code);

	CString parseVersion(CString versionStr);
	BOOL checkVersion(CString userVersion);
	BOOL CheckMacAddr();
	BOOL Update_MacAddr2(CString macAddr);

	BOOL LoadConfigure();

	CString sc_name, tel_num, comp_code, regDate, lastDate;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RegistryDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnDownloadComplete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDownloadFailed(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedbtnsearch();
	//afx_msg void OnBnClickedCompanyLogo();
	CListCtrl m_lstComp;
	CEdit m_edtName;
	afx_msg void OnKeydownLstcomp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLstcomp(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_txtComp_name;
	CStatic m_txtSc_name;
	CStatic m_txtTel_num;
	afx_msg void OnCustomdrawLstcomp(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnNMCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_txtComp_code;
	CEdit m_edtUser;
	CEdit m_edtPlace;

	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
