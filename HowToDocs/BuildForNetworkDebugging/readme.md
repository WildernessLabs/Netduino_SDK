# Building for Network Debugging

The latest boards in the Netduino range all ship with a network stack written in managed C# code.  This stack ([Neduino.IP.Stack](https://github.com/WildernessLabs/Netduino_IP_Stacks)) is capable of being debugging from within Visual Studio.

* [Generate a network debug version of the firmware](#generate-network-debug-firmware)
* [Create a C# project containing your application and the Netduino.IP.Stack code](https://github.com/WildernessLabs/Netduino_IP_Stacks/blob/master/Documentation/DebugProjects.md)

## Generate Network Debug Firmware

The Network.IP.Stack contains the following projects:

* Netduino.IP
* Netduino.IP.Interop
* Netduino.IP.LinkLayers._ChipType_

Where _ChipType_ depends upon the board:

| Board             | ChipType | Link Layer                      |
|-------------------|----------|---------------------------------|
| Netduino Plus 2   | ENC28J60 | Netduino.IP.LinkLayers.ENC28J60 |
| Netduino Ethernet | AX88796C | Netduino.IP.LinkLayers.AX88796C |
| Netduino 3 WiFi   | CC3100   | Netduino.IP.LinkLayers.CC3100   |

The main two components of the firmware that are capable of being debugged are:

* _Netduino.IP_ - IP Protocols such as UDP
* _LinkLayer_ - Interface between the network chip (CC3100 etc) and the native C code

A custom version of the firmware built without the above two components deployed will allow these two components to be debugged.

### Edit the Project File

The first step is to locate the _TinyCLR.proj_ file and comment out the two elements we want to debug.  For the Netduino 3 WiFi board, the file can be found in:

```
Netduino_SDK\Netduino_3_NETMF\Solutions\Netduino3WiFi\TinyCLR\TinyCLR.proj
```

Locate lines such as the following:

```msbuild
<Import Project="$(SPOCLIENT)\Solutions\Netduino3Wifi\DeviceCode\Netduino_IP\Netduino.IP.featureproj" />
<Import Project="$(SPOCLIENT)\Solutions\Netduino3Wifi\DeviceCode\Netduino_IP_Interop\Netduino.IP.Interop.featureproj" />
<Import Project="$(SPOCLIENT)\Solutions\Netduino3Wifi\DeviceCode\Netduino_IP_LinkLayers_CC3100\Netduino.IP.LinkLayers.CC3100.featureproj" />
```

Comment out (or remove) the _Netduino.IP.featureproj_ and _Netduino.IP.LinkLayers.CC3100.featureproj_ lines (remember to change the file depending upon the chip type of your board).

### Build and Deploy the Firmware

Now build and deploy the firmware as detailed on the following pages:

* [Building the Firmware](../FirmwareBuildGuide)
* [Deploying using ST-Link](../STLinkConnection)