@echo off
set path=%path%;"c:\NASM"
set PYTHON_HOME=C:\Python27
cd edk2
if exist conf rd /s /q Conf

if not exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
	echo #####################################################################################
	echo ######################## Visual Studio 2019 is not installed ########################
	echo #####################################################################################
	ping -n 10 127.0.0.0 > NUL
	goto EOF
) else (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
	call edksetup.bat Rebuild
	call edksetup --nt32
)
echo %~d0
%~d0
echo ###################################################################################
echo ###                                                                             ###
echo ### To build the EDK emulation:                                                 ###
echo ###     "BUILD -t VS2019" to build the EDK2                                     ###
echo ###     "rd /s /q build" to clean previous build                                ###
echo ###     "start DBG" to start the UEFI BIOS EMULATION                            ###
echo ###                                                                             ###
echo ### To build the MinnowBoard:                                                   ###
echo ###     cd ..\edk2-platforms\Vlv2TbltDevicePkg ^& Build_IFWI.bat MNW2 Release    ###
echo ###     (the BIOS binary is placed in the STITCH directory)                     ###
echo ###                                                                             ###
echo ### NOTE: EDKEmu build and MinnowBoard build CAN NOT be used alternating in the ###
echo ###       same command box.                                                     ###
echo ###################################################################################
cmd.exe
:EOF