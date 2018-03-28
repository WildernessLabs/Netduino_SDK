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


#ifndef _SECRETLABS_NETMF_IO_SECRETLABS_NETMF_IO_STORAGEDEVICE_H_
#define _SECRETLABS_NETMF_IO_SECRETLABS_NETMF_IO_STORAGEDEVICE_H_

namespace SecretLabs
{
    namespace NETMF
    {
        namespace IO
        {
            struct StorageDevice
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static void SDSPI_Mount( LPCSTR param0, UINT32 param1, UINT32 param2, UINT32 param3, HRESULT &hr );
                static void SDSPI_Unmount( HRESULT &hr );
            };
        }
    }
}
#endif  //_SECRETLABS_NETMF_IO_SECRETLABS_NETMF_IO_STORAGEDEVICE_H_
