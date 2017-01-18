#ifndef DRIVER_H
#define DRIVER_H             

#include "genericdefs.h"
#include "generic.h"
#include "MfUsbUser.h"


#define DRIVERNAME               "MFUSB"
#define LDRIVERNAME             L"MFUSB"

#define FIFO_LEN                (64 * 1024)
#define POLLING_BUFFER_SIZE     (4  * 1024)

///////////////////////////////////////////////////////////////////////////////
// Device extension structure

typedef struct _DEVICE_EXTENSION {
    PDEVICE_OBJECT                DeviceObject;                        // device object this extension belongs to
    PDEVICE_OBJECT                LowerDeviceObject;                   // next lower driver in same stack
    PDEVICE_OBJECT                Pdo;                                 // the PDO
    IO_REMOVE_LOCK                RemoveLock;                          // removal control locking structure
    UNICODE_STRING                operationsInterfaceName;             // name of interface for operations
    UNICODE_STRING                inquiriesInterfaceName;              // name of interface for inquiries 
    DEVQUEUE                      dqWrite;                             // IRP queue
    PGENERIC_EXTENSION            pgx;                                 // device extension for GENERIC.SYS
    LONG                          handles;                             // number open handles
    USB_DEVICE_DESCRIPTOR         dd;                                  // device descriptor
    USBD_CONFIGURATION_HANDLE     hconfig;                             // selected configuration handle
    PUSB_STRING_DESCRIPTOR        devHash;                             // string descriptor
    UNICODE_STRING                manufacturer;                     
    UNICODE_STRING                product;
    UNICODE_STRING                serialNumber;
    UNICODE_STRING                displayName;
    PUSB_CONFIGURATION_DESCRIPTOR pcd;                                 // configuration descriptor
    LANGID                        langid;                              // default language id for strings
    USBD_PIPE_HANDLE              hinpipe;                             // handle of bulk input pipe
    USBD_PIPE_HANDLE              houtpipe;                            // handle of bulk output pipe
    LONG                          inerror;                             // nonzero if had read error
    LONG                          outerror;                            // nonzero if had write error
    ULONG                         maxtransfer;                         // maximum transfer size per URB
    LIST_ENTRY                    ReadIrpList;                         // the list of Read IRPs we have to unblock
    KSPIN_LOCK                    ReadIrpLock;                         // the lock we use to protect ReadIrp
    BOOLEAN                       PollPending;                         // whether or not we have an outstanding read urb
    KSPIN_LOCK                    PollLock;                            // lock to protect polling stuff
    URB                           PollingUrb;                          // the urb we use for polling
    PIRP                          PollingIrp;                          // the irp we use to send the polling urb
    CHAR                          PollingBuffer[POLLING_BUFFER_SIZE];  // the buffer we use for receiving polled data
    CHAR                          Fifo[FIFO_LEN];                      // the buffer for storing data ready to be read by application
    UINT32                        FifoWritePointer;                    
    UINT32                        FifoReadPointer;
    KSPIN_LOCK                    FifoLock;
    PKTHREAD                      RecoveryThread;                      // recovery thread handle
    KEVENT                        RecoveryEvent;                       // event we use to wake up recovery thread
    BOOLEAN                       RecoveryExit;                        // used to terminate recovery thread
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

///////////////////////////////////////////////////////////////////////////////
// Global functions

VOID     RemoveDevice      ( PDEVICE_OBJECT fdo                          );
NTSTATUS SendAwaitUrb      ( PDEVICE_OBJECT fdo, PURB urb                );
VOID     AbortPipe         ( PDEVICE_OBJECT fdo, USBD_PIPE_HANDLE hpipe  );
NTSTATUS ResetPipe         ( PDEVICE_OBJECT fdo, USBD_PIPE_HANDLE hpipe  );
VOID     ResetDevice       ( PDEVICE_OBJECT fdo                          );
NTSTATUS StartDevice       ( PDEVICE_OBJECT fdo, PCM_PARTIAL_RESOURCE_LIST raw, PCM_PARTIAL_RESOURCE_LIST translated);
VOID     StopDevice        ( PDEVICE_OBJECT fdo, BOOLEAN oktouch = FALSE );
VOID     StartIo           ( PDEVICE_OBJECT fdo, PIRP Irp                );
VOID     RecoveryThread    ( PVOID Arg                                   );
VOID     CleanupReads      ( PDEVICE_EXTENSION pdx, PFILE_OBJECT pfo, NTSTATUS status );

NTSTATUS UpdateDeviceInformation( PDEVICE_OBJECT fdo, BOOLEAN Add        );


///////////////////////////////////////////////////////////////////////////////
// I/O request handlers

NTSTATUS DispatchCreate         ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchClose          ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchReadWriteFlush ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchCleanup        ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchInternalControl( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchPower          ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchPnp            ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchWmi            ( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchDeviceIoControl( PDEVICE_OBJECT fdo, PIRP Irp );
NTSTATUS DispatchFlushBuffers   ( PDEVICE_OBJECT fdo, PIRP Irp );

///////////////////////////////////////////////////////////////////////////////
// Helpers

#define INFO_LENGTH 128

//--//

typedef struct _WRCONTEXT {
    URB urb;
    PCHAR data;
    ULONG total;
    ULONG remain;
} RWCONTEXT, *PRWCONTEXT;

//--//

ULONG    GetStatus          ( PDEVICE_OBJECT    fdo                                   );
NTSTATUS OnWriteComplete    ( PDEVICE_OBJECT    fdo, PIRP Irp, PRWCONTEXT ctx         );
NTSTATUS OnPollComplete     ( PDEVICE_OBJECT    fdo, PIRP Irp, PDEVICE_EXTENSION pdx  );
VOID     OnCancelWrite      ( PDEVICE_OBJECT    fdo, IN PIRP Irp                      );
VOID     OnCancelQueuedRead ( PDEVICE_OBJECT    fdo, IN PIRP Irp                      );
VOID     OnCancelFlush      ( PDEVICE_OBJECT    fdo, IN PIRP Irp                      );
NTSTATUS GetStringDescriptor( PDEVICE_OBJECT    fdo, UCHAR istring, PUNICODE_STRING s );
NTSTATUS StartPolling       ( PDEVICE_EXTENSION pdx                                   );
NTSTATUS StopPolling        ( PDEVICE_EXTENSION pdx                                   );
ULONG    CopyFromFifo       ( PDEVICE_EXTENSION pdx, CHAR *Buf, ULONG Len             );
ULONG    CopyToFifo         ( PDEVICE_EXTENSION pdx, CHAR *Buf, ULONG Len             );
ULONG    FifoAvailable      ( PDEVICE_EXTENSION pdx                                   );
VOID     CompleteQueuedReads( PDEVICE_EXTENSION pdx                                   );

extern BOOLEAN win98;
extern UNICODE_STRING servkey;

#endif // DRIVER_H

