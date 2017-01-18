using System;
using Dpws.Client;
using Dpws.Client.Discovery;

#if !Windows && !WindowsCE
using Microsoft.SPOT;
using System.Ext;
#endif

namespace Interop.SimpleService
{
    class DiscoClient
    {
        private DpwsClient m_dpwsClient = null;
        
        /// <summary>
        /// Creates an instance of the eventing client.
        /// </summary>
        /// <param name="client">The DpwsClient instance that will receive events.</param>
        public DiscoClient(DpwsClient client)
        {
            m_dpwsClient = client;
        }

        /// <summary>
        /// Probe for a SimpleService endpoint.
        /// </summary>
        /// <param name="serviceAddress">A string containing the address of the simple service to find.</param>
        /// <returns>
        /// A DpwsMetadata object containing details about a SimpleService, null if
        /// a SImpleService is not found.
        /// </returns>
        public DpwsMetadata GetSimpleService(string serviceAddress)
        {
            // Resolve the service address
            m_dpwsClient.DiscoveryClient.ReceiveTimeout = 10000;
            DpwsServiceDescription resolveMatch = m_dpwsClient.DiscoveryClient.Resolve(serviceAddress);

            // Display results
            string serviceEndpoint = null;
            string targetService = null;
            if (resolveMatch != null)
            {
                // Print resolve match information
                System.Ext.Console.Write("");
                System.Ext.Console.Write("ResolveMatch:");
                System.Ext.Console.Write("  Endpoint Address = " + resolveMatch.Endpoint.Address);
                System.Ext.Console.Write("  Types:");
                targetService = resolveMatch.Endpoint.Address.AbsoluteUri;
                for (int i = 0; i < resolveMatch.ServiceTypes.Count; ++i)
                {
                    System.Ext.Console.Write("    Name = " + resolveMatch.ServiceTypes[i].TypeName);
                    System.Ext.Console.Write("    Namespace = " + resolveMatch.ServiceTypes[i].NamespaceUri);
                }
                System.Ext.Console.Write("  Xaddrs:");
                foreach (string xaddr in resolveMatch.XAddrs)
                {
                    System.Ext.Console.Write("    TransportAddress = " + xaddr);

                    // Since there's no support for IPV6 look for IPV4 address
                    if (xaddr.IndexOf(':') != -1)
                    {
                        serviceEndpoint = xaddr;
                    }
                }

                System.Ext.Console.Write("  Metadata Version = " + resolveMatch.MetadataVersion);
            }
            else
            {
                System.Ext.Console.Write("");
                System.Ext.Console.Write("SimpleDeviceType at address: " + serviceAddress + " failed to resolve. Discovery aborted");
                return null;
            }

            // Mae sure we got an address
            if (serviceEndpoint == null)
            {
                System.Ext.Console.Write("");
                System.Ext.Console.Write("Resolve did not send an IPV4 xaddr. Discovery aborted.");
            }

            // Send DirectedProbe
            DpwsMetadata m_mexDetails = null;

            // Create an empty search types collection
            DpwsServiceTypes searchTypes = new DpwsServiceTypes();

            // Build a SimpleDeviceType search type
            DpwsServiceType searchType1 = new DpwsServiceType("SimpleService", "http://schemas.example.org/SimpleService");
            searchTypes.Add(searchType1);
            
            // Get the SimpleDeviceType service details
            // Note: Uncomment the next line to test DirectedProbe to any MF Dpws service
            // DpwsServiceDescriptions probeMatches = m_dpwsClient.DiscoveryClient.DirectedProbe(serviceEndpoint, m_dpwsClient.DiscoveryClient.DiscoVersion.WellKnownAddress, searchTypes);
            // Note: Uncomment the next line to test against a DiscoveryProxy
            // DpwsServiceDescriptions probeMatches = m_dpwsClient.DiscoveryClient.DirectedProbe(serviceEndpoint, targetService, searchTypes);
            DpwsServiceDescriptions probeMatches = m_dpwsClient.DiscoveryClient.Probe(searchTypes);
            if (probeMatches != null && probeMatches.Count > 0)
            {
                // Select the probe match
                DpwsServiceDescription probeMatch = probeMatches[0];

                // Note: Uncoment the following line to test DirectedResolve against any MF Dpws Service.
                // DpwsServiceDescription resolveMatch1 = m_dpwsClient.DiscoveryClient.DirectedResolve(probeMatch.XAddrs[0], m_dpwsClient.DiscoveryClient.DiscoVersion.WellKnownAddress, serviceAddress);

                // Print the probe match information
                System.Ext.Console.Write("**********************");
                System.Ext.Console.Write("ProbeMatch received: " + probeMatches.Count);
                System.Ext.Console.Write("");
                System.Ext.Console.Write("ProbeMatch:");
                System.Ext.Console.Write("  Endpoint Address = " + probeMatch.Endpoint.Address);
                System.Ext.Console.Write("  Types:");
                for (int i = 0; i < probeMatch.ServiceTypes.Count; ++i)
                {
                    System.Ext.Console.Write("    Name = " + probeMatch.ServiceTypes[i].TypeName);
                    System.Ext.Console.Write("    Namespace = " + probeMatch.ServiceTypes[i].NamespaceUri);
                }
                System.Ext.Console.Write("  Xaddrs:");
                foreach (string xaddr in probeMatch.XAddrs)
                    System.Ext.Console.Write("    TransportAddress = " + xaddr);
                System.Ext.Console.Write("  Metadata Version = " + probeMatch.MetadataVersion);
            }

            // If we had a probe match store the service endpoint and get metadata
            if (probeMatches != null)
            {
                m_dpwsClient.EndpointAddress = serviceAddress;
                m_mexDetails = m_dpwsClient.MexClient.Get(serviceEndpoint);

                if (m_mexDetails == null)
                {
                    System.Ext.Console.Write("");
                    System.Ext.Console.Write("Get did not receive metadata. Discovery aborted.");
                    return null;
                }
                // Display metadata information
                System.Ext.Console.Write("  Metadata:");
                System.Ext.Console.Write("    ThisModel:");
                System.Ext.Console.Write("      Manufacturer: " + m_mexDetails.ThisModel.Manufacturer);
                System.Ext.Console.Write("      ManufacturerUrl: " + m_mexDetails.ThisModel.ManufacturerUrl);
                System.Ext.Console.Write("      ModelName: " + m_mexDetails.ThisModel.ModelName);
                System.Ext.Console.Write("      ModelNumber: " + m_mexDetails.ThisModel.ModelNumber);
                System.Ext.Console.Write("      ModelUrl: " + m_mexDetails.ThisModel.ModelUrl);
                System.Ext.Console.Write("      PresentationUrl: " + m_mexDetails.ThisModel.PresentationUrl);
                System.Ext.Console.Write("    ThisDevice:");
                System.Ext.Console.Write("      FirmwareVersion: " + m_mexDetails.ThisDevice.FirmwareVersion);
                System.Ext.Console.Write("      FriendlyName: " + m_mexDetails.ThisDevice.FriendlyName);
                System.Ext.Console.Write("      SerialNumber: " + m_mexDetails.ThisDevice.SerialNumber);
                if (m_mexDetails.Relationship.Host != null)
                {
                    System.Ext.Console.Write("    Host:");
                    System.Ext.Console.Write("      Service ID: " + m_mexDetails.Relationship.Host.ServiceID);
                    System.Ext.Console.Write("      Address: " + m_mexDetails.Relationship.Host.EndpointRefs[0].Address.AbsoluteUri);
                    System.Ext.Console.Write("      Types:");
                    for (int i = 0; i < m_mexDetails.Relationship.Host.ServiceTypes.Count; ++i)
                    {
                        DpwsServiceType serviceType = m_mexDetails.Relationship.Host.ServiceTypes[i];
                        System.Ext.Console.Write("        Name: " + serviceType.TypeName);
                        System.Ext.Console.Write("        NamespaceUri: " + serviceType.NamespaceUri);
                    }
                }
                if (m_mexDetails.Relationship.HostedServices != null)
                {
                    System.Ext.Console.Write("    HostedServices:");
                    for (int i = 0; i < m_mexDetails.Relationship.HostedServices.Count; ++i)
                    {
                        DpwsMexService hostedService = m_mexDetails.Relationship.HostedServices[i];
                        System.Ext.Console.Write("      Service ID: " + hostedService.ServiceID);
                        System.Ext.Console.Write("      Address: " + hostedService.EndpointRefs[0].Address.AbsoluteUri);
                        System.Ext.Console.Write("      Types:");
                        for (int ii = 0; ii < hostedService.ServiceTypes.Count; ++ii)
                        {
                            System.Ext.Console.Write("        Type name: " + hostedService.ServiceTypes[ii].TypeName);
                            System.Ext.Console.Write("        NamespaceUri: " + hostedService.ServiceTypes[ii].NamespaceUri);
                        }
                    }
                }
            }
            return m_mexDetails;
        }
    }
}
