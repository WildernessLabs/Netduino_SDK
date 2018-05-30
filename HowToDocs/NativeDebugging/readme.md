# Native Debugging

All of the Netduino boards contain solder pads allowing access to the JTAG / SWD interface on the STM32 microcontrollers.  This interface allows the developer to attach a debugging interface such as the ST-Link or Segger J-Link units to the microcontroller.  The debug interface can now be used to upload new firmware and debug code deployed to the microcontroller.

## Adding a Socket

The image below shows the solder pads for the JTAG interface on the Netduino 3 WiFi board:

![Netduino 3 WiFi JTAG Solder Pads](Netduino3JTAGSolderPads.png)

All of the boards also have a small marker on the connector or silk screen indicating the location of pin 1.  In the image above, pin 1 is indicated by the small triangle.

The most convenient way of connecting the debug interface to the board is to solder a socket to the board.  This is relatively easy on the Netduino PLus 2 as the connection point is a plated through hole connection.  It is more difficult with the Netduino 3 Ethernet and the Netduino 3 WiFi as the solder pads are designed for use with surface mount sockets.

The solder pads on all of the boards are designed for a 2 x 5 pin IDC socket with a 1.27mm pitch.  These are readily available through most major suppliers such as DigiKey etc.  Ensure that you order the correct part, through hole or surface mount.



![IDC Socket with Raised Legs](IDCSocketRaisedLegs.png | width=200)

