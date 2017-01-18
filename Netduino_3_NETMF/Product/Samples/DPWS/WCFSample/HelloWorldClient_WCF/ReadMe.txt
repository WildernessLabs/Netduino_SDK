HelloWorldClient_WCF Sample
====================
This sample demonstrates the following:
- WCF client consuming a Web services from a Micro Framework emulator/device

The HelloWorldClient_WCF sample works in conjunction with the HelloWorldServer_MF 
sample.


To build and run this sample:
1. Open HelloWorldClient_WCF.sln in Visual Studio 2010.

2. Open HelloWorldServer_MF\HelloWCFServer.sln in another instance of Visual 
   Studio 2010.

   a. To run a sample in the Emulator, open the Properties page for the project, 
      set the Transport property to Emulator, and then select the emulator you want 
      to use. 

   b. To run a sample on a device, open the Properties page for the project, set 
      the Transport property to the transport that your device supports, and then 
      select the device you want to target. 

3. In HelloWCFServer project, in the Build menu, select Start Debugging (or press 
   F5).

4. In the HelloWorldClient_WCF project, open the file program.cs.  Set a 
   breakpoint on the method Program.Main.
   
5. In HelloWorldClient_WCF, in the Build menu, select Start Debugging (or press F5).  

6. Place other breakpoints to investigate the behavior of other DPWS messaging 
   features. 


Any actual application that uses this code will require modifications of this 
code. This solution provides sample code, rather than ship-ready code, 
and is provided for instructional purposes only.
