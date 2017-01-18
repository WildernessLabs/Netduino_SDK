using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel.Discovery;
using System.Collections.ObjectModel;
using System.ServiceModel;

namespace WCF_SampleClient
{
    class Program
    {
        static Uri serviceAddress;

        // ** DISCOVERY ** //
        static bool FindService()
        {
            try
            {
                DiscoveryClient discoveryClient =
                    new DiscoveryClient(new UdpDiscoveryEndpoint(DiscoveryVersion.WSDiscovery11));

                // Uncomment to support WSDiscoveryApril2005 discovery (and change app.config for binding change)
                //DiscoveryClient discoveryClient =
                //    new DiscoveryClient(new UdpDiscoveryEndpoint(DiscoveryVersion.WSDiscoveryApril2005)); 

                Collection<EndpointDiscoveryMetadata> services = discoveryClient.Find(new FindCriteria(typeof(IServiceHelloWCF))).Endpoints;

                discoveryClient.Close();

                if (services.Count == 0)
                {
                    return false;
                }
                else
                {
                    serviceAddress = services[0].ListenUris[0];
                }
            }
            catch
            {
                return false;
            }

            return true;
        }

        static void Main(string[] args)
        {
            // Use Custom binding Soap11AddressingBinding to connect to an MF server with ProtocolVersion10.
            // urn:uuid:f4c30207-c2cb-493c-8a44-776c1e0ecc7e is the ServiceID auto generated for IServiceHelloWCF service

            while(!FindService())
            {
                Console.WriteLine("IServiceHelloWCF service not found.  Trying again...");
                System.Threading.Thread.Sleep(1000);
            }
            ServiceHelloWCFClient client = new ServiceHelloWCFClient("DefaultBinding_IServiceHelloWCF_IServiceHelloWCF", serviceAddress.AbsoluteUri);

            string resp = client.HelloWCF("World");
            Console.WriteLine(resp);

            System.Diagnostics.Debug.Print("\n\n***********");
            System.Diagnostics.Debug.Print(resp);
            System.Diagnostics.Debug.Print("***********\n\n");

            Console.WriteLine();
            Console.WriteLine("Press any key to exit");
            Console.Read();
        }
    }
}
