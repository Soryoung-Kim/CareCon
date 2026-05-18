#pragma once
// Version management - edit only this file to change version
// After editing, run UpdateVersion.ps1 before building Setup project
#define VER_MAJOR    2
#define VER_MINOR    1
#define VER_PATCH    1
#define VER_BUILD    0

#define _VSTR2(x)           #x
#define _VSTR(x)            _VSTR2(x)

#define APP_FILEVERSION         VER_MAJOR, VER_MINOR, VER_PATCH, VER_BUILD
#define APP_VERSION_STR         _VSTR(VER_MAJOR) "." _VSTR(VER_MINOR) "." _VSTR(VER_PATCH)
#define APP_CAPTION             "BIZSTORY CareCon v" APP_VERSION_STR

#ifndef RC_INVOKED
#define APP_VERSION_WSTR        _T(APP_VERSION_STR)
#endif
