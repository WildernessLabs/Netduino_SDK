Firmware build
--------------

Requirements:
	- ARM MDK 5.10
	- .NETMF SDK v4.3 (https://netmf.codeplex.com/releases/view/611040)

Build steps:

1. Pick which flavor you want to build:
	- Netduino3Wifi is in Netduino_3_NETMF
	- All other variants are in Netduino_2_and_3_NETMF
2. Open cmd.exe and enter the flavor directory
3. Each board directory is in $(FlavorDirectory)\Solutions\
4. Enter the board directory and run:
	- msbuild dotnetmf.proj /t:rebuild /p:flavor=release
5. The build output will be in:
	- $(FlavorDirectory)\BuildOutput
