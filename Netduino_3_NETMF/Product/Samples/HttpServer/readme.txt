HttpServer Sample
=================
This sample demonstrates the following:
-  Using the Http and Https classes to create an HTTP or HTTPS file server.  
-  Setting up certificates.
-  Mapping the host name to the IP address of your PC or device.
-  Validating DNS.
-  Simple HTTP(s) Remote Update for supported platforms

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements Sockets API and the Block Storage API. 
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface 
   and the Microsoft.SPOT.Emulator.BlockStorage interface.
-  MFUpdate support is currently only implemented on the iMXS_net_open platform.

MFUpdate procedure
-  When you build your solution the images are compressed and concatenated 
   automatically in the last step of the build process.  This generates a 
   series of .nmf files (one for each target binary file ER_DAT.nmf, ER_FLASH.nmf, 
   ER_CONFIG.nmf and TinyCLR.nmf).  The TinyCLR.nmf file contains all of the 
   normal firmware images (ER_FLASH, ER_DAT and ER_CONFIG).  When using this
   HTTP(s) server sample you can test firmware uploads by selecting one of these
   .nmf files in the "Install Firmware Update" file upload.  Currently only the
   iMXS_net_open solution supports this sample.  If when you upload the firmware
   image file, the browser will generate a new tab which will be busy until the 
   download is complete (please be patient).  The output window should display
   the current packet progress during the install.  After the download is complete,
   the device will reset and decompress the image.  During this time the device will
   appear unresponsive for around 30 seconds while the image is installed.

To build and run this sample:

A. Initially Running the HttpServer Application
1. Open HttpServer.sln in Visual Studio 2008.

2. In Solution Explorer, right-click the Solution node (not the HttpServer 
   project node) and then click Properties.  The Solution 'HttpServer' Property 
   Page dialog box appears.

3. On the left, expand the Configuration Properties node, and then select the 
   Configuration node.  Select the Build check box and the Deploy check box, 
   then click OK.

4. To run this sample in an emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

5. In the Build menu, select Start Debugging (or press F5).

6. In Visual Studio, in the Output pane, click the Output tab and observe the 
   debugger output.

Continue to the next steps.


B. Setting Up Certificates
To connect from the Web browser to the https server running on the device or on 
an emulator, you need to do the following steps.  The instructions include 
setup steps for both emulators and devices, which require different 
certificates.

1. Install the certificate for the emulator or device on your PC, as follows.  
   In Windows Explorer, go to the directory for the HttpServer sample, and then 
   go to the Certificates subdirectory.  

2. Right-click the appropriate .cer file, and select Open.  
   - If you are using an emulator, use the file NetMFCert.cer, which uses the 
     server zachl-sba1.redmond.corp.microsoft.com.
   - If you are using a device, use the file Device.Microsoft.Com.cer, which 
     uses the server Device.Microsoft.Com.
   The Certificate dialog box appears.
   
3. On the General tab, click the Install Certificate button.  The Certificate 
   Import Wizard appears.  
   
4. Click Next.  The Certificate Store wizard page appears.  
   
5. Select "Place all certificates in the following store".  Click the Browse 
   button.  The Select Certificate Store dialog box appears.  Select "Trusted 
   Root Certification Authorities".  Click OK.  
   
6. Click Next.  Click Finish.  A Security Warning dialog box appears.  

7. Click Yes to install the certificate.  The Certificate Import Wizard dialog 
   box appears, with the message "The import was successful."  Click OK.
   
   If you view this list of certificates using Explorer, in the Certification 
   Path tab, the status should be "OK".

Continue to the next steps.


C. Mapping the host name to the IP address of your PC or device
   (This step is only required if you do not want to see a warning about the 
   certificate's CN name not matching the URL.)

1. Obtain the IP address of your PC (if using an emulator) or device.  To get 
   the IP address of your PC: open a command prompt, and issue the ipconfig 
   command.  Use the "IPv4 Address" value.
   
2. Click the Start menu, point to Programs, point to Accessories, right-click 
   Notepad, and then select Run as administrator.  The User Account Control 
   dialog appears (on Vista); click Continue.  Notepad appears.
   
3. Open the hosts file, which is located as follows in Vista:
   C:\Windows\System32\drivers\etc\hosts
   
4. Add a line that maps map the host name to the IP address of your PC (if 
    using an emulator) or device.  The hosts file stores entries for DNS lookup.
   - For an emulator, map "zachl-sba1.redmond.corp.microsoft.com" to the IP 
     address of your PC.  For example:
     10.20.30.40    zachl-sba1.redmond.corp.microsoft.com
   - For a device, map "Device.Microsoft.Com" to the IP address of your device.
   Save and close the hosts file.

Continue to the next steps.


D. Validating DNS
   Validate that DNS is properly set up.  To do this, open a command prompt.
   - For an emulator, issue the command:
     ping zachl-sba1.redmond.corp.microsoft.com
   - For a device, issue the command:
     ping Device.Microsoft.Com
   Make sure that the device responds to the ping request.

Continue to the next steps.


E. Loading Pages
1. Open a Web browser.  In Internet Explorer, click the Tools menu, and then 
   click Options.  The Internet Options dialog box appears.  
   
2. Click the Connections tab, and then click the "LAN settings" button.  The 
   "Local Area Network (LAN) Settings" dialog box appears.  
   
3. Note the existing settings.  Clear the check box "Automatically detect 
   settings".  Clear the check box "Use a proxy server for your LAN".  Click OK.  
   Click OK.

4. Open the HTTP site:
   - Emulator: http://zachl-sba1.redmond.corp.microsoft.com:81
     The emulator uses port 81 for HTTP, because the PC doesn't allow the 
     emulator to use port 80.
   - Device: http://Device.Microsoft.Com
     If the page http://Device.Microsoft.Com doesn't load, either the mapping of 
     host name Device.Microsoft.Com is invalid, or the server doesn't run.  Try 
     to ping your device on the IP address, and then on Device.Microsoft.Com.
     Try to type http:// followed by your device IP address.  If this works, 
     check the mapping from Device.Microsoft.Com to your device IP address.

   A demonstration Web page appears.  You can use this page to demonstrate HTTP 
   capabilities.

5. Open the HTTPS site:
   - Emulator: https://zachl-sba1.redmond.corp.microsoft.com
   - Device: https://Device.Microsoft.Com
     If the page https://Device.Microsoft.Com doesn't load, try 
     http://Device.Microsoft.Com.  The device runs both HTTP and HTTPS server, 
     so the same page appears.
     If http://Device.Microsoft.Com loads, but https://Device.Microsoft.Com 
     doesn't load, the certificate "Device.Microsoft.Com.cer" was not adding 
     into the list of Trusted Root Certification Authorities.  Check the status 
     of the certificate.

   A demonstration Web page appears.  You can use this page to demonstrate HTTPS 
   capabilities.

Continue to the next steps.


F. Stepping Through the Code
1. Close the HttpServer application.

2. Open the file HttpServer.cs.  Set a breakpoint on the ProcessClientGetRequest 
   method.
   
3. Press F5 to start debugging.  

4. In your Web browser, access the HTTP and HTTPS URLs that are listed above.  
   The breakpoint is hit.  Step through the code.

When finished, restore your Internet Explorer Connection settings.

   
For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
