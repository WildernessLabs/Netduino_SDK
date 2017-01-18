using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;

namespace ServiceHelloWCF
{
    [ServiceBehavior(Namespace = "http://localhost/ServiceHelloWCF")]
    public class ServiceHelloWCF : IServiceHelloWCF
    {
        #region IServiceHelloWCF Members

        public string HelloWCF(string name)
        {
            return "Hello " + name;
        }

        #endregion
    }

}
