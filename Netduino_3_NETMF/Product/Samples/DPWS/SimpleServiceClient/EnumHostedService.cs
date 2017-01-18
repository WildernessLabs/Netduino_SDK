using System;
using System.Text;
using System.Xml;
using Dpws.Device;
using Dpws.Device.Services;
using Ws.Services;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;

namespace hello.Microsoft.WCF.Samples.Adapters
{
    
    
    public class HelloWorld : DpwsHostedService
    {
        
        private IHelloWorld m_service = null;
        
        public HelloWorld(IHelloWorld service)
        {
            // Set the service implementation properties
            m_service = service;

            // Set base service properties
            ServiceNamespace = new WsXmlNamespace("hel", "hello://Microsoft.WCF.Samples.Adapters");
            ServiceID = "urn:uuid:ade3b12c-b46a-4f8f-8da7-4567e405a2bf";
            ServiceTypeName = "HelloWorld";

            // Add service types here
            ServiceOperations.Add(new WsServiceOperation("hello://Microsoft.WCF.Samples.Adapters/HelloWorld", "HelloWorld_SayHelloWorld_InputMessageRequest"));

            // Add event sources here
        }
        
        public virtual Byte[] HelloWorld_SayHelloWorld_InputMessageRequest(WsWsaHeader header, XmlReader reader)
        {
            // Build request object
            MyEnumDataContractSerializer reqDcs;
            reqDcs = new MyEnumDataContractSerializer("MyEnum", "hello://Microsoft.WCF.Samples.Adapters");
            MyEnum req;
            req = ((MyEnum)(reqDcs.ReadObject(reader)));

            // Create response object
            // Call service operation to process request and return response.
            MyEnum resp;
            resp = m_service.SayHelloWorld(req);

            // Create response header
            WsWsaHeader respHeader = new WsWsaHeader("hello://Microsoft.WCF.Samples.Adapters/HelloWorld/HelloWorld_SayHelloWorld_Output" +
                    "MessageResponse", header.MessageID, WsWellKnownUri.WsaAnonymousUri, null, null, null);

            // Create response serializer
            MyEnumDataContractSerializer respDcs;
            respDcs = new MyEnumDataContractSerializer("MyEnum", "hello://Microsoft.WCF.Samples.Adapters");

            // Build response message and return
            return SoapMessageBuilder.BuildSoapMessage(respHeader, respDcs, resp);
        }
    }
}
