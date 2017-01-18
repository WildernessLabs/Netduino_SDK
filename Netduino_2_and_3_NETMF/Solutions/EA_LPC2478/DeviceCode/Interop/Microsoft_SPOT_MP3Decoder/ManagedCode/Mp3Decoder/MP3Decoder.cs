using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace NXP.Interop
{
    /// <summary>Wrapper class around the Helix MP3 fixed point decoder</summary>
    public class MP3Decoder : IDisposable
    {
  
        /// <summary>
        /// buffer we keep the MP3 raw data in
        /// </summary>
        protected byte[] inBuffer;
        
        /// <summary>
        /// Undecoded bytes in InBuffer
        /// </summary>
        protected Int32 bytesLeft;

        /// <summary>
        /// Next byte to be decoded in InBuffer
        /// </summary>
        protected Int32 readIndex;

        /// <summary>
        /// Finds next syncword in InBuffer, starting at readIndex and updating it and bytesLeft
        /// </summary>
        /// <param name="InBuffer">the MP3 stream to be decoded. We need to pass it although it's a field,
        /// since interop marshalling does not support array fields.</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected extern void FindNextFrameStart(byte[] inBuffer);

        /// <summary>
        /// Decodes the next MP3 frame from inBuffer
        /// </summary>
        /// <param name="InBuffer">the MP3 stream to be decoded. We need to pass it although it's a field,
        /// since interop marshalling does not support array fields.</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected extern void NativeMP3Decode(byte[] inBuffer, ref Int16 framesToDecodedeAtomically);

        /// <summary>
        /// Initialize the Helix decoder
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected extern void InitHelixDecoder();

        /// <summary>
        /// Reset internal status of the Helix decoder
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected extern void UninitHelixDecoder();

        /// <summary>
        /// Initializes inBuffer and the handle to the Helix decoder
        /// </summary>
        /// <param name="InBuffer">the MP3 stream to be decoded</param>
        public MP3Decoder(byte[] mp3InBuffer)
        {
            //Initialize internal counter for the MP3 data buffer
            inBuffer = mp3InBuffer;
            bytesLeft = mp3InBuffer.Length;
            readIndex = 0;
            try
            {
                InitHelixDecoder();
            }
            catch (System.Exception)
            {
                throw new HelixInitException();
            }
        }



        /// <summary>
        /// Actual decoding of MP3 data to the DAC output buffer
        /// </summary>
        /// <param name="MaxFrames">Maximum n. of frames to be decoded in this run.
        /// Less frames could be decoded in case of errors or if we run out of MP3 data: in that case MaxFrames will hold
        /// the number of frames we didn't decode.
        /// </param>

        public void Decode(ref Int16 maxFrames, Int16 targetAtomicDecoding)
        {

            Int16 framesToDecodeAtomically = targetAtomicDecoding;

            while (maxFrames > 0)
            {
               
                //call to the native code that does the real work
                try
                {
                    //reset the counter
                    framesToDecodeAtomically = targetAtomicDecoding;
                    NativeMP3Decode(inBuffer, ref framesToDecodeAtomically);
                    if (framesToDecodeAtomically < 0) //reached the end of inBuffer
                    {
                        return;
                    }
                }
                catch (System.InvalidOperationException)
                {
                    //this means we were not able to pause or restart the DAC output. 
                    //I have to use InvOpExc because of the limited options from native

                    //manage this or change it to something else before rethrowing
                    throw new DACUnresponsiveException();
                }
                finally
                {
                    if(framesToDecodeAtomically >=0 )
                        maxFrames -= (Int16) (targetAtomicDecoding - framesToDecodeAtomically);
                }
            }
        }

        /// <summary>
        /// Overload to allow calling with just maxFrames. After every decoded frame we will allow for rescheduling from the CLR
        /// </summary>
        /// <param name="maxFrames">Maximum n. of frames to be decoded in this run.</param>
        public void Decode(ref Int16 maxFrames)
        {
            Decode(ref maxFrames, 1);
        }


        #region IDisposable Members
        /// <summary>
        /// The Helix decoder needs to be uninitialized
        /// </summary>
        public void Dispose()
        {
            UninitHelixDecoder();
        }

        #endregion
    }
}
