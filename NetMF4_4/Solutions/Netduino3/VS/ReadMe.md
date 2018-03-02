## Visual Studio for NETMF Native Development

This guide describes steps needed to setup NETMF native code development from
within Visual Studio IDE, using GCC toolchain and OpenOCD debugger on STM32F746
Nucleo board.

### Prerequisites

1. [Visual Studio 2015 Community](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx) or higher edition,
2. [Visual C++ for IoT Development](https://visualstudiogallery.msdn.microsoft.com/35dbae07-8c1a-4f9d-94b7-bac16cad9c01) extension,
3. [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded) (5.4.1 at the time of writing),
4. OpenOCD debugger - prebuilt binaries with STM32F7 support available at Freddie Chopin's [OpenOCD dev](http://www.freddiechopin.info/en/download/category/10-openocd-dev)

### Build

1. Navigate to `Solutions\STM32F746NUCLEO\VS` folder,
2. To customize toolchain environment and msbuild options copy `build.props.user-example` to `build.props.user` and change the values of the respective properties,
3. Open `STM32F746NUCLEO.sln` solution in Visual Studio,
4. Select the **Release** configuration for **ARM** platform,
5. Build

>_Note: In order to have full debugging information (such as local variable names)
available during debugging, you can either select the **Debug** configuration
or increase the amount of debugging information generated for the **Release**
configuration in GCC .targets file. In either case, make sure the generated
binary fits into the flash memory region._

### Debugging

1. In Visual Studio select **Debugging** in the Project Properties and set the options for **OCD GDB Debugger**
    * _OCD Debugger Executable_ to `<GCC root>\bin\arm-none-eabi-gdb.exe`
    * _OCD Debugger Server Address_ leave `localhost:3333`
    * _OCD Debug Binary_ browse for `TinyBooter.axf` or `TinyClr.axf`, e.g. `<NETMF root>BuildOutput\THUMB2FP\GCC5.4\le\FLASH\release\STM32F746NUCLEO\bin\TinyClr.axf`,
2. Open command prompt in OpenOCD root folder and launch

    `bin\openocd.exe -f <NETMF root>\Solutions\STM32F746NUCLEO\vs\netmf_stm32f746nucleo.cfg`

3. Switch back to Visual Studio and launch **OCD GDB Debugger** command (on the toolbar)

If the connection is successfully established, OpenOCD output should be similar to
```
...
Info : STLINK v2 JTAG v27 API v2 SWIM v15 VID 0x0483 PID 0x374B
Info : using stlink api v2
Info : Target voltage: 3.238270
Info : stm32f7x.cpu: hardware has 8 breakpoints, 4 watchpoints
```
