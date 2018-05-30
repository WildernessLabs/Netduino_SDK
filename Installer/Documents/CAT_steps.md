Start by installing the Windows 10 SDK or WDK
- this will install inf2cat and SignTool
https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk 

Roughly follow this guide:
https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/winusb-installation#automatic-installation-of--winusb-without-an-inf-file 


Secret Labs has/had a VeriSign Class 3 Code Signing cert 
open CertMgr and check the "Trusted Publishers" tab (expired 2013-04-10)

https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/winusb-installation

Inf2cat.exe /driver:f:\inf2cat\winusb\ /os:10_x86,10_x64


make sure that the INF file's DriverVer directive has a current time stamp and version value
can use the stampinf tool to do it automatically

pretty sure the cat file needs to be signed after running inf2cat
https://docs.microsoft.com/en-us/windows-hardware/drivers/install/catalog-files 

How to release-sign https://docs.microsoft.com/en-us/windows-hardware/drivers/install/how-to-release-sign-a-driver-package

Digital signatures overview: https://docs.microsoft.com/en-us/windows-hardware/drivers/install/digital-signatures

Test signing a CAT file: https://docs.microsoft.com/en-us/windows-hardware/drivers/install/test-signing-a-catalog-file 

You'll need a MarkCert Test Certificate: https://docs.microsoft.com/en-us/windows-hardware/drivers/install/makecert-test-certificate 
MakeCert -r -pe -ss PrivateCertStore -n "CN=Wildernesslabs.co(Test)" f:\testcert.cer

Install the TestCert - see kmcs_walkthrough.doc from 2007

used https://support.comodo.com/index.php?/Knowledgebase/Article/View/68/0/time-stamping-server at the time server https://stackoverflow.com/questions/9714798/http-timestamp-verisign-com-scripts-timstamp-dll-not-available



Steps:
Create a driver package that contains:
WinUSB co-installer (Winusbcoinstaller.dll)
KMDF co-installer (Wdfcoinstaller01011.dll)
.inf
Signed catalog file - The contents of the driver package must be digitally signed with a Winqual release signature

SignTool sign /v /s PrivateCertStore Wildernesslabs.co(Test) winusb_netduino.cat

SignTool sign /v /s PrivateCertStore /a winusb_netduino.cat