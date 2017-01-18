To compile the USB drivers you must
1) install the DDK
2) open a command window for building (build environment must be initialized for that cmd window by calling setenv.cmd on the appropriate client directory) 
3) build with the script build_usb_drivers in %SPOCLIENT%\Tools\scripts directory 
   usage: build_usb_drivers.cmd DDK_INSTALLDIR 

   example: build_usb_drivers.cmd c:\WINDDK\6001.18001

Output directories are under the drivers' source tree. Please move the sys files in the corresponding sub-directories in the inf folder before installing. 


NOTE: the build_usb_drivers script will only build the MFUSB_PortingKitSample driver.  The MFUSB_DualInterface_PortingKitSample driver is the same 
as the previous one but can handle 4 endpoints instead of two and allows debugging over USB the first pair of endpoints while reading/writing 
on the other pair. 