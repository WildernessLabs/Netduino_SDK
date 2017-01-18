using System;
using System.Threading;
using Ws.Services;
using Ws.Services.Binding;
using System.Ext;
using localhost.ServiceHelloWCF;
using Dpws.Client;
using Dpws.Client.Discovery;
using Ws.Services.Faults;
using Microsoft.SPOT.Net.NetworkInformation;

namespace Microsoft.SPOT.Sample
{
    public class MFSimpleServiceClient
    {
        public static void Main()
        {
            // Turn console messages on
            Console.Verbose = true;

            NetworkInterface ni;
            
            // Wait for DHCP (on LWIP devices)
            while(true)
            {
                ni = NetworkInterface.GetAllNetworkInterfaces()[0];

                if(ni.IPAddress != "0.0.0.0") break;

                Thread.Sleep(1000);
            }

            // Create a test application thread
            TestApplication testApp = new TestApplication();
            Thread testAppThread = new Thread(new ThreadStart(testApp.Run));
            testAppThread.Start();

            System.Ext.Console.Write("Application started...");
        }
    }

    /// <summary>
    /// SimpleService test application class.
    /// </summary>
    public class TestApplication
    {
        ServiceHelloWCFClientProxy m_clientProxy;

        private bool Discover(ServiceHelloWCFClientProxy proxy)
        {
            DpwsServiceTypes typeProbes = new DpwsServiceTypes();
            typeProbes.Add(new DpwsServiceType("IServiceHelloWCF", "http://localhost/ServiceHelloWCF"));

            DpwsServiceDescriptions descs = proxy.DiscoveryClient.Probe(typeProbes, 1, 5000);

            if(descs.Count > 0)
            {
                proxy.EndpointAddress = descs[0].Endpoint.Address.AbsoluteUri;
                proxy.TransportAddress = descs[0].XAddrs[0];
                return true;
            }

            return false;
        }

        public void Run()
        {

            Uri remoteEp = new Uri("http://localhost:8084/ServiceHelloWCF.svc");
            WS2007HttpBinding binding = new WS2007HttpBinding(new HttpTransportBindingConfig(remoteEp));

            ProtocolVersion ver = new ProtocolVersion11();
            // To enable WSDiscoveryApril2005 and Soap12WSAddressingAugust2004
            //ProtocolVersion ver = new ProtocolVersion10();

            /// ProtocolVersion11 can be used if the corresponding WCF desktop server application
            /// WcfServer uses wsHttpBinding instead of the custom binding "Soap11AddressingBinding"
            m_clientProxy = new ServiceHelloWCFClientProxy(binding, ver);

            m_clientProxy.IgnoreRequestFromThisIP = false;

            if (!Discover(m_clientProxy))
            {
                Debug.Print("Discovery failed, trying direct address");
            }

            HelloWCF req = new HelloWCF();
            req.name = "World";

            try
            {
                HelloWCFResponse resp = m_clientProxy.HelloWCF(req);

                Debug.Print("\n\n*****************");
                Debug.Print(resp.HelloWCFResult);
                Debug.Print("*****************\n\n");
            }
            catch (WsFaultException ex)
            {
                Debug.Print("DPWS Fault: " + ex.Message);
            }
            finally
            {
                m_clientProxy.Dispose();
            }
        }
    }
}
