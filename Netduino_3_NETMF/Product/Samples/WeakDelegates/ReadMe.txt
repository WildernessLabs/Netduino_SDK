WeakDelegates Sample
====================
This sample demonstrates the following:
-  Using the Micro Framework’s WeakDelegate class to notify the object of 
   events.  A WeakDelegate allows the object to have its memory reclaimed as 
   long as the application has no other references to the object (forcing it to 
   stay alive).
-  Defining an event that uses WeakDelegate objects to refer to the objects that 
   have registered interest in the event.
-  Creating an object and registering it with an event supporting WeakDelegates.
-  How a strong reference to the object ensures that it receives notification 
   when the event is raised.
-  How no strong references to the object allows a garbage collection to destroy 
   the object.  Consequently, the object will not receive any further 
   notifications when the event is raised.

This sample runs on the following devices:
-  The .NET Micro Framework SDK 4.0 Emulator.
-  Any physical device.
-  Any custom emulator.


To build and run this sample:
1. Open WeakDelegates.sln in Visual Studio 2008.

2. To run this sample in the Emulator, open the project Properties page, click 
   the .NET Micro Framework tab, set the Transport property to Emulator, and 
   then select the emulator you want to use.

   To run this sample on a device, open the project Properties page, click the 
   .NET Micro Framework tab, set the Transport property to the transport that 
   your device supports, and then select the device you want to target. 

3. Open the file WeakDelegates.cs.  Set a breakpoint on line 63, immediately 
   below the garbage collection.

4. In the Build menu, select Start Debugging (or press F5).  In the Output pane, 
   select the Output tab.  You can see the output of the first RaiseSomeEvent 
   call.

5. Observe that after garbage collection, events are no longer raised.  That is, 
   output is not produced by the second RaiseSomeEvent call.


Any actual application that uses this code will require modifications of this 
code.  This solution provides sample code, rather than ship-ready code, and is 
provided for instructional purposes only.
