IEEE 802.15.4 Sample
=====================

This sample demonstrates the following:
-  Using the IEEE 802.15.4 support for the Micro Framework.
-  Sending and receiving messages for the Network layer of the stack.

This sample runs on the following devices:
-  Any physical device that implements the SPI API.

NOTE: this sample does not run in the standard emulator. This sample need you 
to connect a CC2420 Development kit from Texas Instrument or run on a device
that supports that chipset. 

To build and run this sample:
1. Open two instances of IEEE_802_15_4_Network.sln in Visual Studio 2008.

2. To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. In file Network.cs, set a breakpoint on the Run method.

4. In the Build menu, select Start Debugging (or press F5).  The breakpoint is 
   hit.  

5. Deploy the sencond instance of the sample to a different device, with no 
   breakpoints. 

6. Go back to teh first instance and debug the first device while it joins the 
   802.15.4 Network.

7. Step through the code.


For best results, run the sample on two separate devices while debugging them from 
two instances of Visual Studio.  Any actual application that uses 
this code will require modifications of this code.  This solution provides 
sample code, rather than ship-ready code, and is provided for instructional 
purposes only.
