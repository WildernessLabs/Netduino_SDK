using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using Microsoft.NetMicroFramework.Tools.MFDeployTool.Engine;
using System.Runtime.InteropServices;

namespace Microsoft.NetMicroFramework.Tools.MFDeployTool.PlugIns
{
    public interface IMFDeployForm
    {
        void DumpToOutput(string text);
        void DumpToOutput(string text, bool newLine);

        ReadOnlyCollection<string> Files { get;      }

        MFPortDefinition TransportTinyBooter { get; set;}
    }

    public abstract class MFPlugInMenuItem
    {
        private object m_tag       = null;

        public abstract string Name { get; }
        public object          Tag  { get { return m_tag;  } set { m_tag  = value; } }

        public virtual ReadOnlyCollection<MFPlugInMenuItem> Submenus { get { return null; } }

        public virtual bool    RunInSeparateThread { get { return true; } }
        public virtual bool    RequiresConnection { get { return true; } }
        public virtual void    OnAction(IMFDeployForm form, MFDevice device) { }
        public override string ToString() { return Name; }
    }
}
