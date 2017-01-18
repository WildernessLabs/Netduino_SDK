using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Microsoft.SPOT.InteropAPI
{
    public class API
    {
        private Int32 result;  //TODO: try to access this from native code (which is a static method!)
        private bool resultValid = false;

        public void RequestOp(Int32 a, Int32 b, Byte op)
        {
            try
            {
                result = NativeOperation(a, b, op);
                resultValid = true;
            }
            catch (Exception e)
            {
                resultValid = false;
                Debug.Print("Exception from native code:");
                Debug.Print(e.Message);
            }
        }

        #region Properties: Result, ResultIsValid
        public Int32 Result
        {
            get
            {
                if (!resultValid)
                    throw new InvalidOperationException("Result is not valid");
                resultValid = false;        //a result can be consumed only once, just for fun
                return result;
            }
        }

        public bool ResultIsValid
        {
            get
            {
                return resultValid;
            }
        }
        #endregion

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static Int32 NativeOperation(Int32 a, Int32 b, Byte op);

    }
}
