using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace NXP.Interop
{
    /// <summary>
    /// Class to provide access to the DAC driver from managed code
    /// </summary>
    public class DAC
    {
        /// <summary>
        /// Initializes internal DAC buffers and sets output frequency.
        /// </summary>
        /// <param name="outputFrequency">Sample output frequency (Hz)</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Initialize(Int16 outputFrequency);

        /// <summary>
        /// Reset initial buffers and mark the DAC as disabled
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Uninitialize();

        /// <summary>
        /// Start outputting samples from the internal buffer
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void On();

        /// <summary>
        /// Stop sample output
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Off();

        /// <summary>
        /// Start/stop output from the internal buffer
        /// </summary>
        public extern Boolean Enabled
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }

        /// <summary>
        /// Add a frame (<=0x900 samples) to the internal buffer
        /// </summary>
        /// <param name="Samples">Array of signed 16 bit mono samples to be output</param>
        /// <param name="SamplesNum">Number of samples in Samples</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void AddFrame(Int16[] Samples, Int32 SamplesNum);

        /// <summary>
        /// Add a frame (<=0x900 samples) to the internal buffer
        /// </summary>
        /// <param name="Samples">Byte array of signed 16 bit mono samples to be output</param>
        /// <param name="SamplesNum">Number of samples in Samples</param>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void AddFrame(Byte[] Samples, Int32 SamplesNum);

        /// <summary>
        /// Get samples left in the buffer
        /// </summary>
        /// <returns>Number of samples left in the buffer</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int32 GetBufferLevel();


        /// <summary>
        /// Get frames left in the buffer
        /// </summary>
        /// <returns>Number of frames left in the buffer</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int32 GetFramesLeft();

        /// <summary>
        /// Get interal sample buffer capacity (in frames)
        /// </summary>
        /// <returns>Max number of frames the internal buffer can store</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt32 GetBufferFrameCapacity();
    }
}
