# MINNOWBOARD MAX/TURBOT PLATFORM FIRMWARE

This UEFI firmware is designed for the open hardware MinnowBoard Max/Turbot platforms using
the Intel(R) Atom(tm) Processor E3800 Series, based on UDK2017 core packages.

## **Download the sources**

### Download official released sources:

 Please refer to the release notes to download official released sources.

 Get release notes from https://firmware.intel.com/projects/minnowboard-max

### Download the latest sources:

(Official releases sources is recommanded as it is passed stability testing)
```
1. Create a new directory for use as your WORKSPACE (e.g. "C:\MyWorkspace").

2. Download below repository to this WORKSPACE:

edk2 repository
    git clone https://github.com/tianocore/edk2.git -b UDK2017
    cd edk2
    git checkout vUDK2017

edk2-BaseTools-win32 repository
    git clone https://github.com/tianocore/edk2-BaseTools-win32.git
    cd edk2-BaseTools-win32
    git checkout 0e088c19ab31fccd1d2f55d9e4fe0314b57c0097

edk2-platforms repository
    git clone https://github.com/tianocore/edk2-platforms.git -b devel-MinnowBoardMax-UDK2017

3. Download MinnowBoard MAX Binary Object Modules from https://firmware.intel.com/projects/minnowboard-max

    IA32FamilyCpuPkg
    Vlv2BinaryPkg
    Vlv2MiscBinariesPkg
   
Create a new directory named "silicon" below WORKSPACE (e.g. "C:\MyWorkspace\silicon\"). 
Copy the three folders to "silicon" directory .
```

## **Windows Build Instructions**

### Pre-requisites

* Microsoft Visual Studio.
  - Visual Studio 2008/2010/2012/2013.
* ASL compiler: Available from http://www.acpica.org
  - Install into ```C:\ASL``` to match default tools_def.txt configuration.
* NASM assembler:  Available from: http://www.nasm.us/
  - Install into ```C:\NASM``` to match default tools_def.txt configuration.
* Python 2.7.10:  Available from: https://www.python.org/downloads/release/python-2710/
  - Install Python and make sure that a file with the extension of ".py" will be opened by Python.exe.

### Build

1. Open command window, goto the workspace dir, e.g. c:\MyWorkspace.
2. Type "cd edk2-platforms\Vlv2TbltDevicePkg".
3. Build
   - Type "Build_IFWI.bat /IA32 MNW2 Release" to build a 32 bit release version.
   - Type "Build_IFWI.bat /IA32 MNW2 Debug" to build a 32 bit debug version.
   - Type "Build_IFWI.bat MNW2 Release" to build a 64 bit release version.
   - Type "Build_IFWI.bat MNW2 Debug" to build a 64 bit debug version.

## **Linux Build Instructions**

### Pre-requisites

* Linux System Configuration.
  - Ubuntu 16.04 64 bit
  - GCC version 5 or above
* NASM assembler
  - ``# apt-get install nasm``.
* ASL compiler
  - Download ASL build tool from https://acpica.org/downloads
    make sure the iasl tool version that you installed is "20170728" or newer.
  - The requirements for generating ACPICA tools from source code are:
    ```
     flex: version 2.5.4 or greater
	 bison: version 2.4.1 or greater
	
	 # apt-get install bison flex
	```
  - Generate and install the iasl tool in usr/bin
    ```
	 # make clean
	 # make iasl
	 # make install
	```

### Build

1. Open a command prompt, goto the platform package "edk2-platforms\Vlv2TbltDevicePkg".
2. Build
   - Type "./Build_IFWI.sh MNW2 Release" to build a release version.
   - Type "./Build_IFWI.sh MNW2 Debug" to build a debug version.
   
## **Related Materials**

http://minnowboard.org/ -- Platform Information, including schematics and information on where to purchase boards.

https://firmware.intel.com/projects/minnowboard-max -- Release notes, pre-built release/debug binaries, build instructions, and binary objects required to compile firmware.



