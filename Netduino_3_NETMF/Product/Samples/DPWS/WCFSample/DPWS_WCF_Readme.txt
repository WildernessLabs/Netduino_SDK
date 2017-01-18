WCF Sample
====================
This sample demonstrates the following:
-  A Micro Framework DPWS client (HelloWorldClient_MF) connection to a WCF service 
   (HelloWorldServer_WCF).
-  A WCF client (HelloWorldClient_WCF) connection to a Micro Framework service 
   (HelloWorldServer_MF).

The MF samples runs on the following devices:
-  The .NET Micro Framework SDK Emulator.
-  Any emulator that implements the Microsoft.SPOT.Emulator.Sockets interface.
-  Any .NET Micro Framework devices that supports networking

These samples were created in the following way:
1) In VS2010, Create a Console application (for the WCF Service)
2) Create service contract interface
   For the sample it was created in HelloWorldServer_WCF\IServiceHelloWCF.cs
3) Implement the service contract (HelloWorldService.cs)
4) Create a ServiceHost with a ServiceEndpoint for IServiceHelloWCF and a discovery
   endpoint (v1.1) (Program.cs).
5) Generate the MF proxy code 

   This will produce proxies for both client and server (xxxClientProxy.cs and
   xxxHostedService.cs)

  	a) Using the WCF assembly

       	i. Generate the WSDL
		   MFSvcUtil.exe <WCFServiceAssemblyName>.exe
	       
		   (type MfSvcUtil.exe /? for more options).

		ii. Generate the proxy code
		 	MFSvcUtil.exe <WsdlName>.wsdl 

6) Create a Micro Framework Application (console or window)

7) Add the client proxy code and the serialization code generated in step 6)
   to the project.  You will not need the [ServiceName]HostedService.cs file.

   [ServiceName]ClientProxy.cs
   [ServiceName].cs

8) In you projects Main method create a client proxy object.  The HelloWorldClient_MF
   application implements a Discovery method as well to automatically discover the service.
    
	Uri remoteEp = new Uri("http://localhost:[LocalEndpointPort]/[ServiceName]");
    
	WS2007HttpBinding binding = new WS2007HttpBinding(new HttpTransportBindingConfig(remoteEp));

        m_clientProxy = new ServiceHelloWCFClientProxy(binding, new ProtocolVersion11());


9) Call the WCF operations from client proxy and call Dispose when you are finished.