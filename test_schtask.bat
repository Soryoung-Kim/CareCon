@echo off
schtasks /create /tn "CareCon" /xml "%TEMP%\CareCon_task.xml" /f 2>&1
echo Result: %ERRORLEVEL%
schtasks /query /tn "CareCon" /fo LIST 2>&1
pause
