HelloWCFServer Sample
====================
This sample demonstrates the following:
- Exposing Web services, by using a Device Profiles for Web Services (DPWS) 
  server.
- Reading and writing objects between the client and server.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Sockets API and Ethernet controller 
   API and provides a valid configuration for the Ethernet controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface.

The HelloWCFServer sample works in conjunction with the HelloWorldClient_WCF
sample.


To build and run this sample:
1. Open HelloWCFServer.sln in Visual Studio 2010.

   a. To run a sample in the Emulator, open the Properties page for the project, 
      set the Transport property to Emulator, and then select the emulator you want 
      to use. 

   b. To run a sample on a device, open the Properties page for the project, set 
      the Transport property to the transport that your device supports, and then 
      select the device you want to target. 

2. In HelloWCFServer, in the Build menu, select Start Debugging (or press F5).

3. Open HelloWorldClient_WCF\HelloWorldClient_WCF.sln in another instance of 
   Visual Studio 2010.

4. In the HelloWorldClient_WCF project, open the file Program.cs.  Set a breakpoint 
   on the method Program.Main.
   
5. In HelloWorldClient_WCF, in the Build menu, select Start Debugging (or press F5).  

6. Place other breakpoints to investigate the behavior of other DPWS messaging 
   features. 


This solution provides sample code, rather than ship-ready code, 
and is provided for instructional purposes only.
  
The HelloWCFServer sample was created using MFSvcUtil, which is a code 
generation tool for .NET Micro Framework.  To produce the auto generated 
client and server proxies, run the MFSvcUtil.exe on a WSDL file.  If the WSDL
is not present, it can be generated with the MFSvcUtil tool by running it on 
the web service assembly.
