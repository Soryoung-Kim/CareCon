# ================================================================
# UpdateVersion.ps1
# version.h 의 버전 정보를 읽어 CareCon Setup.vdproj 를 자동 업데이트합니다.
# Setup 프로젝트 빌드 전에 실행하세요.
# 실행: powershell -ExecutionPolicy Bypass -File UpdateVersion.ps1
# ================================================================

$versionHeader = "$PSScriptRoot\CareConn\version.h"
$vdproj        = "$PSScriptRoot\CareCon Setup\CareCon Setup.vdproj"

# version.h 에서 버전 숫자 읽기
$content = Get-Content $versionHeader -Encoding UTF8
$major = ($content | Select-String '^\s*#define\s+VER_MAJOR\s+(\d+)').Matches[0].Groups[1].Value
$minor = ($content | Select-String '^\s*#define\s+VER_MINOR\s+(\d+)').Matches[0].Groups[1].Value
$patch = ($content | Select-String '^\s*#define\s+VER_PATCH\s+(\d+)').Matches[0].Groups[1].Value
$version = "$major.$minor.$patch"

Write-Host "version.h 버전: $version"

# vdproj 업데이트
$vd = Get-Content $vdproj -Encoding UTF8

# ProductVersion 업데이트
$vd = $vd -replace '"ProductVersion" = "8:[0-9.]+"', ('"ProductVersion" = "8:' + $version + '"')

# ProductCode 새 GUID 생성 (Major Upgrade 필수)
$newGuid = [System.Guid]::NewGuid().ToString().ToUpper()
$vd = $vd -replace '"ProductCode" = "8:\{[0-9A-F-]+\}"', ('"ProductCode" = "8:{' + $newGuid + '}"')

Set-Content $vdproj $vd -Encoding UTF8

Write-Host "vdproj 업데이트 완료"
Write-Host "  ProductVersion : $version"
Write-Host "  ProductCode    : {$newGuid}"
Write-Host ""
Write-Host "이제 Visual Studio 에서 CareCon Setup 프로젝트를 빌드하세요."
