@ECHO ON
SET Base=C:\GitHub\Netduino_SDK\NetMF4_4
SET Proj=Netduino3Ethernet
cd %Base%
RMDIR %Base%\BuildOutput /S /Q
setenv_mdk 5.05
cd %Base%\Solutions\%Proj%
msbuild /flp:ErrorsOnly;LogFile=errors.log /v:m
move %Base%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin\Tinybooter.hex %Base%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin\tinyclr.hex\TinyBooter.S19
%SystemRoot%\explorer.exe %Base%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin 