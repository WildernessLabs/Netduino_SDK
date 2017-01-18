////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using Microsoft.SPOT.Net.Security;
using System.Runtime.CompilerServices;
using System.Security.Cryptography.X509Certificates;

namespace Microsoft.SPOT.Net.Security
{
    public class SslStream : NetworkStream
    {
        // Internal flags
        private int _sslContext; /* NOTE: for CC3100, SSL is handled by the BSD Sockets API layer on a per-socket layer, so we just set _sslContext to zero once SSL is enabled. */
        private bool _isServer;

        //--//

        public SslStream(Socket socket)
            : base(socket, false)
        {
            if (SocketType.Stream != (SocketType)_socketType)
            {
                throw new NotSupportedException();
            }

            _sslContext = -1;
            _isServer = false;
        }
        
        public void AuthenticateAsClient(string targetHost, params SslProtocols[] sslProtocols)
        {
            AuthenticateAsClient(targetHost, null, null, SslVerification.NoVerification, sslProtocols);
        }

        public void AuthenticateAsClient(string targetHost, X509Certificate cert, SslVerification verify, params SslProtocols[] sslProtocols)
        {
            AuthenticateAsClient(targetHost, cert, null, verify, sslProtocols);
        }

        public void AuthenticateAsClient(string targetHost, X509Certificate cert, X509Certificate[] ca, SslVerification verify, params SslProtocols[] sslProtocols)
        {
            Authenticate(false, targetHost, cert, ca, verify, sslProtocols);
        }

        public void AuthenticateAsServer(X509Certificate cert, SslVerification verify, params SslProtocols[] sslProtocols)
        {
            AuthenticateAsServer(cert, null, verify, sslProtocols);
        }

        public void AuthenticateAsServer(X509Certificate cert, X509Certificate[] ca, SslVerification verify, params SslProtocols[] sslProtocols)
        {
            Authenticate(true, "", cert, ca, verify, sslProtocols);
        }

        public void UpdateCertificates(X509Certificate cert, X509Certificate[] ca)
        {
            if(_sslContext == -1) throw new InvalidOperationException();

            throw new NotImplementedException();
            //SslNative.UpdateCertificates(_sslContext, cert, ca);
        }

        internal void Authenticate(bool isServer, string targetHost, X509Certificate certificate, X509Certificate[] ca, SslVerification verify, params SslProtocols[] sslProtocols)
        {
            SslProtocols vers = (SslProtocols)0;

            if (-1 != _sslContext) throw new InvalidOperationException();

            for (int i = sslProtocols.Length - 1; i >= 0; i--)
            {
                vers |= sslProtocols[i];
            }

            _isServer = isServer;

            try
            {
                if (isServer)
                {
                    throw new NotImplementedException();
                    //_sslContext = SslNative.SecureServerInit((int)vers, (int)verify, certificate, ca);
                    //SslNative.SecureAccept(_sslContext, _socket);
                }
                else
                {
                    bool secMethodSSL3 = false;
                    bool secMethodTLS1 = false;
                    for (int iProtocol = 0; iProtocol < sslProtocols.Length; iProtocol++)
                    {
                        switch (sslProtocols[iProtocol])
                        {
                            case SslProtocols.None:
                                throw new ArgumentException("sslProtocols");
                            case SslProtocols.SSLv3:
                                secMethodSSL3 = true;
                                break;
                            case SslProtocols.TLSv1:
                                secMethodTLS1 = true;
                                break;
                            case SslProtocols.Default:
                                secMethodSSL3 = true;
                                secMethodTLS1 = true;
                                break;
                        }
                    }
                    byte sslProtocolOption = 0;
                    if (secMethodSSL3 && secMethodTLS1)
                        sslProtocolOption = 4; /* SL_SO_SEC_METHOD_SSLv3_TLSV1_2 */
                    else if (secMethodSSL3)
                        sslProtocolOption = 0; /* SL_SO_SEC_METHOD_SSLV3 */
                    else if (secMethodTLS1)
                        sslProtocolOption = 1; /* SL_SO_SEC_METHOD_TLSV1 */
                    else
                        throw new ArgumentException("sslProtocols");

                    /* NOTE: we now pass a message to the socket asking it to reconnect as SSL (using our specific SSL options) -- and then have it update its handle!
                     *       we should also make sure that no other sockets are opened during the operation--so that no other threads sneak in and steal the last socket handle. */
                    byte[] addressAndHandle = new byte[10];
                    /* fill bytes 0-7 with our socket's target IP address, so we know where to connect to! */
                    addressAndHandle[2] = (byte)((((IPEndPoint)_socket.RemoteEndPoint).Port >> 8) & 0xFF);
                    addressAndHandle[3] = (byte)(((IPEndPoint)_socket.RemoteEndPoint).Port & 0xFF);
                    Array.Copy(((IPEndPoint)_socket.RemoteEndPoint).Address.GetAddressBytes(), 0, addressAndHandle, 4, 4);
                    // byte 8 is our sslProtocolOption
                    addressAndHandle[8] = sslProtocolOption;
                    // byte 9 will be our handle (new handle as output)
                    addressAndHandle[9] = 0xFF; // default handle to 0xFF to represent "no handle"
                    // request "upgrade to SSL/TLS" by closing the current socket and re-connecting via SSL/TLS
                    _socket.GetSocketOption(SocketOptionLevel.Socket, (SocketOptionName)0x400002, addressAndHandle);
                    if (addressAndHandle[9] == 0xFF)
                    {
                        // could not upgrade/re-connect to SSL
                        throw new Exception(); /* NOTE: what is the correct exception? */
                    }
                    else
                    {
                        _socket.m_Handle = addressAndHandle[9];
                    }

                    _sslContext = 0; /* SSL is now enabled on this socket */
                }
            }
            catch
            {
                if (_sslContext != -1)
                {
                    /* TODO: if we could not connect via SSL, do we need to reconnect as non-secure? */
                    _sslContext = -1;
                }

                throw;
            }
        }

        public bool IsServer { get { return _isServer; } }

        public override long Length
        {
            get
            {
                if (_disposed == true) throw new ObjectDisposedException();
                if (_socket == null) throw new IOException();

                return _socket.Available;
            }
        }

        public override bool DataAvailable
        {
            get
            {
                if (_disposed == true) throw new ObjectDisposedException();
                if (_socket == null) throw new IOException();

                return (_socket.Available > 0);
            }
        }

        ~SslStream()
        {
            // Do not re-create Dispose clean-up code here.
            // Calling Dispose(false) is optimal in terms of
            // readability and maintainability.
            Dispose(false);
        }

        [MethodImplAttribute(MethodImplOptions.Synchronized)]
        protected override void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                _disposed = true;

                if(_socket.m_Handle != -1)
                {
                    _socket.Close();
                }
            }
        }

        public override int Read(byte[] buffer, int offset, int size)
        {
            return base.Read(buffer, offset, size);
        }

        public override void Write(byte[] buffer, int offset, int size)
        {
            base.Write(buffer, offset, size);
        }
    }
}


