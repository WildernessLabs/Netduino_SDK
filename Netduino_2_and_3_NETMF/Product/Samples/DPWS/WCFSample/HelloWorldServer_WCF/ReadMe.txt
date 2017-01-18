WCF Server Sample
====================
This sample demonstrates the following:
- Exposing a WCF Web service, to be consumed by using a Micro Framework Device 
  Profiles for Web Services (DPWS) client and server.

The WcfServer sample works in conjunction with the HelloWorldClient_MF sample.


To build and run this sample:
1. Open WcfServer.sln in Visual Studio 2010.

2. In the Build menu, select Start Debugging (or press F5).

3. Open HelloWorldClient_MF\HelloWorldClient.sln in another instance of Visual
   Studio 2010.

   a. To run a sample in the Emulator, open the Properties page for the project, 
      set the Transport property to Emulator, and then select the emulator you want 
      to use. 

   b. To run a sample on a device, open the Properties page for the project, set 
      the Transport property to the transport that your device supports, and then 
      select the device you want to target. 

4. In the HelloWorldClient project, open the file HelloWorldClientApp.cs.  
   Set a breakpoint on the method TestApplication.Run.
   
5. In HelloWorldClient, in the Build menu, select Start Debugging (or press F5).  

6. Place other breakpoints to investigate the behavior of other DPWS messaging 
   features. 


This solution provides sample code, rather than ship-ready code, and is provided 
for instructional purposes only.
  
The HelloWorldClient sample was created using MFSvcUtil, which is a code 
generation tool for .NET Micro Framework.  To produce the auto generated 
client and server proxies, run the MFSvcUtil.exe on a WSDL file.  If the WSDL
is not present, it can be generated with the MFSvcUtil tool by running it on 
the web service assembly (except for WCF assemblies).  For WCF assemblies, the
HttpGet service metadata behavior must be added the service host.  The WSDL
will then be generated during execution of the WCF service at the service address.
Use a web browser to view the WSDL.  You can pass the WSDL's URL directly into 
MfSvcUtil.exe but you must specify an output file name with the -o option.

