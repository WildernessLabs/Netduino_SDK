using System;
using System.Text;
using System.Xml;
using Dpws.Device;
using Dpws.Device.Services;
using Ws.Services;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;
using Ws.Services.Binding;
using Ws.Services.Mtom;
using Ws.Services.Soap;

namespace localhost.ServiceHelloWCF
{
    public class ServiceHelloWCFImplementation : IIServiceHelloWCF
    {
        #region IIServiceHelloWCF Members

        public HelloWCFResponse HelloWCF(HelloWCF req)
        {
            HelloWCFResponse resp = new HelloWCFResponse();

            resp.HelloWCFResult = "Hello " + req.name;

            return resp;
        }

        #endregion
    }
}