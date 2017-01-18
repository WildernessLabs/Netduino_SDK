//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

using System;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Hardware.NXP_LPC22XXDemo
{
    internal class LPC22XXDemoHardwareProvider : HardwareProvider
    {
        static LPC22XXDemoHardwareProvider()
        {
            Microsoft.SPOT.Hardware.HardwareProvider.Register(new LPC22XXDemoHardwareProvider());
        }


        override public void GetSerialPins(string comPort, out Cpu.Pin rxPin, out Cpu.Pin txPin, out Cpu.Pin ctsPin, out Cpu.Pin rtsPin)
        {
            switch (comPort)
            {
                case "COM1":
                    rxPin  = Pins.GPIO_PORT_0_01;
                    txPin  = Pins.GPIO_PORT_0_00;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                case "COM2":
                    rxPin  = Pins.GPIO_PORT_0_09;
                    txPin  = Pins.GPIO_PORT_0_08;
                    ctsPin = Pins.GPIO_NONE;
                    rtsPin = Pins.GPIO_NONE;
                    break;
                default:
                    throw new NotSupportedException();                    
            }
        }
// I2C Driver is not implemented yet
//        override public void GetI2CPins( out Cpu.Pin scl, out Cpu.Pin sda )
//        {
//            scl = Pins.GPIO_PORT_0_02;
//            sda = Pins.GPIO_PORT_0_03;
//        }
// SPI Driver is not implemented yet
//        override public void GetSpiPins( SPI.SPI_module spi_mod, out Cpu.Pin msk, out Cpu.Pin miso, out Cpu.Pin mosi )
//        {
//            switch (spi_mod)
//            {
//                case SPI_module.SPI1:
//                    msk  = Pins.GPIO_PORT_0_05;
//                    miso = Pins.GPIO_PORT_0_06;
//                    mosi = Pins.GPIO_PORT_0_07;
//                    break;
//                default:
//                    throw new NotSupportedException();                    
//            }
//        }        
    }
}

