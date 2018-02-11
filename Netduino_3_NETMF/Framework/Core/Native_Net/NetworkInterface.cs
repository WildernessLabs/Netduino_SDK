////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Reflection;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;

namespace Microsoft.SPOT.Net.NetworkInformation
{
    public enum NetworkInterfaceType
    {
        Unknown = 1,
        Ethernet = 6,
        Wireless80211 = 71,
    }

    public class NetworkInterface
    {
        //set update flags...
        private const int UPDATE_FLAGS_DNS = 0x1;
        private const int UPDATE_FLAGS_DHCP = 0x2;
        private const int UPDATE_FLAGS_DHCP_RENEW = 0x4;
        private const int UPDATE_FLAGS_DHCP_RELEASE = 0x8;
        private const int UPDATE_FLAGS_MAC = 0x10;

        private const uint FLAGS_DHCP = 0x1;
        private const uint FLAGS_DYNAMIC_DNS = 0x2;

        [FieldNoReflection]
        private readonly int _interfaceIndex;

        private uint _flags;
        private uint _ipAddress;
        private uint _gatewayAddress;
        private uint _subnetMask;
        private uint _dnsAddress1;
        private uint _dnsAddress2;
        private NetworkInterfaceType _networkInterfaceType;
        private byte[] _macAddress;

        public NetworkInterface(int interfaceIndex)
        {
            this._interfaceIndex = interfaceIndex;
            _networkInterfaceType = NetworkInterfaceType.Unknown;
        }

        public static NetworkInterface[] GetAllNetworkInterfaces()
        {
            int count = GetNetworkInterfaceCount();
            NetworkInterface[] ifaces = new NetworkInterface[count];

            for (uint i = 0; i < count; i++)
            {
                ifaces[i] = GetNetworkInterface(i);
            }

            return ifaces;
        }



        public static int GetNetworkInterfaceCount()
        {
            MethodInfo methodInfo = Type.GetType("Netduino.IP.Interop.NetworkInterface, Netduino.IP.Interop").GetMethod("GetNetworkInterfaceCount", BindingFlags.Public | BindingFlags.Static);
            return (int)methodInfo.Invoke(null, new object[] { });
        }

        public static NetworkInterface GetNetworkInterface(uint interfaceIndex)
        {
            MethodInfo methodInfo = Type.GetType("Netduino.IP.Interop.NetworkInterface, Netduino.IP.Interop").GetMethod("GetNetworkInterface", BindingFlags.Public | BindingFlags.Static);
            return (NetworkInterface)methodInfo.Invoke(null, new object[] { interfaceIndex });
        }

        
        public void InitializeNetworkInterfaceSettings()
        {
           
            NetworkInterface ni = GetNetworkInterface(0);
            _flags = ni._flags;
            _ipAddress = ni._ipAddress;
            _gatewayAddress = ni._gatewayAddress;
            _subnetMask = ni._subnetMask;
            _dnsAddress1 = ni._dnsAddress1;
            _dnsAddress2 = ni._dnsAddress2;
            _networkInterfaceType = ni._networkInterfaceType;
            _macAddress = ni._macAddress;
            Dump();
        }
        

        /*
        public void InitializeNetworkInterfaceSettings()
        {
            var type = Type.GetType("Netduino.IP.Interop.NetworkInterface, Netduino.IP.Interop");
            if (type == null)
            {
                return;
            }
            MethodInfo methodInfo = type.GetMethod("InitializeNetworkInterfaceSettings", BindingFlags.Public | BindingFlags.Instance);
            if (methodInfo == null)
            {
                return;
            }
            methodInfo.Invoke(this, new Object[] {});
            Debug.Print("InitializeNetworkInterfaceSettings: Invoke method completed");
            Dump();
        }*/


        public void UpdateConfiguration(int updateType)
        {
            
            var type = Type.GetType("Netduino.IP.Interop.NetworkInterface, Netduino.IP.Interop");
            if (type == null)
            {
                return;
            }
            MethodInfo methodInfo = type.GetMethod("UpdateConfiguration", BindingFlags.Public | BindingFlags.Instance);
            if (methodInfo == null)
            {
                return;
            }
            methodInfo.Invoke(this, new Object[] { updateType });
            Debug.Print("UpdateConfiguration: Invoke method completed");
        }

        public static uint IPAddressFromString(string ipAddress)
        {
            if (ipAddress == null)
                throw new System.ArgumentNullException();


            if (ipAddress == null)
                throw new ArgumentNullException();

            ulong ipAddressValue = 0;
            int lastIndex = 0;
            int shiftIndex = 0;
            ulong mask = 0x00000000000000FF;
            ulong octet = 0L;
            int length = ipAddress.Length;

            for (int i = 0; i < length; ++i)
            {
                // Parse to '.' or end of IP address
                if (ipAddress[i] == '.' || i == length - 1)
                    // If the IP starts with a '.'
                    // or a segment is longer than 3 characters or shiftIndex > last bit position throw.
                    if (i == 0 || i - lastIndex > 3 || shiftIndex > 24)
                    {
                        throw new ArgumentException();
                    }
                    else
                    {
                        i = i == length - 1 ? ++i : i;
                        System.Type type = System.Type.GetType("Netduino.IP.LinkLayers.CC3100");
                        MethodInfo[] methodInfos = type.GetMethods(BindingFlags.Public | BindingFlags.Static);
                        var method = type.GetMethod("ConvertStringToInt32", BindingFlags.Public | BindingFlags.Static);
                        System.Int32 stoi32 = (System.Int32)method.Invoke(null, new object[] { ipAddress.Substring(lastIndex, i - lastIndex) });
                        octet = (ulong)(stoi32 & 0x00000000000000FF);
                        ipAddressValue = ipAddressValue + (ulong)((octet << shiftIndex) & mask);
                        lastIndex = i + 1;
                        shiftIndex = shiftIndex + 8;
                        mask = (mask << 8);
                    }
            }

            return (uint)ipAddressValue;
        }

        public string IPAddressToString(uint ipAddress)
        {
            if (SystemInfo.IsBigEndian)
            {
                return string.Concat(
                                ((ipAddress >> 24) & 0xFF).ToString(),
                                 ".",
                                ((ipAddress >> 16) & 0xFF).ToString(),
                                 ".",
                                ((ipAddress >> 8) & 0xFF).ToString(),
                                 ".",
                                ((ipAddress >> 0) & 0xFF).ToString()
                                );
            }
            else
            {
                return string.Concat(
                                ((ipAddress >> 0) & 0xFF).ToString(),
                                 ".",
                                ((ipAddress >> 8) & 0xFF).ToString(),
                                 ".",
                                ((ipAddress >> 16) & 0xFF).ToString(),
                                 ".",
                                ((ipAddress >> 24) & 0xFF).ToString()
                                );
            }
        }

        public void EnableStaticIP(string ipAddress, string subnetMask, string gatewayAddress)
        {

            try
            {
 
                _ipAddress = IPAddressFromString(ipAddress);
                _subnetMask = IPAddressFromString(subnetMask);
                _gatewayAddress = IPAddressFromString(gatewayAddress);
                _flags &= ~FLAGS_DHCP;

                /* change CC3100 to static IP configuration */

                //if (!Netduino.IP.LinkLayers.CC3100SocketNative._isInitialized) Netduino.IP.LinkLayers.CC3100SocketNative.Initialize();

                Type cc3100SocketNativeType = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100");
                bool isInitialized = (bool)(cc3100SocketNativeType.GetField("_isInitialized").GetValue(null));
                if (!isInitialized)
                {
                    Debug.Print("Initializing");
                    cc3100SocketNativeType.GetMethod("Initialize").Invoke(null, new object[] { });
                }
                

                FieldInfo cc3100FieldInfo = cc3100SocketNativeType.GetField("_cc3100");
                cc3100FieldInfo.FieldType.GetMethod("SetIpv4ConfigurationLE").Invoke(cc3100FieldInfo.GetValue(null), new object[] { _ipAddress, _subnetMask, _gatewayAddress, _dnsAddress1 }); ;
                UpdateConfiguration(UPDATE_FLAGS_DHCP);
            }
            catch (Exception ex)
            {
                throw ex;

            }
            finally
            {
                ReloadSettings();
                Dump();
                Debug.Print("Reloaded the settings");

            }
        }

        public void EnableDhcp()
        {
            try
            {
                InitializeNetworkInterfaceSettings();
                _flags |= FLAGS_DHCP;

                /* change CC3100 to DHCP IP configuration */
                //if (!Netduino.IP.LinkLayers.CC3100SocketNative._isInitialized) Netduino.IP.LinkLayers.CC3100SocketNative.Initialize();
                Type cc3100SocketNativeType = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100");
                if (cc3100SocketNativeType == null)
                {
                    Debug.Print("cc3100SocketNativeType is null");
                    return;
                }
                bool isInitialized = (bool)(cc3100SocketNativeType.GetField("_isInitialized").GetValue(null));
                if (!isInitialized)
                {
                    Debug.Print("We are initializing");
                    cc3100SocketNativeType.GetMethod("Initialize").Invoke(null, new object[] { });
                }
               
                FieldInfo cc3100FieldInfo = cc3100SocketNativeType.GetField("_cc3100");
                if (cc3100FieldInfo == null)
                {
                    Debug.Print("cc3100FieldInfo was null");
                    return;
                }
                MethodInfo methodInfo = cc3100FieldInfo.FieldType.GetMethod("SetIpv4ConfigurationAsDhcp");
                if (methodInfo == null)
                {
                    Debug.Print("methodInfo was null");
                    return;
                }

                methodInfo.Invoke(cc3100FieldInfo.GetValue(null), new object[] { });
                Debug.Print("Invoke returned");

                UpdateConfiguration(UPDATE_FLAGS_DHCP);
            }
            finally
            {
                ReloadSettings();
            }
        }

        public void EnableStaticDns(string[] dnsAddresses)
        {
            if (dnsAddresses == null || dnsAddresses.Length == 0 || dnsAddresses.Length > 2)
            {
                throw new ArgumentException();
            }

            uint[] addresses = new uint[2];

            int iAddress = 0;
            for (int i = 0; i < dnsAddresses.Length; i++)
            {
                uint address = IPAddressFromString(dnsAddresses[i]);

                addresses[iAddress] = address;

                if (address != 0)
                {
                    iAddress++;
                }
            }

            try
            {
                _dnsAddress1 = addresses[0];
                _dnsAddress2 = addresses[1];

                _flags &= ~FLAGS_DYNAMIC_DNS;

                UpdateConfiguration(UPDATE_FLAGS_DNS);
            }
            finally
            {
                ReloadSettings();
            }
        }

        public void EnableDynamicDns()
        {
            try
            {
                _flags |= FLAGS_DYNAMIC_DNS;

                UpdateConfiguration(UPDATE_FLAGS_DNS);
            }
            finally
            {
                ReloadSettings();
            }
        }

        public string IPAddress
        {
            get
            {
                UInt32 ipAddress = 0;
                if (IsDhcpEnabled)
                {
                    //if (!Netduino.IP.LinkLayers.CC3100SocketNative._isInitialized) Netduino.IP.LinkLayers.CC3100SocketNative.Initialize();
                    Type cc3100SocketNativeType = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100");
                    bool isInitialized = (bool)(cc3100SocketNativeType.GetField("_isInitialized").GetValue(null));
                    if (!isInitialized)
                    {
                        cc3100SocketNativeType.GetMethod("Initialize").Invoke(null, new object[] { });
                    }

                    try
                    {
                        //Netduino.IP.LinkLayers.CC3100SocketNative._cc3100.GetIpv4ConfigurationLE(out ipAddress, out subnetMask, out gatewayAddress, out dnsAddress);
                        FieldInfo cc3100FieldInfo = cc3100SocketNativeType.GetField("_cc3100");
                        ipAddress = (UInt32)(cc3100FieldInfo.FieldType.GetMethod("GetIPv4ConfigurationLE_IPAddress").Invoke(cc3100FieldInfo.GetValue(null), new object[] { }));
                    }
                    catch
                    {
                        /* FAILSAFE: if we cannot retrieve the IP address from our CC3100 chip, return IPAddress.Any. */
                        ipAddress = 0;
                    }
                }
                else
                {
                    ipAddress = _ipAddress;
                }
                return IPAddressToString(ipAddress);
            }
        }


        public string GatewayAddress
        {
            get
            {
                UInt32 gatewayAddress = 0;
                if (IsDhcpEnabled)
                {
                    //if (!Netduino.IP.LinkLayers.CC3100SocketNative._isInitialized) Netduino.IP.LinkLayers.CC3100SocketNative.Initialize();
                    Type cc3100SocketNativeType = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100");
                    bool isInitialized = (bool)(cc3100SocketNativeType.GetField("_isInitialized").GetValue(null));
                    if (!isInitialized)
                    {
                        cc3100SocketNativeType.GetMethod("Initialize").Invoke(null, new object[] { });
                    }
                    //Netduino.IP.LinkLayers.CC3100SocketNative._cc3100.GetIpv4ConfigurationLE(out ipAddress, out subnetMask, out gatewayAddress, out dnsAddress);
                    FieldInfo cc3100FieldInfo = cc3100SocketNativeType.GetField("_cc3100");
                    gatewayAddress = (UInt32)(cc3100FieldInfo.FieldType.GetMethod("GetIPv4ConfigurationLE_GatewayAddress").Invoke(cc3100FieldInfo.GetValue(null), new object[] { }));
                }
                else
                {
                    gatewayAddress = _gatewayAddress;
                }
                return IPAddressToString(gatewayAddress);
            }
        }

        public string SubnetMask
        {
            get
            {
                UInt32 subnetMask = 0;
                if (IsDhcpEnabled)
                {
                    //if (!Netduino.IP.LinkLayers.CC3100SocketNative._isInitialized) Netduino.IP.LinkLayers.CC3100SocketNative.Initialize();
                    Type cc3100SocketNativeType = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100");
                    bool isInitialized = (bool)(cc3100SocketNativeType.GetField("_isInitialized").GetValue(null));
                    if (!isInitialized)
                    {
                        cc3100SocketNativeType.GetMethod("Initialize").Invoke(null, new object[] { });
                    }
                    //Netduino.IP.LinkLayers.CC3100SocketNative._cc3100.GetIpv4ConfigurationLE(out ipAddress, out subnetMask, out gatewayAddress, out dnsAddress);
                    FieldInfo cc3100FieldInfo = cc3100SocketNativeType.GetField("_cc3100");
                    subnetMask = (UInt32)(cc3100FieldInfo.FieldType.GetMethod("GetIPv4ConfigurationLE_SubnetMask").Invoke(cc3100FieldInfo.GetValue(null), new object[] { }));
                }
                else
                {
                    subnetMask = _subnetMask;
                }
                return IPAddressToString(subnetMask);
            }
        }

        public bool IsDhcpEnabled
        {
            get { return (_flags & FLAGS_DHCP) != 0; }
        }

        public bool IsDynamicDnsEnabled
        {
            get
            {
                return (_flags & FLAGS_DYNAMIC_DNS) != 0;
            }
        }

        public string[] DnsAddresses
        {
            get
            {
                ArrayList list = new ArrayList();

                if (IsDynamicDnsEnabled)
                {
                    UInt32 dnsAddress = 0;
                    //if (!Netduino.IP.LinkLayers.CC3100SocketNative._isInitialized) Netduino.IP.LinkLayers.CC3100SocketNative.Initialize();
                    Type cc3100SocketNativeType = Type.GetType("Netduino.IP.LinkLayers.CC3100SocketNative, Netduino.IP.LinkLayers.CC3100");
                    bool isInitialized = (bool)(cc3100SocketNativeType.GetField("_isInitialized").GetValue(null));
                    if (!isInitialized)
                    {
                        cc3100SocketNativeType.GetMethod("Initialize").Invoke(null, new object[] { });
                    }
                    //Netduino.IP.LinkLayers.CC3100SocketNative._cc3100.GetIpv4ConfigurationLE(out ipAddress, out subnetMask, out gatewayAddress, out dnsAddress);
                    FieldInfo cc3100FieldInfo = cc3100SocketNativeType.GetField("_cc3100");
                    dnsAddress = (UInt32)(cc3100FieldInfo.FieldType.GetMethod("GetIPv4ConfigurationLE_DnsAddress").Invoke(cc3100FieldInfo.GetValue(null), new object[] { }));
                    list.Add(IPAddressToString(dnsAddress));
                }
                else
                {
                    if (_dnsAddress1 != 0)
                    {
                        list.Add(IPAddressToString(_dnsAddress1));
                    }

                    if (_dnsAddress2 != 0)
                    {
                        list.Add(IPAddressToString(_dnsAddress2));
                    }
                }

                return (string[])list.ToArray(typeof(string));
            }
        }

        private void ReloadSettings()
        {
            Thread.Sleep(100);
            InitializeNetworkInterfaceSettings();
        }

        public void ReleaseDhcpLease()
        {
            try
            {
                UpdateConfiguration(UPDATE_FLAGS_DHCP_RELEASE);
            }
            finally
            {
                ReloadSettings();
            }
        }

        public void RenewDhcpLease()
        {
            try
            {
                UpdateConfiguration(UPDATE_FLAGS_DHCP_RELEASE | UPDATE_FLAGS_DHCP_RENEW);
            }
            finally
            {
                ReloadSettings();
            }
        }

        public byte[] PhysicalAddress
        {
            get { return _macAddress; }
            set
            {
                try
                {
                    _macAddress = value;
                    UpdateConfiguration(UPDATE_FLAGS_MAC);
                }
                finally
                {
                    ReloadSettings();
                }
            }
        }

        public NetworkInterfaceType NetworkInterfaceType
        {
            get { return _networkInterfaceType; }
        }

        private void Dump()
        {
            Debug.Print("Flags: " + _flags.ToString());
            Debug.Print("IP Address: " + IPAddressToString(_ipAddress));
            Debug.Print("Gateway Address: " + IPAddressToString(_gatewayAddress));
            Debug.Print("Subnet MASK: " + IPAddressToString(_subnetMask));
            Debug.Print("DNS Address: " + IPAddressToString(_dnsAddress1));
            //if (_flags & FLAGS_DHCP != 0) Debug.Print("DHCP enabled"); else Debug.Print("DHCP disabled");
            //if (_flags & FLAGS_DYNAMIC_DNS != 0) Debug.Print("Dynamic DHCP enabled"); else Debug.Print("Dynamic DHCP disabled");
        }
    }
    
}


