using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Microsoft.NetMicroFramework.Tools.MFDeployTool.Engine
{
    [Serializable]
    public class MFUserExitException : Exception
    {
        public override string Message { get { return Properties.Resources.ExceptionUserExit; } }
    }
    [Serializable]
    public class MFDeviceInUseException : Exception
    {
        public override string Message { get { return Properties.Resources.ExceptionDeviceInUse; } }
    }
    [Serializable]
    public class MFDeviceNoResponseException : Exception
    {
        public override string Message { get { return Properties.Resources.ExceptionNoResponse; } }
    }
    [Serializable]
    public class MFDeviceUnknownDeviceException : Exception
    {
        public override string Message { get { return Properties.Resources.ExceptionUnknownDevice; } }
    }
    [Serializable]
    public class MFInvalidNetworkAddressException : Exception
    {
        public override string Message { get { return Properties.Resources.ExceptionInvalidNetworkAddress; } }
    }
    [Serializable]
    public class MFInvalidMacAddressException : Exception
    {
        public override string Message { get { return Properties.Resources.ExceptionInvalidMacAddress; } }
    }
    [Serializable]
    public class MFInvalidFileFormatException : Exception
    {
        string m_file;

        public MFInvalidFileFormatException(string file)
        {
            m_file = file;
        }
        
        public override string Message { get { return string.Format(Properties.Resources.ExceptionInvalidFileFormat, m_file); } }
    }
    [Serializable]
    public class MFSignatureFailureException : Exception
    {
        string m_file;

        public MFSignatureFailureException(string file)
        {
            FileInfo fi = new FileInfo(file);
            m_file = fi.Name;
        }
        
        public override string Message { get { return string.Format(Properties.Resources.ExceptionSignatureFailure, m_file); } }
    }
}
