using System;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Hardware.AT91SAM9
{
    internal class AT91SAM9HardwareProvider : HardwareProvider
    {
        static AT91SAM9HardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new AT91SAM9HardwareProvider());
        }


        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":
                    rxPin  = Pins.GPIO_PIN_0;
                    txPin  = Pins.GPIO_PIN_1;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM2":
                    rxPin  = Pins.GPIO_PIN_2;
                    txPin  = Pins.GPIO_PIN_3;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }

        override public void GetI2CPins( out Cpu.Pin scl, out Cpu.Pin sda )
        {
            scl = Pins.GPIO_PIN_4;
            sda = Pins.GPIO_PIN_5;
        }

        override public void GetSpiPins( SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi )
        {
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    msk  = Pins.GPIO_PIN_6;
                    miso = Pins.GPIO_PIN_7;
                    mosi = Pins.GPIO_PIN_8;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }        
    }
}
