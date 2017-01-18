FileSystem Sample
=================
This sample demonstrates the following:
-  Using the File System features of the Micro Framework.
-  Using the Touch Panel API.
-  Formatting a volume.
-  Enumerating directories and files.
-  Getting and setting the current directory.
-  Creating files.
-  Writing to files.
-  Deleting files.
-  Moving and renaming directories and files.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device that implements the Block Storage API and provides a 
   valid configuration for the block storage device.
-  Any emulator that implements the Microsoft.SPOT.BlockStorage interface.

To build and run this sample:
1. Open FileSystemSample.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. In file FileSystemSample.cs, set a breakpoint on the RefreshList method.

4. In the Build menu, select Start Debugging (or press F5).  The breakpoint is 
   hit.  No content appears in the emulator screen.

5. Press F5 again.  Content appears in the emulator screen.

6. Click with the mouse on one of the volumes.  The breakpoint is hit.

7. Step through the code.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
