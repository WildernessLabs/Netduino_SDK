using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using GoBus;

namespace NetduinoGo
{
    public class Potentiometer : GoModule
    {
        Guid _moduleGuid = new Guid(new byte[] { 128, 33, 122, 36, 198, 209, 19, 77, 138, 35, 124, 44, 6, 173, 150, 188 }); // potentiometer

        InterruptPort _irqPort;
        AutoResetEvent _irqPortInterruptEvent = new AutoResetEvent(false);

        // 1 byte command, 16 bytes data/GUID, 1 byte CRC8
        private const int _frameLength = 1 + 16 + 1;
        private byte[] _writeFrameBuffer = new byte[_frameLength];
        private byte[] _readFrameBuffer = new byte[_frameLength];

        private SPI.Configuration _spiConfig = null;
        private Microsoft.SPOT.Hardware.SPI _spi;

        double _lastAdcValue = 0;

        public Potentiometer()
        {
            GoSocket[] compatibleSockets = base.GetSocketsByUniqueId(_moduleGuid);
            if (compatibleSockets.Length == 0)
                throw new Exception(); // TODO: find better exception for "module not found."

            Initialize(compatibleSockets[0]);
        }

        public Potentiometer(GoSocket socket)
        {
            Initialize(socket);
        }

        void Initialize(GoSocket socket)
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

        public double GetValue()
        {
            _writeFrameBuffer[0] = 0x80;
            _writeFrameBuffer[1] = 0x01;  // Module command 'Get ADC value'
            _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
            _spi.Write(_writeFrameBuffer);

            // now, wait for irq to be asserted
            bool responseReceived = false;
            int numRetries = 36; //4
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
                        // Received two byte ADC value
                        double adcValue = ((short)((_readFrameBuffer[3] << 8) | _readFrameBuffer[4])) / (double)1023;
                        _lastAdcValue = adcValue;
                        return adcValue;
                    }
                    else if (iRetry >= numRetries - 1)
                    {
                        return _lastAdcValue;
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
                    _writeFrameBuffer[1] = 0x01;  // Module command 'Get ADC value'
                    _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                    _spi.Write(_writeFrameBuffer);
                }

                iRetry++;
            }
        }
    }
}
