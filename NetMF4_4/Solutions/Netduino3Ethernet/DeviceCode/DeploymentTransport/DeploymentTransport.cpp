#include <tinyhal.h>
#include "DeploymentTransport.h"

COM_HANDLE GetTransportHandle(UINT8 transportOption)
{
	switch(transportOption)
	{
		case TRANSPORT_COM1:
			return COM1;
		case TRANSPORT_COM2:
			return COM2;
		case TRANSPORT_COM3:
			return COM3;
		case TRANSPORT_COM4:
			return COM4;
#if defined(PLATFORM_ARM_NetduinoShieldBase)
		case TRANSPORT_COM5:
			return COM5;
#endif
		case TRANSPORT_USB1: 
			return USB1;
		case TRANSPORT_ETH1:
			return COM_SOCKET_DBG;
		default:
			return DEFAULT_DEPLOYMENT_PORT;
	}
}

UINT8 GetTransportOption(COM_HANDLE transportHandle)
{
	switch(transportHandle)
	{
		case COM1:
			return TRANSPORT_COM1;
		case COM2:
			return TRANSPORT_COM2;
		case COM3:
			return TRANSPORT_COM3;
		case COM4:
			return TRANSPORT_COM4;
#if defined(PLATFORM_ARM_NetduinoShieldBase)
		case COM5:
			return TRANSPORT_COM5;
#endif
		case USB1: 
			return TRANSPORT_USB1;
		case COM_SOCKET_DBG:
			return TRANSPORT_ETH1;
		default:
			return TRANSPORT_NONE;
	}
}

void DeploymentTransport_Initialize()
{
    char configName[20] = {'S', 'L', '_', 'T', 'R', 'A', 'N', 'S', 'P', 'O', 'R', 'T', '_', 'C', 'O', 'N', 'F', 'I', 'G', 0}; 

	DEPLOYMENT_TRANSPORT_CONFIGURATION transportConfig;	
	
	// attempt to load our transport configuration
	if( HAL_CONFIG_BLOCK::ApplyConfig(configName, &transportConfig, sizeof(transportConfig)))
	{
		// transport configuration has been retrieved; proceed...
	}
	else
	{
		// no transport configuration could be retrieved; set values to defaults instead...
		transportConfig.DebuggerPort = GetTransportOption(DEFAULT_DEPLOYMENT_PORT);
		transportConfig.MessagingPort = GetTransportOption(DEFAULT_DEPLOYMENT_PORT);
		transportConfig.DebugTextPort = GetTransportOption(DEFAULT_DEPLOYMENT_PORT);
		transportConfig.stdio = GetTransportOption(DEFAULT_DEPLOYMENT_PORT);

        // ...and save our default configuration in the configuration sector
        HAL_CONFIG_BLOCK::UpdateBlockWithName(configName, &transportConfig, sizeof(transportConfig), TRUE);
	}

	// set our transport options to our saved defaults
	HalSystemConfig.DebuggerPorts[0] = GetTransportHandle(transportConfig.DebuggerPort);
	HalSystemConfig.MessagingPorts[0] = GetTransportHandle(transportConfig.MessagingPort);
	HalSystemConfig.DebugTextPort = GetTransportHandle(transportConfig.DebugTextPort);
	HalSystemConfig.stdio = GetTransportHandle(transportConfig.stdio);
}

void DeploymentTransport_Set(COM_HANDLE transportHandle)
{
    char configName[20] = {'S', 'L', '_', 'T', 'R', 'A', 'N', 'S', 'P', 'O', 'R', 'T', '_', 'C', 'O', 'N', 'F', 'I', 'G', 0}; 

	DEPLOYMENT_TRANSPORT_CONFIGURATION transportConfig;
	transportConfig.DebuggerPort = GetTransportOption(transportHandle);
	transportConfig.MessagingPort = GetTransportOption(transportHandle);
	transportConfig.DebugTextPort = GetTransportOption(transportHandle);
	transportConfig.stdio = GetTransportOption(transportHandle);

	// update config sector with the new transport configuration
    HAL_CONFIG_BLOCK::UpdateBlockWithName(configName, &transportConfig, sizeof(transportConfig), TRUE);
	
	// update our current transport (NOTE: does not take effect until after reboot.)
	HalSystemConfig.DebuggerPorts[0] = transportHandle;
	HalSystemConfig.MessagingPorts[0] = transportHandle;
	HalSystemConfig.DebugTextPort = transportHandle;
	HalSystemConfig.stdio = transportHandle;
}

COM_HANDLE DeploymentTransport_Get()
{
    char configName[20] = {'S', 'L', '_', 'T', 'R', 'A', 'N', 'S', 'P', 'O', 'R', 'T', '_', 'C', 'O', 'N', 'F', 'I', 'G', 0}; 

	DEPLOYMENT_TRANSPORT_CONFIGURATION transportConfig;

	if(HAL_CONFIG_BLOCK::ApplyConfig(configName, &transportConfig, sizeof(transportConfig)))
	{
		// return the debugger port (NOTE: it is possible that the transport configuration uses different port(s) for messaging, debugtext, and/or stdio)
		return GetTransportHandle(transportConfig.DebuggerPort);
	}
	else
	{
        // use the default transport configuration
		return DEFAULT_DEPLOYMENT_PORT;
	}
}
