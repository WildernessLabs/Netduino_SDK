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
            //return Netduino.IP.LinkLayers.CC3100SocketNative.socket(family, type, protocol);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("socket", BindingFlags.Public | BindingFlags.Static);
            return (int)methodInfo.Invoke(null, new object[] { family, type, protocol });
        }

        public static void bind(int handle, byte[] address)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.bind(handle, address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("bind", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, address });
        }

        public static void connect(int handle, byte[] address, bool fThrowOnWouldBlock)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.connect(handle, address, fThrowOnWouldBlock);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("connect", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, address, fThrowOnWouldBlock });
        }

        public static int send(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.send(handle, buf, offset, count, flags, timeout_ms);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("send", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms }));
        }

        public static int recv(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.recv(handle, buf, offset, count, flags, timeout_ms);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("recv", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms }));
        }

        public static int close(int handle)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.close(handle);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("close", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle }));
        }

        public static void listen(int handle, int backlog)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.listen(handle, backlog);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("listen", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, backlog });
        }

        public static int accept(int handle)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.accept(handle);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("accept", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle }));
        }

        //No standard non-blocking api
        public static void getaddrinfo(string name, out string canonicalName, out byte[][] addresses)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.getaddrinfo(name, out canonicalName, out addresses);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("getaddrinfo_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { name });
            canonicalName = (string)retObjArray[0];
            addresses = (byte[][])retObjArray[1];
        }

        public static void shutdown(int handle, int how, out int err)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.shutdown(handle, how, out err);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("shutdown_reflection", BindingFlags.Public | BindingFlags.Static);
            err = 0;
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle, how });
            err = (int)retObjArray[0];
        }

        public static int sendto(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms, byte[] address)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.sendto(handle, buf, offset, count, flags, timeout_ms, address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("sendto", BindingFlags.Public | BindingFlags.Static);
            return (int)(methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms, address }));
        }

        public static int recvfrom(int handle, byte[] buf, int offset, int count, int flags, int timeout_ms, ref byte[] address)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.recvfrom(handle, buf, offset, count, flags, timeout_ms, ref address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("recvfrom_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle, buf, offset, count, flags, timeout_ms, address });
            int retVal = (int)retObjArray[0];
            address = (byte[])retObjArray[1];
            return retVal;
        }

        public static void getpeername(int handle, out byte[] address)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.getpeername(handle, out address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("getpeername_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle });
            address = (byte[])retObjArray[0];
        }

        public static void getsockname(int handle, out byte[] address)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.getsockname(handle, out address);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("getsockname_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle });
            address = (byte[])retObjArray[0];
        }

        public static void getsockopt(int handle, int level, int optname, byte[] optval)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.getsockopt(handle, level, optname, optval);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("getsockopt", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, level, optname, optval });
        }

        public static void setsockopt(int handle, int level, int optname, byte[] optval)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.setsockopt(handle, level, optname, optval);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("setsockopt", BindingFlags.Public | BindingFlags.Static);
            methodInfo.Invoke(null, new object[] { handle, level, optname, optval });
        }

        public static bool poll(int handle, int mode, int microSeconds)
        {
			//return Netduino.IP.LinkLayers.CC3100SocketNative.poll(handle, mode, microSeconds);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("poll", BindingFlags.Public | BindingFlags.Static);
            return (bool)(methodInfo.Invoke(null, new object[] { handle, mode, microSeconds }));
        }

        public static void ioctl(int handle, uint cmd, ref uint arg)
        {
			//Netduino.IP.LinkLayers.CC3100SocketNative.ioctl(handle, cmd, ref arg);
            MethodInfo methodInfo = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100").GetMethod("ioctl_reflection", BindingFlags.Public | BindingFlags.Static);
            object[] retObjArray = (object[])methodInfo.Invoke(null, new object[] { handle, cmd, arg });
            arg = (uint)retObjArray[0];
        }

    }
}


