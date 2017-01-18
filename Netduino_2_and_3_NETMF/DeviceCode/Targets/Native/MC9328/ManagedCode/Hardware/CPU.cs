using System;
using System.Collections;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;
using System.IO.Ports;

namespace Microsoft.SPOT.Hardware.FreescaleMXSDemo
{

    /// <summary>
    /// Specifies identifiers for hardware I/O pins.
    /// </summary>
    public static class Pins
    {
        /// <summary>
        /// GPIO port A pin 15.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_A_4  = (Cpu.Pin)4;
        /// <summary>
        /// GPIO port A pin 15.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_A_6  = (Cpu.Pin)6;
        /// <summary>
        /// GPIO port A pin 15.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_A_8  = (Cpu.Pin)8;
        /// <summary>
        /// GPIO port A pin 15.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_A_15 = (Cpu.Pin)15;
        /// <summary>
        /// GPIO port A pin 16.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_A_16 = (Cpu.Pin)16;
        /// <summary>
        /// GPIO port B pin 8.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_8 = (Cpu.Pin)40;
        /// <summary>
        /// GPIO port B pin 9.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_9 = (Cpu.Pin)41;
        /// <summary>
        /// GPIO port B pin 10.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_10 = (Cpu.Pin)42;
        /// <summary>
        /// GPIO port B pin 11.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_11 = (Cpu.Pin)43;
        /// <summary>
        /// GPIO port B pin 12.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_12 = (Cpu.Pin)44;
        /// <summary>
        /// GPIO port B pin 13.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_13 = (Cpu.Pin)45;
        /// <summary>
        /// GPIO port B pin 14.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_14 = (Cpu.Pin)46;
        /// <summary>
        /// GPIO port B pin 15.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_15 = (Cpu.Pin)47;
        /// <summary>
        /// GPIO port B pin 16.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_16 = (Cpu.Pin)48;
        /// <summary>
        /// GPIO port B pin 17.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_17 = (Cpu.Pin)49;
        /// <summary>
        /// GPIO port B pin 18.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_18 = (Cpu.Pin)50;
        /// <summary>
        /// GPIO port B pin 19.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_19 = (Cpu.Pin)51;
        /// <summary>
        /// GPIO port B pin 28.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_28 = (Cpu.Pin)60;
        /// <summary>
        /// GPIO port B pin 29.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_29 = (Cpu.Pin)61;
        /// <summary>
        /// GPIO port B pin 30.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_30 = (Cpu.Pin)62;
        /// <summary>
        /// GPIO port B pin 31.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_B_31 = (Cpu.Pin)63;
        /// <summary>
        /// GPIO port C pin 3.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_3  = (Cpu.Pin)67;
        /// <summary>
        /// GPIO port C pin 4.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_4  = (Cpu.Pin)68;
        /// <summary>
        /// GPIO port C pin 5.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_5  = (Cpu.Pin)69;
        /// <summary>
        /// GPIO port C pin 6.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_6  = (Cpu.Pin)70;
        /// <summary>
        /// GPIO port C pin 7.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_7  = (Cpu.Pin)71;
        /// <summary>
        /// GPIO port C pin 8.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_8  = (Cpu.Pin)72;
        /// <summary>
        /// GPIO port C pin 9.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_9  = (Cpu.Pin)73;
        /// <summary>
        /// GPIO port C pin 10.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_10 = (Cpu.Pin)74;
        /// <summary>
        /// GPIO port C pin 11.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_11 = (Cpu.Pin)75;
        /// <summary>
        /// GPIO port C pin 12.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_12 = (Cpu.Pin)76;
        /// <summary>
        /// GPIO port C pin 13.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_13 = (Cpu.Pin)77;
        /// <summary>
        /// GPIO port C pin 14.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_14 = (Cpu.Pin)78;
        /// <summary>
        /// GPIO port C pin 16.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_16 = (Cpu.Pin)80;
        /// <summary>
        /// GPIO port C pin 17.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_C_17 = (Cpu.Pin)81;
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

    public static class ResistorModes
    {
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="ResistorMode.PullUp"]/*'    />
        public const Port.ResistorMode PullUp   = Port.ResistorMode.PullUp;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="ResistorMode.Disabled"]/*'  />
        public const Port.ResistorMode Disabled = Port.ResistorMode.Disabled;
    }

    public static class InterruptModes
    {
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeLow"]/*'       />
        public const Port.InterruptMode InterruptEdgeLow       = Port.InterruptMode.InterruptEdgeLow ;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeHigh"]/*'      />
        public const Port.InterruptMode InterruptEdgeHigh      = Port.InterruptMode.InterruptEdgeHigh;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeBoth"]/*'      />
        public const Port.InterruptMode InterruptEdgeBoth      = Port.InterruptMode.InterruptEdgeBoth;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeLevelHigh"]/*' />
        public const Port.InterruptMode InterruptEdgeLevelHigh = Port.InterruptMode.InterruptEdgeLevelHigh;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeLevelLow"]/*'  />
        public const Port.InterruptMode InterruptEdgeLevelLow  = Port.InterruptMode.InterruptEdgeLevelLow;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptNone"]/*'          />
        public const Port.InterruptMode InterruptNone          = Port.InterruptMode.InterruptNone;
    }

    public static class SPI_Devices
    {
        public const Microsoft.SPOT.Hardware.SPI.SPI_module SPI1 = Microsoft.SPOT.Hardware.SPI.SPI_module.SPI1;
    }
    
}
