////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace TemperatureSample
{
    /// <summary>
    /// The SPI temperature sensor driver.
    /// </summary>
    class SpiTemperatureSensor : IDisposable
    {
        /// <summary>
        /// Keep this private member around as the SPI object.
        /// </summary>
        private SPI _spi;

        /// <summary>
        /// The default constructor.
        /// </summary>
        public SpiTemperatureSensor()
        {
            // Get a new SPI object that is connected to the temperature sensor
            _spi = new SPI(new SPI.Configuration((Cpu.Pin)5, true, 0, 0, false,
                false, 4000, SPI.SPI_module.SPI1));
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                // dispose managed resources
                if (_spi != null)
                {
                    _spi.Dispose();
                    _spi = null;
                }
            }
            // free native resources
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }


        /// <summary>
        /// Writes the bytes out the SPI port.
        /// </summary>
        /// <param name="bytes">An array of bytes to send to the sensor.</param>
        public void Write(byte[] bytes)
        {
            _spi.Write(bytes);
        }

        /// <summary>
        /// Reads the temperature from the sensor.
        /// </summary>
        /// <returns>A float representation of the temperature.</returns>
        public float ReadTemperature()
        {
            // Create the output and input arrays of bytes.
            byte[] bout = new byte[2];
            byte[] bin = new byte[2];

            // Set both of the output bytes to 0.  This sensor doesn't need any 
            // commands.  All this method does is read the input data, so the 
            // output is simply for SPI clocking.
            bout[0] = 0;
            bout[1] = 0;

            // Write the clocking info, then read the input data.
            _spi.WriteRead(bout, bin);

            // Convert the 2 byte value according to the AD7314 data sheet.

            // Put the raw bytes into an int.  Mask the upper bit, which is a 
            // sign indicator (+/-).
            int s = (((bin[0] << 8) + bin[1]) & 0xBFFF);

            // Convert to a float.
            float f = s;

            // Shift right.
            f /= 128;

            // Detect negative.
            if ((bin[0] & 0x40) == 0x40)
                f = -f;

            // Return the temperature.
            return f;
        }
    }
}
