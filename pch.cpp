// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"
#include "StdioFileEx.h"

MYSQL_ROW sql_row;
MYSQL_RES* sql_result;
MYSQL conn;
MYSQL* mysqlConn;

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.

LONG GetRegistryIntValue(int cls, CString key, CString  pszItem)
{

	LONG     lResult;
	HKEY     hKey = NULL;
	DWORD    dwValue = 0;
	DWORD    dwSize = sizeof(DWORD);

	switch (cls) {
		case 1:	lResult = RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey);	break;
		case 2:	lResult = RegOpenKey(HKEY_CURRENT_USER, key, &hKey);	break;
		case 3:	lResult = RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey);	break;
	}

	if (ERROR_SUCCESS != lResult)
		return -1;

	lResult = RegQueryValueEx(hKey, pszItem, NULL, NULL, (LPBYTE)&dwValue, &dwSize);
	RegCloseKey(hKey);

	if (ERROR_SUCCESS != lResult)
		return -1;
	else
		return dwValue;

}

CString GetRegistryStrValue(int cls, CString key, CString pszItem)
{
	LONG     lResult;
	HKEY     hKey = NULL;
	DWORD    dwSize;

	dwSize = 128;

	switch (cls) {
		case 1:	lResult = RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey);	break;
		case 2:	lResult = RegOpenKey(HKEY_CURRENT_USER, key, &hKey);	break;
		case 3:	lResult = RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey);	break;
	}

	TCHAR pszValue[128];
	RegQueryValueEx(hKey, pszItem, NULL, NULL, (LPBYTE)pszValue, &dwSize);

	RegCloseKey(hKey);

	return pszValue;
}


LONG SetRegistryIntValue(int cls, CString key, CString pszItem, DWORD  dwValue)
{

	LONG     lResult;
	HKEY     hKey = NULL;
	DWORD    dwSize;

	switch (cls) {
		case 1:	lResult = RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey);	break;
		case 2:	lResult = RegOpenKey(HKEY_CURRENT_USER, key, &hKey);	break;
		case 3:	lResult = RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey);	break;
	}

	if (ERROR_SUCCESS != lResult)
	{
		switch (cls) {
		case 1:	RegCreateKeyEx(HKEY_CLASSES_ROOT, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);	break;
		case 2:	RegCreateKeyEx(HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);	break;
		case 3:	RegCreateKeyEx(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);	break;
		}
	}

	dwSize = sizeof(DWORD);
	lResult = RegSetValueEx(hKey, pszItem, NULL, REG_DWORD, (LPBYTE)&dwValue, dwSize);

	RegCloseKey(hKey);
	return lResult;
}

LONG SetRegistryStrValue(int cls, CString key, CString pszItem, CString  pszValue)
{
	LONG     lResult;
	HKEY     hKey = NULL;
	DWORD    dwSize;

	switch (cls) {
		case 1:	lResult = RegOpenKey(HKEY_CLASSES_ROOT, key, &hKey);	break;
		case 2:	lResult = RegOpenKey(HKEY_CURRENT_USER, key, &hKey);	break;
		case 3:	lResult = RegOpenKey(HKEY_LOCAL_MACHINE, key, &hKey);	break;
	}

	if (ERROR_SUCCESS != lResult)
	{
		switch (cls) {
			case 1:	RegCreateKeyEx(HKEY_CLASSES_ROOT, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);	break;
			case 2:	RegCreateKeyEx(HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);	break;
			case 3:	RegCreateKeyEx(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);	break;
		}
	}

	dwSize = pszValue.GetLength();

	lResult = RegSetValueEx(hKey, pszItem, NULL, REG_SZ, (LPBYTE)(LPCTSTR)pszValue, dwSize * 2);

	RegCloseKey(hKey);
	return lResult;
}


void LogWrite(CString cls, CString strLog)
{
	CStdioFileEx sLog;
	CString strPath, mContent;
	CTime today = CTime::GetCurrentTime();

	strPath.Format(_T("C:\\Program Files (x86)\\CareConn\\Log\\CareConn_%04d%02d%02d.log"), today.GetYear(), today.GetMonth(), today.GetDay());

	if (sLog.Open(strPath, CStdioFileEx::modeCreate | CStdioFileEx::modeNoTruncate | CStdioFileEx::modeWrite | CStdioFileEx::typeText) == FALSE)
		return;

	mContent.Format(L"[%04d/%02d/%02d-%02d:%02d:%02d  %s]	%s\r\n", today.GetYear(), today.GetMonth(), today.GetDay(),
		today.GetHour(), today.GetMinute(), today.GetSecond(), cls, strLog);

	USES_CONVERSION;
	char* logText = W2A(mContent);

	sLog.SeekToEnd();
	sLog.Write(logText, UINT(strlen(logText)));
	sLog.Flush();
	sLog.Close();
}