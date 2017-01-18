SimpleService Sample
====================
This sample demonstrates the following:
- Exposing and consuming Web services for devices, by using a Device Profiles 
  for Web Services (DPWS) client and server.
- Discovery.
- One-way and two-way messaging.
- Attachments (MTOM).
- Eventing.
- Reading and writing objects between the client and server.
- Data serialization.
- How to discover hosted service endpoints.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Sockets API and Ethernet controller 
   API and provides a valid configuration for the Ethernet controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface.

The SimpleService sample works in conjunction with the SimpleServiceClient sample.


To build and run this sample:
1. Open SimpleService.sln in Visual Studio 2008.

2. Open SimpleServiceClient.sln in another instance of Visual Studio 2008.

3. To run a sample in the Emulator, open the Properties page for the project, 
   set the Transport property to Emulator, and then select the emulator you want 
   to use. 

   To run a sample on a device, open the Properties page for the project, set 
   the Transport property to the transport that your device supports, and then 
   select the device you want to target. 

4. In SimpleServiceClient, in the Build menu, select Start Debugging (or press 
   F5).

5. In the SimpleServiceClient project, open the file 
   MFCodeGenSimpleServiceClient.cs.  Set a breakpoint on the method
   m_simpleControl_HelloEvent.
   
6. In SimpleService, in the Build menu, select Start Debugging (or press F5).  
   After some time, such as 20-30 seconds, the service sends the Discovery 
   'Hello' message, and the breakpoint is hit. 

7. Place other breakpoints to investigate the behavior of other DPWS messaging 
   features. 


Any actual application that uses this code will require modifications of this 
code. This solution provides sample code, rather than ship-ready code, 
and is provided for instructional purposes only.
  
The SimpleService and SimpleServiceClient samples were created using MFSvcUtil, 
which is a code generation tool for .NET Micro Framework.
