/* Copyright (C) 2012 Secret Labs LLC
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
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace SecretLabs.NETMF.Hardware.NetduinoGo
{
    internal class NetduinoGoHardwareProvider : HardwareProvider
    {
        static NetduinoGoHardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new NetduinoGoHardwareProvider());
        }

        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":
                    rxPin = Pins.GPIO_PIN_A_2;
                    txPin = Pins.GPIO_PIN_A_3;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM2":
                    rxPin = Pins.GPIO_PIN_A_0;
                    txPin = Pins.GPIO_PIN_A_1;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }

        override public void GetI2CPins(out Cpu.Pin scl, out Cpu.Pin sda)
        {
            scl = Pins.GPIO_NONE;
            sda = Pins.GPIO_NONE;
        }

        override public void GetSpiPins(SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi)
        {
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    msk = Pins.GPIO_PIN_A_5;
                    miso = Pins.GPIO_PIN_A_6;
                    mosi = Pins.GPIO_PIN_A_7;
                    break;
                case SPI.SPI_module.SPI2:
                    msk = Pins.GPIO_PIN_B_13;
                    miso = Pins.GPIO_PIN_B_14;
                    mosi = Pins.GPIO_PIN_B_15;
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

        internal const Cpu.Pin GPIO_PIN_A_0 = (Cpu.Pin)0; // USART4_TX
        //
        internal const Cpu.Pin GPIO_PIN_A_1 = (Cpu.Pin)1; // USART4_RX
        //
        internal const Cpu.Pin GPIO_PIN_A_2 = (Cpu.Pin)2; // USART2_TX
        //
        internal const Cpu.Pin GPIO_PIN_A_3 = (Cpu.Pin)3; // USART2_RX
        //
        internal const Cpu.Pin GPIO_PIN_A_4 = (Cpu.Pin)4; // /SPI_CS_SOCKET3
        //
        internal const Cpu.Pin GPIO_PIN_A_5 = (Cpu.Pin)5; // SPI1_CLK
        //
        internal const Cpu.Pin GPIO_PIN_A_6 = (Cpu.Pin)6; // SPI1_MISO
        //
        internal const Cpu.Pin GPIO_PIN_A_7 = (Cpu.Pin)7; // SPI1_MOSI
        //
        //internal const Cpu.Pin GPIO_PIN_A_8 = (Cpu.Pin)8; // /SOCKET4_CS
        //
        //internal const Cpu.Pin GPIO_PIN_A_9 = (Cpu.Pin)8; // /STORAGE_REGISTER_OUTPUT_ENABLE
        //
        //internal const Cpu.Pin GPIO_PIN_B_0 = (Cpu.Pin)16; // /SOCKET5_CS
        //
        //internal const Cpu.Pin GPIO_PIN_B_1 = (Cpu.Pin)17; // /SOCKET6_CS
        //
        //internal const Cpu.Pin GPIO_PIN_B_2 = (Cpu.Pin)18; // STORAGE_REGISTER_CLOCK
        //
        //internal const Cpu.Pin GPIO_PIN_B_5 = (Cpu.Pin)21; // SHIFT_REGISTER_SERIAL_IN
        //
        //internal const Cpu.Pin GPIO_PIN_B_6 = (Cpu.Pin)22; // SOCKET1_LED
        //
        //internal const Cpu.Pin GPIO_PIN_B_7 = (Cpu.Pin)23; // SOCKET2_LED
        //
        //internal const Cpu.Pin GPIO_PIN_B_8 = (Cpu.Pin)24; // SOCKET3_LED
        //
        //internal const Cpu.Pin GPIO_PIN_B_9 = (Cpu.Pin)25; // SOCKET4_LED
        //
        //internal const Cpu.Pin GPIO_PIN_B_10 = (Cpu.Pin)26; // SOCKET8_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_B_11 = (Cpu.Pin)27; // /SOCKET1_CS
        //
        //internal const Cpu.Pin GPIO_PIN_B_12 = (Cpu.Pin)28; // /SOCKET8_CS
        //
        internal const Cpu.Pin GPIO_PIN_B_13 = (Cpu.Pin)29; // SPI2_CLK
        //
        internal const Cpu.Pin GPIO_PIN_B_14 = (Cpu.Pin)30; // SPI2_MISO
        //
        internal const Cpu.Pin GPIO_PIN_B_15 = (Cpu.Pin)31; // SPI2_MOSI
        //
        //internal const Cpu.Pin GPIO_PIN_C_0 = (Cpu.Pin)32; // SOCKET1_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_C_1 = (Cpu.Pin)33; // SOCKET2_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_C_2 = (Cpu.Pin)34; // SOCKET3_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_C_3 = (Cpu.Pin)35; // SOCKET4_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_C_4 = (Cpu.Pin)36; // SOCKET5_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_C_5 = (Cpu.Pin)37; // SOCKET6_GPIO
        //
        //internal const Cpu.Pin GPIO_PIN_C_6 = (Cpu.Pin)38; // SOCKET5_LED
        //
        //internal const Cpu.Pin GPIO_PIN_C_7 = (Cpu.Pin)39; // SOCKET6_LED
        //
        //internal const Cpu.Pin GPIO_PIN_C_8 = (Cpu.Pin)40; // SOCKET7_LED
        //
        //internal const Cpu.Pin GPIO_PIN_C_9 = (Cpu.Pin)41; // SOCKET8_LED
        //
        //internal const Cpu.Pin GPIO_PIN_C_10 = (Cpu.Pin)42; // SHIFT_REGISTER_CLOCK
        //
        //internal const Cpu.Pin GPIO_PIN_C_11 = (Cpu.Pin)43; // /SOCKET2_CS
        //
        //internal const Cpu.Pin GPIO_PIN_C_12 = (Cpu.Pin)44; // SOCKET7_GPIO
        //
        internal const Cpu.Pin GPIO_PIN_C_13 = (Cpu.Pin)45; // /PWR_LED_OFF
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Advanced)]
        public const Cpu.Pin PowerLed = GPIO_PIN_C_13; // /PWR_LED_OFF /* NOTE: we may reverse the logic of this in software so that TRUE turns the light on */
        //
        internal const Cpu.Pin GPIO_PIN_C_14 = (Cpu.Pin)46; // SWITCH1
        public const Cpu.Pin Button = GPIO_PIN_C_14; // SWITCH1
        //
        //internal const Cpu.Pin GPIO_PIN_C_15 = (Cpu.Pin)47; // /SWITCH1_CTRL_OF_RESET
        //
        //internal const Cpu.Pin GPIO_PIN_D_2 = (Cpu.Pin)50; // /SOCKET7_CS
        //
        public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;
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
        public const Microsoft.SPOT.Hardware.SPI.SPI_module SPI1 = Microsoft.SPOT.Hardware.SPI.SPI_module.SPI1;
        public const Microsoft.SPOT.Hardware.SPI.SPI_module SPI2 = Microsoft.SPOT.Hardware.SPI.SPI_module.SPI2;
    }

    public class GoSockets // : GoSocketProvider
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

        public static GoBus.GoSocket Socket1 = (GoBus.GoSocket)1;
        public static GoBus.GoSocket Socket2 = (GoBus.GoSocket)2;
        public static GoBus.GoSocket Socket3 = (GoBus.GoSocket)3;
        public static GoBus.GoSocket Socket4 = (GoBus.GoSocket)4;
        public static GoBus.GoSocket Socket5 = (GoBus.GoSocket)5;
        public static GoBus.GoSocket Socket6 = (GoBus.GoSocket)6;
        public static GoBus.GoSocket Socket7 = (GoBus.GoSocket)7;
        public static GoBus.GoSocket Socket8 = (GoBus.GoSocket)8;
    }
}
