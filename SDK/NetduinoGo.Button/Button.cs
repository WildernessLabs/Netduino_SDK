using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using GoBus;

namespace NetduinoGo
{
    public class Button : GoModule
    {
        Guid _moduleGuid = new Guid(new byte[] { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }); // button

        InterruptPort _interruptPort;

        // 1 byte command, 16 bytes data/GUID, 1 byte CRC8
        private const int _frameLength = 1 + 16 + 1;
        private byte[] _writeFrameBuffer = new byte[_frameLength];
        private byte[] _readFrameBuffer = new byte[_frameLength];

        private SPI.Configuration _spiConfig = null;
        private Microsoft.SPOT.Hardware.SPI _spi;

        private bool _isPressed = false;

        public delegate void ButtonEventHandler(object sender, bool isPressed);
        public event ButtonEventHandler ButtonPressed;
        public event ButtonEventHandler ButtonReleased;

        public Button()
        {
            GoSocket[] compatibleSockets = base.GetSocketsByUniqueId(_moduleGuid);
            if (compatibleSockets.Length == 0)
                throw new Exception(); // TODO: find better exception for "module not found."

            Initialize(compatibleSockets[0]);
        }

        public Button(GoSocket socket)
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
            _interruptPort = new InterruptPort((Cpu.Pin)socketGpioPin, false, Port.ResistorMode.PullUp, Port.InterruptMode.InterruptEdgeBoth);
            _interruptPort.OnInterrupt += _interruptPort_OnInterrupt;

            // read initial button state
            _isPressed = !_interruptPort.Read();
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                // clean up any managed code objects
                _interruptPort.Dispose();
            }
            // clean up any unmanaged code objects

            // dispose of our base object
            base.Dispose(disposing);
        } 

        #region LegacyCompatibility
        // NOTE: this is legacy support for projects using the 1.0.0 button class
        [System.ComponentModel.EditorBrowsable(System.ComponentModel.EditorBrowsableState.Never)]
        public bool ButtonState
        {
            get
            {
                return IsPressed;
            }
        }
        #endregion

        public bool IsPressed
        {
            get
            {
                return _isPressed;
            }
        }

        void _interruptPort_OnInterrupt(uint pin, uint state, DateTime time)
        {
            _isPressed = (state == 0);
            if (!_isPressed)
            {
                // released
                if (ButtonReleased != null)
                    ButtonReleased(this, false);
            }
            else
            {
                // pressed
                if (ButtonPressed != null)
                    ButtonPressed(this, true);
            }
        }
    }
}
