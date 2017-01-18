SocketClient Sample
===================
This sample demonstrates the following:
-  Creating a socket and using it to issue a request to retrieve an HTML page 
   for a web site.  The source tagging for the HTML page is displayed in the 
   debugger’s Output window.
-  Creating a socket that connects to a URL and port.
-  Converting a string to a UTF-8 byte array so that the bytes can be sent over 
   the socket.
-  Reading data over the socket in chunks to build up the entire HTML page.
-  Safely stopping waiting for data if the server is taking too long to respond.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Sockets API and Ethernet controller 
   API and provides a valid configuration for the Ethernet controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface.


To build and run this sample:
1. Open SocketClient.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. Open the file SocketClient.cs.  Place a breakpoint on the GetWebPage method, 
   and on the Debug.Print statement in Main().

4. In Main(), you might need to change the proxy from itgproxy.dns.microsoft.com 
   to a valid value for your site.

5. In the Build menu, select Start Debugging (or press F5).  In the Output pane, 
   click the Output tab.  
   
6. Press F5 again, to get to the Debug.Print statement in Main().  Press F10 to 
   execute that statement.  HTML tagging for the Web page is displayed in the 
   Output pane.

6. Close the application.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
