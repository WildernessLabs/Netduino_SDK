using System;
using System.Net;
using System.Threading;
using Dpws.Device;
using Ws.Services;
using Ws.Services.Utilities;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;
using schemas.example.org.EventingService;
using schemas.example.org.SimpleService;
using schemas.example.org.AttachmentService;
using Interop.SimpleService;

#if !WIndows && !WindowsCE
using System.Ext;
using Ws.Services.Binding;
#endif

namespace Dpws.Device
{

    class Program
    {

        static void Main(string[] args)
        {
            // Wait for DHCP (on LWIP devices)
            while (true)
            {
                IPAddress ip = IPAddress.GetDefaultLocalAddress();

                if (ip != IPAddress.Any) break;

                Thread.Sleep(1000);
            }

            Program p = new Program();
            p.Start();
        }

        //SimpleServiceClient client;

        public void Start()
        {
            // Initialize the binding
            string guid = "urn:uuid:18571766-87df-06e2-bb68-5136c48f483a";
            ProtocolVersion version = new ProtocolVersion11();
            // To enable WSDiscoveryApril2005 and Soap12WSAddressingAugust2004
            //ProtocolVersion version = new ProtocolVersion10();
            Device.Initialize(new WS2007HttpBinding(new HttpTransportBindingConfig(guid, 8084)), version);

            // Set device information
            Device.ThisModel.Manufacturer = "Microsoft Corporation";
            Device.ThisModel.ManufacturerUrl = "http://www.microsoft.com/";
            Device.ThisModel.ModelName = "SimpleService Test Device";
            Device.ThisModel.ModelNumber = "1.0";
            Device.ThisModel.ModelUrl = "http://www.microsoft.com/";
            Device.ThisModel.PresentationUrl = "http://www.microsoft.com/";

            Device.ThisDevice.FriendlyName = "SimpleService";
            Device.ThisDevice.FirmwareVersion = "alpha";
            Device.ThisDevice.SerialNumber = "12345678";

            // Add a Host service type
            Device.Host = new SimpleDeviceHost(version);

            // make sure event sink connections remain open as long as the 
            // the subscription is alive.
            Device.SubscriptionManager.PersistEventConnections = true;

            // Add Dpws hosted service(s) to the device
            Device.HostedServices.Add(new SimpleService(new SimpleServiceImplementation(), version));
            EventingService eventingService = new EventingService(version);
            Device.HostedServices.Add(eventingService);
            Device.HostedServices.Add(new AttachmentService(new AttachmentServiceImplementation(), version));

            // Add a Dpws client to this device. Uncomment to run a client and device
            //client = new SimpleServiceClient();

            // Set this device property if you want to ignore this clients request
            Device.IgnoreLocalClientRequest = false;

            // Turn console messages on
            Console.Verbose = true;
            
            System.Ext.Console.Write( "Start DPWS device service with endpoint address: '" + Device.EndpointAddress + "'" );

            // Start the device
            ServerBindingContext ctx = new ServerBindingContext(version);
            Device.Start(ctx);
            
            //TEST RESTART CAPABILITY
            //Thread.Sleep(3000);
            //Device.Stop();
            //Thread.Sleep(100);
            //Device.Start(ctx);
            
            // Events cause WsFaultExceptions if loopback messages are used.
            if (Device.IgnoreLocalClientRequest)
            {
                // Create and start EventSimulator
                EventSimulator eventSimulator = new EventSimulator(eventingService);
                eventSimulator.StartEventSimulator();
            }
        }
    }
}
