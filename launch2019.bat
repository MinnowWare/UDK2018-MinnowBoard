@echo off

set path=%path%;"c:\NASM";"%~dp0"edk2\openssl-1.0.2r-x64_86-win64;"%~dp0"edk2\MinnowTools;C:\Program Files (x86)\DediProg\SF100;
set PYTHON_HOME=C:\Python27
cd edk2
if exist conf rd /s /q Conf

if not exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" (
	echo #####################################################################################
	echo ######################## Visual Studio 2019 is not installed ########################
	echo #####################################################################################
	ping -n 10 127.0.0.0 > NUL
	goto EOF
) else (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
        %~d0
    cd "%~p0"edk2\MinnowTools
    rem ::::::::::::::::::::::::::::::::::::::::::::::::::::::
    rem ::: build all sources in the MinnowTools directory ::::
    rem ::::::::::::::::::::::::::::::::::::::::::::::::::::::
    echo ###################################################################################
    echo ###              build all sources in the MinnowTools directory                 ###
    echo ###################################################################################
    for %%c in (*.c) do cl /nologo %%c
    rem ping 127.0.0.0 > nul
        cd "%~p0"\edk2
	call edksetup.bat Rebuild
	call edksetup --nt32
)
%~d0
cd "%~p0"
rem echo %~dp0
rem :::::::::::::::::::::::::::::::::::::::::::::
rem ::: test GIT version ::::::::::::::::::::::::
rem :::::::::::::::::::::::::::::::::::::::::::::
gitverchk.exe
echo ###################################################################################
echo ###                                                                             ###
echo ###                             -=[ VS2019 ]=-                                  ###
echo ###                                                                             ###
echo ### To build the MinnowBoard for Release or Debug invoke:                       ###
echo ###     bldREL.bat                                                              ###
echo ###        or                                                                   ###
echo ###     bldDEB.bat                                                              ###
echo ###                                                                             ###
echo ### NOTE: EDKEmulation build (Nt32Pkg) and MinnowBoard build CAN NOT be used    ###
echo ###       alternating in the same command box.                                  ###
echo ###       A new command box needs to be started                                 ###
echo ###################################################################################
%COMSPEC% /k echo Welcome, to the jungle...
:EOF