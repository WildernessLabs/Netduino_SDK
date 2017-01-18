Puzzle Sample
=============
This sample demonstrates the following:
-  Using touch features, which are a subset of the inking features.
-  Drawing graphics.
-  Detecting touch input.
-  Detecting gestures.
-  Setting up the TouchCollectorConfiguration.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Touch API and provides a 
   valid configuration for the Touch panel controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.TouchPanel 
   extensibility interface.


To build and run this sample:
1. Open Puzzle.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. In the Build menu, select Start Debugging (or press F5).

4. Click on a piece that is near the empty slot, and drag in the direction 
   toward the empty slot.  The piece moves into the empty slot.
   
5. Close the application.

6. Open the file Program.cs.  Set a breakpoint on the methods 
   OnTouchGestureChanged and puzzleBoard_Gesture.  

7. Press F5 to start debugging.  

8. Click a square that is next to the empty slot, and drag the square toward the 
   empty slot.  The OnTouchGestureChanged breakpoint is hit.  Step through the 
   code.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
