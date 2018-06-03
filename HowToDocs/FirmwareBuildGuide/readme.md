# Firmware Build Guide

## Clone the Repository

Clone the repository to your local machine.  Ensure the directory containing the clone does not contain any spaces:

```
git clone https://github.com/WildernessLabs/Netduino_SDK.git
```

For the following discussion it is assumed that the repository is cloned into the directory:

```
cd c:\Netduino_SDK
```

## Open a Command Prompt

The firmware is built using number of command line tools.  Some of these tools will have been installed as part of the Visual Studio installation process.  Open the developer command prompt for the version of Visual Studio that has been deployed.  For example, with Visual Studio 2013 this is:

```
Developer Command Prompt for VS2013
````

This can be found in the _Start Menu_.

## Select The Project Type

There are two projects in the Netduino SDK:

| Board               | Project Type           |
|---------------------|------------------------|
| Netduino Plus 2     | Netduino_2_and_3_NETMF |
| Netduino 3 Ethernet | Netduino_2_and_3_NETMF |
| Netduino 3 WiFi     | Netduino_3_NETMF       |

Select the correct project and change directory in the Developer Command Prompt to the location of the project type as above.  For example:

```
cd c:\Netduino_SDK\Netduino_3_NETMF
```

## Setup the Environment Variables

A number of environment variables (including extending the _PATH_ variable) need to be setup before the firmware can be built.  This is done using one of a number of batch and command files.

### Arm GCC

For Arm GCC execute a command similar to the following:

```
setenv_gcc.cmd 7.2.1 c:\gcc7.2.1
```

Where `7.2.1` is the version of GCC installed and `c:\gcc7.2.1` is the location of the GCC compiler.

### Keil MDK

For Keil MDK execute something similar to the following:

```
setenv_mdk.cmd 5.10 c:\Keil_v5\ARM
```

Where `5.10` is the compiler version (this should also be used for version 5.11) and `c:\Keil_v5\ARM` is the location of the MDK binary files.  Note that these values are the default for the Keil MDK 5.11 installation.

Note that a batch file has been created for the 5.10 and 5.11 version of the Keil MDK compiler:

```
setenv_mdk_510.bat
```

## Build the Firmware

Each board has a specific project tailored to the microcontroller on the board.  These projects can be found in board specific solutions directories.

In the Developer Command Prompt, change directory to the board specific solution.  For instance, for the Netduino 3 WiFi board the command will be:

```
cd Solutions\Netduino3WiFi
```

the resultant directory will now be something like:

```
c:\Netduino_SDK\Netduino_3_NETMF\Solutions\Netduino3WiFi
```

The firmware can now be built using _MSBuild_:

```
msbuild dotnetmf.proj /t:build /p:flavor=release;memory=flash
```

Building the firmware takes approximately 5 minutes on a reasonable specification PC.

The above command builds the release version of the firmware.  A debug version can be built with the following command:

```
msbuild dotnetmf.proj /t:build /p:flavor=debug;memory=flash
```

## Deploy New Firmware

The build process places the resulting firmware and intermediate artifacts in a `BuildOutput` directory.  In the case of the Netduino_3_NETMF project (i.e. Netduino 3 WiFi board) this will be:

```
c:\Netduino_SDK\Netduino_3_NETMF\BuildOutput
```

