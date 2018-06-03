# Prerequisites

The firmware requires a number of tools to be installed on the build machine:

* [Visual Studio](#visual-studio)
* [.NET Microframework 4.3 SDK](#microframework-sdk)
* [Keil MDK compiler](#keil-mdk-compiler) or [GCC Compiler](#gcc-compiler)
* [STM32 Cube Programmer](#stm32-cube-programmer)
* [Netduino Updater for deployment](#netduino-updater)

## Visual Studio

The firmware has two elements, native code (compiled using GCC or MDK) and managed code (compiled with the C# compiler).

Visual Studio 2013 Express or Visual Studio 2015 Community can be used as well as the commercially available versions of Visual Studio.

When using Visual Studio 2013, it is also necessary to download and install the [Windows SDK 2013](https://marketplace.visualstudio.com/items?itemName=VisualStudioProductTeam.MicrosoftVisualStudio2013SDK).

Visual Studio 2015 requires installation of the Windows SDK.  This can be installed as follows:

* Go to _Control Panel_ and select _Programs and Features_
* Select _Visual Studio 2015_
* Select the _Change_ option.
* Select _Universal Windows App Development Tools_ and check all of the items in the folder.
* Update the installation.

Finally, it is necessary to ensure that _MSBuild_ is on the path when a command prompt is open:

* Go to _Control Panel_ and open the _System_ applet
* Select _Advanced_ and click on the _Environment Variables_ button.
* Add the location of _MSBuild_ to the Path variable. Example: C:\Program Files (x86)\MSBuild\14.0\bin

## Microframework SDK

The build process requires the installation of the [.NET Microfromaework 4.3 SDK](http://downloads.wildernesslabs.co/NETMF_SDK/netmf-v4.3.2-SDK-QFE2-RTM.zip).  Click on the link to download the zip file containing the installers.

The zip file contains two components:

* .NET Microframework MSI
* Visual Studio extension (_netmfvs2013.vsix_ for Visual Studio 2013 and _netmfvs14.vsix_ for Visual Studio 2015)

Install the _MicroFrameworkSDK.msi_ and the appropriate Visual Studio extension.

## Keil MDK Compiler

The MDK compiler is a commercial compiler available from [Keil](http://www.keil.com/mdk5/).  This compiler produces smaller code than the free [GCC](#gcc-compiler).  MDK is only available on the Windows platform.

The firmware is known to compile with version 5.10+ of the compiler.

When installing the MDK compiler it is necessary to ensure that the installation directory does not contain any space characters.

## GCC Compiler

Arm have release a free multi-platform embedded toolchain for the Arm Cortex-M and Cortex-R processors.  This includes a version of [GCC for Windows, Mac and Linux](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

The GCC compiler is not as efficient as the MDK compiler when it generates object code.  The firmware can be compiled with GCC however the firmware will consume more memory than the equivalent firmware compiled with MDK.  It is also possible that the firmware may not fit in the memory of the Netduino when compiled with GCC.

When installing the GCC compiler it is necessary to ensure that the installation directory does not contain ant space characters.

## STM32 Cube Programmer

ST provide a tool for deploying software to the STM32 family of boards.  _STM32CubeProg_ can be downloaded from the [ST website](http://www.st.com/en/development-tools/stm32cubeprog.html).

Once installed it is advised that the path to _STM32_Programmer_CLI.exe_ is added to the system PATH variable.  This will allow the firmware to be flashed to the Netduino from a batch file.

## Netduino Updater

The firmware can also be deployed using the [Netduino Updater](http://developer.wildernesslabs.co/Netduino/About/Downloads/).