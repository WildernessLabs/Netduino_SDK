//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SecretLabs_NETMF_Diagnostics.h"
#include "SecretLabs_NETMF_Diagnostics_SecretLabs_NETMF_Diagnostics_Transport.h"

#include "..\DeploymentTransport\DeploymentTransport.h"

using namespace SecretLabs::NETMF::Diagnostics;

void Transport::TransportInterface_Set( UINT8 param0, HRESULT &hr )
{
    switch (param0)
    {
        case TRANSPORT_COM1: // COM1
            DeploymentTransport_Set(COM1);
            break;
        case TRANSPORT_COM2: // COM2
            DeploymentTransport_Set(COM2);
            break;
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
        case TRANSPORT_COM3: // COM3
            DeploymentTransport_Set(COM3);
            break;
        case TRANSPORT_COM4: // COM4
            DeploymentTransport_Set(COM4);
            break;
#endif
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
        case TRANSPORT_USB1: // USB1
            DeploymentTransport_Set(USB1);
            break;
#endif
#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
        case TRANSPORT_ETH1: // COM_SOCKET_DBG
            DeploymentTransport_Set(COM_SOCKET_DBG);
            break;
#endif
        default:
            // not a valid transport; abort.
            // TODO: return an "invalid argument" exception
            return;
    }
	
	// reset so transport interface change takes effect
	CPU_Reset();
}

UINT8 Transport::TransportInterface_Get( HRESULT &hr )
{
    switch (DeploymentTransport_Get())
    {
        case COM1: // COM1
            return TRANSPORT_COM1; 
        case COM2: // COM2
            return TRANSPORT_COM2;
        case COM3: // COM3
            return TRANSPORT_COM3; 
        case COM4: // COM4
            return TRANSPORT_COM4;
        case USB1: // USB1
            return TRANSPORT_USB1;
		case COM_SOCKET_DBG: // COM_SOCKET_DBG
			return TRANSPORT_ETH1;
        default: // NONE
            return TRANSPORT_NONE;
    }
}

