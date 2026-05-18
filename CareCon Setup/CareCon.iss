#define MyAppName "CareCon"
#define MyAppVersion "2.0.1"
#define MyAppPublisher "METASTORY"
#define MyAppExeName "CareConLaunch.exe"
#define MyAppInstallDir "{autopf}\METASTORY\CareCon"

[Setup]
AppId={{816E45AF-24AE-4E51-8C63-E22E713C9DF9}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={#MyAppInstallDir}
DefaultGroupName={#MyAppName}
OutputDir=.\Release
OutputBaseFilename=CareConSetup
SetupIconFile=..\실행\CareCon.ico
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "korean"; MessagesFile: "compiler:Languages\Korean.isl"

[Tasks]
Name: "desktopicon"; Description: "바탕 화면에 아이콘 만들기"; GroupDescription: "추가 아이콘:"; Flags: unchecked

[Files]
Source: "..\실행\CareCon.exe";          DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\CareConUpdate.exe";    DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\CareConLaunch.exe";    DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\CareCon.ini";          DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\carecon.cfg";          DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\banner.png";           DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\CareCon.ico";          DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\WebView2Loader.dll";   DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\libmysql.dll";         DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\mfc140u.dll";          DestDir: "{app}"; Flags: ignoreversion
Source: "..\실행\vcruntime140.dll";     DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}";                  Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\CareCon.ico"
Name: "{commondesktop}\{#MyAppName}";          Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\CareCon.ico"; Tasks: desktopicon

[Registry]
Root: HKLM; Subkey: "Software\{#MyAppPublisher}"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\{#MyAppPublisher}\CareCon"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"; Flags: uninsdeletekey

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "CareCon 시작"; Flags: nowait postinstall skipifsilent
