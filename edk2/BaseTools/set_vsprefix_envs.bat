@REM @file
@REM   This stand-alone program is typically called by the toolsetup.bat file,
@REM   however it may be executed directly from the BaseTools project folder
@REM   if the file is not executed within a WORKSPACE\BaseTools folder.
@REM
@REM Copyright (c) 2016-2017, Intel Corporation. All rights reserved.<BR>
@REM
@REM This program and the accompanying materials are licensed and made available
@REM under the terms and conditions of the BSD License which accompanies this
@REM distribution.  The full text of the license may be found at:
@REM   http://opensource.org/licenses/bsd-license.php
@REM
@REM THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
@REM WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
@REM IMPLIED.
@REM

@echo off
pushd .

if defined VS71COMNTOOLS (
  if not defined VS2003_PREFIX (
    set "VS2003_PREFIX=%VS71COMNTOOLS:~0,-14%"
  )
)

if defined VS80COMNTOOLS (
  if not defined VS2005_PREFIX (
    set "VS2005_PREFIX=%VS80COMNTOOLS:~0,-14%"
  )
)

if defined VS90COMNTOOLS (
  if not defined VS2008_PREFIX (
    set "VS2008_PREFIX=%VS90COMNTOOLS:~0,-14%"
  )
  if not defined WINSDK_PREFIX (
    set "WINSDK_PREFIX=c:\Program Files\Microsoft SDKs\Windows\v6.0A\bin\"
  )
  if not defined WINSDKx86_PREFIX (
    set "WINSDKx86_PREFIX=c:\Program Files (x86)\Microsoft SDKs\Windows\v6.0A\bin\"
  )
)

if defined VS100COMNTOOLS (
  if not defined VS2010_PREFIX (
    set "VS2010_PREFIX=%VS100COMNTOOLS:~0,-14%"
  )
  if not defined WINSDK7_PREFIX (
    set "WINSDK7_PREFIX=c:\Program Files\Microsoft SDKs\Windows\v7.0A\Bin\"
  )
  if not defined WINSDK7x86_PREFIX (
    set "WINSDK7x86_PREFIX=c:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Bin\"
  )
)

if defined VS110COMNTOOLS (
  if not defined VS2012_PREFIX (
    set "VS2012_PREFIX=%VS110COMNTOOLS:~0,-14%"
  )
  if not defined WINSDK71_PREFIX (
    set "WINSDK71_PREFIX=c:\Program Files\Microsoft SDKs\Windows\v7.1A\Bin\"
  )
  if not defined WINSDK71x86_PREFIX (
    set "WINSDK71x86_PREFIX=c:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\"
  )
)

if defined VS120COMNTOOLS (
  if not defined VS2013_PREFIX (
    set "VS2013_PREFIX=%VS120COMNTOOLS:~0,-14%"
  )
  if not defined WINSDK8_PREFIX (
    set "WINSDK8_PREFIX=c:\Program Files\Windows Kits\8.0\bin\"
  )
  if not defined WINSDK8x86_PREFIX (
    set "WINSDK8x86_PREFIX=c:\Program Files (x86)\Windows Kits\8.0\bin\"
  )
)

if defined VS140COMNTOOLS (
  if not defined VS2015_PREFIX (
    set "VS2015_PREFIX=%VS140COMNTOOLS:~0,-14%"
  )
  if not defined WINSDK81_PREFIX (
    set "WINSDK81_PREFIX=c:\Program Files\Windows Kits\8.1\bin\"
  )
  if not defined WINSDK81x86_PREFIX (
    set "WINSDK81x86_PREFIX=c:\Program Files (x86)\Windows Kits\8.1\bin\"
  )
)

rem MWDebug@REM set VS2017
rem MWDebugif not defined VS150COMNTOOLS (
rem MWDebug  if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
rem MWDebug    for /f "usebackq tokens=1* delims=: " %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"`) do (
rem MWDebug      if /i "%%i"=="installationPath" call "%%j\VC\Auxiliary\Build\vcvars32.bat"
rem MWDebug    )
rem MWDebug  ) else if exist "%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe" (
rem MWDebug    for /f "usebackq tokens=1* delims=: " %%i in (`"%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"`) do (
rem MWDebug      if /i "%%i"=="installationPath" call "%%j\VC\Auxiliary\Build\vcvars32.bat"
rem MWDebug    )
rem MWDebug  ) else (
rem MWDebug    goto SetWinDDK
rem MWDebug  )
rem MWDebug)
rem MWDebug
rem MWDebugif defined VCToolsInstallDir (
rem MWDebug  if not defined VS2017_PREFIX (
rem MWDebug    set "VS2017_PREFIX=%VCToolsInstallDir%"
rem MWDebug  )
rem MWDebug)
rem MWDebug start 
if defined VS150COMNTOOLS (
    if defined VCToolsInstallDir (
        if not defined VS2017_PREFIX (
            set "VS2017_PREFIX=%VCToolsInstallDir%"
            echo ################################ VS2017_PREFIX ################################ 
            ping 127.0.0.0 -n 1 > NUL
        )
    )
)

if defined VS160COMNTOOLS (
    if defined VCToolsInstallDir (
        if not defined VS2019_PREFIX (
            set "VS2019_PREFIX=%VCToolsInstallDir%"
            echo ################################ VS2019_PREFIX ################################ 
            ping 127.0.0.0 -n 1 > NUL
        )
    )
)
rem MWDebug end
if not defined WINSDK10_PREFIX (
  if defined WindowsSdkVerBinPath (
    set "WINSDK10_PREFIX=%WindowsSdkVerBinPath%"
  ) else if exist "%ProgramFiles(x86)%\Windows Kits\10\bin" (
    set "WINSDK10_PREFIX=%ProgramFiles(x86)%\Windows Kits\10\bin\"
  ) else if exist "%ProgramFiles%\Windows Kits\10\bin" (
    set "WINSDK10_PREFIX=%ProgramFiles%\Windows Kits\10\bin\"
  )
)

:SetWinDDK
if not defined WINDDK3790_PREFIX (
  set WINDDK3790_PREFIX=C:\WINDDK\3790.1830\bin\
)

if not defined IASL_PREFIX (
  set IASL_PREFIX=C:\ASL\
)

popd
