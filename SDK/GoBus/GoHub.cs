using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GoBus
{
    internal class GoHub : GoModule
    {
        static GoHub _rootHub;

        const int _socketCount = 8;

        GoSocket[] _sockets;

        OutputPort[] _socketLeds;

        //bool _disposed = false; 

        OutputPort _powerEnable;

        static GoHub()
        {
            // create our root hub
            _rootHub = new GoHub();
        }

        public static GoHub RootHub
        {
            get
            {
                return _rootHub;
            }
        }

        internal GoHub()
        {
            // initialize our sockets
            _sockets = new GoSocket[_socketCount];
            _socketLeds = new OutputPort[_socketCount];
            for (int iSocket = 0; iSocket < _socketCount; iSocket++)
            {
                _sockets[iSocket] = new GoSocket(iSocket + 1);
                _socketLeds[iSocket] = new OutputPort(GetSocketLedPin(iSocket + 1), false);
                _sockets[iSocket].SetHub(this);
                _sockets[iSocket].SetPowerState(true);
            }

            // TODO: do this in native code during clr startup, to ensure the pullup state.
            // put uart RX pins in input, pull-up mode by creating and then disposing of them.
            InputPort uart1rx = new InputPort((Cpu.Pin)0x01, false, Port.ResistorMode.PullUp);
            InputPort uart2rx = new InputPort((Cpu.Pin)0x03, false, Port.ResistorMode.PullUp);
            uart1rx.Dispose();
            uart2rx.Dispose();

            // enable our power output
            _powerEnable = new OutputPort((Cpu.Pin)0x0A, false);
        }

        //public GoSocket[] GetSockets()
        //{
        //        return _sockets;
        //}

        internal GoSocket GetSocketBySocketNumber(int socketNumber)
        {
            if (socketNumber >0 && socketNumber <= _socketCount)
                return _sockets[socketNumber - 1];
            else
                throw new NotSupportedException();
        }

/*
        protected override void Dispose(bool disposing)
        {
            if (!_disposed)
            {             
                // dispose of managed resources here
            }
            base.Dispose(disposing);
            _disposed = true;
        }
*/

        internal void SetSocketLedState(int socketNumber, bool state)
        {
            _socketLeds[socketNumber - 1].Write(state);
        }

        static OutputPort powershift_outputenable = new OutputPort((Cpu.Pin)10, true);
        static OutputPort powershift_cs = new OutputPort((Cpu.Pin)18, false);
        static OutputPort powershift_clk = new OutputPort((Cpu.Pin)42, false);
        static OutputPort powershift_mosi = new OutputPort((Cpu.Pin)21, false);

        internal void SetSocketPowerState(int socketNumber, bool state)
        {
            // then use spi channel 3 to update power state for all gosockets
            for (int iSocket = 0; iSocket < _sockets.Length; iSocket++)
            {
                if (_sockets[_sockets.Length - iSocket - 1] != null)
                    powershift_mosi.Write(_sockets[_sockets.Length - iSocket - 1].PowerState);
                else
                    powershift_mosi.Write(false);
                powershift_clk.Write(true);
                powershift_clk.Write(false);
            }
            // Latch (rising edge)
            powershift_cs.Write(true);
            powershift_cs.Write(false);
        }

        private Cpu.Pin GetSocketLedPin(int socketNumber)
        {
            Cpu.Pin led;
            Cpu.Pin[] pins;
            GetSocketPhysicalResources(socketNumber, out led, out pins);
            return led;
        }

        internal void GetSocketPhysicalResources(int socketNumber, out Cpu.Pin led, out Cpu.Pin gpio, out SPI.SPI_module spi_mod, out Cpu.Pin spi_cs)
        {
            switch (socketNumber)
            {
                case 1:
                    gpio = (Cpu.Pin)0x20;   //Pins.GPIO_PIN_C_0;
                    spi_mod = SPI.SPI_module.SPI1;
                    spi_cs = (Cpu.Pin)0x1B; //Pins.GPIO_PIN_B_11;
                    led = (Cpu.Pin)0x16;    //Pins.GPIO_PIN_B_6;
                    break;
                case 2:
                    gpio = (Cpu.Pin)0x21;   //Pins.GPIO_PIN_C_1;
                    spi_mod = SPI.SPI_module.SPI1;
                    spi_cs = (Cpu.Pin)0x2B; //Pins.GPIO_PIN_C_11;
                    led = (Cpu.Pin)0x17;    //Pins.GPIO_PIN_B_7;
                    break;
                case 3:
                    gpio = (Cpu.Pin)0x22;   //Pins.GPIO_PIN_C_2;
                    spi_mod = SPI.SPI_module.SPI1;
                    spi_cs = (Cpu.Pin)0x04; //Pins.GPIO_PIN_A_4;
                    led = (Cpu.Pin)0x18;    //Pins.GPIO_PIN_B_8;
                    break;
                case 4:
                    gpio = (Cpu.Pin)0x23;   //Pins.GPIO_PIN_C_3;
                    spi_mod = SPI.SPI_module.SPI1;
                    spi_cs = (Cpu.Pin)0x08; //Pins.GPIO_PIN_A_8;
                    led = (Cpu.Pin)0x19;    //Pins.GPIO_PIN_B_9;
                    break;
                case 5:
                    gpio = (Cpu.Pin)0x24;   //Pins.GPIO_PIN_C_4;
                    spi_mod = SPI.SPI_module.SPI2;
                    spi_cs = (Cpu.Pin)0x10; //Pins.GPIO_PIN_B_0;
                    led = (Cpu.Pin)0x26;    //Pins.GPIO_PIN_C_6;
                    break;
                case 6:
                    gpio = (Cpu.Pin)0x25;   //Pins.GPIO_PIN_C_5;
                    spi_mod = SPI.SPI_module.SPI2;
                    spi_cs = (Cpu.Pin)0x11; //Pins.GPIO_PIN_B_1;
                    led = (Cpu.Pin)0x27;    //Pins.GPIO_PIN_C_7;
                    break;
                case 7:
                    gpio = (Cpu.Pin)0x2C;   //Pins.GPIO_PIN_C_12;
                    spi_mod = SPI.SPI_module.SPI2;
                    spi_cs = (Cpu.Pin)0x32; //Pins.GPIO_PIN_D_2;
                    led = (Cpu.Pin)0x28;    //Pins.GPIO_PIN_C_8;
                    break;
                case 8:
                    gpio = (Cpu.Pin)0x1A;   //Pins.GPIO_PIN_B_10;
                    spi_mod = SPI.SPI_module.SPI2;
                    spi_cs = (Cpu.Pin)0x1C; //Pins.GPIO_PIN_B_12;
                    led = (Cpu.Pin)0x29;    //Pins.GPIO_PIN_C_9;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }

        internal void GetSocketPhysicalResources(int socketNumber, out Cpu.Pin led, out Cpu.Pin[] pins)
        {
            pins = new Cpu.Pin[7];
            SPI.SPI_module spi_mod;
            GetSocketPhysicalResources(socketNumber, out led, out pins[0], out spi_mod, out pins[3]);
            switch (spi_mod)
            {
                case SPI.SPI_module.SPI1:
                    pins[1] = (Cpu.Pin)0x02;    //Pins.GPIO_PIN_A_2;
                    pins[2] = (Cpu.Pin)0x03;    //Pins.GPIO_PIN_A_3;
                    pins[4] = (Cpu.Pin)0x07;    //Pins.GPIO_PIN_A_7;
                    pins[5] = (Cpu.Pin)0x06;    //Pins.GPIO_PIN_A_6;
                    pins[6] = (Cpu.Pin)0x05;    //Pins.GPIO_PIN_A_5;
                    break;
                case SPI.SPI_module.SPI2:
                    pins[1] = (Cpu.Pin)0x00;    //Pins.GPIO_PIN_A_0;
                    pins[2] = (Cpu.Pin)0x01;    //Pins.GPIO_PIN_A_1;
                    pins[4] = (Cpu.Pin)0x2F;    //Pins.GPIO_PIN_B_15;
                    pins[5] = (Cpu.Pin)0x2E;    //Pins.GPIO_PIN_B_14;
                    pins[6] = (Cpu.Pin)0x2D;    //Pins.GPIO_PIN_B_13;
                    break;
                default:
                    throw new NotSupportedException();
            }
        }

        internal new GoSocket[] GetSocketsByUniqueId(System.Guid uniqueId)
        {
            System.Collections.ArrayList sockets = new System.Collections.ArrayList();

            for (int iSocket = 0; iSocket < _sockets.Length; iSocket++)
            {
                if (!_sockets[iSocket].BoundToModule)
                {
                    Guid guid = GetModuleUniqueId(_sockets[iSocket]);
                    if (guid.Equals(uniqueId))
                        sockets.Add(_sockets[iSocket]);
                }
            }

            return (GoSocket[])sockets.ToArray(typeof(GoSocket));   
        }

        internal new Guid[] GetUniqueIdsFromUnboundSockets()
        {
            System.Collections.ArrayList guids = new System.Collections.ArrayList();

            for (int iSocket = 0; iSocket < _sockets.Length; iSocket++)
            {
                if (!_sockets[iSocket].BoundToModule)
                {
                    Guid guid = GetModuleUniqueId(_sockets[iSocket]);
                    if (!guid.Equals(Guid.Empty))
                        guids.Add(guid);
                }
            }

            return (Guid[])guids.ToArray(typeof(Guid));
        }

        internal Guid GetModuleUniqueId(GoSocket socket)
        {
            int frameLength = 1 + 16 + 1;
            byte[] writeFrameBuffer = new byte[frameLength];
            byte[] readFrameBuffer = new byte[frameLength];

            // get socket's physical pins and SPI bus
            Cpu.Pin socketGpioPin;
            SPI.SPI_module socketSpiModule;
            Cpu.Pin socketSpiSlaveSelectPin;
            //
            socket.GetPhysicalResources(out socketGpioPin, out socketSpiModule, out socketSpiSlaveSelectPin);

            SPI.Configuration spiConfig;
            SPI spi;

            try
            {
                spiConfig = new SPI.Configuration(socketSpiSlaveSelectPin, false, 0, 0, false, false, 500, socketSpiModule);
                spi = new SPI(spiConfig);
            }
            catch
            {
                return Guid.Empty;
            }

            // TODO: move the socket4ChipSelectDeassert logic to native code (this suppresses the MCO)
            OutputPort socket4ChipSelectDeassert = null;
            if (Port.ReservePin((Cpu.Pin)0x08, true))
            {
                socket4ChipSelectDeassert = new OutputPort((Cpu.Pin)0x08, true);
            }

            int iEnumAttempt = 0;
            int emptyFrameCount = 0;

            byte basicDeviceType = 0x00;

            try
            {
                while (iEnumAttempt < 72) // NOTE: 72 attempts allow our frame to rotate four full times in case of frame mis-alignment; this should be unnecessary in time.
                {
                    writeFrameBuffer[0] = 0xFE; // 'Enumerate' command
                    for (int i = 1; i < writeFrameBuffer.Length - 1; i++)
                        writeFrameBuffer[i] = 0xFF; // filler -- so that module sends ID
                    writeFrameBuffer[writeFrameBuffer.Length - 1] = CRC8.Compute8(writeFrameBuffer, 0, writeFrameBuffer.Length - 1);
                    spi.WriteRead(writeFrameBuffer, readFrameBuffer);

                    // validate the response...
                    bool replyIsBlank = true;
                    for (int i = 1; i < readFrameBuffer.Length - 1; i++)
                    {
                        if (readFrameBuffer[i] != 0)
                            replyIsBlank = false;
                    }

                    if (replyIsBlank)
                    {
                        // no response; try again (up to 4 times total)
                        emptyFrameCount++;
                        if (emptyFrameCount > 4)
                            return Guid.Empty;
                    }
                    else if (CRC8.Compute8(readFrameBuffer) == 0)
                    {
                        return new Guid(Microsoft.SPOT.Hardware.Utility.ExtractRangeFromArray(readFrameBuffer, 1, readFrameBuffer.Length - 2));
                    }
                    else if (readFrameBuffer[1] != 0)
                    {
                        bool replyIsBasicDevice = true;
                        // did not pass CRC; see if it's a basic device type (first byte is id, then all other bytes including CRC will be zeros)
                        for (int i = 2; i < readFrameBuffer.Length; i++)
                        {
                            if (readFrameBuffer[i] != 0)
                                replyIsBasicDevice = false;
                        }

                        if (replyIsBasicDevice)
                        {
                            if (basicDeviceType == 0)
                            {
                                // first successful query; store this id and we'll verify by second enum
                                basicDeviceType = readFrameBuffer[1];
                            }
                            else
                            {
                                if (basicDeviceType == readFrameBuffer[1])
                                {
                                    return new Guid(Microsoft.SPOT.Hardware.Utility.ExtractRangeFromArray(readFrameBuffer, 1, readFrameBuffer.Length - 2));
                                }
                                else
                                {
                                    // mismatch; could not enumerate successfully
                                    return Guid.Empty;
                                }
                            }
                        }
                    }
                    else
                    {
                        // corrupt message; try again
                    }

                    iEnumAttempt++;
                }
            }
            finally
            {
                if (socket4ChipSelectDeassert != null)
                {
                    socket4ChipSelectDeassert.Dispose();
                    Port.ReservePin((Cpu.Pin)0x08, false);
                }
            }

            // if we reach here, we could not retrieve a GUID.
            return Guid.Empty;
        }
    }
}
