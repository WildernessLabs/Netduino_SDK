HttpClient Sample
=================
This sample demonstrates the following:
-  Using the Micro Framework’s Http and Https classes to create an Http or Https 
   client.
-  Using an Http or Https client to retrieve pages from several different URLs 
   using Http and Https.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Sockets API and the Ethernet 
   controller API and provides a valid configuration for the Ethernet 
   controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets 
   extensibility interface.


To explore this sample:
1. Open HttpClient.sln in Visual Studio 2008.

2. Edit HttpClient.cs to use a proxy that works in your environment.  The 
   source code currently uses "itgproxy.dns.microsoft.com" as the default Web 
   proxy, in the Main() function.

3. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

4. In the Build menu, select Start Debugging (or press F5).

5. In Visual Studio, show the Output window for Debug.  The content of some Web 
   pages is printed in the Output window.

6. Close the application.

7. In file HttpClient.cs, set a breakpoint on the PrintHttpData method.

8. Press F5 to start debugging the HTTP requests and responses. 


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
