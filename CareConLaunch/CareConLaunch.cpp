#include <windows.h>
#include <wchar.h>
#include <stdio.h>

static DWORD RunCommand(const wchar_t* cmd)
{
    wchar_t cmdBuf[MAX_PATH * 2];
    wcscpy_s(cmdBuf, cmd);

    STARTUPINFOW si = { sizeof(si) };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi = { 0 };
    if (!CreateProcessW(NULL, cmdBuf, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
        return (DWORD)-1;

    WaitForSingleObject(pi.hProcess, 5000);
    DWORD exitCode = 1;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return exitCode;
}

static void GetCareConPath(wchar_t* careConPath, size_t len)
{
    wchar_t exeDir[MAX_PATH];
    GetModuleFileNameW(NULL, exeDir, MAX_PATH);
    wchar_t* lastSlash = wcsrchr(exeDir, L'\\');
    if (lastSlash)
        *(lastSlash + 1) = L'\0';
    swprintf_s(careConPath, len, L"%sCareCon.exe", exeDir);
}

static bool RegisterTask(const wchar_t* careConPath)
{
    wchar_t userName[256] = { 0 };
    DWORD userNameLen = 256;
    GetUserNameW(userName, &userNameLen);

    wchar_t computerName[256] = { 0 };
    DWORD computerNameLen = 256;
    GetComputerNameW(computerName, &computerNameLen);

    wchar_t fullUserName[512] = { 0 };
    swprintf_s(fullUserName, 512, L"%s\\%s", computerName, userName);

    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);

    wchar_t xmlPath[MAX_PATH];
    swprintf_s(xmlPath, MAX_PATH, L"%sCareCon_task.xml", tempPath);

    FILE* f = nullptr;
    _wfopen_s(&f, xmlPath, L"w, ccs=UTF-16LE");
    if (!f) return false;

    fwprintf(f,
        L"<?xml version=\"1.0\" encoding=\"UTF-16\"?>\n"
        L"<Task version=\"1.2\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
        L"  <RegistrationInfo/>\n"
        L"  <Triggers/>\n"
        L"  <Principals>\n"
        L"    <Principal id=\"Author\">\n"
        L"      <UserId>%s</UserId>\n"
        L"      <LogonType>InteractiveToken</LogonType>\n"
        L"      <RunLevel>HighestAvailable</RunLevel>\n"
        L"    </Principal>\n"
        L"  </Principals>\n"
        L"  <Settings>\n"
        L"    <MultipleInstancesPolicy>Parallel</MultipleInstancesPolicy>\n"
        L"    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>\n"
        L"    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>\n"
        L"    <ExecutionTimeLimit>PT0S</ExecutionTimeLimit>\n"
        L"    <Priority>7</Priority>\n"
        L"  </Settings>\n"
        L"  <Actions Context=\"Author\">\n"
        L"    <Exec>\n"
        L"      <Command>%s</Command>\n"
        L"    </Exec>\n"
        L"  </Actions>\n"
        L"</Task>\n",
        fullUserName, careConPath);
    fclose(f);

    wchar_t params[MAX_PATH * 2];
    swprintf_s(params, MAX_PATH * 2, L"/create /tn \"CareCon\" /xml \"%s\" /f", xmlPath);

    SHELLEXECUTEINFO sei = { 0 };
    sei.cbSize = sizeof(SHELLEXECUTEINFO);
    sei.fMask = SEE_MASK_NO_CONSOLE | SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = L"runas";
    sei.lpFile = L"schtasks";
    sei.lpParameters = params;
    sei.nShow = SW_HIDE;

    bool result = false;
    if (ShellExecuteEx(&sei) && sei.hProcess)
    {
        WaitForSingleObject(sei.hProcess, 5000);
        DWORD exitCode = 1;
        GetExitCodeProcess(sei.hProcess, &exitCode);
        CloseHandle(sei.hProcess);
        result = (exitCode == 0);
    }

    DeleteFileW(xmlPath);
    return result;
}

static void LaunchDirect(const wchar_t* careConPath)
{
    ShellExecuteW(NULL, L"open", careConPath, NULL, NULL, SW_SHOWNORMAL);
}

// 설치 완료 창(닫기 버튼)이 닫힐 때까지 대기
static void WaitForInstallerExit()
{
    // MsiDialogCloseClass 창이 나타날 때까지 대기 (최대 20초)
    for (int i = 0; i < 40; i++)
    {
        if (FindWindowW(L"MsiDialogCloseClass", NULL)) break;
        Sleep(500);
    }

    // 닫기 클릭으로 창이 사라질 때까지 대기 (최대 150초)
    for (int i = 0; i < 300; i++)
    {
        if (!FindWindowW(L"MsiDialogCloseClass", NULL)) break;
        Sleep(500);
    }

    Sleep(300);
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    wchar_t careConPath[MAX_PATH];
    GetCareConPath(careConPath, MAX_PATH);

    // 인스톨러 Commit에서 호출된 경우: /wait 프로세스를 분리 실행 후 즉시 반환
    // (인스톨러를 블로킹하지 않음)
    if (lpCmdLine && wcsstr(lpCmdLine, L"/installed"))
    {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);

        wchar_t cmdLine[MAX_PATH + 10];
        swprintf_s(cmdLine, L"\"%s\" /wait", exePath);

        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE,
            CREATE_NEW_PROCESS_GROUP | DETACHED_PROCESS, NULL, NULL, &si, &pi);
        if (pi.hProcess) CloseHandle(pi.hProcess);
        if (pi.hThread)  CloseHandle(pi.hThread);
        return 0; // 인스톨러에 즉시 반환
    }

    // 닫기 클릭 대기 후 실행
    if (lpCmdLine && wcsstr(lpCmdLine, L"/wait"))
    {
        WaitForInstallerExit();
    }

    // Task Scheduler로 실행될 CareCon.exe가 포그라운드 권한을 가질 수 있도록 허용
    AllowSetForegroundWindow(ASFW_ANY);

    // schtasks /run 시도
    DWORD exitCode = RunCommand(L"schtasks /run /tn \"CareCon\"");

    if (exitCode == 0)
        return 0; // 성공

    // 실패 시 작업 등록 (UAC 1회)
    if (RegisterTask(careConPath))
    {
        // 등록 성공 후 실행
        exitCode = RunCommand(L"schtasks /run /tn \"CareCon\"");
        if (exitCode == 0)
            return 0;
    }

    // 최후 폴백: 직접 실행 (UAC 발생 가능)
    LaunchDirect(careConPath);

    return 0;
}
