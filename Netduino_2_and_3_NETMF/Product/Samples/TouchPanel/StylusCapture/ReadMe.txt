StylusCapture Sample
====================
This sample demonstrates the following:
-  Capturing the stylus input for a UIElement.
-  Handling the stylus down, up, and move events.
-  Accessing the points of the stylus event.
-  When the stylus output is captured, all subsequent stylus events are relative 
   to the control that captured the stylus.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Touch API and provides a valid
   configuration for the touch controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.TouchPanel 
   interface.


To build and run this sample:
1. Open StylusCapture.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. Open the file Program.cs.  Set a breakpoint on the OnTouchDown method.

4. Press F5 to start debugging.  Click anywhere on the emulator screen to cause 
   a touch event.  The breakpoint is hit.  Step through the code.


Any actual application that uses this code will require modifications of this 
code.  This solution provides sample code, rather than ship-ready code, and is 
provided for instructional purposes only.
