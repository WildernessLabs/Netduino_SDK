using System;
using System.IO.Ports;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Hardware.STM32F746NUCLEO
{
    //
    // GPIO Pins
    //
    public static class Pins
    {
        public const Cpu.Pin None = Cpu.Pin.GPIO_NONE;

        public const Cpu.Pin PA0  = (Cpu.Pin)(0*16 +  0);
        public const Cpu.Pin PA1  = (Cpu.Pin)(0*16 +  1);
        public const Cpu.Pin PA2  = (Cpu.Pin)(0*16 +  2);
        public const Cpu.Pin PA3  = (Cpu.Pin)(0*16 +  3);
        public const Cpu.Pin PA4  = (Cpu.Pin)(0*16 +  4);
        public const Cpu.Pin PA5  = (Cpu.Pin)(0*16 +  5);
        public const Cpu.Pin PA6  = (Cpu.Pin)(0*16 +  6);
        public const Cpu.Pin PA7  = (Cpu.Pin)(0*16 +  7);
        public const Cpu.Pin PA8  = (Cpu.Pin)(0*16 +  8);
        public const Cpu.Pin PA9  = (Cpu.Pin)(0*16 +  9);
        public const Cpu.Pin PA10 = (Cpu.Pin)(0*16 + 10);
        public const Cpu.Pin PA11 = (Cpu.Pin)(0*16 + 11);
        public const Cpu.Pin PA12 = (Cpu.Pin)(0*16 + 12);
        public const Cpu.Pin PA13 = (Cpu.Pin)(0*16 + 13);
        public const Cpu.Pin PA14 = (Cpu.Pin)(0*16 + 14);
        public const Cpu.Pin PA15 = (Cpu.Pin)(0*16 + 15);

        public const Cpu.Pin PB0  = (Cpu.Pin)(1*16 +  0);
        public const Cpu.Pin PB1  = (Cpu.Pin)(1*16 +  1);
        public const Cpu.Pin PB2  = (Cpu.Pin)(1*16 +  2);
        public const Cpu.Pin PB3  = (Cpu.Pin)(1*16 +  3);
        public const Cpu.Pin PB4  = (Cpu.Pin)(1*16 +  4);
        public const Cpu.Pin PB5  = (Cpu.Pin)(1*16 +  5);
        public const Cpu.Pin PB6  = (Cpu.Pin)(1*16 +  6);
        public const Cpu.Pin PB7  = (Cpu.Pin)(1*16 +  7);
        public const Cpu.Pin PB8  = (Cpu.Pin)(1*16 +  8);
        public const Cpu.Pin PB9  = (Cpu.Pin)(1*16 +  9);
        public const Cpu.Pin PB10 = (Cpu.Pin)(1*16 + 10);
        public const Cpu.Pin PB11 = (Cpu.Pin)(1*16 + 11);
        public const Cpu.Pin PB12 = (Cpu.Pin)(1*16 + 12);
        public const Cpu.Pin PB13 = (Cpu.Pin)(1*16 + 13);
        public const Cpu.Pin PB14 = (Cpu.Pin)(1*16 + 14);
        public const Cpu.Pin PB15 = (Cpu.Pin)(1*16 + 15);

        public const Cpu.Pin PC0  = (Cpu.Pin)(2*16 +  0);
        public const Cpu.Pin PC1  = (Cpu.Pin)(2*16 +  1);
        public const Cpu.Pin PC2  = (Cpu.Pin)(2*16 +  2);
        public const Cpu.Pin PC3  = (Cpu.Pin)(2*16 +  3);
        public const Cpu.Pin PC4  = (Cpu.Pin)(2*16 +  4);
        public const Cpu.Pin PC5  = (Cpu.Pin)(2*16 +  5);
        public const Cpu.Pin PC6  = (Cpu.Pin)(2*16 +  6);
        public const Cpu.Pin PC7  = (Cpu.Pin)(2*16 +  7);
        public const Cpu.Pin PC8  = (Cpu.Pin)(2*16 +  8);
        public const Cpu.Pin PC9  = (Cpu.Pin)(2*16 +  9);
        public const Cpu.Pin PC10 = (Cpu.Pin)(2*16 + 10);
        public const Cpu.Pin PC11 = (Cpu.Pin)(2*16 + 11);
        public const Cpu.Pin PC12 = (Cpu.Pin)(2*16 + 12);
        public const Cpu.Pin PC13 = (Cpu.Pin)(2*16 + 13);
        public const Cpu.Pin PC14 = (Cpu.Pin)(2*16 + 14);
        public const Cpu.Pin PC15 = (Cpu.Pin)(2*16 + 15);

        public const Cpu.Pin PD0  = (Cpu.Pin)(3*16 +  0);
        public const Cpu.Pin PD1  = (Cpu.Pin)(3*16 +  1);
        public const Cpu.Pin PD2  = (Cpu.Pin)(3*16 +  2);
        public const Cpu.Pin PD3  = (Cpu.Pin)(3*16 +  3);
        public const Cpu.Pin PD4  = (Cpu.Pin)(3*16 +  4);
        public const Cpu.Pin PD5  = (Cpu.Pin)(3*16 +  5);
        public const Cpu.Pin PD6  = (Cpu.Pin)(3*16 +  6);
        public const Cpu.Pin PD7  = (Cpu.Pin)(3*16 +  7);
        public const Cpu.Pin PD8  = (Cpu.Pin)(3*16 +  8);
        public const Cpu.Pin PD9  = (Cpu.Pin)(3*16 +  9);
        public const Cpu.Pin PD10 = (Cpu.Pin)(3*16 + 10);
        public const Cpu.Pin PD11 = (Cpu.Pin)(3*16 + 11);
        public const Cpu.Pin PD12 = (Cpu.Pin)(3*16 + 12);
        public const Cpu.Pin PD13 = (Cpu.Pin)(3*16 + 13);
        public const Cpu.Pin PD14 = (Cpu.Pin)(3*16 + 14);
        public const Cpu.Pin PD15 = (Cpu.Pin)(3*16 + 15);

        public const Cpu.Pin PE0  = (Cpu.Pin)(4*16 +  0);
        public const Cpu.Pin PE1  = (Cpu.Pin)(4*16 +  1);
        public const Cpu.Pin PE2  = (Cpu.Pin)(4*16 +  2);
        public const Cpu.Pin PE3  = (Cpu.Pin)(4*16 +  3);
        public const Cpu.Pin PE4  = (Cpu.Pin)(4*16 +  4);
        public const Cpu.Pin PE5  = (Cpu.Pin)(4*16 +  5);
        public const Cpu.Pin PE6  = (Cpu.Pin)(4*16 +  6);
        public const Cpu.Pin PE7  = (Cpu.Pin)(4*16 +  7);
        public const Cpu.Pin PE8  = (Cpu.Pin)(4*16 +  8);
        public const Cpu.Pin PE9  = (Cpu.Pin)(4*16 +  9);
        public const Cpu.Pin PE10 = (Cpu.Pin)(4*16 + 10);
        public const Cpu.Pin PE11 = (Cpu.Pin)(4*16 + 11);
        public const Cpu.Pin PE12 = (Cpu.Pin)(4*16 + 12);
        public const Cpu.Pin PE13 = (Cpu.Pin)(4*16 + 13);
        public const Cpu.Pin PE14 = (Cpu.Pin)(4*16 + 14);
        public const Cpu.Pin PE15 = (Cpu.Pin)(4*16 + 15);

        public const Cpu.Pin PF0  = (Cpu.Pin)(5*16 +  0);
        public const Cpu.Pin PF1  = (Cpu.Pin)(5*16 +  1);
        public const Cpu.Pin PF2  = (Cpu.Pin)(5*16 +  2);
        public const Cpu.Pin PF3  = (Cpu.Pin)(5*16 +  3);
        public const Cpu.Pin PF4  = (Cpu.Pin)(5*16 +  4);
        public const Cpu.Pin PF5  = (Cpu.Pin)(5*16 +  5);
        public const Cpu.Pin PF6  = (Cpu.Pin)(5*16 +  6);
        public const Cpu.Pin PF7  = (Cpu.Pin)(5*16 +  7);
        public const Cpu.Pin PF8  = (Cpu.Pin)(5*16 +  8);
        public const Cpu.Pin PF9  = (Cpu.Pin)(5*16 +  9);
        public const Cpu.Pin PF10 = (Cpu.Pin)(5*16 + 10);
        public const Cpu.Pin PF11 = (Cpu.Pin)(5*16 + 11);
        public const Cpu.Pin PF12 = (Cpu.Pin)(5*16 + 12);
        public const Cpu.Pin PF13 = (Cpu.Pin)(5*16 + 13);
        public const Cpu.Pin PF14 = (Cpu.Pin)(5*16 + 14);
        public const Cpu.Pin PF15 = (Cpu.Pin)(5*16 + 15);

        public const Cpu.Pin PG0  = (Cpu.Pin)(6*16 +  0);
        public const Cpu.Pin PG1  = (Cpu.Pin)(6*16 +  1);
        public const Cpu.Pin PG2  = (Cpu.Pin)(6*16 +  2);
        public const Cpu.Pin PG3  = (Cpu.Pin)(6*16 +  3);
        public const Cpu.Pin PG4  = (Cpu.Pin)(6*16 +  4);
        public const Cpu.Pin PG5  = (Cpu.Pin)(6*16 +  5);
        public const Cpu.Pin PG6  = (Cpu.Pin)(6*16 +  6);
        public const Cpu.Pin PG7  = (Cpu.Pin)(6*16 +  7);
        public const Cpu.Pin PG8  = (Cpu.Pin)(6*16 +  8);
        public const Cpu.Pin PG9  = (Cpu.Pin)(6*16 +  9);
        public const Cpu.Pin PG10 = (Cpu.Pin)(6*16 + 10);
        public const Cpu.Pin PG11 = (Cpu.Pin)(6*16 + 11);
        public const Cpu.Pin PG12 = (Cpu.Pin)(6*16 + 12);
        public const Cpu.Pin PG13 = (Cpu.Pin)(6*16 + 13);
        public const Cpu.Pin PG14 = (Cpu.Pin)(6*16 + 14);
        public const Cpu.Pin PG15 = (Cpu.Pin)(6*16 + 15);

        // Board-specific
        public const Cpu.Pin LED1 = PB0;    // Green
        public const Cpu.Pin LED2 = PB7;    // Blue
        public const Cpu.Pin LED3 = PB14;   // Red
        public const Cpu.Pin UserButton = PC13;
    }

    //
    // ST Zio Connectors
    //
    public static class ZioPins
    {
        // Arduino subset

        /* CN9.1   */   public const Cpu.Pin A0  = Pins.PA3;
        /* CN9.3   */   public const Cpu.Pin A1  = Pins.PC0;
        /* CN9.5   */   public const Cpu.Pin A2  = Pins.PC3;
        /* CN9.7   */   public const Cpu.Pin A3  = Pins.PF3;
        /* CN9.9   */   public const Cpu.Pin A4  = Pins.PF5;
        /* CN9.11  */   public const Cpu.Pin A5  = Pins.PF10;

        /* CN7.2   */   public const Cpu.Pin D15 = Pins.PB8;
        /* CN7.4   */   public const Cpu.Pin D14 = Pins.PB9;
        /* CN7.6   */   //                   AVDD
        /* CN7.8   */   //                   GND
        /* CN7.10  */   public const Cpu.Pin D13 = Pins.PA5;
        /* CN7.12  */   public const Cpu.Pin D12 = Pins.PA6;
        /* CN7.14  */   public const Cpu.Pin D11 = Pins.PA7;
        /* CN7.16  */   public const Cpu.Pin D10 = Pins.PD14;
        /* CN7.18  */   public const Cpu.Pin D9  = Pins.PD15;
        /* CN7.20  */   public const Cpu.Pin D8  = Pins.PF12;

        /* CN10.2  */   public const Cpu.Pin D7  = Pins.PF13;
        /* CN10.4  */   public const Cpu.Pin D6  = Pins.PE9;
        /* CN10.6  */   public const Cpu.Pin D5  = Pins.PE11;
        /* CN10.8  */   public const Cpu.Pin D4  = Pins.PF14;
        /* CN10.10 */   public const Cpu.Pin D3  = Pins.PE13;
        /* CN10.12 */   public const Cpu.Pin D2  = Pins.PF15;
        /* CN10.14 */   public const Cpu.Pin D1  = Pins.PG14;
        /* CN10.16 */   public const Cpu.Pin D0  = Pins.PG9;

        // Zio extensions

        /* CN8.2   */   public const Cpu.Pin D43 = Pins.PC8;
        /* CN8.4   */   public const Cpu.Pin D44 = Pins.PC9;
        /* CN8.6   */   public const Cpu.Pin D45 = Pins.PC10;
        /* CN8.8   */   public const Cpu.Pin D46 = Pins.PC11;
        /* CN8.10  */   public const Cpu.Pin D47 = Pins.PC12;
        /* CN8.12  */   public const Cpu.Pin D48 = Pins.PD2;
        /* CN8.14  */   public const Cpu.Pin D49 = Pins.PG2;
        /* CN8.16  */   public const Cpu.Pin D50 = Pins.PG3;

        /* CN9.2   */   public const Cpu.Pin D51 = Pins.PD7;
        /* CN9.4   */   public const Cpu.Pin D52 = Pins.PD6;
        /* CN9.6   */   public const Cpu.Pin D53 = Pins.PD5;
        /* CN9.8   */   public const Cpu.Pin D54 = Pins.PD4;
        /* CN9.10  */   public const Cpu.Pin D55 = Pins.PD3;
        /* CN9.12  */   //                   GND
        /* CN9.13  */   //                   D72
        /* CN9.14  */   public const Cpu.Pin D56 = Pins.PE2;
        /* CN9.15  */   public const Cpu.Pin D71 = Pins.PA7;
        /* CN9.16  */   public const Cpu.Pin D57 = Pins.PE4;
        /* CN9.17  */   public const Cpu.Pin D70 = Pins.PF2;
        /* CN9.18  */   public const Cpu.Pin D58 = Pins.PE5;
        /* CN9.19  */   public const Cpu.Pin D69 = Pins.PF1;
        /* CN9.20  */   public const Cpu.Pin D59 = Pins.PE6;
        /* CN9.21  */   public const Cpu.Pin D68 = Pins.PF0;
        /* CN9.22  */   public const Cpu.Pin D60 = Pins.PE3;
        /* CN9.23  */   //                   GND
        /* CN9.24  */   public const Cpu.Pin D61 = Pins.PF8;
        /* CN9.25  */   public const Cpu.Pin D67 = Pins.PD0;
        /* CN9.26  */   public const Cpu.Pin D62 = Pins.PF7;
        /* CN9.27  */   public const Cpu.Pin D66 = Pins.PD1;
        /* CN9.28  */   public const Cpu.Pin D63 = Pins.PF9;
        /* CN9.29  */   public const Cpu.Pin D65 = Pins.PG0;
        /* CN9.30  */   public const Cpu.Pin D64 = Pins.PG1;

        /* CN7.1   */   public const Cpu.Pin D16 = Pins.PC6;
        /* CN7.3   */   public const Cpu.Pin D17 = Pins.PB15;
        /* CN7.5   */   public const Cpu.Pin D18 = Pins.PB13;
        /* CN7.7   */   public const Cpu.Pin D19 = Pins.PB12;
        /* CN7.9   */   public const Cpu.Pin D20 = Pins.PA15;
        /* CN7.11  */   public const Cpu.Pin D21 = Pins.PC7;
        /* CN7.13  */   public const Cpu.Pin D22 = Pins.PB5;
        /* CN7.15  */   public const Cpu.Pin D23 = Pins.PB3;
        /* CN7.17  */   public const Cpu.Pin D24 = Pins.PA4;
        /* CN7.19  */   public const Cpu.Pin D25 = Pins.PB4;

        /* CN10.1  */   //                   AVDD
        /* CN10.3  */   //                   AGND
        /* CN10.5  */   //                   GND
        /* CN10.7  */   public const Cpu.Pin A6  = Pins.PB1;
        /* CN10.9  */   public const Cpu.Pin A7  = Pins.PC2;
        /* CN10.11 */   public const Cpu.Pin A8  = Pins.PF4;
        /* CN10.13 */   public const Cpu.Pin D26 = Pins.PB6;
        /* CN10.15 */   public const Cpu.Pin D27 = Pins.PB2;
        /* CN10.17 */   //                   GND
        /* CN10.18 */   public const Cpu.Pin D42 = Pins.PE8;
        /* CN10.19 */   public const Cpu.Pin D28 = Pins.PD13;
        /* CN10.20 */   public const Cpu.Pin D41 = Pins.PE7;
        /* CN10.21 */   public const Cpu.Pin D29 = Pins.PD12;
        /* CN10.22 */   //                   GND
        /* CN10.23 */   public const Cpu.Pin D30 = Pins.PD11;
        /* CN10.24 */   public const Cpu.Pin D40 = Pins.PE10;
        /* CN10.25 */   public const Cpu.Pin D31 = Pins.PE2;
        /* CN10.26 */   public const Cpu.Pin D39 = Pins.PE12;
        /* CN10.27 */   //                   GND
        /* CN10.28 */   public const Cpu.Pin D38 = Pins.PE14;
        /* CN10.29 */   public const Cpu.Pin D32 = Pins.PA0;
        /* CN10.30 */   public const Cpu.Pin D37 = Pins.PE15;
        /* CN10.31 */   public const Cpu.Pin D33 = Pins.PB0;
        /* CN10.32 */   public const Cpu.Pin D36 = Pins.PB10;
        /* CN10.33 */   public const Cpu.Pin D34 = Pins.PE0;
        /* CN10.34 */   public const Cpu.Pin D35 = Pins.PB11;
    }

    //
    // ST Morpho Headers
    //
    public static class MorphoPins
    {
        public static class CN11
        {
            public const Cpu.Pin P1  = Pins.PC10;
            public const Cpu.Pin P2  = Pins.PC11;
            public const Cpu.Pin P3  = Pins.PC12;
            public const Cpu.Pin P4  = Pins.PD2;
            //                   P5    VDD
            //                   P6    E5V
            //                   P7    BOOT0
            //                   P8    GND
            public const Cpu.Pin P9  = Pins.PF6;
            //                   P10
            public const Cpu.Pin P11 = Pins.PF7;
            //                   P12   IOREF
            public const Cpu.Pin P13 = Pins.PA13;
            //                   P14   RESET
            public const Cpu.Pin P15 = Pins.PA14;
            //                   P16   +3V3
            public const Cpu.Pin P17 = Pins.PA15;
            //                   P18   +5V
            //                   P19   GND
            //                   P20   GND
            public const Cpu.Pin P21 = Pins.PB7;
            //                   P22   GND
            public const Cpu.Pin P23 = Pins.PC13;
            //                   P24   VIN
            public const Cpu.Pin P25 = Pins.PC14;
            //                   P26
            public const Cpu.Pin P27 = Pins.PC15;
            public const Cpu.Pin P28 = Pins.PA0;
            //                   P29 = Pins.PH0
            public const Cpu.Pin P30 = Pins.PA1;
            //                   P31 = Pins.PH1
            public const Cpu.Pin P32 = Pins.PA4;
            //                   P33   VBAT
            public const Cpu.Pin P34 = Pins.PB0;
            public const Cpu.Pin P35 = Pins.PC2;
            public const Cpu.Pin P36 = Pins.PC1;
            public const Cpu.Pin P37 = Pins.PC3;
            public const Cpu.Pin P38 = Pins.PC0;
            public const Cpu.Pin P39 = Pins.PD4;
            public const Cpu.Pin P40 = Pins.PD3;
            public const Cpu.Pin P41 = Pins.PD5;
            public const Cpu.Pin P42 = Pins.PG2;
            public const Cpu.Pin P43 = Pins.PD6;
            public const Cpu.Pin P44 = Pins.PG3;
            public const Cpu.Pin P45 = Pins.PD7;
            public const Cpu.Pin P46 = Pins.PE2;
            public const Cpu.Pin P47 = Pins.PE3;
            public const Cpu.Pin P48 = Pins.PE4;
            //                   P49   GND
            public const Cpu.Pin P50 = Pins.PE5;
            public const Cpu.Pin P51 = Pins.PF1;
            public const Cpu.Pin P52 = Pins.PF2;
            public const Cpu.Pin P53 = Pins.PF0;
            public const Cpu.Pin P54 = Pins.PF8;
            public const Cpu.Pin P55 = Pins.PD1;
            public const Cpu.Pin P56 = Pins.PF9;
            public const Cpu.Pin P57 = Pins.PD0;
            public const Cpu.Pin P58 = Pins.PG1;
            public const Cpu.Pin P59 = Pins.PG0;
            //                   P60   GND
            public const Cpu.Pin P61 = Pins.PE1;
            public const Cpu.Pin P62 = Pins.PE6;
            public const Cpu.Pin P63 = Pins.PG9;
            public const Cpu.Pin P64 = Pins.PG15;
            public const Cpu.Pin P65 = Pins.PG12;
            public const Cpu.Pin P66 = Pins.PG10;
            //                   P67
            public const Cpu.Pin P68 = Pins.PG13;
            public const Cpu.Pin P69 = Pins.PD9;
            public const Cpu.Pin P70 = Pins.PG11;
        }

        public static class CN12
        {
            public const Cpu.Pin P1  = Pins.PC9;
            public const Cpu.Pin P2  = Pins.PC8;
            public const Cpu.Pin P3  = Pins.PB8;
            public const Cpu.Pin P4  = Pins.PC6;
            public const Cpu.Pin P5  = Pins.PB9;
            public const Cpu.Pin P6  = Pins.PC5;
            //                   P7    AVDD
            //                   P8    U5V
            //                   P9    GND
            public const Cpu.Pin P10 = Pins.PD8;
            public const Cpu.Pin P11 = Pins.PA5;
            public const Cpu.Pin P12 = Pins.PA12;
            public const Cpu.Pin P13 = Pins.PA6;
            public const Cpu.Pin P14 = Pins.PA11;
            public const Cpu.Pin P15 = Pins.PA7;
            public const Cpu.Pin P16 = Pins.PB12;
            public const Cpu.Pin P17 = Pins.PB6;
            public const Cpu.Pin P18 = Pins.PB11;
            public const Cpu.Pin P19 = Pins.PC7;
            //                   P20   GND
            public const Cpu.Pin P21 = Pins.PA9;
            public const Cpu.Pin P22 = Pins.PB2;
            public const Cpu.Pin P23 = Pins.PA8;
            public const Cpu.Pin P24 = Pins.PB1;
            public const Cpu.Pin P25 = Pins.PB10;
            public const Cpu.Pin P26 = Pins.PB15;
            public const Cpu.Pin P27 = Pins.PB4;
            public const Cpu.Pin P28 = Pins.PB14;
            public const Cpu.Pin P29 = Pins.PB5;
            public const Cpu.Pin P30 = Pins.PB13;
            public const Cpu.Pin P31 = Pins.PB3;
            //                   P32   AGND
            public const Cpu.Pin P33 = Pins.PA10;
            public const Cpu.Pin P34 = Pins.PC4;
            public const Cpu.Pin P35 = Pins.PA2;
            public const Cpu.Pin P36 = Pins.PF5;
            public const Cpu.Pin P37 = Pins.PA3;
            public const Cpu.Pin P38 = Pins.PF4;
            //                   P39   GND
            public const Cpu.Pin P40 = Pins.PE8;
            public const Cpu.Pin P41 = Pins.PD13;
            public const Cpu.Pin P42 = Pins.PF10;
            public const Cpu.Pin P43 = Pins.PD12;
            public const Cpu.Pin P44 = Pins.PE7;
            public const Cpu.Pin P45 = Pins.PD11;
            public const Cpu.Pin P46 = Pins.PD14;
            public const Cpu.Pin P47 = Pins.PE10;
            public const Cpu.Pin P48 = Pins.PD15;
            public const Cpu.Pin P49 = Pins.PE12;
            public const Cpu.Pin P50 = Pins.PF14;
            public const Cpu.Pin P51 = Pins.PE14;
            public const Cpu.Pin P52 = Pins.PE9;
            public const Cpu.Pin P53 = Pins.PE15;
            //                   P54   GND
            public const Cpu.Pin P55 = Pins.PE13;
            public const Cpu.Pin P56 = Pins.PE11;
            public const Cpu.Pin P57 = Pins.PF13;
            public const Cpu.Pin P58 = Pins.PF3;
            public const Cpu.Pin P59 = Pins.PF12;
            public const Cpu.Pin P60 = Pins.PF15;
            public const Cpu.Pin P61 = Pins.PG14;
            public const Cpu.Pin P62 = Pins.PF11;
            //                   P63   GND
            public const Cpu.Pin P64 = Pins.PE0;
            public const Cpu.Pin P65 = Pins.PD10;
            public const Cpu.Pin P66 = Pins.PG8;
            public const Cpu.Pin P67 = Pins.PG7;
            public const Cpu.Pin P68 = Pins.PG5;
            public const Cpu.Pin P69 = Pins.PG4;
            public const Cpu.Pin P70 = Pins.PG6;
        }
    }


    public static class SerialPorts
    {
        public const string COM1 = "COM1";
        public const string COM2 = "COM2";
        public const string COM3 = "COM3";
        public const string COM4 = "COM4";
    }


    public static class BaudRates
    {
        public const BaudRate Baud9600   = BaudRate.Baudrate9600;
        public const BaudRate Baud19200  = BaudRate.Baudrate19200;
        public const BaudRate Baud38400  = BaudRate.Baudrate38400;
        public const BaudRate Baud57600  = BaudRate.Baudrate57600;
        public const BaudRate Baud115200 = BaudRate.Baudrate115200;
        public const BaudRate Baud230400 = BaudRate.Baudrate230400;
    }


    public static class ResistorModes
    {
        public const Port.ResistorMode PullUp   = Port.ResistorMode.PullUp;
        public const Port.ResistorMode PullDown = Port.ResistorMode.PullDown;
        public const Port.ResistorMode Disabled = Port.ResistorMode.Disabled;
    }


    public static class InterruptModes
    {
        public const Port.InterruptMode InterruptEdgeLow       = Port.InterruptMode.InterruptEdgeLow;
        public const Port.InterruptMode InterruptEdgeHigh      = Port.InterruptMode.InterruptEdgeHigh;
        public const Port.InterruptMode InterruptEdgeBoth      = Port.InterruptMode.InterruptEdgeBoth;
        public const Port.InterruptMode InterruptEdgeLevelHigh = Port.InterruptMode.InterruptEdgeLevelHigh;
        public const Port.InterruptMode InterruptEdgeLevelLow  = Port.InterruptMode.InterruptEdgeLevelLow;
        public const Port.InterruptMode InterruptNone          = Port.InterruptMode.InterruptNone;
    }


    // public static class SPI_Devices
    // {
    //     public const Microsoft.SPOT.Hardware.SPI.SPI_module SPI3 = Microsoft.SPOT.Hardware.SPI.SPI_module.SPI3;
    // }
}
