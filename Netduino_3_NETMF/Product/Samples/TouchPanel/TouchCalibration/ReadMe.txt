TouchCalibration Sample
=======================
This sample demonstrates the following:
-  Calibrating the touch input system.
-  Handling the stylus down and up events.
-  Accessing the points of the stylus event.
-  Querying the touch system for calibration point counts.
-  Gathering the calibration points from the user.
-  Setting the calibration point data in the touch system.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Touch API and provides a valid
   configuration for the touch controller.
-  Any emulator that implements the Microsoft.SPOT.Emulator.TouchPanel 
   interface.


To build and run this sample:
1. Open TouchCalibration.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, set the 
   Transport property to Emulator, and then select the emulator you want to use. 

   To run this sample on a device, open the project Properties page, set the 
   Transport property to the transport that your device supports, and then 
   select the device you want to target. 

3. Press F5 to start debugging.  Click the center button in the emulator to 
   start the calibration process.  Click the indicated points in the screen.
   
4. Close the application.

5. Open the file Program.cs.  Set a breakpoint on the CalibrateScreen method.

6. Press F5 to start debugging.  

7. Click the center button in the emulator to start the calibration process.  
   The breakpoint is hit.
   
8. Step through the code.  If no line is highlighted in the debugger, that 
   indicates that the emulator window is waiting for your input.

9. Verify that after the calibration process is completed, the coordinates on 
   the screen report the correct values.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
