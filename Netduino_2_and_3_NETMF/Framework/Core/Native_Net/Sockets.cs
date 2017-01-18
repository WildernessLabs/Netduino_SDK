////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Reflection;
using System.Diagnostics;
using System.Runtime.CompilerServices;

[assembly: System.Runtime.CompilerServices.InternalsVisibleTo("System")]

namespace Microsoft.SPOT.Net
{
    internal static class SocketNative
    {
        public const int FIONREAD = 0x4004667F;

        public static int socket(int family, int type, int protocol)
        {
            //return Netduino.IP.SocketsInterface.socket(family, type, protocol);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("socket", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { family, type, protocol }));
        }

        public static void bind(int handle, byte[] address)
        {
            //return Netduino.IP.SocketsInterface.bind(handle, address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("bind", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, address });
        }

        public static void connect(int handle, byte[] address, bool fThrowOnWouldBlock)
        {
            //return Netduino.IP.SocketsInterface.connect(handle, address, fThrowOnWouldBlock);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("connect", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, address, fThrowOnWouldBlock });
        }

        public static int send(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms)
        {
            //return Netduino.IP.SocketsInterface.send(handle, buf, offset, count, flags, timeout_ms);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("send", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms }));
        }

        public static int recv(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms)
        {
            //return Netduino.IP.SocketsInterface.recv(handle, buf, offset, count, flags, timeout_ms);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("recv", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms }));
        }

        public static int close(int handle)
        {
            //return Netduino.IP.SocketsInterface.close(handle);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("close", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle }));
        }

        public static void listen(int handle, int backlog)
        {
            //return Netduino.IP.SocketsInterface.listen(handle, backlog);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("listen", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, backlog });
        }

        public static int accept(int handle)
        {
            //return Netduino.IP.SocketsInterface.accept(handle);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("accept", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle }));
        }

        //No standard non-blocking api
        public static void getaddrinfo(string name, out string canonicalName, out byte[][] addresses)
        {
            //return Netduino.IP.SocketsInterface.getaddrinfo(name, out canonicalName, out addresses);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("getaddrinfo_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { name });
            canonicalName = (string)retObjArray[0];
            addresses = (byte[][])retObjArray[1];
        }

        public static void shutdown(int handle, int how, out int err)
        {
			throw new NotImplementedException();
        }

        public static int sendto(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms, byte[] address)
        {
            //return Netduino.IP.SocketsInterface.sendto(handle, buf, offset, count, flags, timeout_ms, address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("sendto", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms, address }));
        }

        public static int recvfrom(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms, ref byte[] address)
        {
            //return Netduino.IP.SocketsInterface.recvfrom(handle, buf, offset, count, flags, timeout_ms, ref address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("recvfrom_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms, address });
            int retVal = (int)retObjArray[0];
            address = (byte[])retObjArray[1];
            return retVal;
        }

        public static void getpeername(int handle, out byte[] address)
        {
            //Netduino.IP.SocketsInterface.getpeername(handle, out address); 
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("getpeername_reflection", BindingFlags.Public | BindingFlags.Static); 
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle }); 
            address = (byte[])retObjArray[0]; 
        }

        public static void getsockname(int handle, out byte[] address)
        {
            //Netduino.IP.SocketsInterface.getpeername(handle, out address); 
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("getsockname_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle });
            address = (byte[])retObjArray[0];
        }

        public static void getsockopt(int handle, int level, int optname, byte[] optval)
        {
            //Netduino.IP.SocketsInterface.getsockopt(handle, level, optname, optval); 
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("getsockopt", BindingFlags.Public | BindingFlags.Static); 
            methodInfo.Invoke(null, new object[] { handle, level, optname, optval }); 
        }

        public static void setsockopt(int handle, int level, int optname, byte[] optval)
        {
            //Netduino.IP.SocketsInterface.setsockopt(handle, level, optname, optval); 
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("setsockopt", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, level, optname, optval });
        }

        public static bool poll(int handle, int mode, int microSeconds)
        {
            //Netduino.IP.SocketsInterface.poll(handle, mode, microSeconds);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("poll", BindingFlags.Public | BindingFlags.Static);
            return (bool)(methodInfo.Invoke(null, new object[] { handle, mode, microSeconds }));
        }

        public static void ioctl(int handle, uint cmd, ref uint arg)
        {
            //Netduino.IP.SocketsInterface.ioctl(handle, cmd, ref arg);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.SocketsInterface, Netduino.IP").GetMethod("ioctl_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle, cmd, arg });
            arg = (uint)retObjArray[0];
        }
    }
}


