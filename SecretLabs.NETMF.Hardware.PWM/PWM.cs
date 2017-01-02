/* Copyright (C) 2010 Secret Labs LLC
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
    public class PWM : IDisposable
    {
        Microsoft.SPOT.Hardware.PWM _pwm = null;

        Boolean _disposed = false;

        public PWM(Cpu.Pin pin)
        {
            Cpu.PWMChannel channel = GetChannelFromPin(pin);
            _pwm = new Microsoft.SPOT.Hardware.PWM(channel, 100, 0, Microsoft.SPOT.Hardware.PWM.ScaleFactor.Microseconds, false);
        }

        ~PWM()
        {
            Dispose();
        }

        public void Dispose()
        {
            _pwm.Dispose();
        }

        static private Cpu.PWMChannel GetChannelFromPin(Cpu.Pin pin)
        {
            if ((uint)pin >= 51 && (uint)pin <= 54)
                return (Cpu.PWMChannel)(pin - 51);
            else
                return Cpu.PWMChannel.PWM_NONE;
        }

        public void SetDutyCycle(UInt32 dutyCycle)
        {
            if (_disposed)
                throw new System.ObjectDisposedException();

            _pwm.DutyCycle = (double)dutyCycle / 100.0;
        }

        public void SetPulse(UInt32 period, UInt32 duration)
        {
            if (_disposed)
                throw new System.ObjectDisposedException();

            _pwm.Period = period;
            _pwm.Duration = duration;
        }
    }
}