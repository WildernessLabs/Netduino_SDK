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
using System.IO.Ports;
using Microsoft.SPOT.Hardware;

namespace SecretLabs.NETMF.Hardware.NetduinoMini
{
    internal class NetduinoMiniHardwareProvider : HardwareProvider
    {
        static NetduinoMiniHardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new NetduinoMiniHardwareProvider());
        }

        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":
                    rxPin = Pins.GPIO_PIN_A_0;
                    txPin = Pins.GPIO_PIN_A_1;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM2":
                    rxPin = Pins.GPIO_PIN_A_27;
                    txPin = Pins.GPIO_PIN_A_28;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }

        override public void GetI2CPins(out Cpu.Pin scl, out Cpu.Pin sda)
        {
            scl = Pins.GPIO_PIN_A_11;
            sda = Pins.GPIO_PIN_A_10;
        }

        override public void GetSpiPins(SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi)
        {
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    msk = Pins.GPIO_PIN_A_18;
                    miso = Pins.GPIO_PIN_A_16;
                    mosi = Pins.GPIO_PIN_A_17;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }
    }

    // Specifies identifiers for hardware I/O pins.
    public static class Pins
    {
        internal const Cpu.Pin GPIO_PIN_A_0 = (Cpu.Pin)0; // PA0/RXD0
        public const Cpu.Pin GPIO_PIN_12 = GPIO_PIN_A_0; // PA0/RXD0
        //
        internal const Cpu.Pin GPIO_PIN_A_1 = (Cpu.Pin)1; // PA1/TXD0
        public const Cpu.Pin GPIO_PIN_11 = GPIO_PIN_A_1; // PA1/TXD0
        //
        internal const Cpu.Pin GPIO_PIN_A_10 = (Cpu.Pin)10; // PA10/TWD
        public const Cpu.Pin GPIO_PIN_9 = GPIO_PIN_A_10; // PA10/TWD
        //
        internal const Cpu.Pin GPIO_PIN_A_11 = (Cpu.Pin)11; // PA11/TWCK
        public const Cpu.Pin GPIO_PIN_10 = GPIO_PIN_A_11; // PA11/TWCK
        //
        internal const Cpu.Pin GPIO_PIN_A_12 = (Cpu.Pin)12; // PA12/SPI_NPCS0
        public const Cpu.Pin GPIO_PIN_13 = GPIO_PIN_A_12; // PA12/SPI_NPCS0
        //
        internal const Cpu.Pin GPIO_PIN_A_16 = (Cpu.Pin)16; // PA16/SPI0_MISO
        public const Cpu.Pin GPIO_PIN_15 = GPIO_PIN_A_16; // PA16/SPI0_MISO
        //
        internal const Cpu.Pin GPIO_PIN_A_17 = (Cpu.Pin)17; // PA17/SPI0_MOSI
        public const Cpu.Pin GPIO_PIN_14 = GPIO_PIN_A_17; // PA17/SPI0_MOSI
        //
        internal const Cpu.Pin GPIO_PIN_A_18 = (Cpu.Pin)18; // PA18/SPI0_SPCK
        public const Cpu.Pin GPIO_PIN_16 = GPIO_PIN_A_18; // PA18/SPI0_SPCK
        //
        internal const Cpu.Pin GPIO_PIN_A_27 = (Cpu.Pin)27; // PA27/DRXD/PCK3
        //
        internal const Cpu.Pin GPIO_PIN_A_28 = (Cpu.Pin)28; // PA28/DTXD
        //
        internal const Cpu.Pin GPIO_PIN_A_29 = (Cpu.Pin)29; // PA29/FIQ/SPI1_NPCS3 (DTR)
        //
        internal const Cpu.Pin GPIO_PIN_B_19 = (Cpu.Pin)51; // PB19/PWM0/TCLK1
        public const Cpu.Pin GPIO_PIN_17 = GPIO_PIN_B_19; // PB19/PWM0/TCLK1
        //
        internal const Cpu.Pin GPIO_PIN_B_20 = (Cpu.Pin)52; // PB20/PWM1/PCK0
        public const Cpu.Pin GPIO_PIN_18 = GPIO_PIN_B_20; // PB20/PWM1/PCK0
        //
        internal const Cpu.Pin GPIO_PIN_B_21 = (Cpu.Pin)53; // PB21/PWM2/PCK1
        public const Cpu.Pin GPIO_PIN_19 = GPIO_PIN_B_21; // PB21/PWM2/PCK1
        //
        internal const Cpu.Pin GPIO_PIN_B_22 = (Cpu.Pin)54; // PB22/PWM3/PCK2
        public const Cpu.Pin GPIO_PIN_20 = GPIO_PIN_B_22; // PB22/PWM3/PCK2
        //
        internal const Cpu.Pin GPIO_PIN_B_27 = (Cpu.Pin)59; // PB27/TIOA2/PWM0/AD0
        public const Cpu.Pin GPIO_PIN_5 = GPIO_PIN_B_27; // PB27/TIOA2/PWM0/AD0
        //
        internal const Cpu.Pin GPIO_PIN_B_28 = (Cpu.Pin)60; // PB28/TIOB2/PWM1/AD1
        public const Cpu.Pin GPIO_PIN_6 = GPIO_PIN_B_28; // PB28/TIOB2/PWM1/AD1
        //
        internal const Cpu.Pin GPIO_PIN_B_29 = (Cpu.Pin)61; // PB29/PCK1/PWM2/AD2
        public const Cpu.Pin GPIO_PIN_7 = GPIO_PIN_B_29; // PB29/PCK1/PWM2/AD2
        //
        internal const Cpu.Pin GPIO_PIN_B_30 = (Cpu.Pin)62; // PB30/PCK2/PWM3/AD3
        public const Cpu.Pin GPIO_PIN_8 = GPIO_PIN_B_30; // PB30/PCK2/PWM3/AD3
        //
        public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;
    }

    // Specifies identifiers for analog channels.
    public static class AnalogChannels
    {
        public const Cpu.AnalogChannel ANALOG_PIN_5 = Cpu.AnalogChannel.ANALOG_0;
        public const Cpu.AnalogChannel ANALOG_PIN_6 = Cpu.AnalogChannel.ANALOG_1;
        public const Cpu.AnalogChannel ANALOG_PIN_7 = Cpu.AnalogChannel.ANALOG_2;
        public const Cpu.AnalogChannel ANALOG_PIN_8 = Cpu.AnalogChannel.ANALOG_3;
        public const Cpu.AnalogChannel ANALOG_NONE = Cpu.AnalogChannel.ANALOG_NONE;
    }

    // Specifies identifiers for pwm channels.
    public static class PWMChannels
    {
        public const Cpu.PWMChannel PWM_PIN_17 = Cpu.PWMChannel.PWM_0;
        public const Cpu.PWMChannel PWM_PIN_18 = Cpu.PWMChannel.PWM_1;
        public const Cpu.PWMChannel PWM_PIN_19 = Cpu.PWMChannel.PWM_2;
        public const Cpu.PWMChannel PWM_PIN_20 = Cpu.PWMChannel.PWM_3;
        public const Cpu.PWMChannel PWM_NONE = Cpu.PWMChannel.PWM_NONE;
    }

    public static class SerialPorts
    {
        public const string COM1 = "COM1";
        public const string COM2 = "COM2";
    }

    public static class BaudRates
    {
        public const BaudRate Baud2400 = BaudRate.Baudrate2400;
        public const BaudRate Baud9600 = BaudRate.Baudrate9600;
        public const BaudRate Baud19200 = BaudRate.Baudrate19200;
        public const BaudRate Baud38400 = BaudRate.Baudrate38400;
        public const BaudRate Baud57600 = BaudRate.Baudrate57600;
        public const BaudRate Baud115200 = BaudRate.Baudrate115200;
    }

    public static class ResistorModes
    {
        public const Port.ResistorMode PullUp = Port.ResistorMode.PullUp;
        public const Port.ResistorMode Disabled = Port.ResistorMode.Disabled;
    }

    public static class InterruptModes
    {
        public const Port.InterruptMode InterruptEdgeLow = Port.InterruptMode.InterruptEdgeLow;
        public const Port.InterruptMode InterruptEdgeHigh = Port.InterruptMode.InterruptEdgeHigh;
        public const Port.InterruptMode InterruptEdgeBoth = Port.InterruptMode.InterruptEdgeBoth;
        public const Port.InterruptMode InterruptEdgeLevelHigh = Port.InterruptMode.InterruptEdgeLevelHigh;
        public const Port.InterruptMode InterruptEdgeLevelLow = Port.InterruptMode.InterruptEdgeLevelLow;
        public const Port.InterruptMode InterruptNone = Port.InterruptMode.InterruptNone;
    }

    public static class SPI_Devices
    {
        public const Microsoft.SPOT.Hardware.SPI.SPI_module SPI1 = Microsoft.SPOT.Hardware.SPI.SPI_module.SPI1;
    }
}
