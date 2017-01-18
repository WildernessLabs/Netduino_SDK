FtpServer Sample
=================
This sample demonstrates the following:
-  Using the Ftp server support types to create an FTP file server.  
-  Mapping the host name to the IP address of your PC or device.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator or newer version.
-  Any physical device that implements Sockets API and the Block Storage API. 
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface 
   and the Microsoft.SPOT.Emulator.BlockStorage interface.


To build and run this sample:

A. Initially Running the FtpServer Application
1. Open FtpServer.sln in Visual Studio 2008.

2. In Solution Explorer, right-click the Solution node (not the FtpServer 
   project node) and then click Properties.  The Solution 'FtpServer' Property 
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


B. Stepping Through the Code
1. Close the FtpServer application.

2. Open the file FtpServer.cs.  Set a breakpoint on the ProcessClientRequest 
   method, and in particular set one in the switch statement.
   
3. Press F5 to start debugging.  

4. In command shell, connect to the FTP applicaiton running in the emulator.

5. Observe that the switch statement cases are used only for the operations
   related to the directory 'special'. 

When finished, restore your Internet Explorer Connection settings.

   
For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
