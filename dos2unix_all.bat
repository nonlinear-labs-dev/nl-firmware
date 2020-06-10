%~d0
cd %~dp0
for /R %%f in (*.sh *.txt *.in *.service) do dos2unix.exe "%%f"
cd build-tools\epc\hook
for /R %%f in (*.*) do dos2unix.exe "%%f"
cd build-tools\epc\install
for /R %%f in (*.*) do dos2unix.exe "%%f"
pause
