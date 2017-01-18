using System;
using System.Threading;
using Dpws.Client.Discovery;
using Dpws.Client.Eventing;
using Dpws.Client;
using Ws.Services;
using Ws.Services.Utilities;
using Ws.Services.WsaAddressing;
using schemas.example.org.EventingService;
using schemas.example.org.SimpleService;
using schemas.example.org.AttachmentService;
using Ws.Services.Binding;
using Microsoft.SPOT;
using System.Net;

#if !Windows && !WindowsCE
using System.Ext;
#endif

namespace Interop.SimpleService
{
    public class MFSimpleServiceClient
    {
        /// <summary>
        /// This is a sample Micr Framework Dpws client application. This console application is designed
        /// to excercise the features of the MFWsdStack client classes. This application creates an instance of
        /// a SimpleServiceClient class. This class is derived DpwsClientServices. When an instance of this class
        /// is created the MFWsdStack services start automatically. Unlike the Device services that require an actual
        /// Start command. This sample app will wait for a hello discovery request. Wen received it will probe for
        /// services that implement a SimpleService type. Te Dpws Interop sample service implements this type. For each
        /// probe match received (up to 10) a resolve request is sent to endpoints obtained from the resolve matches.
        /// For each resolve match the app sends a Get request to the device hosting the service. The app then uses the
        /// metadata in the get response to call 1way, 2way methods on the service and then subscribes to the SimpleEvent
        /// and ItegerEvent hosted by the interop service. When the client receives each event from the service it sends
        /// an Unsubscribe for each event and waits for another hello. The application processes one hello at a time.
        /// resolve match 
        /// </summary>
        public static void Main()
        {
            // Turn console messages on
            Console.Verbose = true;

            System.Ext.Console.Write("Application started...");

            // Wait for DHCP (on LWIP devices)
            while (true)
            {
                IPAddress ip = IPAddress.GetDefaultLocalAddress();

                if (ip != IPAddress.Any) break;

                Thread.Sleep(1000);
            }

            // Create a test application thread
            TestApplication testApp = new TestApplication();
            Thread testAppThread = new Thread(new ThreadStart(testApp.Run));
            testAppThread.Start();
        }
    }

    /// <summary>
    /// SimpleService test application class.
    /// </summary>
    public class TestApplication : IDisposable
    {
        private Random m_random;
        private object m_threadLock;
        private DpwsMetadata m_selectedService;
        private bool m_inOperation;
        private bool m_inDiscovery;
        private DpwsEventSubscription m_simpleEventSubscription;
        private DpwsEventSubscription m_integerEventSubscription;

        SimpleServiceClientProxy m_simpleServiceClient;
        EventingServiceClientProxy m_eventingServiceClient;
        AttachmentServiceClientProxy m_attachmentServiceClient;
        DiscoClient m_discoClient;

        string m_eventSubscriptionAddress;
        string m_deviceEndpointAddress;
        ProtocolVersion m_version;

        ManualResetEvent m_deviceSelected; // Tells the app a device is selected

        public TestApplication()
        {
            m_threadLock = new object();
            m_inOperation = false;
            m_deviceSelected = new ManualResetEvent(false);

            m_version = new ProtocolVersion11();
            // To enable WSDiscoveryApril2005 and Soap12WSAddressingAugust2004
            //m_version = new ProtocolVersion10();
            m_eventSubscriptionAddress = "";
            m_deviceEndpointAddress = "";

            m_simpleServiceClient = new SimpleServiceClientProxy(new WS2007HttpBinding(), m_version);

            // Event handling requires that we have a defined port (other than 8084)
            WS2007HttpBinding binding = new WS2007HttpBinding(new HttpTransportBindingConfig("urn:uuid:3cb0d1ba-cc3a-46ce-b416-212ac2419b07", 15338, true));
            m_eventingServiceClient = new EventingServiceClientProxy(binding, m_version, new EventingClientImplementation());

            m_attachmentServiceClient = new AttachmentServiceClientProxy(new WS2007HttpBinding(), m_version);

            // Turn listening to this IP on
            m_simpleServiceClient.IgnoreRequestFromThisIP     = false;
            m_eventingServiceClient.IgnoreRequestFromThisIP   = false;
            m_attachmentServiceClient.IgnoreRequestFromThisIP = false;

            m_discoClient = new DiscoClient(m_simpleServiceClient);

            m_random = new Random();
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                // dispose managed resources
                if (m_simpleServiceClient != null)
                {
                    m_simpleServiceClient.Dispose();
                    m_simpleServiceClient = null;
                }

                if (m_eventingServiceClient != null)
                {
                    m_eventingServiceClient.Dispose();
                    m_eventingServiceClient = null;
                }

                if (m_attachmentServiceClient != null)
                {
                    m_attachmentServiceClient.Dispose();
                    m_attachmentServiceClient = null;
                }
            }
            // free native resources
        }

        public void PrintMetadataInfo()
        {
            Debug.Print("Resolving the device...");
            // We need to resolve the device to get the transport address
            DpwsServiceDescription resolveMatch = m_simpleServiceClient.DiscoveryClient.Resolve(m_deviceEndpointAddress);
            //if(true)
            if (resolveMatch != null)
            {
                // Device was located
                string deviceTransportAddr = resolveMatch.XAddrs[0];

                // If transport address ends with '/' remove it
                int length = deviceTransportAddr.Length;
                if (deviceTransportAddr[length - 1] == '/')
                    deviceTransportAddr = deviceTransportAddr.Substring(0, length - 1);

                // Get metadata
                DpwsMexClient mexClient = new DpwsMexClient(m_version);
                DpwsMetadata metadata = mexClient.Get(deviceTransportAddr);
                if (metadata != null)
                {
                    Debug.Print("");
                    Debug.Print("Metadata:");
                    Debug.Print("ThisModel:");
                    Debug.Print("\tManufacturer: " + metadata.ThisModel.Manufacturer);
                    Debug.Print("\tManufacturerUrl: " + metadata.ThisModel.ManufacturerUrl);
                    Debug.Print("\tModelName: " + metadata.ThisModel.ModelName);
                    Debug.Print("\tModelNumber: " + metadata.ThisModel.ModelNumber);
                    Debug.Print("\tModelUrl: " + metadata.ThisModel.ModelUrl);
                    Debug.Print("\tPresentationUrl: " + metadata.ThisModel.PresentationUrl);
                    Debug.Print("ThisDevice:");
                    Debug.Print("\tFirmwareVersion: " + metadata.ThisDevice.FirmwareVersion);
                    Debug.Print("\tFriendlyName: " + metadata.ThisDevice.FriendlyName);
                    Debug.Print("\tSerialNumber: " + metadata.ThisDevice.SerialNumber);
                    DpwsMexService host = metadata.Relationship.Host;
                    if (host != null)
                    {
                        Debug.Print("Host:");
                        Debug.Print("\tServiceID: " + host.ServiceID);
                        Debug.Print("\tAddress: " + host.EndpointRefs[0].Address.AbsoluteUri);
                        Debug.Print("\tTypes:");
                        for (int t = 0; t < host.ServiceTypes.Count; ++t)
                        {
                            DpwsServiceType serviceType = host.ServiceTypes[t];
                            Debug.Print("\t\tName = " + serviceType.TypeName);
                            Debug.Print("\t\tNamespace = " + serviceType.NamespaceUri);
                            Debug.Print("");
                        }
                    }
                    DpwsMexServices hostedServices = metadata.Relationship.HostedServices;
                    if (hostedServices != null)
                    {
                        Debug.Print("HostedServices:");
                        for (int i = 0; i < hostedServices.Count; i++)
                        {
                            DpwsMexService hostedService = hostedServices[i];
                            Debug.Print("\tService ID: " + hostedService.ServiceID);
                            Debug.Print("\tAddress: " + hostedService.EndpointRefs[0].Address.AbsoluteUri);
                            Debug.Print("\tTypes:");
                            for (int t = 0; t < hostedService.ServiceTypes.Count; ++t)
                            {
                                DpwsServiceType serviceType = hostedService.ServiceTypes[t];
                                Debug.Print("\t\tName = " + serviceType.TypeName);
                                Debug.Print("\t\tNamespace = " + serviceType.NamespaceUri);
                                Debug.Print("");
                            }
                        }
                    }
                }
                else
                    Debug.Print("Did not get metadata from device.");
            }
            else
                Debug.Print("Device cannot be resolved.");
        }


        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Method runs a loop waiting for Hello events. When on is received method probes, resolves
        /// and Get's device service information and calls 1way, 2way methods and subscribes to SimpleEvent
        /// and IntegerEvent and waits for them to fire. When they do Unsubscribe is called on the events
        /// and the receive hello's flag is reset.
        /// </summary>
        public void Run()
        {
            // Create Event handlers
            m_simpleServiceClient.ByeEvent += new ByeEventHandler(m_simpleControl_ByeEvent);
            m_simpleServiceClient.HelloEvent += new HelloEventHandler(m_simpleControl_HelloEvent);
            m_simpleServiceClient.SubscriptionEndEvent += new SubscriptionEndEventHandler(m_simpleControl_SubscriptionEndEvent);
            bool firstPass = true;

            bool twoWayAttach = false;

            DpwsServiceTypes typeProbes = new DpwsServiceTypes();
            typeProbes.Add(new DpwsServiceType("SimpleDeviceType", "http://schemas.example.org/SimpleService"));

            // Continuous run loop
            while (true)
            {
                if (firstPass && !m_inDiscovery)
                {
                    m_inDiscovery = true;
                    DpwsServiceDescriptions descs = m_simpleServiceClient.DiscoveryClient.Probe(typeProbes, 3, 5000);

                    for (int i = 0; i < descs.Count; i++)
                    {
                        DpwsServiceDescription desc = descs[i];

                        if (desc.XAddrs != null && desc.XAddrs.Length > 0)
                        {
                            if (CheckConnection(desc.ServiceTypes, desc.Endpoint.Address.AbsoluteUri)) break;
                        }
                    }
                    m_inDiscovery = false;
                }

                // If hello was received and a SimpleService device was found. SeletedService will not be null.
                // Process until Bye is received.
                if (m_deviceSelected.WaitOne(5000, false))
                {
                    // If this is the first time through the loop for this device subscribe to events
                    if (firstPass)
                    {
                        try
                        {
                            // Test service host call
                            System.Ext.Console.Write("Testing Host service...");
                            DpwsSubscribeRequest subReq;
                            subReq = new DpwsSubscribeRequest(m_eventingServiceClient.EventSources["SimpleEvent"], m_eventSubscriptionAddress, m_eventingServiceClient.TransportAddress, "PT1H", null);
                            m_simpleEventSubscription = m_eventingServiceClient.EventingClient.Subscribe(subReq);
                            subReq = new DpwsSubscribeRequest(m_eventingServiceClient.EventSources["IntegerEvent"], m_eventSubscriptionAddress, m_eventingServiceClient.TransportAddress, "PT1H", null);
                            m_integerEventSubscription = m_eventingServiceClient.EventingClient.Subscribe(subReq);

                            firstPass = false;
                        }
                        catch
                        {
                            m_deviceSelected.Reset();
                        }
                    }

                    // Make 1Way and 2Way service calls
                    if (m_deviceSelected.WaitOne(0, false))
                    {
                        try
                        {
                            PrintMetadataInfo();

                            System.Ext.Console.Write("");
                            System.Ext.Console.Write(">>>>>>>>>>>>> Sending 1way(10) request to: " + m_selectedService.ThisDevice.FriendlyName);
                            m_simpleServiceClient.OneWay(new OneWayRequest());
                        }
                        catch (Exception e)
                        {
                            firstPass = true;
                            m_deviceSelected.Reset();
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! OneWay method failed. " + e.Message);
                        }
                    }

                    if (m_deviceSelected.WaitOne(0, false))
                    {
                        int x = System.Math.Abs(m_random.Next()) % 100;
                        int y = System.Math.Abs(m_random.Next()) % 100;

                        System.Ext.Console.Write("");
                        System.Ext.Console.Write(">>>>>>>>>>>>> Sending 2way(" + x.ToString() + ", " + y.ToString() + ") request to: " + m_selectedService.ThisDevice.FriendlyName);
                        try
                        {
                            TwoWayRequest req = new TwoWayRequest();
                            req.X = x;
                            req.Y = y;

                            TwoWayResponse resp = m_simpleServiceClient.TwoWay(req);
                            if (resp.Sum == 0)
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 2way method did not receive a valid response.");
                            }
                            else
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("<<<<<<<<<<<<< 2way response returned " + resp.Sum);
                            }
                        }
                        catch (Exception e)
                        {
                            firstPass = true;
                            m_deviceSelected.Reset();
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TwoWay method failed. " + e.Message);
                        }
                    }

                    // Make 1wayattach or a 2wayattach service calls
                    if (m_deviceSelected.WaitOne(0, false))
                    {
                        if (!twoWayAttach)
                        {
                            // create an instance of the help icon test object
                            HelpIcon helpIcon = new HelpIcon();

                            System.Ext.Console.Write("");
                            System.Ext.Console.Write(">>>>>>>>>>>>> Sending 1wayattach request to: " + m_selectedService.ThisDevice.FriendlyName);
                            try
                            {
                                OneWayAttachmentRequest req = new OneWayAttachmentRequest();
                                req.Param = helpIcon.Data.ToArray();
                                m_attachmentServiceClient.OneWayAttachment(req);
                            }
                            catch (Exception e)
                            {
                                firstPass = true;
                                m_deviceSelected.Reset();
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 1wayattach method failed: " + e.Message);
                            }
                        }
                        else
                        {
                            // create an instance of the help icon test object
                            HelpIcon helpIcon = new HelpIcon();

                            System.Ext.Console.Write("");
                            System.Ext.Console.Write(">>>>>>>>>>>>> Sending 2wayattach request to: " + m_selectedService.ThisDevice.FriendlyName);
                            try
                            {
                                TwoWayAttachmentRequest req = new TwoWayAttachmentRequest();
                                req.Param = helpIcon.Data.ToArray();
                                TwoWayAttachmentResponse resp = m_attachmentServiceClient.TwoWayAttachment(req);

                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("<<<<<<<<<<<<< Sending 2wayattach request succeeded");
                            }
                            catch (Exception e)
                            {
                                firstPass = true;
                                m_deviceSelected.Reset();
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TwoWay method failed. " + e.Message);
                            }
                        }

                        twoWayAttach = !twoWayAttach;
                    }
                }
                else
                {
                    firstPass = true;
                }

                GC.WaitForPendingFinalizers();
            }
        }

        void m_simpleControl_SubscriptionEndEvent(object obj, SubscriptionEndEventArgs SubscriptionEndEventArgs)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("Subsription End event received:");
            System.Ext.Console.Write("  SubscriptionID = " + SubscriptionEndEventArgs.SubscriptionID);
        }

        bool CheckConnection(DpwsServiceTypes servTypes, string servAddr)
        {
            try
            {
                bool simpleDeviceTypeFound = false;
                string types = "";
                for (int i = 0; i < servTypes.Count; ++i)
                {
                    if (servTypes[i].TypeName == "SimpleDeviceType")
                        simpleDeviceTypeFound = true;
                    types += "NamespaceUri: " + servTypes[i].NamespaceUri + " " + "TypeName: " + servTypes[i].TypeName + "\n";
                }
                System.Ext.Console.Write("Types: " + types);
                System.Ext.Console.Write("");

                // If this is not a SimplDeviceType don't discover
                if (simpleDeviceTypeFound == false)
                {
                    System.Ext.Console.Write("");
                    System.Ext.Console.Write("Discovery will only resolve SimpleDeviceType services.");
                }

                // Get SimpleService metadata
                DpwsMetadata deviceMetadata = m_discoClient.GetSimpleService(servAddr);
                if (deviceMetadata != null)
                {
                    SelectedService = deviceMetadata;
                    m_deviceSelected.Set();
                    m_deviceEndpointAddress = servAddr;

                    return true;
                }
            }
            catch
            {
            }

            return false;
        }

        void m_simpleControl_HelloEvent(object obj, DpwsServiceDescription helloEventArgs)
        {
            // If SelectedDevice is set don't do discovery again until Bye event is received
            if (m_deviceSelected.WaitOne(0, false) || m_inDiscovery)
                return;

            m_inDiscovery = true;

            // Print Hello information
            try
            {
                System.Ext.Console.Write("");
                System.Ext.Console.Write("SimpleServiceClient received a hello request.");
                System.Ext.Console.Write("Endpoint address: " + helloEventArgs.Endpoint.Address.ToString());

                CheckConnection(helloEventArgs.ServiceTypes, helloEventArgs.Endpoint.Address.AbsoluteUri);

                System.Ext.Console.Write("Xaddrs: " + ((helloEventArgs.XAddrs == null) ? "none" : helloEventArgs.XAddrs[0]));
                System.Ext.Console.Write("Metadata version: " + helloEventArgs.MetadataVersion);
            }
            finally
            {
                m_inDiscovery = false;
            }
        }

        void m_simpleControl_ByeEvent(object obj, DpwsServiceDescription byeEventArgs)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("SimpleServiceClient received a bye request.");
            System.Ext.Console.Write("Endpoint address: " + byeEventArgs.Endpoint.Address.ToString());
            if (byeEventArgs.XAddrs != null)
            {
                System.Ext.Console.Write("Xaddrs: " + byeEventArgs.XAddrs[0]);
            }
            System.Ext.Console.Write("");

            if (m_deviceSelected.WaitOne(0,false) && m_simpleServiceClient.EndpointAddress == byeEventArgs.Endpoint.Address.ToString())
            {
                m_deviceSelected.Reset();
                m_selectedService = null;
            }
        }

        /// <summary>
        /// Property containing a DpwsMetadata object that defines the service endpoints.
        /// </summary>
        /// <remarks>
        /// Set this property prior to calling the methods. Obtain this information from a discovery
        /// Get reequest.
        /// </remarks>
        public DpwsMetadata SelectedService
        {
            get
            {
                lock (m_threadLock)
                {
                    return m_selectedService;
                }
            }
            set
            {
                lock (m_threadLock)
                {
                    // If an operatoin is pending wait until it finishes
                    while (m_inOperation == true)
                        Thread.Sleep(100);

                    // If existing event subscriptions are pending unsubscribe
                    if (m_integerEventSubscription != null)
                    {
                        try
                        {
                            // Unsubscribe to simple event
                            if (m_simpleEventSubscription != null)
                            {
                                if (!m_eventingServiceClient.EventingClient.Unsubscribe(new Uri(m_eventingServiceClient.EndpointAddress), m_simpleEventSubscription))
                                    System.Ext.Console.Write("Unsubscribe to SimpleEvent at service endpoint " + m_eventingServiceClient.EndpointAddress + " failed!");
                            }

                            // Unsubscribe to integer event
                            if (m_integerEventSubscription != null)
                            {
                                if (!m_eventingServiceClient.EventingClient.Unsubscribe(new Uri(m_eventingServiceClient.EndpointAddress), m_integerEventSubscription))
                                {
                                    System.Ext.Console.Write("");
                                    System.Ext.Console.Write("Unsubscribe to IntegerEvent at service endpoint " + m_eventingServiceClient.EndpointAddress + " failed!");
                                }
                            }
                        }
                        catch
                        {
                        }
                    }

                    // If value is null the controls service endpoint will be cleared
                    // otherwise set the SimpleService and EventingService endpoints
                    if (value != null)
                    {
                        // Locate SimpleService and EventingService in metadata and set their endpoint addresses
                        for (int i = 0; i < value.Relationship.HostedServices.Count; ++i)
                        {
                            DpwsMexService hostedService = value.Relationship.HostedServices[i];
                            if (hostedService.ServiceTypes["EventingService"] != null)
                                m_eventSubscriptionAddress = hostedService.EndpointRefs[0].Address.AbsoluteUri;

                            if (hostedService.ServiceTypes["SimpleService"] != null)
                                m_simpleServiceClient.EndpointAddress = hostedService.EndpointRefs[0].Address.AbsoluteUri;

                            if (hostedService.ServiceTypes["AttachmentService"] != null)
                                m_attachmentServiceClient.EndpointAddress = hostedService.EndpointRefs[0].Address.AbsoluteUri;
                        }

                        // Verify that both addresses were found.
                        if (m_eventingServiceClient.EndpointAddress == null)
                        {
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("EventinService was not found! Events are disabled.");
                        }

                        if (m_simpleServiceClient.EndpointAddress == null)
                        {
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("SimpleService was not found! Method calls are disabled.");
                        }
                    }

                    // Set the controle endpoint to the new endpoint
                    m_selectedService = value;
                }
            }
        }
    }
}
