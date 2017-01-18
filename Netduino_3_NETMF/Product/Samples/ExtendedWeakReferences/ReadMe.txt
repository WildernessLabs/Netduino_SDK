ExtendedWeakReferences Sample
=============================
This sample demonstrates the following:
-  Using the Micro Framework’s ExtendedWeakReference class to persist data 
   across a reboot of the hardware.
-  Retrieving how many times the hardware has been rebooted.
-  Displaying the result in the debugger’s Output window.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Block Storage API and provides a 
   valid configuration for the block storage device.
-  Any emulator that implements the Microsoft.SPOT.Emulator.BlockStorage 
   interface.

To build and run this sample:
1. Open ExtendedWeakReferences.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. Open the file ExtendedWeakReferences.cs.  Place a breakpoint at the beginning 
   of the Main method.

4. In the Build menu, select Start Debugging (or press F5). 

5. In the Output pane in Visual Studio, click the Output tab.

6. Step through the Main method and observe as the code retrieves an 
   ExtendedWeakReference that holds the number of boots and updates it. 

7. Close the application.

8. Re-run the application to observe how the number of boots increases by one 
   unit at each run. 


Any actual application that uses this code will require modifications of this 
code.  This solution provides sample code, rather than ship-ready code, and is 
provided for instructional purposes only.
