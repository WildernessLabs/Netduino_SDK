#include "stddcls.h"
#include "driver.h"

NTSTATUS AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT pdo);
VOID DriverUnload(IN PDRIVER_OBJECT fdo);

static PCWSTR OperationInterface     = L"Operations";
static PCWSTR OperationInterfaceAux  = L"OperationsAux";
static PCWSTR InquiryInterface       = L"Inquiries";

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,
                                IN PUNICODE_STRING RegistryPath)
{
    KdPrint((DRIVERNAME " - Entering DriverEntry: DriverObject %8.8lX\n", DriverObject));

    // Insist that OS support at least the WDM level of the DDK we use

    if(!IoIsWdmVersionAvailable(1, 0))
    {
        KdPrint((DRIVERNAME " - Expected version of WDM (%d.%2.2d) not available\n", 1, 0));
        return STATUS_UNSUCCESSFUL;
    }

    // See if we're running under Win98 or NT:

    win98 = IsWin98();

    if(win98)
        KdPrint((DRIVERNAME " - Running under Windows 98\n"));
    else
        KdPrint((DRIVERNAME " - Running under NT\n"));

    // Initialize function pointers
    DriverObject->DriverUnload = DriverUnload;
    DriverObject->DriverExtension->AddDevice = AddDevice;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    DriverObject->MajorFunction[IRP_MJ_READ] = DispatchReadWriteFlush;
    DriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchReadWriteFlush;
    DriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = DispatchReadWriteFlush;
    DriverObject->MajorFunction[IRP_MJ_CLEANUP] = DispatchCleanup;
    DriverObject->MajorFunction[IRP_MJ_POWER] = DispatchPower;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = DispatchInternalControl;
    DriverObject->MajorFunction[IRP_MJ_PNP] = DispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = DispatchWmi;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceIoControl;

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();
    KdPrint((DRIVERNAME " - Entering DriverUnload: DriverObject %8.8lX\n", DriverObject));
}

///////////////////////////////////////////////////////////////////////////////
NTSTATUS AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT pdo)
{
    PAGED_CODE();

    UNICODE_STRING interfaceName;

    KdPrint((DRIVERNAME " - Entering AddDevice: DriverObject %8.8lX, pdo %8.8lX\n", DriverObject, pdo));

    NTSTATUS status;

    // Create a function device object to represent the hardware we're managing.

    PDEVICE_OBJECT fdo;
    
    ULONG dxsize = (sizeof(DEVICE_EXTENSION) + 7) & ~7;
    ULONG xsize = dxsize + GetSizeofGenericExtension();
    
    status = IoCreateDevice(DriverObject, 
                            xsize, 
                            NULL,
                            FILE_DEVICE_UNKNOWN, 
                            FILE_DEVICE_SECURE_OPEN, 
                            FALSE, 
                            &fdo);
    
    if(!NT_SUCCESS(status)){
        KdPrint((DRIVERNAME " - IoCreateDevice failed - %X\n", status));
        return status;
    }
    
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;

    pdx->OperationsAux.operationsInterfaceName.Buffer = NULL;
    pdx->Operations   .operationsInterfaceName.Buffer = NULL;
    
    pdx->inquiriesInterfaceName.Buffer = NULL;

    RtlInitUnicodeString( &interfaceName, OperationInterface );
    
    status = IoRegisterDeviceInterface(pdo,
                                       &MfUsbGuidOperations,
                                       &interfaceName,
                                       &pdx->Operations.operationsInterfaceName);

    if(!NT_SUCCESS(status)){
        KdPrint((DRIVERNAME " - IoRegisterDeviceInterface failed for operations interface - %X\n", status));
        return status;
    }

    RtlInitUnicodeString( &interfaceName, OperationInterfaceAux );
    
    status = IoRegisterDeviceInterface(pdo,
                                       &MfUsbGuidOperationsAux,
                                       &interfaceName,
                                       &pdx->OperationsAux.operationsInterfaceName);

    if(!NT_SUCCESS(status)){
        KdPrint((DRIVERNAME " - IoRegisterDeviceInterface failed for operations AUX interface - %X\n", status));
        return status;
    }

    RtlInitUnicodeString( &interfaceName, InquiryInterface );
    
    status = IoRegisterDeviceInterface(pdo,
                                       &MfUsbGuidInquiries,
                                       &interfaceName,
                                       &pdx->inquiriesInterfaceName);

    if(!NT_SUCCESS(status)){
        KdPrint((DRIVERNAME " - IoRegisterDeviceInterface failed for inquiry interface - %X\n", status));
        return status;
    }

    // From this point forward, any error will have side effects that need to
    // be cleaned up. Using a do-once allows us to modify the program
    // easily without losing track of the side effects.

    do {
        pdx->DeviceObject = fdo;
        pdx->Pdo = pdo;

        pdx->Operations   .handles      = 0;
        pdx->OperationsAux.handles      = 0;
        pdx->devHash                    = NULL;        
        pdx->RecoveryThread             = NULL;
        pdx->Operations   .PollingIrp   = NULL;
        pdx->OperationsAux.PollingIrp   = NULL;
        pdx->LowerDeviceObject          = NULL;

        pdx->manufacturer.Buffer        = NULL;      
        pdx->product     .Buffer        = NULL;      
        pdx->serialNumber.Buffer        = NULL;      
        pdx->displayName .Buffer        = NULL;                     
        
        // Declare the buffering method we'll use for read/write requests
        fdo->Flags |= DO_BUFFERED_IO;

        // Initialize irp related locks
        KeInitializeSpinLock(&pdx->OperationsAux.ReadIrpLock);
        KeInitializeSpinLock(&pdx->Operations   .ReadIrpLock);
        KeInitializeSpinLock(&pdx->OperationsAux.PollLock);
        KeInitializeSpinLock(&pdx->Operations   .PollLock);

        // Initialize queued read list
        InitializeListHead(&pdx->Operations   .ReadIrpList);
        InitializeListHead(&pdx->OperationsAux.ReadIrpList);
        
        // Initialize fifo
        KeInitializeSpinLock(&pdx->OperationsAux.FifoLock);
        KeInitializeSpinLock(&pdx->Operations   .FifoLock);
        pdx->OperationsAux.FifoReadPointer = pdx->OperationsAux.FifoWritePointer = 0;
        pdx->Operations   .FifoReadPointer = pdx->Operations   .FifoWritePointer = 0;

        // Link our device object into the stack leading to the PDO
        pdx->LowerDeviceObject = IoAttachDeviceToDeviceStack(fdo, pdo);
        if(!pdx->LowerDeviceObject) {
            KdPrint((DRIVERNAME " - IoAttachDeviceToDeviceStack failed\n"));
            status = STATUS_DEVICE_REMOVED;
            break;
        }

        // allocate IRP for polling
        pdx->Operations   .PollingIrp = IoAllocateIrp(pdx->LowerDeviceObject->StackSize, FALSE);
        pdx->OperationsAux.PollingIrp = IoAllocateIrp(pdx->LowerDeviceObject->StackSize, FALSE);

        if(!pdx->Operations.PollingIrp || !pdx->OperationsAux.PollingIrp) {
            KdPrint((DRIVERNAME " - IoAllocateIrp failed\n"));
            status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        // Set power management flags in the device object
        fdo->Flags |= DO_POWER_PAGABLE;

        // Initialize to use the GENERIC.SYS library
        pdx->pgx = (PGENERIC_EXTENSION) ((PUCHAR) pdx + dxsize);

        GENERIC_INIT_STRUCT gis = {sizeof(GENERIC_INIT_STRUCT)};
        gis.DeviceObject = fdo;
        gis.Pdo = pdo;
        gis.Ldo = pdx->LowerDeviceObject;
        gis.RemoveLock = &pdx->RemoveLock;
        gis.StartDevice = StartDevice;
        gis.StopDevice = StopDevice;
        gis.RemoveDevice = RemoveDevice;
        gis.StartIo = StartIo;
        gis.DeviceQueue = &pdx->dqWrite;
        RtlInitUnicodeString(&gis.DebugName, LDRIVERNAME);
        gis.Flags = GENERIC_SURPRISE_REMOVAL_OK;

        status = InitializeGenericExtension(pdx->pgx, &gis);
        if(!NT_SUCCESS(status)) {
            KdPrint((DRIVERNAME " - InitializeGenericExtension failed - %X\n", status));
            break;
        }

        // Clear the "initializing" flag so that we can get IRPs
        fdo->Flags &= ~DO_DEVICE_INITIALIZING;

        // Set up recovery thread
        pdx->RecoveryExit = FALSE;
        KeInitializeEvent(&pdx->RecoveryEvent   , SynchronizationEvent, FALSE);
        KeInitializeEvent(&pdx->RecoveryEventAux, SynchronizationEvent, FALSE);

    } while (FALSE);

    if(!NT_SUCCESS(status)) 
    {
        if(pdx->Operations.PollingIrp)
        {
            IoFreeIrp(pdx->Operations.PollingIrp);

            pdx->Operations.PollingIrp = NULL;
        }

        if(pdx->OperationsAux.PollingIrp)
        {
            IoFreeIrp(pdx->OperationsAux.PollingIrp);

            pdx->OperationsAux.PollingIrp = NULL;
        }

        if(pdx->Operations.operationsInterfaceName.Buffer)
        {
            RtlFreeUnicodeString(&pdx->Operations.operationsInterfaceName);
    
            pdx->Operations.operationsInterfaceName.Buffer = NULL;
        }


        if(pdx->OperationsAux.operationsInterfaceName.Buffer)
        {
            RtlFreeUnicodeString(&pdx->OperationsAux.operationsInterfaceName);
    
            pdx->OperationsAux.operationsInterfaceName.Buffer = NULL;
        }

        if(pdx->inquiriesInterfaceName.Buffer)
        {
            RtlFreeUnicodeString(&pdx->inquiriesInterfaceName);
    
            pdx->inquiriesInterfaceName.Buffer = NULL;
        }

        if(pdx->LowerDeviceObject)
        {
            IoDetachDevice(pdx->LowerDeviceObject);

            pdx->LowerDeviceObject = NULL;
        }
        
        IoDeleteDevice(fdo);
    }

    return status;
}

///////////////////////////////////////////////////////////////////////////////
// Pass-through dispatch routine for IRP_MJ_INTERNAL_DEVICE_CONTROL. This allows
// an upper filter to talk to the device, should it want to.

#pragma LOCKEDCODE
NTSTATUS DispatchInternalControl(PDEVICE_OBJECT fdo, PIRP Irp)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    NTSTATUS status = IoAcquireRemoveLock(&pdx->RemoveLock, Irp);
    if(!NT_SUCCESS(status))
        return CompleteRequest(Irp, status, 0);
    IoSkipCurrentIrpStackLocation(Irp);
    status = IoCallDriver(pdx->LowerDeviceObject, Irp);
    IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
    return status;
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE
NTSTATUS DispatchPnp(PDEVICE_OBJECT fdo, PIRP Irp)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    return GenericDispatchPnp(pdx->pgx, Irp);
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE
NTSTATUS DispatchPower(PDEVICE_OBJECT fdo, PIRP Irp)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    return GenericDispatchPower(pdx->pgx, Irp);
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE
NTSTATUS DispatchWmi(PDEVICE_OBJECT fdo, PIRP Irp)
{
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    IoSkipCurrentIrpStackLocation(Irp);
    return IoCallDriver(pdx->LowerDeviceObject, Irp);
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE
VOID RemoveDevice(IN PDEVICE_OBJECT fdo)
{    
    PAGED_CODE();
    PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fdo->DeviceExtension;
    NTSTATUS status;

    KdPrint((DRIVERNAME " - Remove device...\n"));
    
    if(pdx->LowerDeviceObject)
    {
        IoDetachDevice(pdx->LowerDeviceObject);
        
        pdx->LowerDeviceObject = NULL;
    }
    
    if(pdx->devHash)
    {
        ExFreePool(pdx->devHash);

        pdx->devHash = NULL;
    }
    
    if(pdx->manufacturer.Buffer)
    {
        ExFreePool(pdx->manufacturer.Buffer);

        pdx->manufacturer.Buffer = NULL;
    }
    
    if(pdx->product.Buffer)
    {
        ExFreePool(pdx->product.Buffer);

        pdx->product.Buffer = NULL;
    }
    
    if(pdx->serialNumber.Buffer)
    {
        ExFreePool(pdx->serialNumber.Buffer);

        pdx->serialNumber.Buffer = NULL;
    }
    
    if(pdx->displayName.Buffer)
    {
        ExFreePool(pdx->displayName.Buffer);

        pdx->displayName.Buffer = NULL;
    }
    
    if(pdx->Operations.operationsInterfaceName.Buffer)
    {
        RtlFreeUnicodeString(&pdx->Operations.operationsInterfaceName);

        pdx->Operations.operationsInterfaceName.Buffer = NULL;
    }
    
    if(pdx->OperationsAux.operationsInterfaceName.Buffer)
    {
        RtlFreeUnicodeString(&pdx->OperationsAux.operationsInterfaceName);

        pdx->OperationsAux.operationsInterfaceName.Buffer = NULL;
    }
    
    if(pdx->inquiriesInterfaceName.Buffer)
    {
        RtlFreeUnicodeString(&pdx->inquiriesInterfaceName);

        pdx->inquiriesInterfaceName.Buffer = NULL;
    }
    
    if(pdx->Operations.PollingIrp)
    {
        IoFreeIrp(pdx->Operations.PollingIrp);
        
        pdx->Operations.PollingIrp = NULL;
    }

    
    if(pdx->OperationsAux.PollingIrp)
    {
        IoFreeIrp(pdx->OperationsAux.PollingIrp);
        
        pdx->OperationsAux.PollingIrp = NULL;
    }

    IoDeleteDevice(fdo);
}
