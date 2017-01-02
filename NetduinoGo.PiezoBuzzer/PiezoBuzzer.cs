using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using GoBus;

namespace NetduinoGo
{
    public class PiezoBuzzer : GoModule
    {
        Guid _moduleGuid = new Guid(new byte[] { 0x80, 0xA4, 0x99, 0x8D, 0xFF, 0x15, 0x6C, 0x4B, 0x8F, 0xC5, 0xE4, 0xE8, 0x59, 0x75, 0xE3, 0xE6 }); // piezo buzzer

        InterruptPort _irqPort;
        AutoResetEvent _irqPortInterruptEvent = new AutoResetEvent(false);

        // 1 byte command, 16 bytes data/GUID, 1 byte CRC8
        private const int _frameLength = 1 + 16 + 1;
        private byte[] _writeFrameBuffer = new byte[_frameLength];
        private byte[] _readFrameBuffer = new byte[_frameLength];

        private SPI.Configuration _spiConfig = null;
        private Microsoft.SPOT.Hardware.SPI _spi;

        public PiezoBuzzer()
        {
            GoSocket[] compatibleSockets = base.GetSocketsByUniqueId(_moduleGuid);
            if (compatibleSockets.Length == 0)
                throw new Exception(); // TODO: find better exception for "module not found."

            Initialize(compatibleSockets[0]);
        }

        public PiezoBuzzer(GoSocket socket)
        {
            Initialize(socket);
        }

        private void Initialize(GoSocket socket)
        {
            // now try to bind to the socket (and verify our module's uniqueId)
            if (!base.BindSocket(socket, _moduleGuid))
                throw new ArgumentException();

            // get socket's physical pins and SPI bus
            Cpu.Pin socketGpioPin;
            SPI.SPI_module socketSpiModule;
            Cpu.Pin socketSpiSlaveSelectPin;
            //
            socket.GetPhysicalResources(out socketGpioPin, out socketSpiModule, out socketSpiSlaveSelectPin);

            _spiConfig = new SPI.Configuration(socketSpiSlaveSelectPin, false, 0, 0, false, false, 500, socketSpiModule);
            _spi = new SPI(_spiConfig);

            // wire up event handlers
            _irqPort = new InterruptPort((Cpu.Pin)socketGpioPin, false, Port.ResistorMode.PullUp, Port.InterruptMode.InterruptEdgeLow);
            _irqPort.OnInterrupt += _irqPort_OnInterrupt;
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                // clean up any managed code objects
                _irqPort.Dispose();
                _spi.Dispose();
            }
            // clean up any unmanaged code objects

            // dispose of our base object
            base.Dispose(disposing);
        } 

        void _irqPort_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            _irqPortInterruptEvent.Set();
        }

        public void SetFrequency(double frequency)
        {
            byte frequencyHighByte;
            byte frequencyLowByte;

            if (frequency <= 0)
            {
                frequencyHighByte = 0;
                frequencyLowByte = 0;
            }
            else
            {
                // The Piezo Buzzer module time base is 1000 kHz
                int divider = (int)(1000000F / frequency + 0.5);

                // The data sent to module is direct value for the TIM2 ARRH, ARRL
                // registers, to save expensive division on STM8S.
                frequencyHighByte = (byte)(divider >> 8);  // ARRH
                frequencyLowByte = (byte)(divider);        // ARRL
            }

            int iRetry = 0;
            while (iRetry < 36)
            {
                _writeFrameBuffer[0] = 0x80;
                _writeFrameBuffer[1] = 0x02;  // Module command 'SetColor'
                _writeFrameBuffer[2] = frequencyHighByte;
                _writeFrameBuffer[3] = frequencyLowByte;
                _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                _spi.Write(_writeFrameBuffer);

                double verifyFrequency;
                GetFrequency(out verifyFrequency);

                if (frequency - verifyFrequency <= 100)
                    return;

                iRetry++;
            }
        }

        private void GetFrequency(out double frequency)
        {
            _writeFrameBuffer[0] = 0x80;
            _writeFrameBuffer[1] = 0x01;  // Module command 'Get frequency'
            for (int i = 2; i < _writeFrameBuffer.Length - 1; i++)
                _writeFrameBuffer[i] = 0x00;
            _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
            _spi.Write(_writeFrameBuffer);

            // now, wait for irq to be asserted
            bool responseReceived = false;
            int numRetries = 4;
            int iRetry = 0;
            while (true)
            {
                responseReceived = _irqPortInterruptEvent.WaitOne(10, false);

                if (responseReceived || (iRetry >= numRetries - 1))
                {
                    // parse and return the response
                    _writeFrameBuffer[0] = 0x00; // get data from module
                    _writeFrameBuffer[1] = 0x00;
                    _writeFrameBuffer[2] = 0x00;
                    _writeFrameBuffer[3] = 0x00;
                    _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                    _spi.WriteRead(_writeFrameBuffer, _readFrameBuffer);

                    if (_readFrameBuffer[2] == 2)
                    {
                        // Received three byte RGB value
                        byte frequencyHighByte = _readFrameBuffer[3];
                        byte frequencyLowByte = _readFrameBuffer[4];
                        int divider = (int)((frequencyHighByte << 8) | frequencyLowByte);
                        frequency = (divider > 0) ? 1000000F / divider : 0F;
                        return;
                    }
                    else if (iRetry >= numRetries - 1)
                    {
                        frequency = 0F;
                        return;
                    }
                    else
                    {
                        // failed to receive response; retry
                        responseReceived = false;
                    }
                }
                else
                {
                    // TODO: limit our retries to a handful of retries (or maybe a few dozen -- quickly)
                    // retry infinitely
                    _writeFrameBuffer[0] = 0x80;
                    _writeFrameBuffer[1] = 0x01;  // Module command 'Get frequency'
                    _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                    _spi.Write(_writeFrameBuffer);
                }

                iRetry++;
            }
        }

    }
}
