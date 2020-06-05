%~d0
cd %~dp0
for /R %%f in (*.sh *.txt) do dos2unix.exe "%%f"
pause
