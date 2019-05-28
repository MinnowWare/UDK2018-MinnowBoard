# UDK2018-Minnowboard

## Goal
Get the MinnowBoard running with UDK2018.

NOTE: Visual Studio is here only used for editing the project. The build process is still pure EDK!
      (the startup error message of VS2017 can be ignored)

## HowTo
1. install a build machine according to https://github.com/MinnowWare/HowTo-setup-an-UEFI-Development-PC
2. get the GIT repository WebURL

  ![cloneimage](cloneaddr.png)
  
3. in the file explorer open the builddrive

4. right click to open the context menu -> Git clone

  ![cloneimage2](clonedest.png)

5. download the repository

6. run startBldEnv.bat

7. To build the EDK emulation:

       "BUILD -t VS2017" to build the EDK2
       
       "rd /s /q build" to clean previous build
       
       "start DBG" to start the UEFI BIOS EMULATION

8. To build the MinnowBoard:
		cd ..\edk2-platforms\Vlv2TbltDevicePkg
		
		Build_IFWI.bat MNW2 Release
		
		(the BIOS binary is placed in the STITCH directory)

NOTE: EDKEmu build and MinnowBoard build CAN NOT be used alternating in the
      same command box.

## Revision History

### 20190528
* removed CdePkg from main tree
* add submodules CdePkg and CdeSrcPkg
* [BIOS binary](https://github.com/MinnowWare/UDK2018-MinnowBoard/blob/master/edk2-platforms/Vlv2TbltDevicePkg/Stitch/MNW2MAX1.X64.0100.R01.1905282259.bin)

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
