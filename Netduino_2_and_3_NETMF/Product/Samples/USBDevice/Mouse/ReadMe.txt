USBMouse Sample
===============
This sample demonstrates the following:
-  Using a .NET Micro Framework device (a mouse) as a USB device for a desktop 
   computer.
-  Writing to the USBStream.
-  Configuring the USB controller on the fly. 
-  Creating a USBStream object using USBController.
-  Configuring a USBPort.
-  Configuring the mouse to operate in relative or absolute mode.  In this 
   sample, the mouse initially operates in relative mode.  Therefore, all mouse 
   movements are sent to the PC as relative coordinates from the current point. 

This sample runs on the following devices:
-  Devices that have a USB device port and are running firmware set up for 
   debugging and deployment over TCP/IP or a serial port.  For a USB device 
   sample that supports USB debugging, please check the .NET Micro Framework 
   website: http://www.microsoft.com/netmf
-  Any emulator that implements the Microsoft.SPOT.Emulator.Usb
   interface.  The Emulator in the .NET Micro Framework SDK doesn't implement 
   that interface.
  

Set your USB configuration either programmatically (through C#) or from a 
configuration file (through MFDeploy).

Deploying Programmatically
1. Deploy and run the sample normally.
2. Press the Select button to write a new configuration.

Deploying from a Configuration File
1. Open the MFDeploy tool.
2. Select your Micro Framework device in the Device area.
3. Open the Target menu, choose Configuration, then choose USB Configuration. 
4. From the USB Configuration dialog, upload the MouseConfig.xml file to your 
   device.
5. Close MFDeploy and deploy the USB sample code to your device through Visual 
   Studio.
   

To build and run this sample:
1. Open Mouse.sln in Visual Studio 2008.

2. To run this sample in an emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. In the Build menu, select Start Debugging (or press F5).

4. To move the mouse position, click the buttons on the device, or click the 
   emulator buttons with the mouse.  The buttons do the following:
      Left, Right: Change the X-direction.
      Up, Down:    Change the Y-direction.
      Select:      Toggles between relative and absolute mode.  In absolute 
                   mode, absolute coordinates are sent to the desktop each time 
                   you press the Left, Right, Up, or Down buttons.
      Back:	       Emulates the Left mouse button.
      Home:        Emulates the Right mouse button.
      Menu:        Exits the program.


For best results, run the sample on a device.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
