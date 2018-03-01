# Building Firmware

## Installation

### Prerequisites

The build system requires the installation of several software components:

* Visual Studio 2017
* GCC or Keil MDK 5.11
* Git (Optional)

The build system requires two Visual Studio components to be installed:

* Visual Studio Desktop development with C++
* Visual Studio extensions development

### Download the Repository

Download the [Netduino SDK repository](https://github.com/WildernessLabs/Netduino_SDK) using _git clone_ or by downloading and expanding the zip file.

### Setup Compiler Environment Variables

Several environment variables are used by the installation process and build system.  These are configured by the _setenv_???_ commands.

Open the _Developer Command Prompt for Visual Studio 2017_ and change to the directory where the repository has been extracted / cloned.

For GCC 7.2.1 installed in _c:\gcc7.2.1_ execute the following command:

_setenv_gcc 7.2.1 c:\gcc7.2.1_

For Keil MDK 5.11 execute the following command:

_setenv_mdk 5.10_

Keep the command prompt open as this will be used in the following steps.

### Install Additional Tools

A number of tools are required and these can be installed using a PowerShell script by executing the following:

_powershell ./Install-BinTools.ps1_

If you receive an error message regarding the execution not beinallowed due to your execution policy settings then try the following:

_powershell -ExecutionPolicy ByPass ./Install-BinTools.ps1_

### Install the Microsoft NETMF Cryptographic Library

Download and install the [Microsoft NETMF Cryptographic libraries](https://github.com/NETMF/netmf-interpreter/releases/download/NETMFCryptoLibraries/NetMFCryptoLibs.msi)

