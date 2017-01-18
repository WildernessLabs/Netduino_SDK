//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
//-----------------------------------------------------------------------------


#ifndef _DAC_NXP_INTEROP_DAC_H_
#define _DAC_NXP_INTEROP_DAC_H_

namespace NXP
{
    namespace Interop
    {
        struct DAC
        {
            // Helper Functions to access fields of managed object
            // Declaration of stubs. These functions are implemented by Interop code developers
            static void Initialize( CLR_RT_HeapBlock* pMngObj, INT16 param0, HRESULT &hr );
            static void Uninitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void On( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void Off( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 get_Enabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void set_Enabled( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr );
            static void AddFrame( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, INT32 param1, HRESULT &hr );
            static void AddFrame( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, HRESULT &hr );
            static INT32 GetBufferLevel( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT32 GetFramesLeft( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static UINT32 GetBufferFrameCapacity( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        };
    }
}
#endif  //_DAC_NXP_INTEROP_DAC_H_
