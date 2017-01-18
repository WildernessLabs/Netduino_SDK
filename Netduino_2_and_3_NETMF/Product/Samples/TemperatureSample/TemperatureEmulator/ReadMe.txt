TemperatureEmulator Sample
==========================
This sample demonstrates the following:
-  Using the emulator features.
-  Creating a custom emulator.
-  Creating a custom hardware component.
-  Adding Windows UI components to an emulator to simulate input and output that 
   would normally be provided by actual hardware.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.

The TemperatureEmulator sample emulates a temperature sensor.  The Temperature 
sample uses the emulated temperature sensor.


To build and run this sample:
1. Open TemperatureEmulator.sln (not TemperatureSample.sln) in Visual Studio 
   2008.

2. In the Build menu, select Start Debugging (or press F5).  The first time this 
   sample is run, it installs a new emulator that is available for .NET Micro 
   Framework applications to run in.  

3. Open TemperatureSample.sln in Visual Studio 2008.

4. To run the TemperatureSample sample in the .NET Micro Framework Emulator, 
   open the project Properties page, click the .NET Micro Framework tab, set the 
   Transport property to Emulator, and then select the emulator you want to use.

   To run the TemperatureSample sample on a device, open the project Properties 
   page, click the .NET Micro Framework tab, set the Transport property to the 
   transport that your device supports, and then select the device you want to 
   target. 

5. Click the buttons in the .NET Micro Framework Emulator to see how the 
   application works.

6. Close the application.

7. Open the file Program.cs.  Set a breakpoint on the OnButtonUp method.

8. In the Build menu, select Start Debugging (or press F5).

9. Click a button in the .NET Micro Framework Emulator.  The breakpoint is hit.  
   Step through the code.
