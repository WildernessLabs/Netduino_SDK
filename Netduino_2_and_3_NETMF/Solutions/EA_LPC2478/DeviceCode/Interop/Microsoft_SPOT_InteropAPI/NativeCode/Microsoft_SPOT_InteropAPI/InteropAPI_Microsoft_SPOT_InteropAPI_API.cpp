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


#include "InteropAPI.h"
#include "InteropAPI_Microsoft_SPOT_InteropAPI_API.h"

using namespace Microsoft::SPOT::InteropAPI;

INT32 API::NativeOperation( INT32 firstOperand, INT32 secondOperand, UINT8 opCode, HRESULT &hr )
{
	//param0 = first operand
	//param1 = second operand
	//param2 = operation code, 0 = add, 1 = sub, 2 = mul, 3 = div

	switch (opCode)
	{
		case 0:
			return firstOperand + secondOperand;
			break;
		case 1:
			return firstOperand - secondOperand;
			break;
		case 2:
			return firstOperand * secondOperand;
			break;
		case 3:
			if(secondOperand != 0)
				return (UINT32)(firstOperand/secondOperand);
			hr = CLR_E_INVALID_ARGUMENT;  //this will throw an exception
			break;
		default:
			hr = CLR_E_INVALID_OPERATION; //this will throw an exception
	}

    //we reach this only when we ask for an exception to be generated.
    return 0;
}
