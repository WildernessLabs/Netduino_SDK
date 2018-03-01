@ECHO ON
SET Base=C:\SVN\IngenuityMicro\NETMF\Firmware\NETMF4_4\netmf-interpreter
SET Proj=Netduino3
cd %Base%
RMDIR %Base%\BuildOutput\THUMB2FP /S /Q
setenv_mdk 5.05
cd %Base%\Solutions\%Proj%
msbuild /flp:ErrorsOnly;LogFile=errors.log /v:m
move %Base%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin\Tinybooter.hex %Base%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin\tinyclr.hex\TinyBooter.S19
%SystemRoot%\explorer.exe %Base%\BuildOutput\THUMB2FP\MDK5.05\le\FLASH\release\%Proj%\bin 