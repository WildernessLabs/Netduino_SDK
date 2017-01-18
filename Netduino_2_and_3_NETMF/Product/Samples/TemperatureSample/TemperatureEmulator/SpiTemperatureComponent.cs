////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Threading;
using System.Windows.Forms;
using System.Xml;
using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Gpio;
using Microsoft.SPOT.Emulator.Spi;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Emulator.Temperature.SpiTemperatureDevice
{
    delegate void SetTemperatureDelegate(double temperature);

    /// <summary>
    /// Emulates the SPI temperature sensor.  This class is derived from a 
    /// standard SpiDevice.
    /// </summary>
    public class SpiTemperatureComponent : SpiDevice
    {
        /// <summary>
        /// Enables custom processing when setting up a component.
        /// </summary>
        public override void SetupComponent()
        {
            base.SetupComponent();
        }

        // This object is used to lock thread access to the _temperature member.
        private object tempLock = new object();

        // This member stores the current temperature emulatored by the sensor.
        private double _temperature = 22;

        /// <summary>
        /// Changes the temperature.  Invoked as a delegate from the UI thread.
        /// </summary>
        private void SetTemperature(double temperature)
        {
            _temperature = temperature;
        }

        /// <summary>
        /// Sets the temperature.
        /// </summary>
        public double Temperature
        {
            set
            {
                if (this.InvokeRequired)
                {
                    this.Invoke(new SetTemperatureDelegate(this.SetTemperature),
                        value);
                }
                else
                {
                    _temperature = value;
                }
            }
        }

        /// <summary>
        /// Simulates the write method of an SPI device for an array of bytes.
        /// </summary>
        /// <param name="data">An array of bytes sent to the SPI device.</param>
        protected override byte[] Write(byte[] data)
        {
            // Create a buffer in which to send back the data.
            byte[] bytes = new byte[2];

            // Store the current temperature for manipulation.
            double temp = _temperature;

            // The following algorithm is specific to how the real AD7314 
            // temperature sensor works.

            // Negative numbers are indicated by a bit being set in the value 
            // that is sent from the sensor.  The value itself will always be 
            // positive, so invert it here if needed.
            if (temp < 0)
                temp = -temp;

            // Shift the value left.
            temp *= 128;

            // Truncate the value.
            int nTemp = (int)temp;

            // Copy the bytes into the returned array.
            bytes[0] = (byte)((nTemp & 0xFF00) >> 8);
            bytes[1] = (byte)(nTemp & 0xFF);

            // If the temperature is negative, set the proper bit.
            if (_temperature < 0)
                bytes[0] |= 0x40;

            // Return the temperature value as an array of 2 bytes.
            return bytes;
        }

        /// <summary>
        /// Simulates the write method of an SPI device for an array of ushort 
        /// values.
        /// </summary>
        /// <param name="data">An array of ushort values to send to the SPI 
        /// device.
        /// </param>
        protected override ushort[] Write(ushort[] data)
        {
            // Call the base class implementation.
            return base.Write(data);
        }
    }
}
