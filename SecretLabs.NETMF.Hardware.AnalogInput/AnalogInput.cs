/* Copyright (C) 2010-2012 Secret Labs LLC
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace SecretLabs.NETMF.Hardware
{
    public class AnalogInput : IDisposable
    {
        Microsoft.SPOT.Hardware.AnalogInput _analogInput = null;

        Boolean _disposed = false;

        // ADC constants and values
        const Int32 TEN_BIT_ADC_MAX_VALUE = 1023;   // 10-bit ADC maximum value
        Int32 _minValue = 0;                        // default instance minimum value
        Int32 _maxValue = TEN_BIT_ADC_MAX_VALUE;    // default instance maximum value

        public AnalogInput(Cpu.Pin pin)
        {
            Cpu.AnalogChannel channel = GetChannelFromPin(pin);
            _analogInput = new Microsoft.SPOT.Hardware.AnalogInput(channel);
        }

        ~AnalogInput()
        {
            Dispose();
        }

        public void Dispose()
        {
            _analogInput.Dispose();
        }

        static private Cpu.AnalogChannel GetChannelFromPin(Cpu.Pin pin)
        {
            if ((uint)pin >= 59 && (uint)pin <= 62)
            {
                // AD0-AD3 (peripheral B on pins PB27-PB30)
                return (Cpu.AnalogChannel)(pin - 59);
            }
            else if ((uint)pin >= 10 && (uint)pin <= 11)
            {
                // AD4-AD5 (multiplexed externally to MCU with TWD/TWCK)
                return (Cpu.AnalogChannel)(pin - 6);
            }
            else
            {
                return Cpu.AnalogChannel.ANALOG_NONE;
            }
        }

        public void SetRange(int minValue, int maxValue)
        {
            _minValue = minValue;
            _maxValue = maxValue;
        }

        public Int32 Read()
        {
            if (_disposed)
                throw new System.ObjectDisposedException();

            // retrieve our 10-bit ADC reading (based on a range of 0 to AREF)
            // convert our reading to the desired range and return it to the user.
            return (Int32)((((Int64)_analogInput.ReadRaw() * (_maxValue - _minValue)) / TEN_BIT_ADC_MAX_VALUE) + _minValue);
        }
    }
}
