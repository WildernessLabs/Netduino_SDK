#include "stddcls.h"
#include "genericdefs.h"

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE

GENERICAPI VOID GenericInitializeRemoveLock( PGENERIC_REMOVE_LOCK lock, ULONG tag, ULONG minutes, ULONG maxcount )
{                  
    PAGED_CODE();
    
    KeInitializeEvent(&lock->evRemove, NotificationEvent, FALSE);

    lock->usage    = 1;
    lock->removing = FALSE;
}                  

///////////////////////////////////////////////////////////////////////////////

#pragma LOCKEDCODE

GENERICAPI NTSTATUS GenericAcquireRemoveLock( PGENERIC_REMOVE_LOCK lock, PVOID tag )
{
    InterlockedIncrement( &lock->usage );
    
    if(lock->removing)
    {                   
        if(InterlockedDecrement( &lock->usage ) == 0)
        {
            KeSetEvent( &lock->evRemove, 0, FALSE );
        }
        
        return STATUS_DELETE_PENDING;
    }                   

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE

GENERICAPI VOID GenericReleaseRemoveLock( PGENERIC_REMOVE_LOCK lock, PVOID tag )
{
    if(InterlockedDecrement(&lock->usage) == 0)
    {
        KeSetEvent( &lock->evRemove, 0, FALSE );
    }
}

///////////////////////////////////////////////////////////////////////////////

#pragma PAGEDCODE

GENERICAPI VOID GenericReleaseRemoveLockAndWait( PGENERIC_REMOVE_LOCK lock, PVOID tag )
{
    PAGED_CODE();
    
    lock->removing = TRUE;
    
    GenericReleaseRemoveLock( lock, tag  );
    GenericReleaseRemoveLock( lock, NULL );
    
    KeWaitForSingleObject( &lock->evRemove, Executive, KernelMode, FALSE, NULL );
}

