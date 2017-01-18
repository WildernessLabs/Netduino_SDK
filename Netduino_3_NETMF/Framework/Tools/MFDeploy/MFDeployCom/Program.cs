using System;
using System.Collections.Generic;
using System.Windows.Forms;
using _MFD = Microsoft.NetMicroFramework.Tools.MFDeployTool;

namespace Microsoft.NetMicroFramework.Tools.MFDeployCom
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            _MFD.Program.Main();
        }
    }
}