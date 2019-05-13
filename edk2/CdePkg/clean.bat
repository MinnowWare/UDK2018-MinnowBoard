@echo off
echo clean up...

rem if exist %~p0x64\Efi64\*.pdb del /s %~p0x64\Efi64\*.pdb
rem if exist %~p0x64\Efi64\*.map del /s %~p0x64\Efi64\*.map
rem if exist %~p0x64\Efi64\*.ilk del /s %~p0x64\Efi64\*.ilk
rem 
rem if exist %~p0x64\Win64\*.pdb del /s %~p0x64\Win64\*.pdb
rem if exist %~p0x64\Win64\*.map del /s %~p0x64\Win64\*.map
rem if exist %~p0x64\Win64\*.ilk del /s %~p0x64\Win64\*.ilk
rem 
rem if exist %~p0x64\Win64.msft\*.pdb del /s %~p0x64\Win64.msft\*.pdb
rem if exist %~p0x64\Win64.msft\*.map del /s %~p0x64\Win64.msft\*.map
rem if exist %~p0x64\Win64.msft\*.ilk del /s %~p0x64\Win64.msft\*.ilk

if exist %~p0Debug rd /s /q %~p0Debug
if exist %~p0x64 rd /s /q %~p0x64

rem for /F  %%d in ('dir /b /AD') do if exist %%d\x64 rd /s /q %%d\x64
for /F  %%d in ('dir /b /AD') do if exist %%d\doxygen.tmp rd /s /q %%d\doxygen.tmp
for /F  %%d in ('dir /b /AD') do if exist %%d\Release rd /s /q %%d\Release
