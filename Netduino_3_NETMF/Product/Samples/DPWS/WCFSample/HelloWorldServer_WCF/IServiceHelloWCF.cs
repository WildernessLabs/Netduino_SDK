using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace ServiceHelloWCF
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IService1" in both code and config file together.
    [ServiceContract(Namespace = "http://localhost/ServiceHelloWCF")]
    public interface IServiceHelloWCF
    {
        [OperationContract]
        string HelloWCF(string name);
    }
}
