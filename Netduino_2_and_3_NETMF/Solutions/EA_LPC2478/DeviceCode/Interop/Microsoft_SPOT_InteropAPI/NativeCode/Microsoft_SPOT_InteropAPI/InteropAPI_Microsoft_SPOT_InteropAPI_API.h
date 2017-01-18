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


#ifndef _INTEROPAPI_MICROSOFT_SPOT_INTEROPAPI_API_H_
#define _INTEROPAPI_MICROSOFT_SPOT_INTEROPAPI_API_H_

namespace Microsoft
{
    namespace SPOT
    {
        namespace InteropAPI
        {
            struct API
            {
                // Helper Functions to access fields of managed object
                static INT32& Get_result( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_InteropAPI_Microsoft_SPOT_InteropAPI_API::FIELD__result ); }

                static INT8& Get_resultValid( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_InteropAPI_Microsoft_SPOT_InteropAPI_API::FIELD__resultValid ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 NativeOperation( INT32 param0, INT32 param1, UINT8 param2, HRESULT &hr );
            };
        }
    }
}
#endif  //_INTEROPAPI_MICROSOFT_SPOT_INTEROPAPI_API_H_
