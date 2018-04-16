////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Oberon microsystems, Inc.
//
// USB Host Mode PAL Driver
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "USBHost.h"
#include "USBHost_decl.h"

#define USBH_FLUSH_RETRY_COUNT 1000


// should be replaced by a concept for a plugable driver 
void MSC_MountDevice();
void MSC_UnmountDevice();



//--//

int USBH_GetControllerCount()
{
    return USBH_Driver::GetControllerCount();
}

BOOL USBH_Initialize( int Controller )
{
    return USBH_Driver::Initialize( Controller );
}

BOOL USBH_Uninitialize( int Controller )
{
    return USBH_Driver::Uninitialize( Controller );
}

const USB_DYNAMIC_CONFIGURATION * USBH_GetConfiguration( int Controller )
{
    return USBH_Driver::GetConfiguration( Controller );
}

BOOL USBH_GetString( int Controller, int index, USBH_STRING_DESC* string )
{
    return USBH_Driver::GetString( Controller, index, string );
}

BOOL USBH_ControlRequest( int Controller, const USB_SETUP_PACKET* request, BYTE* data, int length )
{
    return USBH_Driver::ControlRequest( Controller, request, data, length );
}

int USBH_ErrorState( int Controller, int ep )
{
    return USBH_Driver::ErrorState( Controller, ep );
}

BOOL USBH_ResetError( int Controller, int ep )
{
    return USBH_Driver::ResetError( Controller, ep );
}

BOOL USBH_OpenStream( int UsbStream, int writeEP, int readEP )
{
    return USBH_Driver::OpenStream( UsbStream, writeEP, readEP );
}

BOOL USBH_CloseStream( int UsbStream )
{
    return USBH_Driver::CloseStream( UsbStream );
}

void USBH_DiscardData( int UsbStream, BOOL fTx )
{
    USBH_Driver::DiscardData(UsbStream, fTx);
}

int USBH_Write( int UsbStream, const char* Data, size_t size )
{
    return USBH_Driver::Write( UsbStream, Data, size );
}

int USBH_Read( int UsbStream, char* Data, size_t size )
{
    return USBH_Driver::Read( UsbStream, Data, size );
}

BOOL USBH_Flush( int UsbStream )
{
    return USBH_Driver::Flush(UsbStream);
}

UINT32 USBH_GetEvent( int Controller, UINT32 Mask )
{
    return USBH_Driver::GetEvent( Controller, Mask );
}

UINT32 USBH_SetEvent( int Controller, UINT32 Event )
{
    return USBH_Driver::SetEvent( Controller, Event );
}

UINT32 USBH_ClearEvent( int Controller, UINT32 Event )
{
    return USBH_Driver::ClearEvent( Controller, Event );
}

UINT8 USBH_GetStatus( int Controller )
{
    return USBH_Driver::GetStatus( Controller );
}

//--//

int USBH_Driver::GetControllerCount()
{
    return TOTAL_USBH_CONTROLLER;
}

void USBH_UnitializeAll()
{
    for (int i = 0; i < TOTAL_USBH_CONTROLLER; i++) {
        USBH_Uninitialize(i);
    }
}

BOOL USBH_Driver::Initialize( int Controller )
{
    NATIVE_PROFILE_PAL_COM();

    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );

    GLOBAL_LOCK(irq);

    if(State == NULL) return FALSE;
    
    //State->Configuration = NULL;

    /* now we actually initialize everything */
    if(State->Initialized == FALSE)
    {
        State->ControllerNum = Controller;
        State->CurrentState  = USB_DEVICE_STATE_UNINITIALIZED;

        if( S_OK != CPU_USBH_Initialize( Controller ) )
        {
            return FALSE;       // If Hardware initialization fails
        }

        State->Initialized   = TRUE;

        // Ensure driver gets unitialized during soft reboot
        HAL_AddSoftRebootHandler(USBH_UnitializeAll);

        return TRUE;
    }
    
    return FALSE;
}

BOOL USBH_Driver::Uninitialize( int Controller )
{
    NATIVE_PROFILE_PAL_COM();

    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
    
    if(NULL == State) return FALSE;
    
    GLOBAL_LOCK(irq);

    if(State->Initialized == FALSE) return FALSE;

    // free configuration data
    if (State->Configuration) private_free((void*)State->Configuration);
    
    // Stop all activity on the specified Controller
    State->Initialized = FALSE;
    State->CurrentState = USB_DEVICE_STATE_UNINITIALIZED;

    CPU_USBH_Uninitialize( Controller );

    return TRUE;
}

//
// The GetConfiguration method returns the configuration read fro the attached device.
//
const USB_DYNAMIC_CONFIGURATION * USBH_Driver::GetConfiguration( int Controller )
{
    NATIVE_PROFILE_PAL_COM();

    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if(NULL == State) return NULL;
    
    return State->Configuration;
}


/* Basic Setup Request Packets */
// get descriptor request
static USB_SETUP_PACKET USBH_GetDescriptorRequest = { 
    USB_SETUP_DIRECTION_HOST | USB_SETUP_RECIPIENT_DEVICE,   // bmRequestType
    USB_GET_DESCRIPTOR,                                      // bRequest
    0,                                                       // wValue
    0,                                                       // wIndex
    0                                                        // wLength
};
// set address request
const static USB_SETUP_PACKET USBH_SetAddressRequest = {  
    USB_SETUP_DIRECTION_DEVICE | USB_SETUP_RECIPIENT_DEVICE, // bmRequestType
    USB_SET_ADDRESS,                                         // bRequest
    1,                                                       // wValue
    0,                                                       // wIndex
    0                                                        // wLength
};
// set configuration request
const static USB_SETUP_PACKET USBH_SetConfigurationRequest = { 
    USB_SETUP_DIRECTION_DEVICE | USB_SETUP_RECIPIENT_DEVICE, // bmRequestType
    USB_SET_CONFIGURATION,                                   // bRequest
    1,                                                       // wValue
    0,                                                       // wIndex
    0                                                        // wLength
};
// set feature request
static USB_SETUP_PACKET USBH_ClearEPFeatureRequest = { 
    USB_SETUP_DIRECTION_DEVICE | USB_SETUP_RECIPIENT_ENDPOINT, // bmRequestType
    USB_CLEAR_FEATURE,                                       // bRequest
    0,                                                       // wValue
    0,                                                       // wIndex
    0                                                        // wLength
};


BOOL USBH_Driver::GetString ( int Controller, int index, USBH_STRING_DESC* string )
{
    NATIVE_PROFILE_PAL_COM();

    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if (index == 0 || NULL == State || State->CurrentState != USB_DEVICE_STATE_CONFIGURED) return FALSE;

    // handle special IDs
    if (index >= USBH_ID_ManufacturerString) {
        const USB_DEVICE_DESCRIPTOR* desc = (const USB_DEVICE_DESCRIPTOR*)State->Configuration;
        if (!desc || desc->header.marker != USB_DEVICE_DESCRIPTOR_MARKER) return FALSE;
        if (index == USBH_ID_ManufacturerString) index = desc->iManufacturer;
        else if (index == USBH_ID_ProductString) index = desc->iProduct;
        else /* index == USBH_ID_SerialNumber */ index = desc->iSerialNumber;
    }
    
    // complete the request
    USBH_GetDescriptorRequest.wValue = USB_STRING_DESCRIPTOR_TYPE << 8 | index;
    USBH_GetDescriptorRequest.wLength = 255;
    
    // if string available in stored configuration, just return the string
    const USB_DESCRIPTOR_HEADER* header;
    header = USB_FindRecord( (USB_CONTROLLER_STATE*)State,
                             USB_STRING_DESCRIPTOR_MARKER, &USBH_GetDescriptorRequest );
    if (header) {
        // copy plain string descriptor
        memcpy(string,
               &((USB_STRING_DESCRIPTOR_HEADER*)header)->bLength,
               ((USB_STRING_DESCRIPTOR_HEADER*)header)->bLength);
        return TRUE;
    }
    
    // otherwise read from device
    return ControlRequest(Controller, &USBH_GetDescriptorRequest,
                          (BYTE*)string, sizeof(USBH_STRING_DESC));
}

BOOL USBH_Driver::ControlRequest ( int Controller, const USB_SETUP_PACKET* request,
                                   BYTE* data, int length )
{
    NATIVE_PROFILE_PAL_COM();
    
    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );
    
    if (NULL == State || State->CurrentState != USB_DEVICE_STATE_CONFIGURED) return FALSE;

    State->AsynchResult = -1;
    
    if (!USBH_AsynchRequest(Controller, request, data, length, NULL)) return FALSE;
    
    // wait 100ms for completion
    int n = 0;
    while (State->AsynchResult < 0) {
        n++;
        if (n >= 1000) return FALSE; // timeout
        HAL_Time_Sleep_MicroSeconds(100);
    }
    
    return State->AsynchResult == USBH_EP_OK;
}

    
int USBH_Driver::ErrorState( int Controller, int ep )
{
    NATIVE_PROFILE_PAL_COM();
    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );
    ep &= ~USB_ENDPOINT_DIRECTION_IN; // !!!
    return State->EndpointStatus[ep];
}

BOOL USBH_Driver::ResetError( int Controller, int ep )
{
    NATIVE_PROFILE_PAL_COM();
    BYTE data[4]; // dummy data

    // send clear feature request to endpoint
    USBH_ClearEPFeatureRequest.wIndex = ep;
    return ControlRequest(Controller, &USBH_ClearEPFeatureRequest, data, 0);
    
    // if successful, State->EndpointStatus[ep] is reset in USBH_ControlCallback
}


BOOL USBH_Driver::OpenStream( int UsbStream, int writeEP, int readEP )
{
    NATIVE_PROFILE_PAL_COM();

    int Controller  = ConvertCOM_UsbController ( UsbStream );
    int StreamIndex = ConvertCOM_UsbStreamIndex( UsbStream );

    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if( NULL == State || !State->Initialized )     // If no such controller exists (or it is not initialized)
        return FALSE;

    // Check the StreamIndex and the two endpoint numbers for validity (both endpoints cannot be zero)
    readEP &= ~USB_ENDPOINT_DIRECTION_IN; // !!!
    if( StreamIndex >= USBH_MAX_QUEUES
        || (readEP == USB_NULL_ENDPOINT && writeEP == USB_NULL_ENDPOINT)
        || (readEP != USB_NULL_ENDPOINT && (readEP < 1 || readEP >= USBH_MAX_QUEUES))
        || (writeEP != USB_NULL_ENDPOINT && (writeEP < 1 || writeEP >= USBH_MAX_QUEUES)) )
        return FALSE;

    // The Stream must be currently closed
    if( State->streams[StreamIndex].RxEP != USB_NULL_ENDPOINT || State->streams[StreamIndex].TxEP != USB_NULL_ENDPOINT )
        return FALSE;

    // The requested endpoints must have been configured
    if( (readEP != USB_NULL_ENDPOINT && State->Queues[readEP] == NULL) || (writeEP != USB_NULL_ENDPOINT && State->Queues[writeEP] == NULL) )
        return FALSE;

    // The requested endpoints can only be used in the direction specified by the configuration
    if( (readEP != USB_NULL_ENDPOINT && State->IsTxQueue[readEP]) || (writeEP != USB_NULL_ENDPOINT && !State->IsTxQueue[writeEP]) )
        return FALSE;

    // The specified endpoints must not be in use by another stream
    for( int stream = 0; stream < USBH_MAX_QUEUES; stream++ )
    {
        if( readEP != USB_NULL_ENDPOINT && (State->streams[stream].RxEP == readEP || State->streams[stream].TxEP == readEP) )
            return FALSE;
        if( writeEP != USB_NULL_ENDPOINT && (State->streams[stream].RxEP == writeEP || State->streams[stream].TxEP == writeEP) )
            return FALSE;
    }

    // All tests pass, assign the endpoints to the stream
    {
        GLOBAL_LOCK(irq);

        State->streams[StreamIndex].RxEP = readEP;
        State->streams[StreamIndex].TxEP = writeEP;
        
        State->CurrentPacketOffset[readEP] = 0;
    }

    return TRUE;
}

BOOL USBH_Driver::CloseStream ( int UsbStream )
{
    NATIVE_PROFILE_PAL_COM();

    int Controller  = ConvertCOM_UsbController ( UsbStream );
    int StreamIndex = ConvertCOM_UsbStreamIndex( UsbStream );

    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if( NULL == State || !State->Initialized )
        return FALSE;
    
    if( StreamIndex >= USBH_MAX_QUEUES )
        return FALSE;

    {
        int endpoint;
        GLOBAL_LOCK(irq);

        // Close the Rx stream
        endpoint = State->streams[StreamIndex].RxEP;
        if( endpoint != USB_NULL_ENDPOINT && State->Queues[endpoint] )
        {
            State->Queues[endpoint]->Initialize();      // Clear the queue
        }
        State->streams[StreamIndex].RxEP = USB_NULL_ENDPOINT;

        // Close the TX stream
        endpoint = State->streams[StreamIndex].TxEP;
        if( endpoint != USB_NULL_ENDPOINT && State->Queues[endpoint] )
        {
            State->Queues[endpoint]->Initialize();      // Clear the queue
        }
        State->streams[StreamIndex].TxEP = USB_NULL_ENDPOINT;
    }
    
    return TRUE;
}

int USBH_Driver::Write( int UsbStream, const char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();

    int Controller  = ConvertCOM_UsbController ( UsbStream );
    int StreamIndex = ConvertCOM_UsbStreamIndex( UsbStream );
    int endpoint;
    int totWrite = 0;
    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if( NULL == State || StreamIndex >= USBH_MAX_QUEUES )
    {
        return -1;
    }

    if(size == 0   ) return 0;
    if(Data == NULL)
    {
        return -1;
    }

    // If the Controller is not yet initialized
    if(State->CurrentState != USB_DEVICE_STATE_CONFIGURED)
    {
        // No data can be sent until the controller is initialized
        return -1;
    }
    
    endpoint = State->streams[StreamIndex].TxEP;
    // If no Write side to stream (or if not yet open)
    if( endpoint == USB_NULL_ENDPOINT || State->Queues[endpoint] == NULL )
    {
        return -1;
    }
    else
    {
        GLOBAL_LOCK(irq);

        const char*   ptr         = Data;
        UINT32        count       = size;
        BOOL          Done        = FALSE;
        UINT32        WaitLoopCnt = 0;

        // This loop packetizes the data and sends it out.  All packets sent have
        // the maximum size for the given endpoint except for the last packet which
        // will always have less than the maximum size - even if the packet length
        // must be zero for this to occur.   This is done to comply with standard
        // USB bulk-mode transfers.
        while(!Done)
        {

            USB_PACKET64* Packet64 = State->Queues[endpoint]->Push();

            if(Packet64)
            {
                UINT32 max_move;

                if(count > State->MaxPacketSize[endpoint])
                    max_move = State->MaxPacketSize[endpoint];
                else
                    max_move = count;

                if(max_move)
                {
                    memcpy( Packet64->Buffer, ptr, max_move );
                }

                // we are done when we send a non-full length packet
                if(max_move < State->MaxPacketSize[endpoint])
                {
                    Done = TRUE;
                }

                Packet64->Size  = max_move;
                count          -= max_move;
                ptr            += max_move;

                totWrite       += max_move;

                WaitLoopCnt = 0;
            }
            else
            {
                return totWrite;
            }
        }

        // here we have a post-condition that IRQs are disabled for all paths through conditional block above

        if(State->CurrentState == USB_DEVICE_STATE_CONFIGURED)
        {
            CPU_USBH_StartOutput( State, endpoint );
        }

        return totWrite;
    }
}

int USBH_Driver::Read( int UsbStream, char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();

    int Controller  = ConvertCOM_UsbController ( UsbStream );
    int StreamIndex = ConvertCOM_UsbStreamIndex( UsbStream );
    int endpoint;
    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if( NULL == State || StreamIndex >= USBH_MAX_QUEUES )
    {
        return 0;
    }

    /* not configured, no data can go in or out */
    if( State->CurrentState != USB_DEVICE_STATE_CONFIGURED )
    {
        return 0;
    }

    endpoint = State->streams[StreamIndex].RxEP;
    // If no Read side to stream (or if not yet open)
    if( endpoint == USB_NULL_ENDPOINT || State->Queues[endpoint] == NULL )
    {
        return 0;
    }

    {
        GLOBAL_LOCK(irq);

        USB_PACKET64* Packet64 = NULL;
        UINT8*        ptr      = (UINT8*)Data;
        UINT32        count    = 0;
        UINT32        remain   = size;

        CPU_USBH_RxEnable( State, endpoint ); // start receiver

        while(count < size)
        {
            UINT32 max_move;

            if(!Packet64) Packet64 = State->Queues[endpoint]->Peek();

            if(!Packet64)
            {
                USBH_ClearEvent( Controller, 1 << endpoint );
                break;
            }

            max_move = Packet64->Size - State->CurrentPacketOffset[endpoint];
            if(remain < max_move) max_move = remain;

            memcpy( ptr, &Packet64->Buffer[ State->CurrentPacketOffset[endpoint] ], max_move );

            State->CurrentPacketOffset[endpoint] += max_move;
            ptr                                  += max_move;
            count                                += max_move;
            remain                               -= max_move;

            /* if we're done with this packet, move onto the next */
            if(State->CurrentPacketOffset[endpoint] == Packet64->Size)
            {
                State->CurrentPacketOffset[endpoint] = 0;

                State->Queues[endpoint]->Pop();

                CPU_USBH_RxEnable( State, endpoint ); // restart receiver
                
                // do not concatenate short packets
                if (Packet64->Size < State->MaxPacketSize[endpoint]) break;
                
                Packet64 = NULL;
            }
        }

        return count;
    }
}

BOOL USBH_Driver::Flush( int UsbStream )
{
    NATIVE_PROFILE_PAL_COM();

    int Controller  = ConvertCOM_UsbController ( UsbStream );
    int StreamIndex = ConvertCOM_UsbStreamIndex( UsbStream );
    int endpoint;
    int retries = USBH_FLUSH_RETRY_COUNT;
    int queueCnt;
    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );

    if( NULL == State || StreamIndex >= USBH_MAX_QUEUES )
    {
        return FALSE;
    }

    /* not configured, no data can go in or out */
    if(State->CurrentState != USB_DEVICE_STATE_CONFIGURED)
    {
        return TRUE;
    }

    endpoint = State->streams[StreamIndex].TxEP;
    // If no Write side to stream (or if not yet open)
    if( endpoint == USB_NULL_ENDPOINT || State->Queues[endpoint] == NULL )
    {
        return FALSE;
    }

    queueCnt = State->Queues[endpoint]->NumberOfElements();
    
    // interrupts were disabled or USB interrupt was disabled for whatever reason, so force the flush
    while(State->Queues[endpoint]->IsEmpty() == false && retries > 0)
    {
        CPU_USBH_StartOutput( State, endpoint );

        HAL_Time_Sleep_MicroSeconds_InterruptEnabled(100); // don't call Events_WaitForEventsXXX because it will turn off interrupts

        int cnt = State->Queues[endpoint]->NumberOfElements();
        retries  = (queueCnt == cnt) ? retries-1: USBH_FLUSH_RETRY_COUNT;
        queueCnt = cnt;
    }

    if(retries <=0)
    {
        State->Queues[endpoint]->Initialize();
    }

    return TRUE;
}

void USBH_Driver::DiscardData( int UsbStream, BOOL fTx )
{
    int Controller  = ConvertCOM_UsbController ( UsbStream );
    int StreamIndex = ConvertCOM_UsbStreamIndex( UsbStream );
    int endpoint;
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );

    if( State == NULL )
        return;

    if( !State->Initialized || State->Configuration == NULL )
        return;

    if(fTx)
    {
        endpoint = State->streams[StreamIndex].TxEP;
    }
    else
    {
        endpoint = State->streams[StreamIndex].RxEP;
    }
    
    // If no Read side to stream (or if not yet open)
    if( endpoint == USB_NULL_ENDPOINT || State->Queues[endpoint] == NULL )
    {
        return;
    }

    if( State->Queues[endpoint] )
    {
        State->Queues[endpoint]->Initialize();
    }
}

UINT32 USBH_Driver::GetEvent( int Controller, UINT32 Mask )
{
    GLOBAL_LOCK(irq);

    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );

    if( State )
        return (State->Event & Mask);
    else
        return 0;
}

UINT32 USBH_Driver::SetEvent( int Controller, UINT32 Event )
{
    GLOBAL_LOCK(irq);

    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );

    if( State == NULL )
        return 0;

    UINT32 OldEvent = State->Event;

    State->Event |= Event;

    if(OldEvent != State->Event)
    {
        Events_Set( SYSTEM_EVENT_FLAG_USB_IN );
    }

//printf("SetEv %d\r\n",State->Event);
    return OldEvent;
}

UINT32 USBH_Driver::ClearEvent( int Controller, UINT32 Event )
{
    GLOBAL_LOCK(irq);

    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );

    if( State == NULL )
        return 0;

    UINT32 OldEvent = State->Event;

    State->Event &= ~Event;

    for( int ctrl = 0; ctrl < TOTAL_USBH_CONTROLLER; ctrl++ ) {
        State = CPU_USBH_GetState( ctrl );
        if (State->Event != 0) return OldEvent;
    }
    // no USB controller has pending events 
    Events_Clear( SYSTEM_EVENT_FLAG_USB_IN );

//printf("ClrEV %d\r\n",State->Event);
    return OldEvent;
}

UINT8 USBH_Driver::GetStatus( int Controller )
{
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );

    if( State == NULL )
        return USB_DEVICE_STATE_NO_CONTROLLER;

    return State->CurrentState;
}

//--//

// Initialize the controller state whenever a device is attached
void USBH_InitializeState( USBH_CONTROLLER_STATE* State )
{
    // Set all streams to unused
    for( int stream = 0; stream < USBH_MAX_QUEUES; stream++ )
    {
        State->streams[stream].RxEP = USB_NULL_ENDPOINT;
        State->streams[stream].TxEP = USB_NULL_ENDPOINT;
    }
    
    // set defaults
    State->Address = 0;          // use default address
    State->MaxPacketSize[0] = 8; // start with 8 byte packets
    State->ConfigError = 0;      // reset configuration error count
    State->Configuration = NULL; // reset device configuration
    State->Request = NULL;       // reset request state
    
}

void USBH_ClearQueues( USBH_CONTROLLER_STATE *State, BOOL ClrRxQueue, BOOL ClrTxQueue )
{
    GLOBAL_LOCK(irq);

    if(ClrRxQueue)
    {
        for(int endpoint = 0; endpoint < USBH_MAX_QUEUES; endpoint++)
        {
            if( State->Queues[endpoint] && !State->IsTxQueue[endpoint] )
                State->Queues[endpoint]->Initialize();
        }
    }

    if( ClrTxQueue )
    {
        for(int endpoint = 0; endpoint < USBH_MAX_QUEUES; endpoint++)
        {
            if( State->Queues[endpoint] && State->IsTxQueue[endpoint] )
                State->Queues[endpoint]->Initialize();
        }
    }
}

void USBH_SetupChannels( USBH_CONTROLLER_STATE* State )
{
    // get endpoints
    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;
    while( USB_NextEndpoint((USB_CONTROLLER_STATE*)State, ep, itfc) )
    {
        CPU_USBH_OpenChannel(State, ep);
    }
}

//--//


// Device Configuration

void USBH_Config1( USBH_CONTROLLER_STATE* State );
void USBH_Config2( int Controller, int result );
void USBH_Config3( int Controller, int result );
void USBH_Config4( USBH_CONTROLLER_STATE* State );
void USBH_Config5( int Controller, int result );
void USBH_Config6( int Controller, int result );
void USBH_Config7( int Controller, int result );
void USBH_Config8( int Controller, int result );
void USBH_Config9( int Controller, int result );
void USBH_Config10( int Controller, int result );
void USBH_Config11( int Controller, int result );
void USBH_Config12( int Controller, int result );
void USBH_Config13( int Controller, int result );
void USBH_Config14( int Controller, int result );

// temporary storage used during device configuration
// rounded up to full words to avoid alignment problems during buffer copy
static BYTE USBH_DeviceDesc[20];
static BYTE USBH_ConfigDesc[12];
static BYTE USBH_ManuString[4];
static BYTE USBH_ProdString[4];
static BYTE USBH_SnumString[4];

// Error check helper
BOOL USBH_Aborted( int Controller, int result )
{
    if (result == USBH_EP_OK) return FALSE;
    
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
    
    UINT8 error = State->ConfigError;
    if (++error < 3) {
        // reset state to Attached to restart device configuration
        State->DeviceState = USB_DEVICE_STATE_ATTACHED;
    } else {
        // give up and set state to Unusable
        State->DeviceState = USB_DEVICE_STATE_UNINITIALIZED;
    }
    USBH_StateCallback(State);
    State->ConfigError = error;
    return TRUE;
}

// Asynchronuous Control request
BOOL USBH_AsynchRequest ( int Controller, const USB_SETUP_PACKET* request, BYTE* data, int length, USBH_RESULT_CALLBACK callback )
{
    NATIVE_PROFILE_PAL_COM();

    USBH_CONTROLLER_STATE * State = CPU_USBH_GetState( Controller );
    
    if (State->Request) { // control state machine is busy
        return FALSE;
    }
    
    State->ControlData = data;
    State->ControlCount = length;
    State->ControlError = 0;
    State->ResultCallback = callback;
    State->Request = request;
    State->Data = data - 1; // prepare data pointer
    
    // start setup transaction
    return CPU_USBH_StartSetup(State);
}

// String read helper
void USBH_ReadString( UINT8 Controller, UINT32 index, BYTE* buffer,
                      UINT32 size, USBH_RESULT_CALLBACK callback)
{
    if (index) {
        USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
        
        if (buffer == NULL) { // store to allocated configuration area
            USB_STRING_DESCRIPTOR_HEADER* string
                = (USB_STRING_DESCRIPTOR_HEADER*)((BYTE*)State->Configuration + State->ConfigOffs);
            
            string->header.marker = USB_STRING_DESCRIPTOR_MARKER;
            string->header.iValue = index;
            string->header.size = sizeof(USB_DESCRIPTOR_HEADER) + size;
            buffer = &string->bLength;
            
            State->ConfigOffs += sizeof(USB_DESCRIPTOR_HEADER) + size;
        }
            
        USBH_GetDescriptorRequest.wValue = USB_STRING_DESCRIPTOR_TYPE << 8 | index;
        USBH_GetDescriptorRequest.wLength = size;
        
        USBH_AsynchRequest(Controller, &USBH_GetDescriptorRequest,
                           buffer, size, callback);
    } else { // no string
        callback(Controller, USBH_EP_OK);
    }
}

// Device configuration starts here

// Read partial Device Descriptor (first 8 bytes)
void USBH_Config1( USBH_CONTROLLER_STATE* State )
{
    USBH_GetDescriptorRequest.wValue = USB_DEVICE_DESCRIPTOR_TYPE << 8;
    USBH_GetDescriptorRequest.wLength = 8;
    
    USBH_AsynchRequest(State->ControllerNum, &USBH_GetDescriptorRequest,
                        USBH_DeviceDesc, 8, USBH_Config2);
}

// Set control channel packet size
// Set device address
void USBH_Config2( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
        
    State->MaxPacketSize[0] = USBH_DeviceDesc[7]; // control channel packet size
    CPU_USBH_SetControl(State); // update max packet size
    
    USBH_AsynchRequest(Controller, &USBH_SetAddressRequest, USBH_DeviceDesc, 0, USBH_Config3);
}

// Set new address
// Wait for device ready
void USBH_Config3( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
        
    State->Address = 1;
    CPU_USBH_SetControl(State); // update device address
    
    // continue after 2ms to give device time to change address
    State->timer.Abort();
    State->timer.InitializeForISR((HAL_CALLBACK_FPN)USBH_Config4, State);
    State->timer.EnqueueDelta(2000); // 2ms
}

// Set device state to Addressed
// Read full Device Descriptor
void USBH_Config4( USBH_CONTROLLER_STATE* State )
{
    State->DeviceState = USB_DEVICE_STATE_ADDRESS;
    USBH_StateCallback(State);
    
    State->ConfigSize = sizeof(USB_DESCRIPTOR_HEADER) + USB_DEVICE_DESCRIPTOR_LENGTH;
    
    USBH_GetDescriptorRequest.wValue = USB_DEVICE_DESCRIPTOR_TYPE << 8;
    USBH_GetDescriptorRequest.wLength = USB_DEVICE_DESCRIPTOR_LENGTH;
    
    USBH_AsynchRequest(State->ControllerNum, &USBH_GetDescriptorRequest,
                        USBH_DeviceDesc, USB_DEVICE_DESCRIPTOR_LENGTH, USBH_Config5);
}

// Read primary Configuration Descriptor
void USBH_Config5( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    USBH_GetDescriptorRequest.wValue = USB_CONFIGURATION_DESCRIPTOR_TYPE << 8;
    USBH_GetDescriptorRequest.wLength = USB_CONFIGURATION_DESCRIPTOR_LENGTH;
    
    USBH_AsynchRequest(Controller, &USBH_GetDescriptorRequest,
                        USBH_ConfigDesc, USB_CONFIGURATION_DESCRIPTOR_LENGTH, USBH_Config6);
}

// Read manufacturer string header
void USBH_Config6( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    int stringIdx = USBH_DeviceDesc[14]; // iManufacturer
    USBH_ManuString[0] = 0;
    
    USBH_ReadString(Controller, stringIdx, USBH_ManuString,
                    USB_STRING_DESCRIPTOR_HEADER_LENGTH, USBH_Config7);
}

// Read product string header
void USBH_Config7( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    int stringIdx = USBH_DeviceDesc[15]; // iProduct
    USBH_ProdString[0] = 0;
        
    USBH_ReadString(Controller, stringIdx, USBH_ProdString,
                    USB_STRING_DESCRIPTOR_HEADER_LENGTH, USBH_Config8);
}

// Read serial number header
void USBH_Config8( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    int stringIdx = USBH_DeviceDesc[16]; // iSerialNumber
    USBH_SnumString[0] = 0;
        
    USBH_ReadString(Controller, stringIdx, USBH_SnumString,
                    USB_STRING_DESCRIPTOR_HEADER_LENGTH, USBH_Config9);
}

// Allocate descriptor space
// Read all Configuration Descriptors
void USBH_Config9( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
        
    UINT32 confDescSize = *(UINT16*)&USBH_ConfigDesc[2]; // wTotalLength
    State->ConfigSize += sizeof(USB_DESCRIPTOR_HEADER) + confDescSize
                       + sizeof(USB_DESCRIPTOR_HEADER) + USBH_ManuString[0] // string size
                       + sizeof(USB_DESCRIPTOR_HEADER) + USBH_ProdString[0] // string size
                       + sizeof(USB_DESCRIPTOR_HEADER) + USBH_SnumString[0] // string size
                       + sizeof(USB_DESCRIPTOR_HEADER); // end marker
    
    if (State->Configuration) private_free((void*)State->Configuration);

    USB_DEVICE_DESCRIPTOR* dev = (USB_DEVICE_DESCRIPTOR*)private_malloc(State->ConfigSize);
    State->Configuration = (USB_DYNAMIC_CONFIGURATION*)dev;
    
    // write device descriptor
    dev->header.marker = USB_DEVICE_DESCRIPTOR_MARKER;
    dev->header.iValue = 0;
    dev->header.size = sizeof(USB_DEVICE_DESCRIPTOR);
    memcpy(&dev->bLength, USBH_DeviceDesc, USB_DEVICE_DESCRIPTOR_LENGTH);
    
    // write configuration header
    USB_CONFIGURATION_DESCRIPTOR* conf =
        (USB_CONFIGURATION_DESCRIPTOR*)((BYTE*)dev + sizeof(USB_DEVICE_DESCRIPTOR));
    conf->header.marker = USB_CONFIGURATION_DESCRIPTOR_MARKER;
    conf->header.iValue = 0;
    conf->header.size = sizeof(USB_DESCRIPTOR_HEADER) + confDescSize;
    
    State->ConfigOffs = sizeof(USB_DEVICE_DESCRIPTOR)
                        + sizeof(USB_DESCRIPTOR_HEADER) + confDescSize;

    // read configuration descriptors
    USBH_GetDescriptorRequest.wValue = USB_CONFIGURATION_DESCRIPTOR_TYPE << 8;
    USBH_GetDescriptorRequest.wLength = confDescSize;
    
    USBH_AsynchRequest(Controller, &USBH_GetDescriptorRequest,
                        &conf->bLength, confDescSize, USBH_Config10);
}

// Read manufacturer string
void USBH_Config10( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    int stringIdx = USBH_DeviceDesc[14]; // iManufacturer
    int stringSize = USBH_ManuString[0]; // bLength
    
    USBH_ReadString(Controller, stringIdx, NULL, stringSize, USBH_Config11);
}

// Read product string
void USBH_Config11( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    int stringIdx = USBH_DeviceDesc[15]; // iProduct
    int stringSize = USBH_ProdString[0]; // bLength
    
    USBH_ReadString(Controller, stringIdx, NULL, stringSize, USBH_Config12);
}

// Read serial number
void USBH_Config12( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    int stringIdx = USBH_DeviceDesc[16]; // iSerialNumber
    int stringSize = USBH_SnumString[0]; // bLength
    
    USBH_ReadString(Controller, stringIdx, NULL, stringSize, USBH_Config13);
}

// Finish configuration set
// Set Device Configuration
void USBH_Config13( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
        
    // add end marker
    USB_DESCRIPTOR_HEADER* end
        = (USB_DESCRIPTOR_HEADER*)((BYTE*)State->Configuration + State->ConfigOffs);
    end->marker = USB_END_DESCRIPTOR_MARKER;
    end->iValue = 0;
    end->size = 0;
    
    USBH_AsynchRequest(Controller, &USBH_SetConfigurationRequest,
                        USBH_DeviceDesc, 0, USBH_Config14);
}

// Check configuration
// Set device state to Configured
void USBH_Config14( int Controller, int result )
{
    if (USBH_Aborted(Controller, result)) return;
    
    USBH_CONTROLLER_STATE *State = CPU_USBH_GetState( Controller );
        
    int err = UsbConfigurationCheck( State->Configuration );
    if( err == USB_CONFIG_ERR_OK ) {
        // set state to Configured
        State->DeviceState = USB_DEVICE_STATE_CONFIGURED;
    } else {
        hal_printf("USB Host Configuration Error %d\r\n", err);
        State->DeviceState = USB_DEVICE_STATE_UNINITIALIZED;
    }
    USBH_StateCallback(State);
}


//--//

UINT8 USBH_ControlCallback( USBH_CONTROLLER_STATE* State, UINT8 result )
{        
    if (result == USBH_EP_OK) { // transaction terminated successfully
        // prepare data packet
        UINT8* data = State->Data;
        if (data < State->ControlData) { // first data packet
            data = State->ControlData;
            State->DataSize = State->MaxPacketSize[0];
        } else {
            data += State->DataSize;  // next data packet
        }
        
        const USB_SETUP_PACKET* request = State->Request;
        UINT8 requestType = request->bmRequestType;
        
        UINT8* end = State->ControlData + State->ControlCount;
        INT32 size = end - data;
        if (size < 0) { // done
            if (requestType == USB_SETUP_DIRECTION_DEVICE | USB_SETUP_RECIPIENT_ENDPOINT
             && request->bRequest == USB_CLEAR_FEATURE) { // clear ep error state
                int ep = request->wIndex & ~USB_ENDPOINT_DIRECTION_IN; // !!!
                State->EndpointStatus[ep] = USBH_EP_OK;
            }
            // terminate transfer
        } else if (size == 0 || State->DataSize < State->MaxPacketSize[0]) { // end of data
            State->ControlCount -= size; // real count
            // send reply
            State->Data = end + 1;
            State->DataSize = 0; // empty packet, reversed direction
            return USB_SETUP_DIRECTION(requestType) ? USBH_TRANSFER_OUT : USBH_TRANSFER_IN;
        } else { // more data
            if (size > State->MaxPacketSize[0]) size = State->MaxPacketSize[0];
            State->Data = data;
            State->DataSize = size;
            return USB_SETUP_DIRECTION(requestType) ? USBH_TRANSFER_IN : USBH_TRANSFER_OUT;
        }
    } else { // transaction aborted
        if (result == USBH_EP_ERROR) { // communication error
            // repeat whole transfer (up to 3 times)
            if (++State->ControlError < 3) {
                State->Data = State->ControlData - 1; // reset state
                CPU_USBH_StartSetup(State); // start setup transaction
                return USBH_TRANSFER_DONE;
            }
            // terminate after 3 tries
        } else { // USBH_EP_STALL (unsupported request)
            // terminate immediately
        }
    }
    
    // inform higher levels
    USBH_RESULT_CALLBACK callback = State->ResultCallback;
    State->ResultCallback = NULL;
    State->Request = NULL;
    State->AsynchResult = result;
    if (callback) (*callback)(State->ControllerNum, result);
    return USBH_TRANSFER_DONE;
}

//--//

void USBH_StateCallback( USBH_CONTROLLER_STATE* State )
{
    UINT8 oldState = State->CurrentState;
    UINT8 newState = State->DeviceState;
    
    if(oldState != newState)
    {
        State->CurrentState = newState;

        switch(newState)
        {
        case USB_DEVICE_STATE_DETACHED:
//            hal_printf("USB_DEVICE_STATE_DETACHED\r\n");
            if (State->Configuration) private_free((void*)State->Configuration);
            State->Configuration = NULL;
            // call DetachedEvent Callback
            
            // Stop device handling in main Thread
            State->timer.Abort();
            State->timer.InitializeForUserMode((HAL_CALLBACK_FPN)MSC_UnmountDevice);
            State->timer.EnqueueDelta(10); // 10us

            break;

        case USB_DEVICE_STATE_ATTACHED:
//            hal_printf("USB_DEVICE_STATE_ATTACHED\r\n");
            USBH_InitializeState(State); // reset host controller state
            // Start device reset after 100ms
            State->timer.Abort();
            State->timer.InitializeForUserMode((HAL_CALLBACK_FPN)CPU_USBH_PortReset, State);
            State->timer.EnqueueDelta(100000); // 100ms
            break;

        case USB_DEVICE_STATE_DEFAULT:
//            hal_printf("USB_DEVICE_STATE_DEFAULT\r\n");

            // Start device configuration after 20ms
            State->timer.Abort();
            State->timer.InitializeForISR((HAL_CALLBACK_FPN)USBH_Config1, State);
            State->timer.EnqueueDelta(20000); // 20ms
            break;

        case USB_DEVICE_STATE_ADDRESS:
//            hal_printf("USB_DEVICE_STATE_ADDRESS\r\n");
            break;

        case USB_DEVICE_STATE_CONFIGURED:
//            hal_printf("USB_DEVICE_STATE_CONFIGURED\r\n");

            if (oldState != USB_DEVICE_STATE_SUSPENDED) {
                /* the device is now ready to communicate */
                /* setup the channels and buffers */
                USBH_SetupChannels( State );
                
                /* initialize all of the Queues */
                USBH_ClearQueues( State, TRUE, TRUE );
                
                // call AttachedEvent Callback
                
                // search for device driver
                
                // Start device handling in main Thread
                State->timer.Abort();
                State->timer.InitializeForUserMode((HAL_CALLBACK_FPN)MSC_MountDevice);
                State->timer.EnqueueDelta(1000); // 1ms

            }
            break;

        case USB_DEVICE_STATE_SUSPENDED:
//            hal_printf("USB_DEVICE_STATE_SUSPENDED\r\n");
            break;

        case USB_DEVICE_STATE_UNINITIALIZED: // configuration failed
//            hal_printf("USB_DEVICE_STATE_UNINITIALIZED\r\n");

            // call ErrorEvent Callback
            break;

        default:
            ASSERT(0);
            break;
        }
    }
}

USB_PACKET64* USBH_RxEnqueue( USBH_CONTROLLER_STATE* State, int endpoint, BOOL& DisableRx )
{
    ASSERT_IRQ_MUST_BE_OFF();
    ASSERT( State && (endpoint < USBH_MAX_QUEUES) );
    ASSERT( State->Queues[endpoint] && !State->IsTxQueue[endpoint] )

    USB_PACKET64* Packet64 = State->Queues[endpoint]->Push();

    DisableRx = State->Queues[endpoint]->IsFull();

    USBH_SetEvent( State->ControllerNum, 1 << endpoint );

    return Packet64;
}

USB_PACKET64* USBH_TxDequeue( USBH_CONTROLLER_STATE* State, int endpoint, BOOL Done )
{
    ASSERT_IRQ_MUST_BE_OFF();
    ASSERT( State && (endpoint < USBH_MAX_QUEUES) );
    ASSERT( State->Queues[endpoint] && State->IsTxQueue[endpoint] )

    if(Done)
    {
        return State->Queues[endpoint]->Pop();
    }
    else
    {
        return State->Queues[endpoint]->Peek();
    }
}

