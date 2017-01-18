HelloWorldClient Micro Framework Sample
=======================================
This sample demonstrates the following:
- Consuming WCF Web services for devices, by using a Device Profilefor Web 
  Services (DPWS) client.
- Reading and writing objects from the WCF server.
- Data serialization.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Sockets API and Ethernet controller 
   API and provides a valid configuration for the Ethernet controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface.

The HelloWorldClient sample works in conjunction with the HelloWorldServer_WCF sample.

NOTE: DPWS discovery (the TestApplication.Discover method) does not work when both 
the HelloWorldClient_MF and HelloWorldServer_WCF are run on the same machine.  The
sample will still work however because the endpoint is set manually on discovery 
failure.  Also be aware that if you are testing the WCF-DPWS samples in a secure
network you may need to configure the firewall and IPSec settings in order for this 
sample to work.

To build and run this sample:
1. Open HelloWorldClient.sln in Visual Studio 2010.

2. Open HelloWorldServer_WCF\WCFServer.sln in another instance of Visual Studio 2010.

3. To run a sample in the Emulator, open the Properties page for the project, 
   set the Transport property to Emulator, and then select the emulator you want 
   to use. 

   To run a sample on a device, open the Properties page for the project, set 
   the Transport property to the transport that your device supports, and then 
   select the device you want to target. 

4. In HelloWorldClient, in the Build menu, select Start Debugging (or press 
   F5).

5. In the HelloWorldServer_WCF\WcfServer project, open the file 
   ServiceHelloWCF.svc.cs.  Set a breakpoint on the method
   ServiceHelloWCF.HelloWCF.
   
6. In HelloWorldClient, in the Build menu, select Start Debugging (or press F5).  

7. Place other breakpoints to investigate the behavior of other DPWS messaging 
   features. 


This solution provides sample code, rather than ship-ready code, and is provided 
for instructional purposes only.
  
The HelloWorldClient sample was created using MFSvcUtil, which is a code 
generation tool for .NET Micro Framework.  To produce the auto generated 
client and server proxies, run the MFSvcUtil.exe on a WSDL file.  If the WSDL
is not present, it can be generated with the MFSvcUtil tool by running it on 
the web service assembly.
