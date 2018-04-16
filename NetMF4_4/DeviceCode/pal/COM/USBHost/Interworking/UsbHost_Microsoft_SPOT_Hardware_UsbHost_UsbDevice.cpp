////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "UsbHost.h"

#include <TinyCLR_Runtime.h>

#include <SPOT_native.h>

#include <spot_hardware_usb_native.h>
#include "..\UsbHost_decl.h"


HRESULT UnmarshalConfigDescriptor( CLR_RT_HeapBlock* descriptorReference, const USB_CONFIGURATION_DESCRIPTOR* nativeConfiguration );


HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::GetString___STRING__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();
    
    CLR_RT_HeapBlock* pThis;
    CLR_INT32 controllerIndex, index;
    USBH_STRING_DESC strDesc;
    BOOL retVal;

    pThis = stack.This(); FAULT_ON_NULL(pThis);
    
    controllerIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::FIELD__m_deviceIndex ].NumericByRef().s4;
    if( controllerIndex >= TOTAL_USBH_CONTROLLER )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    
    index = stack.Arg1().NumericByRef().s4;

    retVal = USBH_GetString( controllerIndex, index, &strDesc );
    
    if (retVal) {
        TINYCLR_SET_AND_LEAVE(CLR_RT_HeapBlock_String::CreateInstance(
            stack.PushValue(),            // returned reference
            (CLR_UINT16*)&strDesc.string, // unicode string
            strDesc.bLength - 2 >> 1 ));  // descriptor size to number of characters
    } else {
        stack.PushValueAndClear(); // return null
    }

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::ControlRequest___BOOLEAN__SZARRAY_U1__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();

    CLR_RT_HeapBlock* pThis;
    CLR_INT32 controllerIndex;
    CLR_RT_TypedArray_UINT8 request;
    CLR_RT_TypedArray_UINT8 data;
    BOOL retVal;

    pThis = stack.This(); FAULT_ON_NULL(pThis);
    
    controllerIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::FIELD__m_deviceIndex ].NumericByRef().s4;
    if( controllerIndex >= TOTAL_USBH_CONTROLLER )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }
    
    TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, request ) );
    TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 2, data ) );

    retVal = USBH_ControlRequest( controllerIndex,
        (const USB_SETUP_PACKET *)request.GetBuffer(),
        data.GetBuffer(), data.GetSize() );

    stack.SetResult_Boolean( retVal );

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::get_Configuration___MicrosoftSPOTHardwareUsbMicrosoftSPOTHardwareUsbClientConfiguration( CLR_RT_StackFrame& stack )
{
    // copied from UsbClient_UsbController::get_Configuration
    
    TINYCLR_HEADER();

    CLR_INT32         controllerIndex, nDescriptors, descriptorIndex;

    CLR_RT_HeapBlock* pThis;                // Points to the UsbDevice object (this)
    CLR_RT_HeapBlock* configuration;        // Pointer to configuration structure
    CLR_RT_HeapBlock* descriptorArrayRef;   // Pointer to reference to managed array of descriptors
    CLR_RT_HeapBlock_Array* descriptors;    // This will point to the array of descriptors
    const USB_DYNAMIC_CONFIGURATION* nativeConfig;
    const USB_DESCRIPTOR_HEADER*     nativeDescriptor;

    CLR_RT_HeapBlock& configurationRef = stack.PushValueAndClear(); 

    pThis = stack.This(); FAULT_ON_NULL(pThis);

    controllerIndex = pThis[ Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::FIELD__m_deviceIndex ].NumericByRef().s4;
    if( controllerIndex >= TOTAL_USBH_CONTROLLER )
    {
        TINYCLR_SET_AND_LEAVE(CLR_E_INVALID_PARAMETER);
    }

    TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( configurationRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration ));

    // Obtain a pointer to the null Descriptor array reference in the Configuration
    configuration = configurationRef.Dereference(); FAULT_ON_NULL(configuration);
    descriptorArrayRef = &(configuration[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration::FIELD__descriptors ]);

    // Discover how many descriptors there are in the native configuration
    nativeConfig = USBH_GetConfiguration( controllerIndex );
    FAULT_ON_NULL(nativeConfig);      // If no configuration has been set
    nativeDescriptor = (const USB_DESCRIPTOR_HEADER*)nativeConfig;
    for( nDescriptors = 0; nativeDescriptor->marker != USB_END_DESCRIPTOR_MARKER; nDescriptors++ )
    {
        nativeDescriptor = nativeDescriptor->next(nativeDescriptor);
    }

    // Allocate an array of Descriptors for the returned Configuration
    TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( *descriptorArrayRef, nDescriptors, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__Descriptor ));
    descriptors = descriptorArrayRef->DereferenceArray(); FAULT_ON_NULL(descriptors);

    // Fill the array with information from the native Configuration
    nativeDescriptor = (const USB_DESCRIPTOR_HEADER*)nativeConfig;     // Return to the start of the native Configuration
    descriptorIndex = 0;
    while( nativeDescriptor->marker != USB_END_DESCRIPTOR_MARKER )
    {
        CLR_UINT32        size;
        CLR_RT_HeapBlock* descriptorRef;       // Pointer to descriptor class reference
        CLR_RT_HeapBlock* descriptor;          // Pointer to descriptor class

        // Get a pointer to the Descriptor class to fill in
        descriptorRef = (CLR_RT_HeapBlock*)descriptors->GetElement( descriptorIndex ); FAULT_ON_NULL(descriptorRef);

        // for each descriptorREf, we need to protect it from GC while we go through the loop

        switch( nativeDescriptor->marker )
        {
        case USB_DEVICE_DESCRIPTOR_MARKER:
            {
                const USB_DEVICE_DESCRIPTOR* nativeDevice;

                nativeDevice = (const USB_DEVICE_DESCRIPTOR*)nativeDescriptor;
                // Create a Configuration.DeviceDescriptor in the array
                TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *descriptorRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__DeviceDescriptor ));
                descriptor   = descriptorRef->Dereference(); FAULT_ON_NULL(descriptor);
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Descriptor      ::FIELD__index           ].SetInteger( (CLR_INT32)0                             );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__bcdDevice       ].SetInteger( (CLR_INT16)nativeDevice->bcdDevice       );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__idVendor        ].SetInteger( (CLR_INT16)nativeDevice->idVendor        );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__idProduct       ].SetInteger( (CLR_INT16)nativeDevice->idProduct       );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__iManufacturer   ].SetInteger( (CLR_INT8 )nativeDevice->iManufacturer   );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__iProduct        ].SetInteger( (CLR_INT8 )nativeDevice->iProduct        );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__iSerialNumber   ].SetInteger( (CLR_INT8 )nativeDevice->iSerialNumber   );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__bDeviceClass    ].SetInteger( (CLR_INT8 )nativeDevice->bDeviceClass    );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__bDeviceSubClass ].SetInteger( (CLR_INT8 )nativeDevice->bDeviceSubClass );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__bDeviceProtocol ].SetInteger( (CLR_INT8 )nativeDevice->bDeviceProtocol );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__bMaxPacketSize0 ].SetInteger( (CLR_INT8 )nativeDevice->bMaxPacketSize0 );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__DeviceDescriptor::FIELD__bcdUSB          ].SetInteger( (CLR_INT16)nativeDevice->bcdUSB          );
            }
            break;

        case USB_CONFIGURATION_DESCRIPTOR_MARKER:
            {
                const USB_CONFIGURATION_DESCRIPTOR* nativeConfiguration;

                nativeConfiguration = (const USB_CONFIGURATION_DESCRIPTOR*)nativeDescriptor;
                // Create a Configuration.ConfigurationDescriptor in the array
                TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *descriptorRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__ConfigurationDescriptor));
                descriptor = descriptorRef->Dereference(); FAULT_ON_NULL(descriptor);
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Descriptor             ::FIELD__index          ].SetInteger( (CLR_INT32)0                        );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__ConfigurationDescriptor::FIELD__iConfiguration ].SetInteger( nativeConfiguration->iConfiguration );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__ConfigurationDescriptor::FIELD__bmAttributes   ].SetInteger( nativeConfiguration->bmAttributes   );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__ConfigurationDescriptor::FIELD__bMaxPower      ].SetInteger( nativeConfiguration->bMaxPower      );
                TINYCLR_CHECK_HRESULT(UnmarshalConfigDescriptor( descriptorRef, nativeConfiguration ));
            }
            break;

        case USB_STRING_DESCRIPTOR_MARKER:
            {
                const USB_STRING_DESCRIPTOR_HEADER* nativeString;

                nativeString = (const USB_STRING_DESCRIPTOR_HEADER*)nativeDescriptor;
                size = (nativeString->header.size - sizeof(USB_STRING_DESCRIPTOR_HEADER)) / sizeof(USB_STRING_CHAR);        // Size of string in characters
                // Create a Configuration.StringDescriptor in the array
                TINYCLR_CHECK_HRESULT( g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *descriptorRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__StringDescriptor) );
                descriptor = descriptorRef->Dereference(); FAULT_ON_NULL(descriptor);
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Descriptor::FIELD__index ].SetInteger( (CLR_INT32)nativeString->header.iValue );
                {
                    CLR_UINT16 tempString[ USB_STRING_DESCRIPTOR_MAX_LENGTH ];              // Word aligned storage for uh
                    USB_STRING_CHAR* nativeText = (USB_STRING_CHAR*)&nativeString[ 1 ];     // Native text begins right after header

                    // Copy byte aligned USB character storage to word aligned UTF16 temporary storage for uh
                    for(CLR_UINT32 i = 0; i < size; i++)
                    {
                        tempString[ i ] = (CLR_UINT16)nativeText[ i ];
                    }

                    // Create the UTF8 based string object using the supplied UTF16 string
                    TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance( descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__StringDescriptor::FIELD__sString ], tempString, size ));
                }
            }
            break;

        case USB_GENERIC_DESCRIPTOR_MARKER:
            {
                CLR_UINT8*        payloadData;
                CLR_UINT8*        nativeData;
                CLR_RT_HeapBlock* payloadArrayRef;
                CLR_RT_HeapBlock_Array* payload;
                const USB_GENERIC_DESCRIPTOR_HEADER* nativeGeneric;

                nativeGeneric = (const USB_GENERIC_DESCRIPTOR_HEADER*)nativeDescriptor;
                size = nativeGeneric->header.size - sizeof(USB_GENERIC_DESCRIPTOR_HEADER);      // Size of payload in bytes

                // Create a Configuration.GenericDescriptor in the array
                TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *descriptorRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__GenericDescriptor ));
                descriptor = descriptorRef->Dereference(); FAULT_ON_NULL(descriptor);
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Descriptor::FIELD__index                ].SetInteger( (CLR_INT32)0 );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__GenericDescriptor::FIELD__bmRequestType ].SetInteger( nativeGeneric->bmRequestType );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__GenericDescriptor::FIELD__bRequest      ].SetInteger( nativeGeneric->bRequest );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__GenericDescriptor::FIELD__wValue        ].SetInteger( nativeGeneric->wValue );
                descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__GenericDescriptor::FIELD__wIndex        ].SetInteger( nativeGeneric->wIndex );

                // Copy the payload
                payloadArrayRef = &(descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__GenericDescriptor::FIELD__payload ]);
                TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( *payloadArrayRef, size, g_CLR_RT_WellKnownTypes.m_UInt8 ));
                payload = payloadArrayRef->DereferenceArray(); FAULT_ON_NULL(payload);
                payloadData = payload->GetFirstElement(); FAULT_ON_NULL(payloadData);
                nativeData  = (CLR_UINT8*)(&nativeGeneric[ 1 ]);
                memcpy( payloadData, nativeData, size );
            }
            break;

        default:
            TINYCLR_SET_AND_LEAVE(CLR_E_FAIL);
            break;
        }

        descriptorIndex++;                                              // Move to next managed array element
        nativeDescriptor = nativeDescriptor->next(nativeDescriptor);    // Move to next native Descriptor
    }

    TINYCLR_NOCLEANUP();
}

HRESULT Library_UsbHost_Microsoft_SPOT_Hardware_UsbHost_UsbDevice::get_Count___STATIC__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER();
    
    CLR_INT32 retVal = USBH_GetControllerCount();
    stack.SetResult_I4( retVal );

    TINYCLR_NOCLEANUP_NOLABEL();
}


// Parsing out the configuration descriptor is somewhat messy, so it is put in a separate routine
// to make things appear prettier than they really are.
// descriptorReference is a poitner to a reference to the ConfigurationDescriptor object
HRESULT UnmarshalConfigDescriptor( CLR_RT_HeapBlock* descriptorReference, const USB_CONFIGURATION_DESCRIPTOR* nativeConfiguration )
{
    TINYCLR_HEADER();

    CLR_UINT32 nInterfaces;                            // Holds the number of interfaces
    CLR_UINT32 interfaceIndex;                         // Index into managed array of interfaces
    CLR_INT32  endpointIndex = 0;                      // Index into managed array of endpoints
    CLR_INT32  classIndex = 0;                         // Index into managed array of class descriptors
    CLR_UINT16 totalLength;                            // Holds the total length of the native configuration descriptor
    CLR_UINT16 length;                                 // Running length as compared to totalLength
    USB_CLASS_DESCRIPTOR_HEADER* nativeDescriptor;     // Points to the current native descriptor
    CLR_RT_HeapBlock* descriptor;                      // Pointer to managed descriptor
    CLR_RT_HeapBlock* interfaceArrayRef;               // Pointer to reference to array of managed descriptors
    CLR_RT_HeapBlock* usbInterface = NULL;             // Pointer to current managed interface descriptor
    CLR_RT_HeapBlock_Array* interfaces;                // Pointer to managaed array of interfaces
    CLR_RT_HeapBlock_Array* endpoints = NULL;          // Pointer to reference to array of endpoints
    CLR_RT_HeapBlock_Array* classes   = NULL;          // Pointer to reference to array of class descriptors

    // In order to search through the native config descriptor, its length must be known
    totalLength = nativeConfiguration->wTotalLength - nativeConfiguration->bLength;
    
    // First, count the number of interfaces
    length           = 0;
    nInterfaces      = 0;
    nativeDescriptor = (USB_CLASS_DESCRIPTOR_HEADER*)&nativeConfiguration[ 1 ];
    while( length < totalLength )
    {
        if( nativeDescriptor->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE )
        {
            nInterfaces++;
        }

        if( nativeDescriptor->bLength == 0 ) TINYCLR_SET_AND_LEAVE(CLR_E_FAIL);

        length += nativeDescriptor->bLength;

        nativeDescriptor  = (USB_CLASS_DESCRIPTOR_HEADER*)&(((CLR_UINT8*)nativeDescriptor)[ nativeDescriptor->bLength ]);
    }

    FAULT_ON_NULL( descriptorReference );
    descriptor = descriptorReference->Dereference(); FAULT_ON_NULL(descriptor);

    // Create the array of UsbInterfaces
    interfaceArrayRef = &(descriptor[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__ConfigurationDescriptor::FIELD__interfaces ]);
    TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( *interfaceArrayRef, nInterfaces, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__UsbInterface ));
    interfaces = interfaceArrayRef->DereferenceArray(); FAULT_ON_NULL(interfaces);

    // Fill in the information for all UsbInterfaces
    nativeDescriptor = (USB_CLASS_DESCRIPTOR_HEADER*)&nativeConfiguration[ 1 ];        // Back to top of Configuration list
    if( nativeDescriptor->bDescriptorType != USB_INTERFACE_DESCRIPTOR_TYPE )
        TINYCLR_SET_AND_LEAVE(CLR_E_FAIL)       // The Configuration has been checked, so this should never happen
    length         = 0;
    interfaceIndex = 0;
    while( length < totalLength )
    {
        CLR_RT_HeapBlock* usbInterfaceRef;      // Pointer to reference to managed interface descriptor
        CLR_RT_HeapBlock* endpointArrayRef;     // Pointer to reference to managed array of endpoints
        CLR_RT_HeapBlock* classArrayRef = NULL; // Pointer to reference to managed array of class descriptors

        if( nativeDescriptor->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE )
        {
            CLR_INT32  nEndpoints;                             // Number of endpoints for the current interface descriptor
            CLR_INT32  nClasses;                               // Number of class descriptors for the current interface descriptor
            CLR_UINT16 subLength;                              // Number of bytes from end if interface descriptor to end of entire configuration descriptor
            USB_INTERFACE_DESCRIPTOR*    nativeInterface;      // Pointer to the current native interface descriptor
            USB_CLASS_DESCRIPTOR_HEADER* subDescriptor;        // Pointer to native descriptor associated with a single interface descriptor

            nativeInterface = (USB_INTERFACE_DESCRIPTOR*)nativeDescriptor;

            // usbInterface is a pointer to an interface reference
            usbInterfaceRef = (CLR_RT_HeapBlock*)interfaces->GetElement( interfaceIndex++ );
            TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *usbInterfaceRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__UsbInterface ));
            usbInterface = usbInterfaceRef->Dereference(); FAULT_ON_NULL(usbInterface);

            usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__bInterfaceNumber   ].SetInteger(nativeInterface->bInterfaceNumber);
            usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__bInterfaceClass    ].SetInteger(nativeInterface->bInterfaceClass);
            usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__bInterfaceSubClass ].SetInteger(nativeInterface->bInterfaceSubClass);
            usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__bInterfaceProtocol ].SetInteger(nativeInterface->bInterfaceProtocol);
            usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__iInterface         ].SetInteger(nativeInterface->iInterface);

            // The number of endpoints for the interface must be counted
            nEndpoints    = 0;
            nClasses      = 0;
            subDescriptor = (USB_CLASS_DESCRIPTOR_HEADER*)&(((CLR_UINT8*)nativeDescriptor)[ nativeDescriptor->bLength ]);     // Start with descriptor after interface descriptor
            subLength     = length + nativeDescriptor->bLength;
            while( subLength < totalLength )
            {
                if( subDescriptor->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE )
                    nEndpoints++;
                else if( subDescriptor->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE )
                    break;
                else
                    nClasses++;
                
                subLength     += subDescriptor->bLength;
                subDescriptor  = (USB_CLASS_DESCRIPTOR_HEADER*)&(((CLR_UINT8*)subDescriptor)[ subDescriptor->bLength ]);
            }

            // Allocate array for the endpoints - endpoints is a pointer to a reference to an array
            endpointArrayRef = &(usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__endpoints ]);
            TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( *endpointArrayRef, nEndpoints, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__Endpoint ));
            endpoints = endpointArrayRef->DereferenceArray();
            endpointIndex = 0;

            if(nClasses > 0)
            {
                classArrayRef = &(usbInterface[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__UsbInterface::FIELD__classDescriptors ]);
                TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( *classArrayRef, nClasses, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__ClassDescriptor ));
                classes = classArrayRef->DereferenceArray();
            }
            else
            {
                classes = NULL;
            }
            classIndex = 0;
        }
        else if (nativeDescriptor->bDescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE )
        {
            USB_ENDPOINT_DESCRIPTOR* nativeEndpoint;       // Pointer to native endpoint
            CLR_RT_HeapBlock*        endpointRef;          // Pointer to reference to endpoint
            CLR_RT_HeapBlock*        endpoint;             // Pointer to endpoint reference

            nativeEndpoint = (USB_ENDPOINT_DESCRIPTOR*)nativeDescriptor;
            FAULT_ON_NULL(endpoints);
            endpointRef = (CLR_RT_HeapBlock*)endpoints->GetElement( endpointIndex++ ); FAULT_ON_NULL(endpointRef);
            TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *endpointRef, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__Endpoint ));
            endpoint = endpointRef->Dereference(); FAULT_ON_NULL(endpoint);
            endpoint[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Endpoint::FIELD__bEndpointAddress ].SetInteger(nativeEndpoint->bEndpointAddress & 0x7F);
            endpoint[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Endpoint::FIELD__bmAttributes     ].SetInteger(nativeEndpoint->bmAttributes | (nativeEndpoint->bEndpointAddress & 0x80));
            endpoint[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Endpoint::FIELD__wMaxPacketSize   ].SetInteger(nativeEndpoint->wMaxPacketSize);
            endpoint[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__Endpoint::FIELD__bInterval        ].SetInteger(nativeEndpoint->bInterval);
        }
        else        // Assume it to be some kind of interface class descriptor
        {
            // Calculate the payload size
            CLR_UINT32        size;                  // Number of bytes in payload (native or managed)
            CLR_RT_HeapBlock* classReference;        // Pointer to Interface class reference
            CLR_RT_HeapBlock* interfaceClass;        // Pointer to Interface class managed object
            CLR_RT_HeapBlock* payloadReference;      // Pointer to payload array reference
            CLR_UINT8*        payloadData;           // Pointer to managed array data
            CLR_UINT8*        nativePayload;         // Pointer to native payload
            CLR_RT_HeapBlock_Array* payload;         // Pointer to managed array of payload bytes

            // Get size of native payload
            size = nativeDescriptor->bLength - sizeof(USB_CLASS_DESCRIPTOR_HEADER);
            
            // Create the new class object
            FAULT_ON_NULL(classes);
            classReference   = (CLR_RT_HeapBlock*)classes->GetElement( classIndex++ ); FAULT_ON_NULL(classReference);
            TINYCLR_CHECK_HRESULT(g_CLR_RT_ExecutionEngine.NewObjectFromIndex( *classReference, g_CLR_RT_WellKnownTypes.m_UsbClientConfiguration__ClassDescriptor ));
            interfaceClass   = classReference->Dereference(); FAULT_ON_NULL(interfaceClass);
            payloadReference = &(interfaceClass[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__ClassDescriptor::FIELD__payload ]);
            TINYCLR_CHECK_HRESULT(CLR_RT_HeapBlock_Array::CreateInstance( *payloadReference, size, g_CLR_RT_WellKnownTypes.m_UInt8 ));
            payload          = payloadReference->DereferenceArray(); FAULT_ON_NULL(payload);

            // Copy the native interface class descriptor values to the managed class
            interfaceClass[ Library_spot_hardware_usb_native_Microsoft_SPOT_Hardware_UsbClient_Configuration__ClassDescriptor::FIELD__bDescriptorType ].SetInteger(nativeDescriptor->bDescriptorType);
            payloadData   = payload->GetFirstElement(); FAULT_ON_NULL(payloadData);
            nativePayload = (CLR_UINT8*)&nativeDescriptor[ 1 ];
            memcpy( payloadData, nativePayload, size );
        }

        // Get next descriptor
        length += nativeDescriptor->bLength;
        nativeDescriptor = (USB_CLASS_DESCRIPTOR_HEADER*)&(((CLR_UINT8*)nativeDescriptor)[ nativeDescriptor->bLength ]);
    }

    TINYCLR_NOCLEANUP();
}

