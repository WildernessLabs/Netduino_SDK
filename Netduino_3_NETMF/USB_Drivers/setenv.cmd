@echo off

set USB_DIR=%CD%

if /I %FLAVOR_WIN% == release (
    set DDK_FLAVOR=fre
) else (
    set DDK_FLAVOR=chk
)

echo %DDK_BASEDIR%\bin\setenv.bat %DDK_BASEDIR% %DDK_FLAVOR% %*
call %DDK_BASEDIR%\bin\setenv.bat %DDK_BASEDIR% %DDK_FLAVOR% %*

set DDK

cd /d %USB_DIR%
