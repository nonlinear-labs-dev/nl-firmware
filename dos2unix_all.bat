%~d0
cd %~dp0
for /R %%f in (*.sh *.txt ohook nlhook) do if not exist "%%f\*" dos2unix.exe "%%f"
pause
