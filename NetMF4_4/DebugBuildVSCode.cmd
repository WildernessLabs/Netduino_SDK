setlocal
call setenv_gcc 7.2.1 c:\gcc7.2.1
cd Solutions\STM32F4Discovery
msbuild dotnetmf.proj /t:build /p:flavor=debug;memory=flash /filelogger /flp:ErrorsOnly  /clp:verbosity=minimal /maxcpucount /v:m 
@REM must rename the image file to include the HEX extension
cd ..\..\BuildOutput\THUMB2FP\GCC7.2\le\FLASH\debug\STM32F4Discovery\bin\tinyclr.hex
rename ER_FLASH ER_FLASH.hex
endlocal&&exit /B %ERRORLEVEL%