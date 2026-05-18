
// CareConn.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "define.h"

#define WM_DOWNLOAD_COMPLETE (WM_APP + 1)
#define WM_DOWNLOAD_FAILED (WM_APP + 2)

//#define CARECON 1
//#define LUCOMS  2
//#define INET  3
//#define A_TYPE  1   // White Skin
//#define B_TYPE  2   // Dark Skin
//
//#define UI_COMPANY  CARECON
//#define UI_TYPE     A_TYPE

// CCareConnApp:
// 이 클래스의 구현에 대해서는 CareConn.cpp을(를) 참조하세요.
//

class CCareConnApp : public CWinApp
{
public:
	CCareConnApp();


private:
	ULONG_PTR m_GdiplusToken;

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	void linkURL(CString url);
	bool IsURLAlreadyOpened(CString url);

	UINT DBPort;
	CString DBHost;
	CString UI_Type, UI_Company;

	CString activeMacCode, carecon_compCode, strVersion;
	CString companyName, manager, place;	// 학교/기업명, 담당자, 설치장소
	CString comp_idx;		
	CString comp_code;
	CString homepage;
	CString dbName;

	CString LogoImage;		// Logo Image
	CString bannerImg[3];
	CString bannerURL[3];
	CString bannerIDX[3];
	int		bannerNum;

	CString buttonType[4], buttonName[4], buttonURL[4];
	CString noticeType[2], noticeContent[2], noticeURL[2];
	int		noticeNum;
	int		receiptNo;
	CString receiptIdx[4], receiptWriter[4], receiptSubject[4], receiptDate[4], receiptCodeName[4];

	BOOL bCompanyLogo;

// 구현입니다.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCareConnApp theApp;

// 실행 파일이 위치한 디렉토리를 반환 (끝에 '\' 포함)
inline CString GetInstallDir()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    wchar_t* lastSlash = wcsrchr(path, L'\\');
    if (lastSlash) *(lastSlash + 1) = L'\0';
    return CString(path);
}
