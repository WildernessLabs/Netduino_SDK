using System;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Hardware.Mote2
{
    internal class Mote2HardwareProvider : HardwareProvider
    {
        static Mote2HardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new Mote2HardwareProvider());
        }


        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":           // STDUART
                    rxPin  = Pins.GPIO_PORT_PIN_46;
                    txPin  = Pins.GPIO_PORT_PIN_47;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM2":           // BTUART
                    rxPin  = Pins.GPIO_PORT_PIN_42;
                    txPin  = Pins.GPIO_PORT_PIN_43;
                    ctsPin = Pins.GPIO_PORT_PIN_44;
                    rtsPin = Pins.GPIO_PORT_PIN_45;
                    break;
                case "COM3":           // FFUART
                    rxPin  = Pins.GPIO_PORT_PIN_96;
                    txPin  = Pins.GPIO_PORT_PIN_99;
                    ctsPin = Pins.GPIO_PORT_PIN_100;
                    rtsPin = Pins.GPIO_PORT_PIN_98;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }

        override public void GetSpiPins( SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi )
        {
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    msk  = Pins.GPIO_PORT_PIN_23;
                    miso = Pins.GPIO_PORT_PIN_26;
                    mosi = Pins.GPIO_PORT_PIN_25;
                    break;
                case SPI.SPI_module.SPI2:
                    msk  = Pins.GPIO_PORT_PIN_36;
                    miso = Pins.GPIO_PORT_PIN_11;
                    mosi = Pins.GPIO_PORT_PIN_38;
                    break;
                case SPI.SPI_module.SPI3:
                    // These are the SSP3 pins that drive the CC2420 on the Mote2
                    msk  = Pins.GPIO_PORT_PIN_34;
                    miso = Pins.GPIO_PORT_PIN_41;
                    mosi = Pins.GPIO_PORT_PIN_35;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }

        override public void GetI2CPins( out Cpu.Pin scl, out Cpu.Pin sda )
        {
            scl = Pins.GPIO_PORT_PIN_117;
            sda = Pins.GPIO_PORT_PIN_118;
        }
    }
}
