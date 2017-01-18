#include "stddcls.h"
#include "driver.h"


//--//

#define DisplayNameId          4
#define DeviceId               5
#define LangId                 0x409
#define StringDescriptorBytes  64

#pragma LOCKEDDATA
static PCWSTR EmptyString            = L"";

#pragma LOCKEDDATA
static PCWSTR OperationInterfaceFile = L"\\Operations";

//--//

#pragma LOCKEDCODE
NTSTATUS DispatchCreate( PDEVICE_OBJECT fdo, PIRP Irp )
{
    PIO_STACK_LOCATION stack;
    PDEVICE_EXTENSION  pdx;
    UNICODE_STRING     interfaceName; 
    NTSTATUS           status;
    
    pdx    = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    stack  = IoGetCurrentIrpStackLocation( Irp );
    status = STATUS_SUCCESS;

    RtlInitUnicodeString( &interfaceName, OperationInterfaceFile );
    
    if(0 == RtlCompareUnicodeString( &interfaceName, &stack->FileObject->FileName, TRUE))
    {    
        // we allow only exclusive access to the device 
        if(InterlockedIncrement(&pdx->handles) != 1)
        {
            InterlockedDecrement( &pdx->handles );       
           
            status = STATUS_ACCESS_DENIED;
           
            KdPrint((DRIVERNAME " - ACCESS DENIED\n"));
        }
    }

    return CompleteRequest( Irp, status, 0 );
}

//--//

#pragma LOCKEDCODE
NTSTATUS DispatchClose(PDEVICE_OBJECT fdo, PIRP Irp)
{
    UNICODE_STRING     interfaceName; 
    
    PDEVICE_EXTENSION  pdx   = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation( Irp );

    RtlInitUnicodeString( &interfaceName, OperationInterfaceFile );
    
    if(0 == RtlCompareUnicodeString( &interfaceName, &stack->FileObject->FileName, TRUE))
    {    
        InterlockedDecrement( &pdx->handles );
    }

    KdPrint((DRIVERNAME " - handles after close: '%d'\n", pdx->handles));
        
    return CompleteRequest( Irp, STATUS_SUCCESS, 0 );
}

//--//

#pragma LOCKEDCODE
VOID CleanupReads( PDEVICE_EXTENSION pdx, PFILE_OBJECT pfo, NTSTATUS status )
{
    PIO_STACK_LOCATION ReadStack;
    PLIST_ENTRY        head, next, current;
    LIST_ENTRY         CancelList;
    PIRP               ReadIrp;
    KIRQL              OldIrql;
    
    // manually cancel all pending reads that match this file handle
    InitializeListHead( &CancelList );

    KeAcquireSpinLock (&pdx->ReadIrpLock, &OldIrql );

    head = &pdx->ReadIrpList;

    for( next=head->Flink; next != head; )
    {
        ReadIrp   = CONTAINING_RECORD( next, IRP, Tail.Overlay.ListEntry );
        ReadStack = IoGetCurrentIrpStackLocation( ReadIrp );

        // advance next pointer now, as we want to muck with it
        current = next;
        next    = next->Flink;

        // skip IRPs that do not match
        // if pfo == NULL, then don't skip anything
        if(pfo && pfo != ReadStack->FileObject )
        {
            KdPrint((DRIVERNAME " - Skipping IRP, it belongs to a different handle\n"));
            continue;
        }

        // add it to the cancel list 
        RemoveEntryList( current              );
        InsertTailList ( &CancelList, current );
    }

    KeReleaseSpinLock( &pdx->ReadIrpLock, OldIrql );

    // now complete the IRPs we cancelled 
    while (!IsListEmpty(&CancelList))
    {
        next    = RemoveHeadList( &CancelList );
        ReadIrp = CONTAINING_RECORD(next, IRP, Tail.Overlay.ListEntry);
        
        IoSetCancelRoutine ( ReadIrp, NULL             );
        IoReleaseRemoveLock( &pdx->RemoveLock, ReadIrp );
        CompleteRequest    ( ReadIrp, status, 0        );
    }
}

//--//

#pragma LOCKEDCODE
NTSTATUS DispatchCleanup(PDEVICE_OBJECT fdo, PIRP Irp)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

    // library will take care of writes for us 
    CleanupRequests(&pdx->dqWrite, stack->FileObject, STATUS_CANCELLED);

    // manually clean up our reads
    CleanupReads(pdx, stack->FileObject, STATUS_CANCELLED);

    return CompleteRequest(Irp, STATUS_SUCCESS, 0);
}

//--//

#pragma LOCKEDCODE
NTSTATUS DispatchReadWriteFlush(PDEVICE_OBJECT fdo, PIRP Irp)
{
    KIRQL OldIrql;
    ULONG count;
    
    PDEVICE_EXTENSION  pdx   = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation( Irp );

    if(stack->MajorFunction == IRP_MJ_WRITE)
    {
        if(stack->Parameters.Write.Length == 0)
        {
            return CompleteRequest( Irp, STATUS_SUCCESS, 0 );
        }

        // write 
        IoMarkIrpPending(Irp);
        
        StartPacket( &pdx->dqWrite, fdo, Irp, OnCancelWrite );
        
    }
    else if(stack->MajorFunction == IRP_MJ_READ) 
    {
        if(stack->Parameters.Read.Length == 0)
        {
            return CompleteRequest( Irp, STATUS_SUCCESS, 0 );
        }
        
        // read 
        IoAcquireRemoveLock( &pdx->RemoveLock, Irp   );
        IoMarkIrpPending   ( Irp                     );
        IoSetCancelRoutine ( Irp, OnCancelQueuedRead );
        
        KeAcquireSpinLock( &pdx->ReadIrpLock, &OldIrql                     );
        InsertTailList   ( &pdx->ReadIrpList, &Irp->Tail.Overlay.ListEntry );
        KeReleaseSpinLock( &pdx->ReadIrpLock, OldIrql                      );

        CompleteQueuedReads(pdx);
    } 
    else 
    {
        // flush
        IoMarkIrpPending( Irp );
        StartPacket     ( &pdx->dqWrite, fdo, Irp, OnCancelWrite );
    }

    return STATUS_PENDING;
}

//--//

#pragma LOCKEDCODE
NTSTATUS DispatchDeviceIoControl( PDEVICE_OBJECT fdo, PIRP Irp )
{
    ULONG    info;
    NTSTATUS status = STATUS_SUCCESS;
    
    PDEVICE_EXTENSION  pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);

    status = IoAcquireRemoveLock( &pdx->RemoveLock, Irp );
    
    if(!NT_SUCCESS(status))
    {
        return CompleteRequest(Irp, status, 0);
    }

    info = 0;
    
    switch(stack->Parameters.DeviceIoControl.IoControlCode)
    {
        case IOCTL_MFUSB_PORT_NAME:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < pdx->operationsInterfaceName.Length)
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = pdx->operationsInterfaceName.Length;

                    if(pdx->operationsInterfaceName.Buffer != NULL)
                    {
                        RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, pdx->operationsInterfaceName.Buffer, info);
                    }
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_MANUFACTURER:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < pdx->manufacturer.Length)
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = pdx->manufacturer.Length;

                    if(pdx->manufacturer.Buffer != NULL)
                    {
                        RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, pdx->manufacturer.Buffer, info);
                    }
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_PRODUCT:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < pdx->product.Length)
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = pdx->product.Length;
                    
                    if(pdx->product.Buffer != NULL)
                    {
                        RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, pdx->product.Buffer, info);
                    }
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_SERIAL_NUMBER:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < pdx->serialNumber.Length)
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = pdx->serialNumber.Length;

                    if(pdx->serialNumber.Buffer != NULL)
                    {
                        RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, pdx->serialNumber.Buffer, info);
                    }
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_DISPLAY_NAME:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < pdx->displayName.Length)
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = pdx->displayName.Length;

                    if(pdx->displayName.Buffer != NULL)
                    {
                        RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, pdx->displayName.Buffer, info);
                    }
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_DEVICE_HASH:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < pdx->devHash->bLength)
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = pdx->devHash->bLength;

                    if(info > 0)
                    {
                        RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &pdx->devHash->bString, info);
                    }
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_READ_AVAILABLE:
            {
                ULONG available = FifoAvailable(pdx);
                
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(ULONG))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = sizeof(ULONG);
                    
                    RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &available, sizeof(ULONG));
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_VENDOR_ID:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(USHORT))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = sizeof(USHORT);
                    
                    RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &pdx->dd.idVendor, info);
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        case IOCTL_MFUSB_PRODUCT_ID:
            {
                if(stack->Parameters.DeviceIoControl.OutputBufferLength < sizeof(USHORT))
                {
                    status = STATUS_BUFFER_TOO_SMALL;
                } 
                else 
                {
                    info = sizeof(USHORT);
                    
                    RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &pdx->dd.idProduct, info);
                    
                    status = STATUS_SUCCESS;
                }
            }
            break;
            
        default:
            KdPrint((DRIVERNAME " - Unrecognized IOCTL\n"));
            
            status = STATUS_INVALID_PARAMETER;
            
            break;
    }

    IoReleaseRemoveLock( &pdx->RemoveLock, Irp );

    if(!NT_SUCCESS(status))
    {
        info = 0;
    }

    return CompleteRequest( Irp, status, info );
}

//--//

#pragma LOCKEDCODE
VOID OnCancelWrite(IN PDEVICE_OBJECT fdo, IN PIRP Irp)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    
    CancelRequest( &pdx->dqWrite, Irp );
    
    IoReleaseRemoveLock( &pdx->RemoveLock, Irp );
}

//--//

#pragma LOCKEDCODE
VOID OnCancelQueuedRead( IN PDEVICE_OBJECT fdo, IN PIRP Irp )
{
    KIRQL             OldIrql;
    
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    // remove IRP from queue 
    KeAcquireSpinLock(&pdx->ReadIrpLock, &OldIrql);
    
    if(&Irp->Tail.Overlay.ListEntry != NULL)
    {
        RemoveEntryList( &Irp->Tail.Overlay.ListEntry );
    }
    KeReleaseSpinLock( &pdx->ReadIrpLock, OldIrql );

    // drop back down to normal IRQL 
    IoReleaseCancelSpinLock(Irp->CancelIrql);

    // balance out remove lock from DispatchReadWriteFlush 
    IoReleaseRemoveLock( &pdx->RemoveLock, Irp );
    
    // give IRP back to the io manager 
    CompleteRequest( Irp, STATUS_CANCELLED, 0 );
}

//--//

#pragma LOCKEDCODE
NTSTATUS SendAwaitUrb( PDEVICE_OBJECT fdo, PURB urb )
{
    IO_STATUS_BLOCK iostatus;
    KEVENT          event;
    NTSTATUS        status;                
    
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    KeInitializeEvent( &event, NotificationEvent, FALSE );

    PIRP Irp = IoBuildDeviceIoControlRequest(
                                              IOCTL_INTERNAL_USB_SUBMIT_URB,
                                              pdx->LowerDeviceObject, 
                                              NULL, 
                                              0, 
                                              NULL, 
                                              0, 
                                              TRUE, 
                                              &event, 
                                              &iostatus
                                            );

    if(!Irp) 
    {
        KdPrint((DRIVERNAME " - Unable to allocate IRP for sending URB\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    PIO_STACK_LOCATION stack           = IoGetNextIrpStackLocation(Irp);
    stack->Parameters.Others.Argument1 = (PVOID)urb;
    status                             = IoCallDriver(pdx->LowerDeviceObject, Irp);

    if(status == STATUS_PENDING)
    {
        KeWaitForSingleObject( &event, Executive, KernelMode, FALSE, NULL );
        status = iostatus.Status;
    }

    return status;
}

//--//
 
#pragma LOCKEDCODE
NTSTATUS UpdateDeviceInformation( PDEVICE_OBJECT fdo )
{
    PUSB_STRING_DESCRIPTOR desc;
    UNICODE_STRING         dummy;
    URB                    urb;    
    int                    id;      
    NTSTATUS               status;
    
    KdPrint((DRIVERNAME " - UpdateDeviceInformation\n"));

    status = STATUS_SUCCESS;

    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    // initialize buffers
    pdx->manufacturer.Buffer = NULL;
    pdx->product     .Buffer = NULL;
    pdx->serialNumber.Buffer = NULL;
    pdx->displayName .Buffer = NULL;
    desc                     = NULL;
    
    // allocate buffers for all strings
    pdx->manufacturer.Buffer = (WCHAR*)ExAllocatePoolWithTag(NonPagedPool, StringDescriptorBytes, SPOT_TAG );
    pdx->product     .Buffer = (WCHAR*)ExAllocatePoolWithTag(NonPagedPool, StringDescriptorBytes, SPOT_TAG );
    pdx->serialNumber.Buffer = (WCHAR*)ExAllocatePoolWithTag(NonPagedPool, StringDescriptorBytes, SPOT_TAG );
    pdx->displayName .Buffer = (WCHAR*)ExAllocatePoolWithTag(NonPagedPool, StringDescriptorBytes, SPOT_TAG );

    if( 
        (pdx->manufacturer.Buffer == NULL) ||
        (pdx->product     .Buffer == NULL) ||
        (pdx->serialNumber.Buffer == NULL) ||
        (pdx->displayName .Buffer == NULL)   
      )
    {
        KdPrint((DRIVERNAME " - Unable to allocate memory for device info\n"));        
        status =  STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }
    
    pdx->manufacturer.Length        = 0;
    pdx->manufacturer.MaximumLength = StringDescriptorBytes;    
    pdx->product     .Length        = 0;
    pdx->product     .MaximumLength = StringDescriptorBytes;    
    pdx->serialNumber.Length        = 0;
    pdx->serialNumber.MaximumLength = StringDescriptorBytes;    
    pdx->displayName .Length        = 0;
    pdx->displayName .MaximumLength = StringDescriptorBytes;
    
    // allocate buffer for device string descriptor requests
    desc = (PUSB_STRING_DESCRIPTOR)ExAllocatePoolWithTag(NonPagedPool, sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes, SPOT_TAG );
    
    if(!desc)
    {
        KdPrint((DRIVERNAME " - Unable to allocate %X bytes for string descriptor\n", sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes));
        status =  STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }

    
    if(0 != pdx->dd.iManufacturer)
    {
        // Manufacturer
        UsbBuildGetDescriptorRequest(&urb, 
                                 sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                 USB_STRING_DESCRIPTOR_TYPE,
                                 pdx->dd.iManufacturer,
                                 LangId, 
                                 desc, 
                                 NULL, 
                                 sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes,
                                 NULL);
        
        status = SendAwaitUrb(fdo, &urb);

        if(NT_SUCCESS(status))
        {            
            desc->bString[ (desc->bLength / 2) - 1 ] = L'\0';

            RtlInitUnicodeString( &dummy            , desc->bString );
            RtlCopyUnicodeString( &pdx->manufacturer, &dummy        );
        }
        else
        {
            RtlInitUnicodeString( &dummy            , EmptyString );
            RtlCopyUnicodeString( &pdx->manufacturer, &dummy      );
            KdPrint(( DRIVERNAME " - could not retrieve manufacturer %08x\n", status ));                
        }
    }

    if(0 != pdx->dd.iProduct)
    {
        // Product
        UsbBuildGetDescriptorRequest(&urb, 
                                 sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                 USB_STRING_DESCRIPTOR_TYPE,
                                 pdx->dd.iProduct,
                                 LangId, 
                                 desc, 
                                 NULL, 
                                 sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes,
                                 NULL);
        
        status = SendAwaitUrb(fdo, &urb);

        if(NT_SUCCESS(status))
        {            
            desc->bString[ (desc->bLength / 2) - 1 ] = L'\0';

            RtlInitUnicodeString( &dummy       , desc->bString );
            RtlCopyUnicodeString( &pdx->product, &dummy        );
            
        }
        else
        {
            RtlInitUnicodeString( &dummy       , EmptyString );
            RtlCopyUnicodeString( &pdx->product, &dummy      );       
            KdPrint(( DRIVERNAME " - could not retrieve product %08x\n", status ));          
        }
    }

    if(0 != pdx->dd.iSerialNumber)
    {
        // Serial Number
        UsbBuildGetDescriptorRequest(&urb, 
                                 sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                 USB_STRING_DESCRIPTOR_TYPE,
                                 pdx->dd.iSerialNumber,
                                 LangId, 
                                 desc, 
                                 NULL, 
                                 sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes,
                                 NULL);
        
        status = SendAwaitUrb(fdo, &urb);

        if(NT_SUCCESS(status))
        {            
            desc->bString[ (desc->bLength / 2) - 1 ] = L'\0';
            
            RtlInitUnicodeString( &dummy            , desc->bString );
            RtlCopyUnicodeString( &pdx->serialNumber, &dummy        );
            
        }
        else
        {          
            RtlInitUnicodeString( &dummy            , EmptyString );
            RtlCopyUnicodeString( &pdx->serialNumber, &dummy      );
            KdPrint(( DRIVERNAME " - could not retrieve serial number %08x\n", status ));            
        }
    }
    
    // DisplayName
    UsbBuildGetDescriptorRequest(&urb, 
                             sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                             USB_STRING_DESCRIPTOR_TYPE,
                             DisplayNameId,
                             LangId, 
                             desc, 
                             NULL, 
                             sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes,
                             NULL);
    
    status = SendAwaitUrb(fdo, &urb);

    if(NT_SUCCESS(status))
    {            
        desc->bString[ (desc->bLength / 2) - 1 ] = L'\0';
        

        RtlInitUnicodeString( &dummy           , desc->bString );
        RtlCopyUnicodeString( &pdx->displayName, &dummy        );
        
    }
    else
    {
        RtlInitUnicodeString( &dummy            , EmptyString );
        RtlCopyUnicodeString( &pdx->displayName, &dummy      );
        KdPrint(( DRIVERNAME " - could not retrieve display name %08x\n", status ));    
    }

cleanup:
    
    if(desc) ExFreePool( desc );
    
    if(!NT_SUCCESS(status))
    {
        if(pdx->manufacturer.Buffer) ExFreePool( pdx->manufacturer.Buffer );
        if(pdx->product     .Buffer) ExFreePool( pdx->product     .Buffer );
        if(pdx->serialNumber.Buffer) ExFreePool( pdx->serialNumber.Buffer );
        if(pdx->displayName .Buffer) ExFreePool( pdx->displayName .Buffer );

        pdx->manufacturer.Buffer = NULL;
        pdx->product     .Buffer = NULL;
        pdx->serialNumber.Buffer = NULL;
        pdx->displayName .Buffer = NULL;       
    }
    
    return status;
}

//--//

NTSTATUS StartDevice( PDEVICE_OBJECT fdo, PCM_PARTIAL_RESOURCE_LIST raw, PCM_PARTIAL_RESOURCE_LIST translated )
{
    USB_CONFIGURATION_DESCRIPTOR  tcd;
    PUSB_CONFIGURATION_DESCRIPTOR pcd;
    PUSB_STRING_DESCRIPTOR        desc;            
    HANDLE                        RecoveryHandle;
    PURB                          selurb;
    URB                           urb;     
    NTSTATUS                      status;

    status = STATUS_SUCCESS; 
    
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    selurb = NULL;
    pcd    = NULL;

    // Read our device descriptor. The only real purpose to this would be to find out how many
    // configurations there are so we can read their descriptors. In this simplest of examples,
    // there's only one configuration.
    
    KdPrint((DRIVERNAME " - Start device\n"));

    UsbBuildGetDescriptorRequest(
                                  &urb, 
                                  sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                  USB_DEVICE_DESCRIPTOR_TYPE,
                                  0, 
                                  LangId, 
                                  &pdx->dd, 
                                  NULL, 
                                  sizeof(pdx->dd), 
                                  NULL
                                );
 
    status = SendAwaitUrb( fdo, &urb );
    
    if(!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME " - Error %X trying to retrieve device descriptor\n", status));
        goto cleanup;
    }

    // allocate the buffer for the descriptor; take extra space to null terminate the bString member
    desc = (PUSB_STRING_DESCRIPTOR)ExAllocatePoolWithTag(NonPagedPool, sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes, SPOT_TAG );

    if(!desc)
    {
        KdPrint((DRIVERNAME " - Unable to allocate %X bytes for string descriptor\n", sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes));
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }
    
    pdx->devHash = desc;

    UsbBuildGetDescriptorRequest(&urb, 
                                 sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                 USB_STRING_DESCRIPTOR_TYPE,
                                 DeviceId, 
                                 LangId, 
                                 desc, 
                                 NULL, 
                                 sizeof(USB_STRING_DESCRIPTOR) + StringDescriptorBytes,
                                 NULL);
    
    status = SendAwaitUrb(fdo, &urb);
    
    if(!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME " - Error %X trying to retrieve string descriptor for DeviceId\n", status));
        goto cleanup;
    }
        
    // null terminate the buffer; we allocated one more wchar_t for the purpose 
    desc->bString[ (desc->bLength / 2) - 1 ] = L'\0';

    UpdateDeviceInformation( fdo );

    // Read the descriptor of the first configuration. This requires two steps. The first step
    // reads the fixed-size configuration descriptor alone. The second step reads the
    // configuration descriptor plus all imbedded interface and endpoint descriptors.

    UsbBuildGetDescriptorRequest(&urb, 
                                 sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                 USB_CONFIGURATION_DESCRIPTOR_TYPE,
                                 0, 
                                 LangId, 
                                 &tcd, 
                                 NULL, 
                                 sizeof(tcd), 
                                 NULL);

    status = SendAwaitUrb(fdo, &urb);
    
    if(!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME " - Error %X trying to read configuration descriptor 1\n", status));
        goto cleanup;
    }

    ULONG size = tcd.wTotalLength;
    
    pcd = (PUSB_CONFIGURATION_DESCRIPTOR) ExAllocatePoolWithTag(NonPagedPool, size, SPOT_TAG);
    
    if(!pcd)
    {
        KdPrint((DRIVERNAME " - Unable to allocate %X bytes for configuration descriptor\n", size));
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }

    UsbBuildGetDescriptorRequest(&urb, 
                                 sizeof(_URB_CONTROL_DESCRIPTOR_REQUEST), 
                                 USB_CONFIGURATION_DESCRIPTOR_TYPE,
                                 0, 
                                 LangId, 
                                 pcd, 
                                 NULL, 
                                 size, 
                                 NULL);

    status = SendAwaitUrb(fdo, &urb);
    if(!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME " - Error %X trying to read configuration descriptor 1\n", status));
        goto cleanup;
    }
                           
    // Locate the descriptor for the one and only interface we expect to find

    PUSB_INTERFACE_DESCRIPTOR pid = USBD_ParseConfigurationDescriptorEx(pcd, 
                                                                        pcd,
                                                                        -1, 
                                                                        -1, 
                                                                        -1, 
                                                                        -1, 
                                                                        -1);

    ASSERT(pid);
                           
    // Create a URB to use in selecting a configuration.

    USBD_INTERFACE_LIST_ENTRY interfaces[2] = {
        {pid, NULL},
        {NULL, NULL},        // fence to terminate the array
    };

    selurb = USBD_CreateConfigurationRequestEx(pcd, interfaces);
    if(!selurb)
    {
        KdPrint((DRIVERNAME " - Unable to create configuration request\n"));
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }

    // Verify that the interface describes exactly the endpoints we expect
    if(pid->bNumEndpoints != 2)
    {
        KdPrint((DRIVERNAME " - %d is the wrong number of endpoints\n", pid->bNumEndpoints));
        status = STATUS_DEVICE_CONFIGURATION_ERROR;
        goto cleanup;
    }

    PUSB_ENDPOINT_DESCRIPTOR ped = (PUSB_ENDPOINT_DESCRIPTOR) pid;
    ped = (PUSB_ENDPOINT_DESCRIPTOR) USBD_ParseDescriptors(pcd, tcd.wTotalLength, ped, USB_ENDPOINT_DESCRIPTOR_TYPE);
    if(!ped || 0 == (ped->bEndpointAddress & 0x80) || ped->bmAttributes != USB_ENDPOINT_TYPE_BULK || ped->wMaxPacketSize > 64)
    {
        KdPrint((DRIVERNAME " - Endpoint has wrong attributes\n"));
        status = STATUS_DEVICE_CONFIGURATION_ERROR;
        goto cleanup;
    }
    ++ped;
    if(!ped || 0 != (ped->bEndpointAddress & 0x80) || ped->bmAttributes != USB_ENDPOINT_TYPE_BULK || ped->wMaxPacketSize > 64)
    {
        KdPrint((DRIVERNAME " - Endpoint has wrong attributes\n"));
        status = STATUS_DEVICE_CONFIGURATION_ERROR;
        goto cleanup;
    }
    ++ped;

    PUSBD_INTERFACE_INFORMATION pii = interfaces[0].Interface;
    ASSERT(pii->NumberOfPipes == pid->bNumEndpoints);

    // Initialize the maximum transfer size for each of the endpoints. The
    // default would be PAGE_SIZE. The firmware itself only has a 4096-byte
    // ring buffer, though. We need to restrict the test applet to that many
    // bytes. In order to exercise the multi-segment aspect of the transfer code,
    // therefore, reduce the maximum transfer size to 1024 bytes.

    pii->Pipes[0].MaximumTransferSize = USBD_DEFAULT_MAXIMUM_TRANSFER_SIZE;
    pii->Pipes[1].MaximumTransferSize = USBD_DEFAULT_MAXIMUM_TRANSFER_SIZE;
    pdx->maxtransfer = USBD_DEFAULT_MAXIMUM_TRANSFER_SIZE;

    // Submit the set-configuration request

    status = SendAwaitUrb(fdo, selurb);
    if(!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME " - Error %X trying to select configuration\n", status));
        goto cleanup;
    }

    // Save the configuration and pipe handles
    pdx->hconfig = selurb->UrbSelectConfiguration.ConfigurationHandle;
    pdx->hinpipe = pii->Pipes[0].PipeHandle;
    pdx->houtpipe = pii->Pipes[1].PipeHandle;

    // Transfer ownership of the configuration descriptor to the device extension

    pdx->pcd = pcd;
    pcd = NULL;

    // Enable the interface
    IoSetDeviceInterfaceState(&pdx->operationsInterfaceName, TRUE);        

    // Enable the interface
    IoSetDeviceInterfaceState(&pdx->inquiriesInterfaceName, TRUE);                    

    // create recovery thread                     
    status = PsCreateSystemThread(&RecoveryHandle, 0, NULL, NULL, NULL, RecoveryThread, (PVOID)pdx);

    if(!NT_SUCCESS(status))
    {
        KdPrint((DRIVERNAME " - PsCreateSystemThread failed with error %08x\n", status));
        goto cleanup;
    }
    
    status = ObReferenceObjectByHandle( RecoveryHandle,
                                        SYNCHRONIZE,
                                        NULL,
                                        KernelMode,
                                        (PVOID*)&pdx->RecoveryThread,
                                        NULL );

    ASSERT(NT_SUCCESS(status));

    ZwClose(RecoveryHandle);
    
    // Start polling
    status = StartPolling(pdx);
    
    if(!NT_SUCCESS(status))
    {   
        KdPrint((DRIVERNAME " - StartPolling failed 0x%08x\n", status));

        if(pdx->RecoveryThread)
        {
            // shutdown recovery thread
            pdx->RecoveryExit = TRUE; 
            KeSetEvent(&pdx->RecoveryEvent, 0, FALSE);    
            // wait for polling thread to exit 
            KeWaitForSingleObject(pdx->RecoveryThread, Executive, KernelMode, FALSE, NULL);     
            ObDereferenceObject(pdx->RecoveryThread);
            pdx->RecoveryThread = NULL;
        }
        
        goto cleanup;
    }
    
cleanup:
    if(selurb) ExFreePool(selurb);
    if(pcd   ) ExFreePool(pcd   );

    // get rid of return codes like STATUS_PENDING
    if(NT_SUCCESS(status)) 
    {
        return STATUS_SUCCESS;
    }
    
    return status;
}

//--//

#pragma LOCKEDCODE
NTSTATUS OnWriteComplete(PDEVICE_OBJECT fdo, PIRP Irp, PRWCONTEXT ctx)
{   
    PIO_STACK_LOCATION stack;
    NTSTATUS           status;
    ULONG              length;
    KIRQL              OldIrql;
    ULONG              count;
    
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    status = Irp->IoStatus.Status;
    
    if(NT_SUCCESS(status) && ctx->remain && !Irp->Cancel)
    {
        /* we have more to transfer */
        length = ctx->remain;
        if(length > pdx->maxtransfer)
            length = pdx->maxtransfer;
        
        UsbBuildInterruptOrBulkTransferRequest(&ctx->urb,
                                               sizeof(_URB_BULK_OR_INTERRUPT_TRANSFER),
                                               pdx->houtpipe,
                                               ctx->data,
                                               NULL,
                                               length,
                                               (USBD_TRANSFER_DIRECTION_OUT | USBD_SHORT_TRANSFER_OK),
                                               NULL);

        ctx->data += length;
        ctx->remain -= length;
        
        stack = IoGetNextIrpStackLocation(Irp);
        stack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
        stack->Parameters.Others.Argument1 = (PVOID)&ctx->urb;
        stack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;

        IoSetCompletionRoutine(Irp, 
                               (PIO_COMPLETION_ROUTINE)OnWriteComplete,
                               (PVOID) ctx,
                               TRUE,
                               TRUE,
                               TRUE);

        IoCallDriver(pdx->LowerDeviceObject, Irp);

        return STATUS_MORE_PROCESSING_REQUIRED;
    }
    
    if(NT_SUCCESS(status)) 
    {
        Irp->IoStatus.Information = ctx->total;
    } 
    else 
    {
        KdPrint((DRIVERNAME " - finished with error!\n"));
    }
    
    ExFreePool(ctx);

    StartNextPacket    ( &pdx->dqWrite, fdo    );
    IoReleaseRemoveLock( &pdx->RemoveLock, Irp );
    
    return STATUS_SUCCESS;
}

//--//

#pragma LOCKEDCODE
VOID StartIo( PDEVICE_OBJECT fdo, PIRP Irp )
{
    PIO_STACK_LOCATION stack;
    PRWCONTEXT         ctx;
    ULONG              length, i;
    KIRQL              OldIrql;    
    NTSTATUS           status;

    if((Irp == NULL) || (Irp->AssociatedIrp.SystemBuffer == NULL))
    {
        CompleteRequest(Irp, STATUS_INVALID_PARAMETER, 0);
        return;        
    }
    
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    status = IoAcquireRemoveLock(&pdx->RemoveLock, Irp);

    if(!NT_SUCCESS(status))
    {
        CompleteRequest(Irp, status, 0);
        return;
    }

    stack = IoGetCurrentIrpStackLocation(Irp);

    if(stack->MajorFunction == IRP_MJ_WRITE)
    {
        // start a write operation 
        ctx = (PRWCONTEXT) ExAllocatePoolWithTag(NonPagedPool, sizeof(RWCONTEXT), SPOT_TAG);
    
        if(ctx == NULL)
        {
            KdPrint((DRIVERNAME " - Could not allocate transfer context\n"));
            
            StartNextPacket    ( &pdx->dqWrite, fdo                    );
            CompleteRequest    ( Irp, STATUS_INSUFFICIENT_RESOURCES, 0 );
            IoReleaseRemoveLock( &pdx->RemoveLock, Irp                 );
            
            return;
        }

        RtlZeroMemory(ctx, sizeof(RWCONTEXT));

        ctx->total = ctx->remain = stack->Parameters.Write.Length;
        ctx->data  = (PCHAR)Irp->AssociatedIrp.SystemBuffer;
        
        length = ctx->remain;
        if(length > pdx->maxtransfer)
            length = pdx->maxtransfer;

        UsbBuildInterruptOrBulkTransferRequest(&ctx->urb,
                                               sizeof(_URB_BULK_OR_INTERRUPT_TRANSFER),
                                               pdx->houtpipe,
                                               ctx->data,
                                               NULL,
                                               length,
                                               (USBD_TRANSFER_DIRECTION_OUT | USBD_SHORT_TRANSFER_OK),
                                               NULL);

        ctx->data += length;
        ctx->remain -= length;
        
        stack = IoGetNextIrpStackLocation(Irp);
        stack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
        stack->Parameters.Others.Argument1 = (PVOID)&ctx->urb;
        stack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;

        IoSetCompletionRoutine(Irp, 
                               (PIO_COMPLETION_ROUTINE)OnWriteComplete,
                               (PVOID) ctx,
                               TRUE,
                               TRUE,
                               TRUE);

        IoCallDriver(pdx->LowerDeviceObject, Irp);
    }
    else 
    {
        // flush 
        KdPrint((DRIVERNAME " - Flushed!\n"));
        
        StartNextPacket    ( &pdx->dqWrite, fdo     );
        CompleteRequest    ( Irp, STATUS_SUCCESS, 0 );
        IoReleaseRemoveLock( &pdx->RemoveLock, Irp  );
    }

    return;
}

//--//

#pragma LOCKEDCODE
VOID StopDevice(IN PDEVICE_OBJECT fdo, BOOLEAN oktouch /* = FALSE */)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    
    // Stop polling
    StopPolling(pdx);

    // Disable the operational interface
    IoSetDeviceInterfaceState(&pdx->operationsInterfaceName, FALSE);

    // Disable the inquiry interface
    IoSetDeviceInterfaceState(&pdx->inquiriesInterfaceName, FALSE);
    
    // wait for polling thread to exit 
    if(pdx->RecoveryThread)
    { 
        // stop recovery thread
        pdx->RecoveryExit = TRUE;
        KeSetEvent(&pdx->RecoveryEvent, 0, FALSE);
        
        KeWaitForSingleObject(pdx->RecoveryThread, Executive, KernelMode, FALSE, NULL);    

        ObDereferenceObject(pdx->RecoveryThread);

        pdx->RecoveryThread = NULL;
    }

    // If it's okay to touch our hardware (i.e., we're processing an IRP_MN_STOP_DEVICE),
    // deconfigure the device.
    if(oktouch) 
    {
        URB urb;
        
        UsbBuildSelectConfigurationRequest(&urb, sizeof(_URB_SELECT_CONFIGURATION), NULL);

        NTSTATUS status = SendAwaitUrb(fdo, &urb);

        if(!NT_SUCCESS(status))
        {
            KdPrint((DRIVERNAME " - Error %X trying to deconfigure device\n", status));
        }
    }

    if(pdx->pcd)
    {
        ExFreePool(pdx->pcd);
        
        pdx->pcd = NULL;
    }
}

//--//

#pragma LOCKEDCODE
NTSTATUS StartPolling(PDEVICE_EXTENSION pdx)
{
    PIO_STACK_LOCATION stack;
    KIRQL              OldIrql;
    NTSTATUS           status;

    ASSERT(pdx->PollingIrp);

    KeAcquireSpinLock(&pdx->PollLock, &OldIrql);
    
    if(pdx->PollPending)
    {
        KeReleaseSpinLock(&pdx->PollLock, OldIrql);
        return STATUS_SUCCESS;
    }

    KeReleaseSpinLock(&pdx->PollLock, OldIrql);

    status = IoAcquireRemoveLock(&pdx->RemoveLock, pdx->PollingIrp);
    
    if(!NT_SUCCESS(status))
    {
        KeAcquireSpinLock(&pdx->PollLock, &OldIrql);
        pdx->PollPending = FALSE;
        KeReleaseSpinLock(&pdx->PollLock, OldIrql);
        
        return status;
    }
  
    // safe to do since we passed in FALSE for the second parameter
    // of IoAllocateIrp
    IoInitializeIrp(pdx->PollingIrp, sizeof(IRP), pdx->LowerDeviceObject->StackSize);

    IoSetCompletionRoutine(
                            pdx->PollingIrp,
                            (PIO_COMPLETION_ROUTINE) OnPollComplete,
                            pdx,
                            TRUE,
                            TRUE,
                            TRUE
                          );

    stack = IoGetNextIrpStackLocation(pdx->PollingIrp);

    memset(&pdx->PollingUrb, 0, sizeof(URB));
    
    UsbBuildInterruptOrBulkTransferRequest(
                                            &pdx->PollingUrb,
                                            sizeof(_URB_BULK_OR_INTERRUPT_TRANSFER),
                                            pdx->hinpipe,
                                            &pdx->PollingBuffer[0],
                                            NULL,
                                            POLLING_BUFFER_SIZE,
                                            (USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK),
                                            NULL
                                          );

    stack->MajorFunction                            = IRP_MJ_INTERNAL_DEVICE_CONTROL;
    stack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;
    stack->Parameters.Others.Argument1              = &pdx->PollingUrb;

    pdx->PollPending = TRUE;
    
    return IoCallDriver(pdx->LowerDeviceObject, pdx->PollingIrp);
}

//--//

#pragma LOCKEDCODE
NTSTATUS StopPolling(PDEVICE_EXTENSION pdx)
{
    PIO_STACK_LOCATION stack;
    PLIST_ENTRY        ListEntry;
    KIRQL              OldIrql;
    BOOLEAN            Pending;
    PIRP               ReadIrp;
    
    KeAcquireSpinLock(&pdx->PollLock, &OldIrql);
    Pending = pdx->PollPending;
    KeReleaseSpinLock(&pdx->PollLock, OldIrql);

    if(Pending)
    {
        IoCancelIrp(pdx->PollingIrp);
    }
    
    // finish any outstanding reads 
    KeAcquireSpinLock(&pdx->ReadIrpLock, &OldIrql);

    while(!IsListEmpty(&pdx->ReadIrpList))
    {
        ListEntry = RemoveHeadList(&pdx->ReadIrpList);
        KeReleaseSpinLock(&pdx->ReadIrpLock, OldIrql);
        
        ReadIrp = CONTAINING_RECORD(ListEntry, IRP, Tail.Overlay.ListEntry);

        IoSetCancelRoutine(ReadIrp, NULL);
        IoReleaseRemoveLock(&pdx->RemoveLock, ReadIrp);
        CompleteRequest(ReadIrp, STATUS_NO_SUCH_DEVICE, 0);

        KeAcquireSpinLock(&pdx->ReadIrpLock, &OldIrql);
    }
    KeReleaseSpinLock(&pdx->ReadIrpLock, OldIrql);

    // clear the read fifo 
    KeAcquireSpinLock(&pdx->FifoLock, &OldIrql);
    pdx->FifoReadPointer = pdx->FifoWritePointer = 0;
    KeReleaseSpinLock(&pdx->FifoLock, OldIrql);

    return STATUS_SUCCESS;
}

//--//

#pragma LOCKEDCODE
UINT32 GetPortStatus(PDEVICE_EXTENSION pdx)
{
    KEVENT event;
    IO_STATUS_BLOCK iostatus;
    PIRP Irp;
    ULONG result;
    NTSTATUS status;
    PIO_STACK_LOCATION stack;

    Irp = IoBuildDeviceIoControlRequest(
                                         IOCTL_INTERNAL_USB_GET_PORT_STATUS,
                                         pdx->LowerDeviceObject,
                                         NULL,
                                         0,
                                         NULL,
                                         0,
                                         TRUE,
                                         &event,
                                         &iostatus
                                       );
    
    stack = IoGetNextIrpStackLocation(Irp);
    
    stack->Parameters.Others.Argument1 = (PVOID)&status;
    
    status = IoCallDriver(pdx->LowerDeviceObject, Irp);

    if(STATUS_PENDING == status)
    {
        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
        
        status = iostatus.Status;
    }

    return status;
}

//--//

#pragma LOCKEDCODE
VOID RecoveryThread(PVOID Arg)
{
    PIO_STACK_LOCATION stack;
    IO_STATUS_BLOCK    IoStatus;
    KEVENT             Event;
    PIRP               Irp;
    NTSTATUS           status;

    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION)Arg;

    while(1)
    {
        // the main part of the driver will signal us when we need to fix things
        KeWaitForSingleObject(&pdx->RecoveryEvent, Executive, KernelMode, FALSE, NULL);
    
        if(pdx->RecoveryExit) break;
        
        KeInitializeEvent(&Event, SynchronizationEvent, FALSE);

        // reset our device
        Irp = IoBuildDeviceIoControlRequest(
                                             IOCTL_INTERNAL_USB_RESET_PORT,
                                             pdx->LowerDeviceObject,
                                             NULL,
                                             0,
                                             NULL,
                                             0,
                                             TRUE,
                                             &Event,
                                             &IoStatus
                                           );

        stack = IoGetNextIrpStackLocation(Irp);
        
        status = IoCallDriver(pdx->LowerDeviceObject, Irp);

        if(STATUS_PENDING == status)
        {
            KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
            
            status = IoStatus.Status;
        }

        // start polling again
        StartPolling(pdx);
    }

    PsTerminateSystemThread(STATUS_SUCCESS);
}

//--//

#pragma LOCKEDCODE
NTSTATUS OnPollComplete(PDEVICE_OBJECT fdo, PIRP Irp, PDEVICE_EXTENSION pdx)
{
    PIO_STACK_LOCATION stack;
    KIRQL              OldIrql;
    PIRP               ReadIrp;
    ULONG              len, count;
    NTSTATUS           status;

    stack = IoGetNextIrpStackLocation(Irp);

    KeAcquireSpinLock(&pdx->PollLock, &OldIrql);
    pdx->PollPending = FALSE;
    KeReleaseSpinLock(&pdx->PollLock, OldIrql);

    if(NT_SUCCESS(Irp->IoStatus.Status)){
        len = pdx->PollingUrb.UrbBulkOrInterruptTransfer.TransferBufferLength;
        
        // Data comes into the driver here, where we write it into the fifo.
        // The fifo will complete queued read IRPs if needed 
        count = CopyToFifo(pdx, &pdx->PollingBuffer[0], len);

        CompleteQueuedReads(pdx);

        if(FALSE == Irp->Cancel)
        {
            // if we haven't been cancelled then keep going
            status = StartPolling(pdx);
        }
    } else {
        // fail current outstanding reads
        CleanupReads(pdx, NULL, Irp->IoStatus.Status);
        
        // wake up the recovery thread
        KeSetEvent(&pdx->RecoveryEvent, 0, FALSE);
    }

    IoReleaseRemoveLock(&pdx->RemoveLock, Irp);

    // tell the io manager not to perform default handling of irp 
    return STATUS_MORE_PROCESSING_REQUIRED;
}

//--//

#pragma LOCKEDCODE
VOID CompleteQueuedReads(PDEVICE_EXTENSION pdx)
{
    PIO_STACK_LOCATION stack;
    PLIST_ENTRY ListEntry;
    KIRQL       OldIrql;
    PIRP        ReadIrp;
    ULONG       requested, got, available;
    NTSTATUS    status;

    // we have at least one read to complete 
    available = FifoAvailable(pdx);

    while(available)
    {
        KeAcquireSpinLock(&pdx->ReadIrpLock, &OldIrql);
        if(IsListEmpty(&pdx->ReadIrpList))
        {
            KeReleaseSpinLock(&pdx->ReadIrpLock, OldIrql);
            break;
        }
        ListEntry = RemoveHeadList(&pdx->ReadIrpList);
        KeReleaseSpinLock(&pdx->ReadIrpLock, OldIrql);

        ReadIrp   = CONTAINING_RECORD(ListEntry, IRP, Tail.Overlay.ListEntry);
        stack     = IoGetCurrentIrpStackLocation(ReadIrp);
        requested = stack->Parameters.Read.Length;


        if(ReadIrp->AssociatedIrp.SystemBuffer)
        {
            got    = CopyFromFifo(pdx, (CHAR *)ReadIrp->AssociatedIrp.SystemBuffer, requested);
            status = STATUS_SUCCESS;
        }
        else
        {
            got    = 0;
            status = STATUS_INVALID_PARAMETER;
        }
        
        IoSetCancelRoutine(ReadIrp, NULL);
        
        IoReleaseRemoveLock(&pdx->RemoveLock, ReadIrp);
        CompleteRequest(ReadIrp, status, got);

        available = FifoAvailable(pdx);
    }
}

//--//

#pragma LOCKEDCODE
ULONG CopyToFifo(PDEVICE_EXTENSION pdx, CHAR *Buf, ULONG Len)
{
    KIRQL OldIrql;
    ULONG i, next;

    KeAcquireSpinLock(&pdx->FifoLock, &OldIrql);
    for(i=0; i<Len; i++)
    {
        next = pdx->FifoWritePointer + 1;
        
        if(next >= FIFO_LEN) next = 0;

        if(next == pdx->FifoReadPointer) break;

        pdx->Fifo[pdx->FifoWritePointer] = Buf[i];
        
        pdx->FifoWritePointer = next;
    }
    KeReleaseSpinLock(&pdx->FifoLock, OldIrql);
  
    return i;
}

//--//

#pragma LOCKEDCODE
ULONG CopyFromFifo(PDEVICE_EXTENSION pdx, CHAR *Buf, ULONG Len)
{
    KIRQL OldIrql;
    ULONG i;

    KeAcquireSpinLock(&pdx->FifoLock, &OldIrql);
    for(i=0; i<Len; i++)
    {
        if(pdx->FifoReadPointer == pdx->FifoWritePointer) break;
        
        Buf[i] = pdx->Fifo[pdx->FifoReadPointer++];
        
        if(pdx->FifoReadPointer >= FIFO_LEN)
        {
            pdx->FifoReadPointer = 0;
        }
    }
    KeReleaseSpinLock(&pdx->FifoLock, OldIrql);

    return i;
}

//--//

#pragma LOCKEDCODE
ULONG FifoAvailable(PDEVICE_EXTENSION pdx)
{
    KIRQL OldIrql;
    ULONG len, i;

    KeAcquireSpinLock(&pdx->FifoLock, &OldIrql);
    
    if(pdx->FifoReadPointer <= pdx->FifoWritePointer)
    {
        len = (pdx->FifoWritePointer - pdx->FifoReadPointer);
    }
    else 
    {
        len = ((FIFO_LEN - pdx->FifoReadPointer) + pdx->FifoWritePointer);
    }

    KeReleaseSpinLock(&pdx->FifoLock, OldIrql);

    return len;
}

