# Building Firmware

## Installation

### Prerequisites

The build system requires the installation of several software components:

* Visual Studio 2017
* GCC or Keil MDK 5.11
* Git (Optional)

The build system requires two Visual Studio components to be installed:

* Visual Studio Desktop development with C++
    - Ensure that C++/CLI Support is installed
* Visual Studio extensions development
* Windows Universal Platform development
    - Required to ensure NET Framework 4.5 is installed correctly.

### Download the Repository

Download the [Netduino SDK repository](https://github.com/WildernessLabs/Netduino_SDK) using _git clone_ or by downloading and expanding the zip file.

### Setup Compiler Environment Variables

Several environment variables are used by the installation process and build system.  These are configured by the _setenv_ commands.

Open the _Developer Command Prompt for Visual Studio 2017_ and change to the directory where the repository has been extracted / cloned.

For GCC 7.2.1 installed in _c:\gcc7.2.1_ execute the following command:

_setenv_gcc 7.2.1 c:\gcc7.2.1_

For Keil MDK 5.11 execute the following command:

_setenv_mdk 5.10_

Keep the command prompt open as this will be used in the following steps.

> **Note:**
>
> Some versions of _Visual Studio Developer Prompt_ will change the current working directory following the execution of the _setenv_ command. Ensure that you change back to the Netduino SDK directory if this occurs.

### Install Additional Tools

A number of tools are required and these can be installed using a PowerShell script by executing the following:

_powershell ./Install-BinTools.ps1_

If you receive an error message regarding the execution not being allowed due to your execution policy settings then try the following:

_powershell -ExecutionPolicy ByPass ./Install-BinTools.ps1_

### Install the Microsoft NETMF Cryptographic Library

Download and install the [Microsoft NETMF Cryptographic libraries](https://github.com/NETMF/netmf-interpreter/releases/download/NETMFCryptoLibraries/NetMFCryptoLibs.msi)

### Modify the GCC Floating Point Library Location

The exact location of these files will depend upon the actual installation directory for GCC.  For the following, assume that the compiler is installed in _c:\gcc-arm_.

The build assumes that the floating point library for GCC is in the following location:

c:\gcc-arm\arm-non-eabi\lib\armv7e

In the 7.2.1 release of GCCm the STM32F4 libraries are actually located further down the directory tree.  The libraries need to be moved in order to ensure that the firmware can be built.  For GCC 7.2.1 move:

c:\gcc-arm\arm-none-eabi\lib\thumb\v7e-m\fpv4-ap

to

c:\gcc-arm\arm-none-eabi\lib

Now rename the directory _fpv5-sp_ to _armv7e-m_.

Also, rename the _armv7e-m\hard_ to _armv7e-m\fpu_.

## Building the Firmware

* Open a _Visual Studio Developer_ command prompt.
* Setup the compiler using the appropriate _setenv_ command (see _Setup Compiler Environment Variables_ above)
* 