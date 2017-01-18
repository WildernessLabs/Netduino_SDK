using System;
using Microsoft.SPOT;

namespace NXP.Interop
{
    public class HelixInitException : System.Exception
    {

    }

    public class DACUnresponsiveException : System.Exception
    {
    
    }

    public class DecodingException : System.Exception
    {

    }

	//Unused as of now. I did not remove them just to avoid a firmware signature change
    public class DACBufferFullException : DecodingException
    {

    }

    public class EndOfInBufferException : DecodingException
    {

    }
}
