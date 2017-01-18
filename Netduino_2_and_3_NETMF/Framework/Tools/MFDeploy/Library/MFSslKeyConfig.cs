using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.NetMicroFramework.Tools.MFDeployTool.PlugIns;
using _DBG = Microsoft.SPOT.Debugger;
using System.Runtime.InteropServices;
using dotNetMFCrypto;

namespace Microsoft.NetMicroFramework.Tools.MFDeployTool.Engine
{
    [StructLayout(LayoutKind.Sequential)]
    internal unsafe struct HAL_SslKeyConfiguration : IHAL_CONFIG_BASE
    {
        public HAL_CONFIG_BLOCK Header;
        public UInt32 Enabled;
        public UInt64 Seed;
        internal TINYBOOTER_KEY_CONFIG PrivateSslKey;

        public HAL_CONFIG_BLOCK ConfigHeader
        {
            get { return Header; }
            set { Header = value; }
        }

        public int Size
        {
            get
            {
                int size = 0;

                unsafe
                {
                    size = sizeof(HAL_SslKeyConfiguration);
                }

                return size;
            }
        }
    }

    public class MFSslKeyConfig
    {
        const string c_CfgName = "SSL_SEED_KEY";
        HAL_SslKeyConfiguration m_cfg = new HAL_SslKeyConfiguration();
        MFDevice m_dev;

        public MFSslKeyConfig(MFDevice dev)
        {
            m_dev = dev;
        }

        public void Save()
        {
            Random      rand    = new Random();
            double      d       = rand.NextDouble();
            MFKeyConfig keyCfg  = new MFKeyConfig();
            KeyPair     keys    = keyCfg.CreateKeyPair();

            MFConfigHelper cfgHelper = new MFConfigHelper(m_dev);

            m_cfg.Enabled = 1;
            m_cfg.Seed = (UInt64)((double)UInt64.MaxValue * d);

            unsafe
            {
                fixed (byte* key = m_cfg.PrivateSslKey.SectorKey)
                {
                    for (int i = 0; i < keys.PrivateKey.Length && i < MFKeyConfig.PrivateKeySize; i++)
                    {
                        key[i++] = keys.PrivateKey[i];
                    }
                }
            }

            cfgHelper.WriteConfig(c_CfgName, m_cfg, true);

            cfgHelper.Dispose();
        }

    }
}
