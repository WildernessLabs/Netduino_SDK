Ink Canvas
==========
This sample demonstrates the following:
-  The InkCanvas features of the .NET Micro Framework.
-  The InkCanvas features are a subset of the full inking features.
-  Subclassing the InkCanvas class for your own project.
-  Receiving an ink object and drawing it in different colors.
-  Setting up the TouchCollectorConfiguration.
-  Writing a palette class that allows the user to choose the color to draw the 
   stroke in.
-  Writing a simple button class that allows you to click on it to perform an 
   action.
-  Handling Multi-Touch gestures (rotation, zoom and pan).  

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Touch API and provides a valid 
   configuration for the touch controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.TouchPanel 
   interface.


To build and run this sample:
1. Open InkCanvasSample.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. In the Build menu, select Start Debugging (or press F5).

4. On the right side of the emulator screen, click a color, and then draw the 
   letter 'A' in the middle of the emulator screen.
   
   The sample supports the following actions:
      Rotate  -- CTRL+R + mouse left-click and drag
      Zoom In -- CTRL+Z + mouse left-click and drag
      Pan Out -- CTRL+P + mouse left-click and drag
      For Zoom and Pan, dragging up or right makes the image bigger.  Dragging 
      down or left makes the image smaller.

5. Press and hold CTRL+R, and then click and hold the left mouse button while 
   moving the mouse.  The drawing is rotated as you move the mouse.

6. Press and hold CTRL+Z, and then click and hold the left mouse button while 
   moving the mouse.  The drawing zooms in (gets bigger) as you move the mouse.

7. Close the application.

8. Open the file Program.cs.  Set a breakpoint on the OnTouchGestureChanged 
   method.

9. Press F5 to start debugging.  Click a color.  Draw the character 'A'.  The 
   breakpoint is hit.  Step through the code, including the switch statement.

10. Press F5 to resume debugging.  Use your keyboard and mouse to cause a Rotate 
    event.  The breakpoint is hit.  Step through the code, including the switch 
    statement.

11. Press F5 to resume debugging.  Use your keyboard and mouse to cause a Zoom 
    event.  The breakpoint is hit.  Step through the code, including the switch 
    statement.



For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
