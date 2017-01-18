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
using System.Collections;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;
using System.IO.Ports;

namespace Microsoft.SPOT.Hardware.NXP_LPC22XXDemo
{

    /// <summary>
    /// Specifies identifiers for hardware I/O pins.
    /// </summary>
    public static class Pins
    {
        /// <summary>
        /// GPIO port 0 pin 0.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_00  = (Cpu.Pin)0;
        /// <summary>
        /// GPIO port 0 pin 1.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_01  = (Cpu.Pin)1;
        /// <summary>
        /// GPIO port 0 pin 2.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_02  = (Cpu.Pin)2;
        /// <summary>
        /// GPIO port 0 pin 3.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_03  = (Cpu.Pin)3;
        /// <summary>
        /// GPIO port 0 pin 4.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_04  = (Cpu.Pin)4;
        /// <summary>
        /// GPIO port 0 pin 5.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_05 = (Cpu.Pin)5;
        /// <summary>
        /// GPIO port 0 pin 6.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_06 = (Cpu.Pin)6;
        /// <summary>
        /// GPIO port 0 pin 7.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_07 = (Cpu.Pin)7;
        /// <summary>
        /// GPIO port 0 pin 8.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_08 = (Cpu.Pin)8;
        /// <summary>
        /// GPIO port 0 pin 9.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_09 = (Cpu.Pin)9;
        /// <summary>
        /// GPIO port 0 pin 10.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_10 = (Cpu.Pin)10;
        /// <summary>
        /// GPIO port 0 pin 11.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_11 = (Cpu.Pin)11;
        /// <summary>
        /// GPIO port 0 pin 12.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_12 = (Cpu.Pin)12;
        /// <summary>
        /// GPIO port 0 pin 13.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_13 = (Cpu.Pin)13;
        /// <summary>
        /// GPIO port 0 pin 14.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_14 = (Cpu.Pin)14;
        /// <summary>
        /// GPIO port 0 pin 15.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_15 = (Cpu.Pin)15;
        /// <summary>
        /// GPIO port 0 pin 16.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_16 = (Cpu.Pin)16;
        /// <summary>
        /// GPIO port 0 pin 17.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_17 = (Cpu.Pin)17;
        /// <summary>
        /// GPIO port 0 pin 18.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_18 = (Cpu.Pin)18;
        /// <summary>
        /// GPIO port 0 pin 19.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_19 = (Cpu.Pin)19;
        /// <summary>
        /// GPIO port 0 pin 20.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_20 = (Cpu.Pin)20;
        /// <summary>
        /// GPIO port 0 pin 21.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_21 = (Cpu.Pin)21;
        /// <summary>
        /// GPIO port 0 pin 22.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_22 = (Cpu.Pin)22;
        /// <summary>
        /// GPIO port 0 pin 23.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_23 = (Cpu.Pin)23;
        /// <summary>
        /// GPIO port 0 pin 24.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_24 = (Cpu.Pin)24;
        /// <summary>
        /// GPIO port 0 pin 25.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_25 = (Cpu.Pin)25;
        /// <summary>
        /// GPIO port 0 pin 26.
        /// </summary>
        /// Reserved        public const Cpu.Pin GPIO_PORT_0_26 = (Cpu.Pin)26;
        /// <summary>
        /// GPIO port 0 pin 27.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_27 = (Cpu.Pin)27;
        /// <summary>
        /// GPIO port 0 pin 28.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_28 = (Cpu.Pin)28;
        /// <summary>
        /// GPIO port 0 pin 29.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_29 = (Cpu.Pin)29;
        /// <summary>
        /// GPIO port 0 pin 30.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_30 = (Cpu.Pin)30;
        /// <summary>
        /// GPIO port 0 pin 31.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_0_31 = (Cpu.Pin)31;
        /// <summary>
        /// Null pin identifier.
        /// </summary>
        
        public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;
    }

    /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial"]/*' />
    public static class SerialPorts
    {
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial.COM1"]/*' />
        public const string COM1 = "COM1";
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial.COM1"]/*' />
        public const string COM2 = "COM2";
    }
    
    /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate"]/*' />
    public static class BaudRates
    {
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud19200"]/*'  />
        public const BaudRate Baud19200  = BaudRate.Baudrate19200;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud38400"]/*'  />
        public const BaudRate Baud38400  = BaudRate.Baudrate38400;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud57600"]/*'  />
        public const BaudRate Baud57600  = BaudRate.Baudrate57600;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud115200"]/*' />
        public const BaudRate Baud115200 = BaudRate.Baudrate115200;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud230400"]/*' />
        public const BaudRate Baud230400 = BaudRate.Baudrate230400;
    }
    
}

