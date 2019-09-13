# UDK2018-Minnowboard / [CdePkg](https://github.com/MinnowWare/CdePkg#cdepkg)
* [Introduction](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#introduction)
* [Goal](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#goal)
* [HowTo/install/build](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#howtoinstallbuildedit)
	* [Installation](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#installation)
	* [Directory structure](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#directory-structure)
	* [Build](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#build)
	* [Edit](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#edit)
* [Known Bugs](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#known-bugs)
* [Revision History](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/README.md#revision-history)


![pci1](https://minnowboard.org/wp-content/uploads/2017/10/MBTurbot-quad-core-Top-0001-171002-1-555x370.png)

https://minnowboard.org/

## Introduction
The MinnowBoard familiy is an *open source* Personal Computer hardware originally created by a company called
*ADI Engineering*, that belongs now to [Silicom](https://www.silicom-usa.com/)

Intel provides [binary modules](https://firmware.intel.com/projects/minnowboard-max) and maintains the [build environment](https://github.com/tianocore/edk2-platforms/blob/master/Platform/Intel/Vlv2TbltDevicePkg/Readme.md)
to get the MinnowBoard running with the open source [UEFI BIOS Tianocore\EDK2](https://github.com/tianocore/edk2.git)

UDK2018-MinnowBoard is the Git-*Superproject* to build the MinnowBoard UEFI BIOS

## Goal
1. 	**The main aspect is, to introduce the [**_CdePkg_**](https://github.com/MinnowWare/CdePkg#cdepkg) and
	the [**_CdeValidationPkg_**](https://github.com/MinnowWare/CdeValidationPkg#cdevalidationpkg)to the
	UEFI/Tianocore open source community.**

2. get the MinnowBoard and the EDK2 Emulation (Nt32Pkg) running with latest released UDK2018 and latest Visual Studio VS2019 buildenvironment
3. use OpenSSL_1_1_0-stable in the component CryptoPkg\Library\OpensslLib


It is considered as a improvement over traditional UEFI BIOS development, since it introduces
* stable, precise, chipset (ACPI timer) independant C library conform [`clock()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/clock?view=vs-2019) for POST and UEFI Shell in millisecond resolution
* unlimited (buffer less) [`printf()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=vs-2019)-family and [`scanf()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l?view=vs-2019)-family implementation
* Standard C conform format specifiers for [`printf()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=vs-2019)-family and [`scanf()`](https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/scanf-scanf-l-wscanf-wscanf-l?view=vs-2019)-family
* satisfies all the Microsoft C compiler intrinsic requirements with one and the same library (C++ not supported)
* introduction of *Hosted Environment* instead of *Freestanding Environment* for POST driver

This improvements shall enable UEFI and [modernFW](https://github.com/intel/ModernFW#modernfw-project)
to provide a Standard C interface for any "C"-(open)source-based extention.

NOTE: Visual Studio is here only used for editing the project. The build process is still pure EDK!
      (the startup error message of VS2017/VS2019 can be ignored)

## HowTo/install/build/Edit
### Installation
1. install a build machine according to https://github.com/MinnowWare/HowTo-setup-an-UEFI-Development-PC#howto-setup-an-uefi-development-pc

2. clone the UDK2018-MinnowBoard project `--recursive`<br>
	i.  git.exe clone --progress --recursive -v "https://github.com/MinnowWare/UDK2018-MinnowBoard.git" "A:\UDK2018-MinnowBoard"<br>
	or<br>
	ii. right click to open the context menu -> Git clone<br>
![cloneimage2](clonedest.png)
### Directory structure
The given MinnowBoard SourceCode tree is extended by files and directories shown below:

![dirarch](https://github.com/MinnowWare/pictures/blob/master/directory.png)


### Build
3. To build the EDK emulation:<br>
`edk2\launch2019.bat` to setup EDK2 / emulation (Nt32Pkg) build environment<br>
`rd /s /q build` in the `edk2`directory to clean previous EDK2 / emulation build<br>
`bldDBG.BAT` (Debug) <del> or `bldREL.BAT` (Release)</del> to start the build process in the EDK2 directory<br>
`start DBG` to start the UEFI BIOS EMULATION<br>

4. To build the MinnowBoard:<br>
`.\launch2019.bat` to setup MinnowBoard (`Vlv2TbltDevicePkg`) build environment<br>
`rd /s /q build` in the `.\`directory to clean previous `Vlv2TbltDevicePkg` build<br>
`bldREL.BAT` (Release) or `bldDBG.BAT` (Debug) to start the build process in the `.\`directory<br>
If a DediProg is connected to the system, the BIOS is automatically updated in the target system MinnowBoard<br>

**NOTE: EDKEmulation build and MinnowBoard build CAN NOT be used alternating in the
      same command box.**

### Edit

UDK2018-MinnowBoard is the Git-[Superproject **UDK2018-MinnowBoard**](https://github.com/MinnowWare/UDK2018-MinnowBoard) to build the MinnowBoard UEFI BIOS.

It includes the *edk2* which is the Git-[Subproject **edk2-vUDK2018**](https://github.com/MinnowWare/edk2-vUDK2018), that in turn
includes the *Cde*-directories as GIT-[Subproject **CdePkg**](https://github.com/MinnowWare/CdePkg) and 
GIT-[Subproject **CdeValidationPkg**](https://github.com/MinnowWare/CdeValidationPkg).



## Known Bugs
1. fixed 20190830: <del>*Capsule* doesn't work.</del>
2. before BIOS build previous *CdePkg* build and FV files has to be deleted manually (clrCdePkgBuild.bat)

## Revision History
### 20190906/improve tool chain check
* add IASL version check
* add PYTHON version check
* add NASM version check
### 20190901
* add Dediprog flash update
* [BIOS binary](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/edk2-platforms/Vlv2TbltDevicePkg/Stitch/MNW2MAX1.X64.0101.R01.1909012230.bin)

### 20190830
* CdeValPkg branch merge

### 20190830\OpenSSL
* add OpenSSL_1_1_0-stable to CryptoPkg\Library\OpensslLib

### 20190830
* improved build and startup batches
* [BIOS binary](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/edk2-platforms/Vlv2TbltDevicePkg/MNW2MAX1.X64.0101.R01.1908292234.bin)

### 20190829
* convert EDK2 directory to submodule, to demonstrate *CdePkg* separated from entire MinnowBoard environment
* update EDK2 to *20190828/OpenSSL*
* integrate *Release 1.01 Firmware Download (Aug 2019)* from https://firmware.intel.com/projects/minnowboard-max

### 20190822
* place CDE*.SLN solution files into EDK2 directory, to be able to use Visual Studio solutions
  EDK2 directory without MinnowBoard build environment
* improve CdeValidationPkg

### 20190811
* add validation driver/executables for all `ctype.h`- and `wctype.h`-related functions:
    (ctypeALLPei, ctypeALLDxe, wctypeALLPei, wctypeALLDxe)
* remove previously used single function test drivers

### 20190802/1
* WinNTx86-64 (Microsoft C Library)
* WinNTx86-64 (Torito C Library)

### 20190730
* add all <em>CTYPE.H()</em> -functions to PEI post phase

###	20190728
* add all <em>CTYPE.H</em>-functions for DXE

### 20190727/2
* add <em>clockPei()</em>

### 20190726
* initial version of branch CdeValPkg
* add MFNBAR as bare/nacked parameter for CDEMOFINE macro
* add clockDxe commandline to CdeLoadOptions.h
* update CdeSrcPkg/b81394c620206ebbc300216652cd43d7f4ac94e3


### 20190709
* add initial PEI Support, rudimentary functional range
* [BIOS binary](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/edk2-platforms/Vlv2TbltDevicePkg/Stitch/MNW2MAX1.X64.0100.R01.1907070918.bin)

### 20190626
* add VS2019 support (for both MinnowBoard- and Emulationbuild)

### 20190605
* removed CdePkg from main tree
* add submodules CdePkg and CdeSrcPkg
* [BIOS binary](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/edk2-platforms/Vlv2TbltDevicePkg/Stitch/MNW2MAX1.X64.0100.R01.1906052251.bin)

### 20190513
* CdePkg: initial revision - standard C Library and standard headerfile usage

### 20190225
* enable emulation build and debug
* improve MinnowBoard build

### 20190224
* added missing files from UDK2017 to build MinnowBoard in UDK2018
* fixed: build MinnowBoard

### 20190222 - initial checkin
* downloaded the entire source code (see HowTo)
* configured as a VS2017 solution - just for editing the source
* not able to build
