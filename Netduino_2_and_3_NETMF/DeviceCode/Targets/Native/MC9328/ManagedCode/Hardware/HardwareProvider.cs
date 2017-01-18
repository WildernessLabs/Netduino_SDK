using System;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Hardware.FreescaleMXSDemo
{
    internal class MXSDemoHardwareProvider : HardwareProvider
    {
        static MXSDemoHardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new MXSDemoHardwareProvider());
        }


        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":
                    rxPin  = Pins.GPIO_PORT_C_12;
                    txPin  = Pins.GPIO_PORT_C_11;
                    ctsPin = Pins.GPIO_PORT_C_9;
                    rtsPin = Pins.GPIO_PORT_C_10;
                    break;
                case "COM2":
                    rxPin  = Pins.GPIO_PORT_B_31;
                    txPin  = Pins.GPIO_PORT_B_30;
                    ctsPin = Pins.GPIO_PORT_B_28;
                    rtsPin = Pins.GPIO_PORT_B_29;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }

        override public void GetI2CPins( out Cpu.Pin scl, out Cpu.Pin sda )
        {
            scl = Pins.GPIO_PORT_A_16;
            sda = Pins.GPIO_PORT_A_15;
        }

        override public void GetSpiPins( SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi )
        {
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    msk  = Pins.GPIO_PORT_C_14;
                    miso = Pins.GPIO_PORT_C_16;
                    mosi = Pins.GPIO_PORT_C_17;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }        
    }
}
