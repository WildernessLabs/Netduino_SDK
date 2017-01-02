using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GoBus
{
    public abstract class GoModule : IDisposable 
    {
        bool _disposed = false;
        bool _boundToSocket = false;
        GoSocket _socket = null;

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing) 
        { 
            if (!_disposed)
            {
                if (disposing)
                {
                    // TODO: clean up any managed code objects (disconnect from our module, power it down, turn off the LED, etc.)
                    if (_boundToSocket)
                        UnbindSocket();
                }
                // TODO: clean up any unmanaged code objects
            }
            _disposed = true;
        } 

        ~GoModule()
        {
            Dispose(false);
        }

        protected GoSocket[] GetSocketsByUniqueId(System.Guid uniqueId)
        {
            return GoBus.GoHub.RootHub.GetSocketsByUniqueId(uniqueId);
        }

        protected Guid[] GetUniqueIdsFromUnboundSockets()
        {
            return GoBus.GoHub.RootHub.GetUniqueIdsFromUnboundSockets();
        }

        protected bool BindSocket(GoSocket socket)
        {
            return BindSocket(socket, Guid.Empty);
        }

        protected bool BindSocket(GoSocket socket, System.Guid uniqueId)
        {
            // make sure that the socket is not in use.
            if (socket.BoundToModule)
                return false;

            // if we were passed a uniqueId, make sure that the module matches.
            if (!uniqueId.Equals(Guid.Empty))
            {
                // validate the module's GUID
                if (GoBus.GoHub.RootHub.GetModuleUniqueId(socket).Equals(uniqueId) == false)
                    return false;
            }

            GoBus.GoHub.RootHub.SetSocketLedState((int)socket, true);
            socket.BoundToModule = true;
            _socket = socket;
            _boundToSocket = true;

            return true;
        }

        protected void UnbindSocket()
        {
            if (_socket != null)
            {
                GoBus.GoHub.RootHub.SetSocketLedState((int)_socket, false);
                _socket.BoundToModule = false;
            }
            _socket = null;
            _boundToSocket = false;
        }

        protected void SetSocketPowerState(bool state)
        {
            if (_socket != null)
            {
                _socket.SetPowerState(state);
            }
        }
    }
}
