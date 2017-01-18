using System;
using System.Threading;
using Dpws.Device;
using Ws.Services;
using Ws.Services.Utilities;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;
using System.Ext;
using Ws.Services.Binding;
using localhost.ServiceHelloWCF;
using Microsoft.SPOT.Net.NetworkInformation;

namespace Dpws.Device
{

    class Program
    {

        static void Main(string[] args)
        {
            NetworkInterface ni;

            // Wait for DHCP (on LWIP devices)
            while (true)
            {
                ni = NetworkInterface.GetAllNetworkInterfaces()[0];

                if (ni.IPAddress != "0.0.0.0") break;

                Thread.Sleep(1000);
            }

            Program p = new Program();
            p.Start();
        }

        public void Start()
        {
            // Initialize the binding
            //Guid g = Guid.NewGuid();
            string guid = "urn:uuid:18571766-87df-06e2-bb68-5136c48f483f";

            ProtocolVersion version = new ProtocolVersion11();
            
            // ProtocolVersion10 can be used only if the corresponding HelloWorldClient_WCF application
            // uses a custom binding with Soap12WSAddressingAugust2004 text message encoding.
            //ProtocolVersion version = new ProtocolVersion10();

            Device.Initialize(new WS2007HttpBinding(new HttpTransportBindingConfig(guid, 8084)), version);

            // Set device information
            Device.ThisModel.Manufacturer = "Microsoft Corporation";
            Device.ThisModel.ManufacturerUrl = "http://www.microsoft.com/";
            Device.ThisModel.ModelName = "SimpleService Test Device";
            Device.ThisModel.ModelNumber = "1.0";
            Device.ThisModel.ModelUrl = "http://www.microsoft.com/";
            Device.ThisModel.PresentationUrl = "http://www.microsoft.com/";

            Device.ThisDevice.FriendlyName = "HelloWorldService";
            Device.ThisDevice.FirmwareVersion = "alpha";
            Device.ThisDevice.SerialNumber = "32345678";

            // Add a Host service type
            Device.Host = new HelloWCFService(version);

            // Add Dpws hosted service(s) to the device
            Device.HostedServices.Add(new IServiceHelloWCF(new ServiceHelloWCFImplementation()));

            // Set this device property if you want to ignore this clients request
            Device.IgnoreLocalClientRequest = false;

            // Turn console messages on
            Console.Verbose = true;
            
            System.Ext.Console.Write( "Start DPWS device service with endpoint address: '" + Device.EndpointAddress + "'" );

            ServerBindingContext ctx = new ServerBindingContext(version);

            // Start the device
            Device.Start(ctx);
        }
    }
}
