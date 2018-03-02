@echo off

if NOT "%1"=="" GOTO :ARGSOK
if NOT "%2"=="" GOTO :ARGSOK

@echo.
@echo Error: Invalid Arguments!
@echo.
@echo Usage: build SPOCLIENT SolutionName [msbuild args]
@echo.
GOTO :EOF

:ARGSOK

if NOT "%NETMFVS_SETENV%"=="" GOTO :VARSOK

@echo.
@echo NETMFVS_SETENV variable is not set.
@echo.
GOTO :EOF

:VARSOK

setlocal
cd /d %1
call %NETMFVS_SETENV%
call %1\setenv_gcc 7.2.1 c:\gcc7.2.1
cd %1\Solutions\%2
msbuild dotnetmf.proj %~3 %NETMFVS_MSBUILD%
endlocal&&exit /B %ERRORLEVEL%