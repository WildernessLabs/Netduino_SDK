////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "UsbHost.h"

#include <TinyCLR_Runtime.h>

#include "..\UsbHost_decl.h"


// Mainly copied from spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_UsbStream.cpp


HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::nativeOpen___I4__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT32         streamIndex, deviceIndex, writeEndpoint, readEndpoint;
    CLR_RT_HeapBlock* pThis;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    deviceIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_deviceIndex ].NumericByRef().s4;
    writeEndpoint   = stack.Arg1().NumericByRef().s4;
    readEndpoint    = stack.Arg2().NumericByRef().s4;

    for( streamIndex = 0; streamIndex < USBH_MAX_QUEUES; streamIndex++ )
    {
        CLR_INT32 stream = (deviceIndex << USB_CONTROLLER_SHIFT) + streamIndex;
        if( USBH_OpenStream( stream, writeEndpoint, readEndpoint ) )  break;
    }
    if( streamIndex >= USBH_MAX_QUEUES )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    stack.SetResult_I4( streamIndex );

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::nativeClose___VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT32         deviceIndex, streamIndex, stream;
    CLR_RT_HeapBlock* pThis;

    pThis = stack.This();  FAULT_ON_NULL(pThis);

    deviceIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_deviceIndex ].NumericByRef().s4;
    streamIndex     = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_streamIndex     ].NumericByRef().s4;

    stream = (deviceIndex << USB_CONTROLLER_SHIFT) + streamIndex;
    if( !USBH_CloseStream( stream ) )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::nativeRead___I4__SZARRAY_U1__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT32               retVal = 0;
    CLR_INT32               deviceIndex, streamIndex, stream;
    CLR_UINT32              offset, count;
    CLR_RT_HeapBlock*       pThis;
    CLR_RT_HeapBlock_Array* array;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    deviceIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_deviceIndex ].NumericByRef().s4;
    streamIndex     = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_streamIndex     ].NumericByRef().s4;
    array           = stack.Arg1().DereferenceArray(); FAULT_ON_NULL(array);
    offset          = stack.Arg2().NumericByRef().u4;
    count           = stack.Arg3().NumericByRef().u4;
    
    stream = (deviceIndex << USB_CONTROLLER_SHIFT) + streamIndex;
    if( array->m_numOfElements < (offset + count) )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    else
    {
        retVal = USBH_Read( stream, (char*)array->GetFirstElement()+offset, count );
    }
    stack.SetResult_I4( retVal );

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::nativeWrite___I4__SZARRAY_U1__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT32               retVal = 0;
    CLR_INT32               deviceIndex, streamIndex, stream;
    CLR_UINT32              offset, count;
    CLR_RT_HeapBlock*       pThis;
    CLR_RT_HeapBlock_Array* array;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    deviceIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_deviceIndex ].NumericByRef().s4;
    streamIndex     = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_streamIndex     ].NumericByRef().s4;
    array           = stack.Arg1().DereferenceArray(); FAULT_ON_NULL( array );
    offset          = stack.Arg2().NumericByRef().u4;
    count           = stack.Arg3().NumericByRef().u4;

    stream = (deviceIndex << USB_CONTROLLER_SHIFT) + streamIndex;
    if( array->m_numOfElements < (offset + count) )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    else
    {
        retVal = USBH_Write( stream, (char*)array->GetFirstElement()+offset, count );
    }
    stack.SetResult_I4( retVal );

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::nativeFlush___VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_INT32         deviceIndex, streamIndex, stream;
    CLR_RT_HeapBlock* pThis;

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    deviceIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_deviceIndex ].NumericByRef().s4;
    streamIndex     = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbStream::FIELD__m_streamIndex     ].NumericByRef().s4;

    stream = (deviceIndex << USB_CONTROLLER_SHIFT) + streamIndex;
    USBH_Flush( stream );

    TINYCLR_NOCLEANUP();
}
