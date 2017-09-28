# Netduino SDK

Welcome to the Wilderness Labs' Netduino SDK repo. Here you will find:

 * Netduino Firmware
 * Netduino SDK Install Projects (Windows)
 * Visual Studio and Xamarin Studio IDE Plugins
 * Netduino Specific .NET Micro Framework (.NETMF) Code

## Netduino .NET Micro Framework Code and SDK

In order to run on Netduino, the .NET Micro Framework needs to be modified with hardware specific mappings and code. Each piece of hardware that .NETMF runs on needs not only chip-specific pin mappings, but in the case of Netduino, an entirely different IP stack is used to power the networking portion of the framework, which includes code for both the Ethernet and WiFi models of Netduino. 

### Hidden Gems

Because this is an overlay of the published .NETMF SDK, we have kept the directory structure largely the same as the officially published sources. As such, it can be difficult to find things in the repo. Here are some hidden gems that may be of interest, however:

 * **[Microsoft Samples](Netduino_3_NETMF/Product)** - These are the official samples Microsoft published that run on .NETMF. In many cases, it's probably much better to see the Netduino specific, curated samples on the [Wilderness Labs Developer Site](developer.wildernesslabs.co/Samples/Netduino/), but here they are, in any case.
 * **[.NETMF Framework Code](Netduino_3_NETMF/CLR/Libraries)** - While .NETMF apps are actually interpreted by the .NETMF CLR, the entire .NETMF stack is native C++ code. If you're looking to extend core framework functionality, this is the place to find the core library code to see how it works.
 
## Firmware

The firmware for Netduino includes both the TinyCLR OS that runs on Netduino and the compiled .NETMF framework and runtime code. The actual building process of the firmware is a complex and byzantine set of steps, due to the special requirements of porting .NETMF, but we've done our hardest to turn it into a single step compilation.

### Firmware Build Requirements

 * Windows
 * ARM MDK 5.10+
 * .NETMF SDK v4.3 

The Netduino Firmware can be built with GCC, however, the GCC compiler is not optimized for small, microcontroller footprints, so the output may not fit onto the flash space on a Netduino. For this reason, the ARM MDK compiler (v5.10 or better) is recommended and/or required to build firmware that will fit. 

The current build configuration is known working with ARM MDK v5.10, though newer versions may also work. GCC may be able to be used to prototype and or playground fixes and changes. 

The ARM MDK compiler only runs on Windows. The included build configuration is known to be good on Windows 10.

Finally, the [.NET Micro Framework v4.3 SDK](http://downloads.wildernesslabs.co/NETMF_SDK/netmf-v4.3.2-SDK-QFE2-RTM.zip) must be installed on the build machine.

### Building the Netduino Firmware 

To build the Netduino firmware:

1. Pick which flavor you want to build:
	- Netduino3Wifi is in Netduino_3_NETMF
	- All other variants (N3, N3E, N2, N2+) are in Netduino_2_and_3_NETMF
2. Open `cmd.exe` and change the directory to the flavor you wish to build.
3. Each board directory is in `$(FlavorDirectory)\Solutions\`
4. Change to the board directory and run:
```
msbuild dotnetmf.proj /t:rebuild /p:flavor=release
```
5. When it has completed, the build output will be in:
```
$(FlavorDirectory)\BuildOutput
```

## IDE Plugins


### Windows (Visual Studio)

Though .NETMF apps are known to be buildable in Visual Studio 2017, official support is only for Visual Studio 2013 and 2015. 

If you're interested in helping to create a Visual 2017 add-in, please let us know. :)

### Mac (Visual Studio, Xamarin Studio, MonoDevelop)

On Mac, the .NETMF plugin is the same for Visual Studio, Xamarin Studio, and MonoDevelop, and can be found [here](SDK/Xamarin%20Studio%20NETMF%20Plugin).


## Contributing

Netduino is open source hardware, running open source software. We strive to make contributing easy, and the future of the product is, in large part, up to the community. 

Right now, the highest priority outstanding work items that would be great community contributions are:

 * [Visual Studio 2017 Support](https://github.com/WildernessLabs/Netduino_SDK/issues/4)
 * [SSL Fix](https://github.com/WildernessLabs/Netduino_SDK/issues/2)

If you're interested in contributing, please join the conversation at [community.wildernesslabs.co](http://community.wildernesslabs.co)!
