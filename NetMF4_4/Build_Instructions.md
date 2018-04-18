# NET MF 4.4 Build Instructions

## Requirements

Windows PC with Visual Studio 2015. Tested using Visual Studio 2015 Professional Update 3 on Windows 10 64.

## Visual Studio Installation

Make sure Visual Studio 2015 is installed and up to date: [Click here to download](https://www.visualstudio.com/vs/older-downloads/)

Verify or Install the C++ 2015 Build tools: 
- Run the VS 2015 installer
- Under **Programming Languages -> Visual C++**, check *Common Tools for Visual C++ 2015* and *Microsoft Foundation Classes for C++*
- Under **Common Tools**, check *Visual Studio Extensibility Update 3*

## Setup (one-time steps)
- Clone the [Netduino SDK](https://github.com/WildernessLabs/Netduino_SDK) repo
- Install the [.NET Micro Framework Cryptographic Libraries](https://github.com/NETMF/netmf-interpreter/releases)
- Open the crypto libraries install location 
- Copy the **lib** folder into the existing **crypto** folder of the cloned repo
- Open **PowerShell** as an Admin
- Enable execution: **PS > Set-ExecutionPolicy unrestricted**
- Navigate to the NETMF 4.4 Folder in the cloned repo
- Execute **Install-BinTools.ps1**
- Execute **Update-DeviceSdkAssembly.ps1**
- Close PowerShell

## Build the SDK
- Open the **Developer Command Prompt for Visual Studio 2015**
- (Optional) Validate C++ Build Tools install by calling **Set VSSDK140Install**
- Navigate to the NETMF 4.4 Folder in the cloned repo
- Execute **build_sdk.cmd** (build takes 20+ minutes)





