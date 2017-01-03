using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GoBus
{
    public class GoSocket
    {
        int _socketNumber;

        bool _boundToModule = false;
        bool _powerState = false;

        GoHub _hub;

        internal GoSocket(int socketNumber)
        {
            _socketNumber = socketNumber;
        }

        public void GetPhysicalResources(out Cpu.Pin gpio, out SPI.SPI_module spi_mod, out Cpu.Pin spi_cs)
        {
            Cpu.Pin led;
            _hub.GetSocketPhysicalResources(_socketNumber, out led, out gpio, out spi_mod, out spi_cs);
        }

        public void GetPhysicalResources(out Cpu.Pin[] pins)
        {
            Cpu.Pin led;
            _hub.GetSocketPhysicalResources(_socketNumber, out led, out pins);
        }

        // overload the explicit cast of int to allow a socket object to be refernced via cast
        public static explicit operator GoSocket(int socketNumber)
        {
            return GoBus.GoHub.RootHub.GetSocketBySocketNumber(socketNumber);
        }

        // overload the explicit case of 
        public static explicit operator long(GoSocket socket)
        {
            return socket._socketNumber;
        }

        internal bool BoundToModule
        {
            get
            {
                return _boundToModule;
            }
            set
            {
                _boundToModule = value;
            }
        }

        internal void SetHub(GoHub hub)
        {
            _hub = hub;
        }

        internal bool PowerState
        {
            get
            {
                return _powerState;
            }
        }

        internal void SetPowerState(bool state)
        {
            _powerState = state;
            _hub.SetSocketPowerState(_socketNumber, _powerState);
        }
    }
}
