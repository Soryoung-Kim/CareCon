@echo off
schtasks /create /tn "CareCon" /tr "\"%~dp0CareCon.exe\"" /sc ondemand /rl highest /ru SYSTEM /f
