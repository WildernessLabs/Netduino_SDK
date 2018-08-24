@ECHO ON
SET Proj=Netduino3
cd %SPOCLIENT%
RMDIR %SPOCLIENT%\BuildOutput /S /Q
call setenv_mdk 5.05
cd %SPOCLIENT%\Solutions\%Proj%
msbuild /flp:ErrorsOnly;LogFile=errors.log /v:m
pushd .
cd %SPOCLIENT%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin\tinyclr.hex
move ..\Tinybooter.hex .
ren ER_FLASH ER_FLASH.hex
ren ER_CONFIG ER_CONFIG.hex
%SystemRoot%\explorer.exe %SPOCLIENT%\BuildOutput\THUMB2FP\%COMPILER_TOOL_VERSION%\le\%FLAVOR_MEMORY%\%FLAVOR_DAT%\%Proj%\bin\tinyclr.hex
popd