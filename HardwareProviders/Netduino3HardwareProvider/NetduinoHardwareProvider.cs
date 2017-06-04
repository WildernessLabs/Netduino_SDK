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

namespace SecretLabs.NETMF.Hardware.Netduino
{
    internal class NetduinoHardwareProvider : HardwareProvider
    {
        static NetduinoHardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new NetduinoHardwareProvider());
        }

        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":
                    rxPin = Pins.GPIO_PIN_C_7;
                    txPin = Pins.GPIO_PIN_C_6;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM2":
                    rxPin = Pins.GPIO_PIN_A_3;
                    txPin = Pins.GPIO_PIN_A_2;
                    ctsPin = Pins.GPIO_PIN_A_0;
                    rtsPin = Pins.GPIO_PIN_A_1;
                    break;
                case "COM3":
                    rxPin = Pins.GPIO_PIN_A_1;
                    txPin = Pins.GPIO_PIN_A_0;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM4":
                    rxPin = Pins.GPIO_PIN_B_7;
                    txPin = Pins.GPIO_PIN_B_6;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM5":
                    rxPin = Pins.GPIO_PIN_D_9;
                    txPin = Pins.GPIO_PIN_D_8;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM6":
                    rxPin = Pins.GPIO_PIN_E_7;
                    txPin = Pins.GPIO_PIN_E_8;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM7":
                    rxPin = Pins.GPIO_PIN_E_0;
                    txPin = Pins.GPIO_PIN_E_1;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM8":
                    rxPin = Pins.GPIO_PIN_D_5;
                    txPin = Pins.GPIO_PIN_D_6;
                    ctsPin = Pins.GPIO_PIN_D_4;
                    rtsPin = Pins.GPIO_PIN_D_3;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }

        override public void GetI2CPins(out Cpu.Pin scl, out Cpu.Pin sda)
        {
            scl = Pins.GPIO_PIN_B_6;
            sda = Pins.GPIO_PIN_B_7;
        }

        override public void GetSpiPins(SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi)
        {
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    msk = Pins.GPIO_PIN_B_13;
                    miso = Pins.GPIO_PIN_B_14;
                    mosi = Pins.GPIO_PIN_B_15;
                    break;
                case SPI.SPI_module.SPI2:
                    msk = Pins.GPIO_PIN_A_5;
                    miso = Pins.GPIO_PIN_B_4;
                    mosi = Pins.GPIO_PIN_A_7;
                    break;
                case SPI.SPI_module.SPI3:
                    msk = Pins.GPIO_PIN_C_10;
                    miso = Pins.GPIO_PIN_C_11;
                    mosi = Pins.GPIO_PIN_C_12;
                    break;
                case SPI.SPI_module.SPI4:
                    msk = Pins.GPIO_PIN_E_2;
                    miso = Pins.GPIO_PIN_E_13;
                    mosi = Pins.GPIO_PIN_E_6;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }
    }

    // Specifies identifiers for hardware I/O pins.
    public static class Pins
    {
        #region "hide class members to emulate a static enum"
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool Equals(object objA, object objB)
        {
            return object.Equals(objA, objB);
        }

        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool ReferenceEquals(object objA, object objB)
        {
            return object.ReferenceEquals(objA, objB);
        }
        #endregion

        internal const Cpu.Pin GPIO_PIN_A_0 = (Cpu.Pin)0;
        public static Cpu.Pin GPIO_PIN_D8 { get { return GPIO_PIN_A_0; } }
        //
        internal const Cpu.Pin GPIO_PIN_A_1 = (Cpu.Pin)1;
        public static Cpu.Pin GPIO_PIN_D7 { get { return GPIO_PIN_A_1; } }
        //
        internal const Cpu.Pin GPIO_PIN_A_2 = (Cpu.Pin)2;
        public static Cpu.Pin GPIO_PIN_D3 { get { return GPIO_PIN_A_2; } }
        //
        internal const Cpu.Pin GPIO_PIN_A_3 = (Cpu.Pin)3;
        public static Cpu.Pin GPIO_PIN_D2 { get { return GPIO_PIN_A_3; } }
        //
        //internal const Cpu.Pin GPIO_PIN_A_4 = (Cpu.Pin)4;
        //
        internal const Cpu.Pin GPIO_PIN_A_5 = (Cpu.Pin)5;
        //
        internal const Cpu.Pin GPIO_PIN_A_6 = (Cpu.Pin)6;
        //
        internal const Cpu.Pin GPIO_PIN_A_7 = (Cpu.Pin)7;
        //
        //internal const Cpu.Pin GPIO_PIN_A_8 = (Cpu.Pin)8;
        //
        //internal const Cpu.Pin GPIO_PIN_A_9 = (Cpu.Pin)9;
        //
        internal const Cpu.Pin GPIO_PIN_A_10 = (Cpu.Pin)10;
        public static Cpu.Pin ONBOARD_LED { get { return GPIO_PIN_A_10; } }
        //
        //internal const Cpu.Pin GPIO_PIN_B_0 = (Cpu.Pin)16;
        //
        //internal const Cpu.Pin GPIO_PIN_B_1 = (Cpu.Pin)17;
        //
        //internal const Cpu.Pin GPIO_PIN_B_2 = (Cpu.Pin)18;
        //
        //internal const Cpu.Pin GPIO_PIN_B_3 = (Cpu.Pin)19;
        //
        internal const Cpu.Pin GPIO_PIN_B_4 = (Cpu.Pin)20;
        //
        internal const Cpu.Pin GPIO_PIN_B_5 = (Cpu.Pin)21;
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static Cpu.Pin ONBOARD_SW1 { get { return GPIO_PIN_B_5; } }
        public static Cpu.Pin ONBOARD_BTN { get { return GPIO_PIN_B_5; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_6 = (Cpu.Pin)22;
        public static Cpu.Pin GPIO_PIN_SCL { get { return GPIO_PIN_B_6; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_7 = (Cpu.Pin)23;
        public static Cpu.Pin GPIO_PIN_SDA { get { return GPIO_PIN_B_7; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_8 = (Cpu.Pin)24;
        public static Cpu.Pin GPIO_PIN_D5 { get { return GPIO_PIN_B_8; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_9 = (Cpu.Pin)25;
        public static Cpu.Pin GPIO_PIN_D6 { get { return GPIO_PIN_B_9; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_10 = (Cpu.Pin)26;
        public static Cpu.Pin GPIO_PIN_D10 { get { return GPIO_PIN_B_10; } }
        //
        //internal const Cpu.Pin GPIO_PIN_B_11 = (Cpu.Pin)27;
        //
        internal const Cpu.Pin GPIO_PIN_B_12 = (Cpu.Pin)28;
        public static Cpu.Pin GPIO_PIN_D4 { get { return GPIO_PIN_B_12; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_13 = (Cpu.Pin)29;
        public static Cpu.Pin GPIO_PIN_D13 { get { return GPIO_PIN_B_13; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_14 = (Cpu.Pin)30;
        public static Cpu.Pin GPIO_PIN_D12 { get { return GPIO_PIN_B_14; } }
        //
        internal const Cpu.Pin GPIO_PIN_B_15 = (Cpu.Pin)31;
        public static Cpu.Pin GPIO_PIN_D11 { get { return GPIO_PIN_B_15; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_0 = (Cpu.Pin)32;
        public static Cpu.Pin GPIO_PIN_A0 { get { return GPIO_PIN_C_0; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_1 = (Cpu.Pin)33;
        public static Cpu.Pin GPIO_PIN_A1 { get { return GPIO_PIN_C_1; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_2 = (Cpu.Pin)34;
        public static Cpu.Pin GPIO_PIN_A2 { get { return GPIO_PIN_C_2; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_3 = (Cpu.Pin)35;
        public static Cpu.Pin GPIO_PIN_A3 { get { return GPIO_PIN_C_3; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_4 = (Cpu.Pin)36;
        public static Cpu.Pin GPIO_PIN_A4 { get { return GPIO_PIN_C_4; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_5 = (Cpu.Pin)37;
        public static Cpu.Pin GPIO_PIN_A5 { get { return GPIO_PIN_C_5; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_6 = (Cpu.Pin)38;
        public static Cpu.Pin GPIO_PIN_D1 { get { return GPIO_PIN_C_6; } }
        //
        internal const Cpu.Pin GPIO_PIN_C_7 = (Cpu.Pin)39;
        public static Cpu.Pin GPIO_PIN_D0 { get { return GPIO_PIN_C_7; } }
        //
        //internal const Cpu.Pin GPIO_PIN_C_8 = (Cpu.Pin)40;
        //
        //internal const Cpu.Pin GPIO_PIN_C_9 = (Cpu.Pin)41;
        //
        internal const Cpu.Pin GPIO_PIN_C_10 = (Cpu.Pin)42;
        //
        internal const Cpu.Pin GPIO_PIN_C_11 = (Cpu.Pin)43;
        //
        internal const Cpu.Pin GPIO_PIN_C_12 = (Cpu.Pin)44;
        //
        //internal const Cpu.Pin GPIO_PIN_C_13 = (Cpu.Pin)45;
        //
        //internal const Cpu.Pin GPIO_PIN_C_14 = (Cpu.Pin)46;
        //
        //internal const Cpu.Pin GPIO_PIN_C_15 = (Cpu.Pin)47;
        //
        //internal const Cpu.Pin GPIO_PIN_D_2 = (Cpu.Pin)50;
        //
        internal const Cpu.Pin GPIO_PIN_D_3 = (Cpu.Pin)51;
        //
        internal const Cpu.Pin GPIO_PIN_D_4 = (Cpu.Pin)52;
        //
        internal const Cpu.Pin GPIO_PIN_D_5 = (Cpu.Pin)53;
        //
        internal const Cpu.Pin GPIO_PIN_D_6 = (Cpu.Pin)54;
        //        
        internal const Cpu.Pin GPIO_PIN_D_8 = (Cpu.Pin)56;
        //
        internal const Cpu.Pin GPIO_PIN_D_9 = (Cpu.Pin)57;
        //
        internal const Cpu.Pin GPIO_PIN_D_11 = (Cpu.Pin)59;
        //
        internal const Cpu.Pin GPIO_PIN_E_0 = (Cpu.Pin)64;
        //
        internal const Cpu.Pin GPIO_PIN_E_1 = (Cpu.Pin)65;
        //
        internal const Cpu.Pin GPIO_PIN_E_2 = (Cpu.Pin)66;
		//
		internal const Cpu.Pin GPIO_PIN_E_5 = (Cpu.Pin)69;
		public static Cpu.Pin GPIO_PIN_D9 { get { return GPIO_PIN_E_5; } }
		//
		internal const Cpu.Pin GPIO_PIN_E_6 = (Cpu.Pin)70;
        //
        internal const Cpu.Pin GPIO_PIN_E_7 = (Cpu.Pin)71;
        //
        internal const Cpu.Pin GPIO_PIN_E_8 = (Cpu.Pin)72;
        //
        internal const Cpu.Pin GPIO_PIN_E_13 = (Cpu.Pin)77;
        //
        public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;
    }

    // Specifies identifiers for analog channels.
    public static class AnalogChannels
    {
        #region "hide class members to emulate a static enum"
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool Equals(object objA, object objB)
        {
            return object.Equals(objA, objB);
        }

        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool ReferenceEquals(object objA, object objB)
        {
            return object.ReferenceEquals(objA, objB);
        }
        #endregion

        public static Cpu.AnalogChannel ANALOG_PIN_A0 { get { return Cpu.AnalogChannel.ANALOG_0; } }
        public static Cpu.AnalogChannel ANALOG_PIN_A1 { get { return Cpu.AnalogChannel.ANALOG_1; } }
        public static Cpu.AnalogChannel ANALOG_PIN_A2 { get { return Cpu.AnalogChannel.ANALOG_2; } }
        public static Cpu.AnalogChannel ANALOG_PIN_A3 { get { return Cpu.AnalogChannel.ANALOG_3; } }
        public static Cpu.AnalogChannel ANALOG_PIN_A4 { get { return Cpu.AnalogChannel.ANALOG_4; } }
        public static Cpu.AnalogChannel ANALOG_PIN_A5 { get { return Cpu.AnalogChannel.ANALOG_5; } }
        public const Cpu.AnalogChannel ANALOG_NONE = Cpu.AnalogChannel.ANALOG_NONE;
    }

    // Specifies identifiers for pwm channels.
    public static class PWMChannels
    {
        #region "hide class members to emulate a static enum"
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool Equals(object objA, object objB)
        {
            return object.Equals(objA, objB);
        }

        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool ReferenceEquals(object objA, object objB)
        {
            return object.ReferenceEquals(objA, objB);
        }
        #endregion

        public static Cpu.PWMChannel PWM_PIN_D5 { get { return Cpu.PWMChannel.PWM_0; } }
        public static Cpu.PWMChannel PWM_PIN_D6 { get { return Cpu.PWMChannel.PWM_1; } }
        public static Cpu.PWMChannel PWM_PIN_D9 { get { return Cpu.PWMChannel.PWM_2; } }
        public static Cpu.PWMChannel PWM_PIN_D10 { get { return Cpu.PWMChannel.PWM_3; } }
        public static Cpu.PWMChannel PWM_PIN_D3 { get { return Cpu.PWMChannel.PWM_4; } }
        public static Cpu.PWMChannel PWM_PIN_D11 { get { return Cpu.PWMChannel.PWM_5; } }
        public static Cpu.PWMChannel PWM_ONBOARD_LED { get { return Cpu.PWMChannel.PWM_6; } }
        public const Cpu.PWMChannel PWM_NONE = Cpu.PWMChannel.PWM_NONE;
    }

    public static class SerialPorts
    {
        #region "hide class members to emulate a static enum"
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool Equals(object objA, object objB)
        {
            return object.Equals(objA, objB);
        }

        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool ReferenceEquals(object objA, object objB)
        {
            return object.ReferenceEquals(objA, objB);
        }
        #endregion

        public static string COM1 { get { return "COM1"; } }
        public static string COM2 { get { return "COM2"; } }
        public static string COM3 { get { return "COM3"; } }
        public static string COM4 { get { return "COM4"; } }
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
        public const Port.ResistorMode Disabled = Port.ResistorMode.Disabled;
        public const Port.ResistorMode PullDown = Port.ResistorMode.PullDown;
        public const Port.ResistorMode PullUp = Port.ResistorMode.PullUp;
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
        #region "hide class members to emulate a static enum"
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool Equals(object objA, object objB)
        {
            return object.Equals(objA, objB);
        }

        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public static new bool ReferenceEquals(object objA, object objB)
        {
            return object.ReferenceEquals(objA, objB);
        }
        #endregion

        public static Microsoft.SPOT.Hardware.SPI.SPI_module SPI1 { get { return Microsoft.SPOT.Hardware.SPI.SPI_module.SPI1; } }
    }
}
