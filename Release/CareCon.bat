@echo off
timeout /t 5 /nobreak
set INSTALL_PATH=%~1
start "" "%INSTALL%\CareCon.exe"
exit