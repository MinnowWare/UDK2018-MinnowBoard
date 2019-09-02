# UDK2018-Minnowboard

## Goal
1. Get the MinnowBoard running with UDK2018 and VS2019.
2. Demonstrate the [**_CdePkg_**](https://github.com/MinnowWare/CdePkg#cdepkg) and the [**_CdeValidationPkg_**](https://github.com/MinnowWare/CdeValidationPkg#cdevalidationpkg) for *Standard C Library* conform Hosted Environment for UEFI driver development

NOTE: Visual Studio is here only used for editing the project. The build process is still pure EDK!
      (the startup error message of VS2017/VS2019 can be ignored)

## HowTo
### 1. install a build machine according to https://github.com/MinnowWare/HowTo-setup-an-UEFI-Development-PC

### 2. clone the UDK2018-MinnowBoard project `--recursive`ly

i.  git.exe clone --progress --recursive -v "https://github.com/MinnowWare/UDK2018-MinnowBoard.git" "A:\UDK2018-MinnowBoard"<br>
	or<br>
ii. right click to open the context menu -> Git clone<br>
![cloneimage2](clonedest.png)

### 3. To build the EDK emulation:
`edk2\launch2019.bat` to setup EDK2 / emulation (Nt32Pkg) build environment<br>
`rd /s /q build` in the `edk2`directory to clean previous EDK2 / emulation build<br>
`bldDBG.BAT` (Debug) <del> or `bldREL.BAT` (Release)</del> to start the build process in the EDK2 directory<br>
`start DBG` to start the UEFI BIOS EMULATION<br>

### 4. To build the MinnowBoard:
`.\launch2019.bat` to setup MinnowBoard (`Vlv2TbltDevicePkg`) build environment<br>
`rd /s /q build` in the `.\`directory to clean previous `Vlv2TbltDevicePkg` build<br>
`bldREL.BAT` (Release) or `bldDBG.BAT` (Debug) to start the build process in the `.\`directory<br>
If a DediProg is connected to the system, the BIOS is automatically updated in the target system MinnowBoard<br>

**NOTE: EDKEmulation build and MinnowBoard build CAN NOT be used alternating in the
      same command box.**

## Known Bugs
1. fixed 20190830: <del>*Capsule* doesn't work.</del>
2. before BIOS build previous *CdePkg* build and FV files has to be deleted manually (clrCdePkgBuild.bat)

## Revision History
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
