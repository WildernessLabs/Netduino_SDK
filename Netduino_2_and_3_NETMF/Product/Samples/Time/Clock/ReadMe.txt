Clock Sample
============
This sample demonstrates the following:
-  Using the basic features of touch input.
-  Handling the stylus down, up and move events.
-  Accessing the points of the stylus event.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Time, Ethernet and Sockets API and 
   provides a valid configuration for the Ethernet controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Time and 
   Microsoft.SPOT.Emulator.Sockets extensibility interface.

The IP address of the time server that is used in this sample is an arbitrary 
value.  You can change this IP address to a public time server; see: 
http://tf.nist.gov/tf-cgi/servers.cgi 
You might need to change your proxy settings, to access the public time server.


To build and run this sample:
1. Open Clock.sln in Visual Studio 2008.

2. In Solution Explorer, right-click the Solution node (not the Clock project 
   node) and then click Properties.  The Solution 'Clock' Property Page dialog 
   box appears.

3. On the left, expand the Configuration Properties node, and then select the 
   Configuration node.  Select the Build check box and the Deploy check box, 
   then click OK.

4. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

5. In the Debug menu, select Start Debugging (or press F5).  A digital clock 
   appears on the device screen.  The time display doesn't change until you 
   click buttons.

6. Click the Up, Down, or Select (middle) buttons on the device.
      Up:     Resets the clock to an arbitrary local time.
      Select: Synchronizes the clock to the server time.
      Down:   Updates the display to show the current local time.

7. Close the application.

8. In the Debug menu, select Start Debugging (or press F5).

9. Open the file Program.cs, and set a breakpoint on the methods 
   TimeService_SystemTimeChanged and OnButtonUp.

10. Click the Up, Down, or Select (middle) buttons on the device.  The breakpoint 
    is hit.  Step through the OnButtonUp method and see the time change 
    accordingly. 


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
