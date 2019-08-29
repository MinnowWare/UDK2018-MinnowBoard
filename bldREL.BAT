@echo off
cd edk2-platforms\Vlv2TbltDevicePkg
call Build_IFWI.bat MNW2 Release 
cd %~p0
call edk2\Conf\BiosId.bat
if exist edk2-platforms\Vlv2TbltDevicePkg\Stitch\%BIOS_ID%.bin (
    echo ##############################################################################
    echo ############################ BUILD SUCCESS ###################################
    echo ##############################################################################
    @dir /s /b edk2-platforms\Vlv2TbltDevicePkg\Stitch\%BIOS_ID%.bin
) else (
    echo ##############################################################################
    echo #### BUILD FAIL ## BUILD FAIL ## BUILD FAIL ## BUILD FAIL ## BUILD FAIL ######
    echo ##############################################################################
)