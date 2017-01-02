using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using GoBus;

namespace NetduinoGo
{
    public class RgbLed : GoModule
    {
        Guid _moduleGuid = new Guid(new byte[] { 128, 57, 232, 43, 85, 88, 235, 72, 171, 158, 72, 211, 253, 174, 140, 238 }); // rgb led

        InterruptPort _irqPort;
        AutoResetEvent _irqPortInterruptEvent = new AutoResetEvent(false);

        // 1 byte command, 16 bytes data/GUID, 1 byte CRC8
        private const int _frameLength = 1 + 16 + 1;
        private byte[] _writeFrameBuffer = new byte[_frameLength];
        private byte[] _readFrameBuffer = new byte[_frameLength];

        private byte _red = 0;
        private byte _green = 0;
        private byte _blue = 0;
        private double _brightness = (double)1 / 3; // default brightness to 33%

        private SPI.Configuration _spiConfig = null;
        private Microsoft.SPOT.Hardware.SPI _spi;

        public RgbLed()
        {
            GoSocket[] compatibleSockets = base.GetSocketsByUniqueId(_moduleGuid);
            if (compatibleSockets.Length == 0)
                throw new Exception(); // TODO: find better exception for "module not found."

            Initialize(compatibleSockets[0]);
        }

        public RgbLed(GoSocket socket)
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

        public void SetBrightness(double brightness)
        {
            _brightness = brightness;
            UpdateColor(); // update the rgb led's colors
        }

        public void SetColor(byte red, byte green, byte blue)
        {
            _red = red;
            _green = green;
            _blue = blue;

            UpdateColor(); // update the rgb led's colors
        }

        private void UpdateColor()
        {
            // adjust red, green, and blue for brightness
            byte red = (byte)((double)_red * _brightness);
            byte green = (byte)((double)_green * _brightness);
            byte blue = (byte)((double)_blue * _brightness);

            int iRetry = 0;
            while (iRetry < 36)
            {
                _writeFrameBuffer[0] = 0x80;
                _writeFrameBuffer[1] = 0x02;  // Module command 'SetColor'
                _writeFrameBuffer[2] = red;   // Red
                _writeFrameBuffer[3] = green; // Green
                _writeFrameBuffer[4] = blue;  // Blue
                _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                _spi.Write(_writeFrameBuffer);

                byte verifyRed, verifyGreen, verifyBlue;
                GetColor(out verifyRed, out verifyGreen, out verifyBlue);

                if (red == verifyRed && green == verifyGreen && blue == verifyBlue)
                    return;

                iRetry++;
            }

        }

        private void GetColor(out byte red, out byte green, out byte blue)
        {
            _writeFrameBuffer[0] = 0x80;
            _writeFrameBuffer[1] = 0x01;  // Module command 'Get RGB LED color values'
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
                responseReceived = _irqPortInterruptEvent.WaitOne(3, false);

                if (responseReceived || (iRetry >= numRetries - 1))
                {
                    // parse and return the response
                    _writeFrameBuffer[0] = 0x00; // get data from module
                    _writeFrameBuffer[1] = 0x00;
                    _writeFrameBuffer[2] = 0x00;
                    _writeFrameBuffer[3] = 0x00;
                    _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                    _spi.WriteRead(_writeFrameBuffer, _readFrameBuffer);

                    if (_readFrameBuffer[2] == 3)
                    {
                        // Received three byte RGB value
                        red = _readFrameBuffer[3];
                        green = _readFrameBuffer[4];
                        blue = _readFrameBuffer[5];
                        return;
                    }
                    else if (iRetry >= numRetries - 1)
                    {
                        red = 0;
                        blue = 0;
                        green = 0;
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
                    _writeFrameBuffer[1] = 0x01;  // Module command 'Get RGB LED color values'
                    _writeFrameBuffer[_writeFrameBuffer.Length - 1] = CRC8.Compute8(_writeFrameBuffer, 0, _writeFrameBuffer.Length - 1);
                    _spi.Write(_writeFrameBuffer);
                }

                iRetry++;
            }
        }
    }
}
