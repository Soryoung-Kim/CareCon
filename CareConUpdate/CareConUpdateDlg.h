
// CareConUpdateDlg.h: 헤더 파일
//

#pragma once

#define WM_DOWNLOAD_COMPLETE (WM_APP + 1)
#define WM_DOWNLOAD_FAILED (WM_APP + 2)
#define WM_UNZIP_FAILED (WM_APP + 3)

UINT DownloadThread(LPVOID pParam);

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
    HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText);
    HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError) { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo) { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed) { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, IUnknown* punk) { return E_NOTIMPL; }

    CProgressCtrl* m_pProgressCtrl;
    CDownloadCallback() : m_pProgressCtrl(nullptr) {}
};


// CCareConUpdateDlg 대화 상자
class CCareConUpdateDlg : public CDialogEx
{
// 생성입니다.
public:
	CCareConUpdateDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARECONUPDATE_DIALOG };
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
    afx_msg LRESULT OnDownloadComplete(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDownloadFailed(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

public:
	CProgressCtrl m_ProgressCtrl;
    CDownloadCallback m_Callback;
};
