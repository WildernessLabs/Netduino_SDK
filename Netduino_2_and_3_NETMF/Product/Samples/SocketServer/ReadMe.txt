SocketServer Sample
===================
This sample demonstrates the following:
-  Creating a socket and using it to listen for incoming client requests, and 
   then returning an HTML page back to the client.
-  Creating a socket that listens on a specified port.
-  Accepting incoming requests and processing client requests synchronously or 
   asynchronously using additional threads.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Sockets API and Ethernet controller 
   API and provides a valid configuration for the Ethernet controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface.


To build and run this sample:
1. Open SocketServer.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. Open the file SocketServer.cs, and set a breakpoint on the ProcessRequest 
   method.

4. In the Build menu, select Start Debugging (or press F5).

5. Open your Web browser and go to: http://127.0.0.1:12000/  Observe your 
   breakpoint being hit. 

   If you are using a firewall, you might need to explicitly allow the emulator 
   to open the following port address: http://127.0.0.1:12000/

6. Position the Web browser window so you can see it next to the debugger.

7. Step through the sample.  When you exit the ProcessRequest method, your Web 
   browser displays an HTML Web page.

8. Close the application.


Any actual application that uses this code will require modifications of this 
code.  This solution provides sample code, rather than ship-ready code, and is 
provided for instructional purposes only.
