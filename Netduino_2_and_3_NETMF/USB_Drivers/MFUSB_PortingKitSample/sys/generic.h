#ifndef GENERIC_H
#define GENERIC_H

#ifdef __cplusplus
#define GENERICAPI extern "C" 
#else
#define GENERICAPI
#endif

///////////////////////////////////////////////////////////////////////////////

#define SPOT_TAG (ULONG)'topS'

typedef VOID (__stdcall *PQNOTIFYFUNC)(PVOID);

typedef struct _DEVQUEUE {
    LIST_ENTRY      head;
    KSPIN_LOCK      lock;
    PDRIVER_STARTIO StartIo;
    LONG            stallcount;
    PIRP            CurrentIrp;
    KEVENT          evStop;
    PQNOTIFYFUNC    notify;
    PVOID           notifycontext;
    NTSTATUS        abortstatus;
} DEVQUEUE, *PDEVQUEUE;

GENERICAPI VOID     InitializeQueue(PDEVQUEUE pdq, PDRIVER_STARTIO StartIo);
GENERICAPI VOID     StartPacket(PDEVQUEUE pdq, PDEVICE_OBJECT fdo, PIRP Irp, PDRIVER_CANCEL cancel);
GENERICAPI PIRP     StartNextPacket(PDEVQUEUE pdq, PDEVICE_OBJECT fdo);
GENERICAPI VOID     CancelRequest(PDEVQUEUE pdq, PIRP Irp);
GENERICAPI VOID     CleanupRequests(PDEVQUEUE pdq, PFILE_OBJECT fop, NTSTATUS status);
GENERICAPI VOID     StallRequests(PDEVQUEUE pdq);
GENERICAPI VOID     RestartRequests(PDEVQUEUE pdq, PDEVICE_OBJECT fdo);
GENERICAPI PIRP     GetCurrentIrp(PDEVQUEUE pdq);
GENERICAPI BOOLEAN  CheckBusyAndStall(PDEVQUEUE pdq);
GENERICAPI VOID     WaitForCurrentIrp(PDEVQUEUE pdq);
GENERICAPI VOID     AbortRequests(PDEVQUEUE pdq, NTSTATUS status);
GENERICAPI VOID     AllowRequests(PDEVQUEUE pdq);
GENERICAPI NTSTATUS AreRequestsBeingAborted(PDEVQUEUE pdq);
GENERICAPI NTSTATUS StallRequestsAndNotify(PDEVQUEUE pdq, PQNOTIFYFUNC notify, PVOID context);

// Multiple queue routines added in version 1.3:

GENERICAPI VOID     CleanupAllRequests(PDEVQUEUE* q, ULONG nq, PFILE_OBJECT fop, NTSTATUS status);
GENERICAPI VOID     StallAllRequests(PDEVQUEUE* q, ULONG nq);
GENERICAPI VOID     RestartAllRequests(PDEVQUEUE* q, ULONG nq, PDEVICE_OBJECT fdo);
GENERICAPI BOOLEAN  CheckAnyBusyAndStall(PDEVQUEUE* q, ULONG nq, PDEVICE_OBJECT fdo);
GENERICAPI VOID     WaitForCurrentIrps(PDEVQUEUE* q, ULONG nq);
GENERICAPI VOID     AbortAllRequests(PDEVQUEUE* q, ULONG nq, NTSTATUS status);
GENERICAPI VOID     AllowAllRequests(PDEVQUEUE* q, ULONG nq);
GENERICAPI NTSTATUS StallAllRequestsAndNotify(PDEVQUEUE* q, ULONG nq, PQNOTIFYFUNC notify, PVOID context);

///////////////////////////////////////////////////////////////////////////////

// The support routines for remove locking were not originally declared in WDM.H because they
// weren't implemented in Win98 Gold. The following declarations provide equivalent
// functionality on all WDM platforms.

typedef struct _GENERIC_REMOVE_LOCK {
    LONG usage;                    // reference count
    BOOLEAN removing;            // true if removal is pending
    KEVENT evRemove;            // event to wait on
} GENERIC_REMOVE_LOCK, *PGENERIC_REMOVE_LOCK;

GENERICAPI VOID     GenericInitializeRemoveLock(PGENERIC_REMOVE_LOCK lock, ULONG tag, ULONG minutes, ULONG maxcount);
GENERICAPI NTSTATUS GenericAcquireRemoveLock(PGENERIC_REMOVE_LOCK lock, PVOID tag);
GENERICAPI VOID     GenericReleaseRemoveLock(PGENERIC_REMOVE_LOCK lock, PVOID tag);
GENERICAPI VOID     GenericReleaseRemoveLockAndWait(PGENERIC_REMOVE_LOCK lock, PVOID tag);

// NTDDK.H and WDM.H declares the documented support functions as macros which
// we need to redefine.

#if !defined(_NTDDK_)
#error You must include WDM.H or NTDDK.H before GENERIC.H
#endif

#undef IoInitializeRemoveLock
#define IoInitializeRemoveLock GenericInitializeRemoveLock

#undef IoAcquireRemoveLock
#define IoAcquireRemoveLock GenericAcquireRemoveLock

#undef IoReleaseRemoveLock
#define IoReleaseRemoveLock GenericReleaseRemoveLock

#undef IoReleaseRemoveLockAndWait
#define IoReleaseRemoveLockAndWait GenericReleaseRemoveLockAndWait

// NTDDK.H/WDM.H declares an IO_REMOVE_LOCK structure.

#define _IO_REMOVE_LOCK _GENERIC_REMOVE_LOCK
#define IO_REMOVE_LOCK GENERIC_REMOVE_LOCK
#define PIO_REMOVE_LOCK PGENERIC_REMOVE_LOCK

///////////////////////////////////////////////////////////////////////////////

struct _GENERIC_EXTENSION;        // opaque
typedef struct _GENERIC_EXTENSION* PGENERIC_EXTENSION;

typedef NTSTATUS           (__stdcall *PSTART_DEVICE)(PDEVICE_OBJECT, PCM_PARTIAL_RESOURCE_LIST raw, PCM_PARTIAL_RESOURCE_LIST translated);
typedef VOID               (__stdcall *PSTOP_DEVICE)(PDEVICE_OBJECT, BOOLEAN);
typedef VOID               (__stdcall *PREMOVE_DEVICE)(PDEVICE_OBJECT);
typedef BOOLEAN            (__stdcall *PQUERYFUNCTION)(PDEVICE_OBJECT);
typedef BOOLEAN            (__stdcall *PQUERYPOWERFUNCTION)(PDEVICE_OBJECT, DEVICE_POWER_STATE, DEVICE_POWER_STATE);
typedef VOID               (__stdcall *PCONTEXTFUNCTION)(PDEVICE_OBJECT, DEVICE_POWER_STATE, DEVICE_POWER_STATE, PVOID);
typedef VOID               (__stdcall *PFLUSHIOFUNCTION)(PDEVICE_OBJECT, UCHAR, UCHAR, DEVICE_POWER_STATE, DEVICE_POWER_STATE);
typedef DEVICE_POWER_STATE (__stdcall *PGETDSTATEFUNCTION)(PDEVICE_OBJECT, SYSTEM_POWER_STATE, DEVICE_POWER_STATE);

struct QSIO {PDEVQUEUE DeviceQueue; PDRIVER_STARTIO StartIo;};

typedef struct _GENERIC_INIT_STRUCT {
    ULONG                Size;                  // Size of this structure.
    PDEVICE_OBJECT       DeviceObject;          // The device object being registered.
    PDEVICE_OBJECT       Pdo;                   // The PDO below this device object.
    PDEVICE_OBJECT       Ldo;                   // Immediately lower device object.
    PGENERIC_REMOVE_LOCK RemoveLock;            // Address of remove lock in containing device extension (optional).
    PDEVQUEUE            DeviceQueue;           // Address of queue object for read/write IRPs (optional).
    PDRIVER_STARTIO      StartIo;               // Start I/O routine for reads & writes (required only if DeviceQueue non-NULL).
    PSTART_DEVICE        StartDevice;           // Routine to initialize device configuration.
    PSTOP_DEVICE         StopDevice;            // Routine to release device configuration.
    PREMOVE_DEVICE       RemoveDevice;          // Routine to remove device object.
    UNICODE_STRING       DebugName;             // Name to use in debug messages.
    ULONG                Flags;                 // Option flags.
    PQUERYFUNCTION       OkayToStop;            // Routine to decide whether it's okay to stop device now (optional).
    PQUERYFUNCTION       OkayToRemove;          // Routine to decide whether it's okay to remove device now (optional).
    PQUERYPOWERFUNCTION  QueryPower;            // Routine to decide whether a proposed device power change is okay (optional).
    PCONTEXTFUNCTION     SaveDeviceContext;     // Routine to save device context before power down (optional).
    PCONTEXTFUNCTION     RestoreDeviceContext;  // Routine to restore device context after power up (optional).
    DEVICE_POWER_STATE   PerfBoundary;          // (Optional) Power state below which context restore inordinately expensive.
    ULONG                NumberOfQueues;        // Number of device queues in DeviceQueues array (1.3)
    PFLUSHIOFUNCTION     FlushPendingIo;        // Encourage pending I/O to finish (optional) (2.0)
    PGETDSTATEFUNCTION   GetDevicePowerState;   // Get D-state for given S-State (optional) (1.10)
    ULONG Reserved[6];                          // Reserved for future use (1.3)
    struct QSIO Queues[1];                      // (Optional) NumberOfQueues pointers to DEVQUEUE objects and their corresponding StartIo functions.
} GENERIC_INIT_STRUCT, *PGENERIC_INIT_STRUCT;

#define GENERIC_AUTOLAUNCH            0x00000001    // register for AutoLaunch
#define GENERIC_USAGE_PAGING          0x00000002    // device supports DeviceUsageTypePaging
#define GENERIC_USAGE_DUMP            0x00000004    // device supports DeviceUsageTypeDumpFile
#define GENERIC_USAGE_HIBERNATE       0x00000008    // device supports DeviceUsageTypeHibernation
#define GENERIC_PENDING_IOCTLS        0x00000010    // driver may cache asynchronous IOCTLs
#define GENERIC_SURPRISE_REMOVAL_OK   0x00000020    // surprise removal of device is okay
#define GENERIC_IDLE_DETECT           0x00000040    // device supports generic idle detection scheme
#define GENERIC_CLIENT_FLAGS          0x0000007F    // mask to select client-controllable flags

// Enumeration for "wf" argument to GenericWakeupControl:

enum WAKEFUNCTION {
    EnableWakeup,                    // enable system wakeup
    DisableWakeup,                   // disable system wakeup
    ManageWaitWake,                  // request or cancel WAIT_WAKE IRP, as appropriate
    CancelWaitWake,                  // unconditionally cancel WAIT_WAKE
    TestWaitWake,                    // test whether WAIT_WAKE enabled (ver 1.3)
};

GENERICAPI PDEVICE_CAPABILITIES  GenericGetDeviceCapabilities(PGENERIC_EXTENSION pdx);
GENERICAPI VOID     CleanupGenericExtension(PGENERIC_EXTENSION pdx); // 1.3
GENERICAPI NTSTATUS GenericCacheControlRequest(PGENERIC_EXTENSION pdx, PIRP Irp, PIRP* pIrp);
GENERICAPI VOID     GenericCleanupAllRequests(PGENERIC_EXTENSION pdx, PFILE_OBJECT fop, NTSTATUS status);
GENERICAPI VOID     GenericCleanupControlRequests(PGENERIC_EXTENSION pdx, NTSTATUS status, PFILE_OBJECT fop);
GENERICAPI NTSTATUS GenericDeregisterInterface(PGENERIC_EXTENSION pdx, const GUID* guid);
GENERICAPI NTSTATUS GenericDispatchPnp(PGENERIC_EXTENSION, PIRP);
GENERICAPI NTSTATUS GenericDispatchPower(PGENERIC_EXTENSION, PIRP);
GENERICAPI NTSTATUS GenericEnableInterface(PGENERIC_EXTENSION pdx, const GUID* guid, BOOLEAN enable);
GENERICAPI PVOID    GenericGetSystemAddressForMdl(PMDL mdl);
GENERICAPI ULONG    GenericGetVersion(); // 1.3
GENERICAPI NTSTATUS GenericHandlePowerIoctl(PGENERIC_EXTENSION pdx, PIRP Irp);
GENERICAPI NTSTATUS GenericIdleDevice(PGENERIC_EXTENSION pdx, DEVICE_POWER_STATE state, BOOLEAN wait = FALSE);
GENERICAPI VOID     GenericMarkDeviceBusy(PGENERIC_EXTENSION);
GENERICAPI VOID     GenericRegisterForIdleDetection(PGENERIC_EXTENSION, ULONG ConservationTimeout, ULONG PerformanceTimeout, DEVICE_POWER_STATE State);
GENERICAPI NTSTATUS GenericRegisterInterface(PGENERIC_EXTENSION pdx, const GUID* guid);
GENERICAPI VOID     GenericSaveRestoreComplete(PVOID context);
GENERICAPI VOID     GenericSetDeviceState(PGENERIC_EXTENSION pdx, PNP_DEVICE_STATE pnpstate);
GENERICAPI PIRP     GenericUncacheControlRequest(PGENERIC_EXTENSION pdx, PIRP* pIrp);
GENERICAPI NTSTATUS GenericWakeupControl(PGENERIC_EXTENSION pdx, enum WAKEFUNCTION wf);
GENERICAPI NTSTATUS GenericWakeupFromIdle(PGENERIC_EXTENSION pdx, BOOLEAN wait = FALSE);
GENERICAPI ULONG    GetSizeofGenericExtension(VOID);
GENERICAPI NTSTATUS InitializeGenericExtension(PGENERIC_EXTENSION, PGENERIC_INIT_STRUCT);
GENERICAPI BOOLEAN  IsWin98();

#endif

