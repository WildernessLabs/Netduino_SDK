SimpleWPFApplication Sample
======================
This sample demonstrates the following:
-  Using Windows Presentation Foundation.
-  Presenting a sliding menu system with animation of the menu icons
-  Mapping hardware GPIO pins to buttons.
-  Making UI elements respond to button clicks.
-  Loading a font resource and displaying text using the font resource.
-  Loading a bitmap resource, resizing it, and displaying it.
-  Resizing UI elements.
-  Changing focus among UI elements.
-  Using various UI elements including StackPanel, ListBox, ListBoxItem, and 
   Canvas.
-  Using various shapes including Ellipse, Line, Polygon, Rectangle.
-  Defining your own panel.
-  Defining your own shape.
-  Working with colors, solid brushes and linear gradient brushes.
-  Drawing freehand elements using standard drawing such as Rectangle, Polygon, 
   Ellipse, and Line, without defining a panel for each shape.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the LCD API.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Lcd interface.


To build and run this sample:
1. Open SimpleWPFApplication.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. In the Build menu, select Start Debugging (or press F5).

4. Click the left and right buttons of the emulator to see what demos are 
   available.  Click the center button to view a demo.  
   
5. Click Left/Right/Up/Down buttons to use a demo.  Click the center button to 
   return to the list of demos.

6. Close the application.

7. In file SimpleWPFApplication.cs, set a breakpoint on each of the OnButtonDown 
   methods.

8. Press F5 to start debugging.  Click the Right button of the emulator.  The 
   debugger stops at the first breakpoint; examine the code for that 
   OnButtonDown method.

9. Press F5 to proceed to each breakpoint, and examine the code for each 
   OnButtonDown method.  When a line is not highlighted in the debugger, the 
   emulator is waiting for you to click a button in the emulator.

10. See the DiagonalPanel class.  It shows how to define your own panel.

11. See the Cross class.  It shows how to define your own shape.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
