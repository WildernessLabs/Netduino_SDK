using System;
using System.Collections.Generic;
using System.Text;

namespace Microsoft.NetMicroFramework.Tools.MFDeployTool.Engine
{
    [Serializable]
    public class MFInvalidConfigurationSectorException : Exception
    {
        public override string Message { get { return Properties.Resources.InvalidCfgSector; } }
    }
    [Serializable]
    public class MFInvalidKeyLengthException : Exception
    {
        public override string Message { get { return Properties.Resources.InvalidKeyLength; } }
    }
    [Serializable]
    public class MFTinyBooterConnectionFailureException : Exception
    {
        public override string Message { get { return Properties.Resources.TinyBooterConnectionFailure; } }
    }
    [Serializable]
    public class MFInvalidConfigurationDataException : Exception
    {
        public override string Message { get { return Properties.Resources.InvalidConfig; } }
    }
    [Serializable]
    public class MFConfigSectorEraseFailureException : Exception
    {
        public override string Message { get { return Properties.Resources.ConfigSectorEraseFailure; } }
    }
    [Serializable]
    public class MFConfigSectorWriteFailureException : Exception
    {
        public override string Message { get { return Properties.Resources.ConfigSectorWriteFailure; } }
    }
    [Serializable]
    public class MFConfigurationSectorOutOfMemoryException : Exception
    {
        public override string Message { get { return Properties.Resources.ConfigSectorOutOfMemory; } }
    }
}
