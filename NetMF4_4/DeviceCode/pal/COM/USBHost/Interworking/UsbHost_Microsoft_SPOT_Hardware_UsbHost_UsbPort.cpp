////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "UsbHost.h"

#include <TinyCLR_Runtime.h>

#include "..\UsbHost_decl.h"


// Mainly copied from spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbController.cpp


HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbPort::get_Status___MicrosoftSPOTHardwareUsbMicrosoftSPOTHardwareUsbClientUsbControllerPortState( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT32         status = USB_DEVICE_STATE_UNINITIALIZED;
    CLR_INT32         portIndex;
    CLR_RT_HeapBlock* pThis;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    portIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbPort::FIELD__m_portIndex ].NumericByRef().s4;

    if( portIndex >= TOTAL_USBH_CONTROLLER )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    else
    {
        status = USBH_GetStatus( portIndex );
    }

    stack.SetResult_I4( status );
        
    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbPort::nativeStart___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT8          retVal = FALSE;
    CLR_INT32         portIndex;
    CLR_RT_HeapBlock* pThis;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    portIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbPort::FIELD__m_portIndex ].NumericByRef().s4;

    if( portIndex >= TOTAL_USB_CONTROLLER )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    else
    {
        retVal = USBH_Initialize( portIndex );
    }

    stack.SetResult_I4( retVal );

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbPort::nativeStop___BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT8          retVal = FALSE;
    CLR_INT32         portIndex;
    CLR_RT_HeapBlock* pThis;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    portIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbPort::FIELD__m_portIndex ].NumericByRef().s4;

    if( portIndex >= TOTAL_USB_CONTROLLER )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    else
    {
        retVal = USBH_Uninitialize( portIndex );
    }
    stack.SetResult_I4( retVal );

    TINYCLR_NOCLEANUP();
}
