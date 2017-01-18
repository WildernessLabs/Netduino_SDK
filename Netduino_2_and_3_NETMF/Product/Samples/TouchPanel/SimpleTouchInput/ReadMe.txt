SimpleTouchInput Sample
=======================
This sample demonstrates the following:
-  Using the most basic features of touch input.
-  Handling the stylus down, up and move events.
-  Accessing the points of the stylus event.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Touch API and provides a valid
   configuration for the touch controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.TouchPanel 
   interface.


To build and run this sample:
1. Open SimpleTouchInput.sln in Visual Studio 2008.

2. In Solution Explorer, right-click the Solution node (not the project node) 
   and then click Properties.  The Solution 'SimpleTouchInput' Property Page 
   dialog box appears.

3. On the left, expand the Configuration Properties node, and then select the 
   Configuration node.  Select the Build check box and the Deploy check box, 
   then click OK.

4. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

5. In the Build menu, select Start Debugging (or press F5).  The text "Click 
   Anywhere" appears in the screen.  
   
6. Click, drag, and release the mouse button, causing touch events to occur.  
   The coordinates at which the mouse button was clicked and released are 
   displayed in the screen, and the path you traced with the mouse is displayed.

7. Close the application.

8. Open the file Program.cs.  Set a breakpoint on the methods OnTouchDown, 
   OnTouchUp, and OnTouchMove.

9. Press F5 to start debugging.  When the text "Click Anywhere" appears, click, 
   drag, and release the mouse button to cause touch events to occur.  The 
   breakpoints are hit.  
   
10. Step through the code.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
