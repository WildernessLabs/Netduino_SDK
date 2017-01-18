//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     .NET Micro Framework MFSvcUtil.Exe
//     Runtime Version:2.0.00001.0001
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------


using System;
using System.Text;
using System.Xml;
using Dpws.Device;
using Dpws.Device.Services;
using Ws.Services;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;
using Ws.Services.Binding;
using Ws.Services.Soap;

namespace schemas.example.org.EventingService
{
    
    
    public class EventingService : DpwsHostedService
    {
        
        public EventingService(ProtocolVersion version) : 
                base(version)
        {

            // Set base service properties
            ServiceNamespace = new WsXmlNamespace("eve", "http://schemas.example.org/EventingService");
            ServiceID = "urn:uuid:cd35d6ba-3e2f-440b-bc2f-b7c375dc6536";
            ServiceTypeName = "EventingService";

            // Add service types here

            // Add event sources here
            EventSources.Add(new DpwsWseEventSource("eve", "http://schemas.example.org/EventingService", "SimpleEvent"));
            EventSources.Add(new DpwsWseEventSource("eve", "http://schemas.example.org/EventingService", "IntegerEvent"));
            this.AddEventServices();
        }
               
        public virtual void SimpleEvent(SimpleEventRequest eventReq)
        {

            // Create temp event source object, set the event action and create the event header
            DpwsWseEventSource eventSource;
            eventSource = EventSources["SimpleEvent"];
            String action;
            action = "http://schemas.example.org/EventingService/SimpleEvent";
            WsWsaHeader header;
            header = new WsWsaHeader(action, null, null, null, null, null);
            WsMessage msg = new WsMessage(header, eventReq, WsPrefix.Wse);

            // Create event serializer and write the event object
            SimpleEventRequestDataContractSerializer eventDcs;
            eventDcs = new SimpleEventRequestDataContractSerializer("SimpleEventRequest", "http://schemas.example.org/EventingService");
            msg.Serializer = eventDcs;

            // Fire event
            Dpws.Device.Device.SubscriptionManager.FireEvent(this, eventSource, msg);
        }
        
        public virtual void IntegerEvent(IntegerEventRequest eventReq)
        {

            // Create temp event source object, set the event action and create the event header
            DpwsWseEventSource eventSource;
            eventSource = EventSources["IntegerEvent"];
            String action;
            action = "http://schemas.example.org/EventingService/IntegerEvent";
            WsWsaHeader header;
            header = new WsWsaHeader(action, null, null, null, null, null);
            WsMessage msg = new WsMessage(header, eventReq, WsPrefix.Wse);

            // Create event serializer and write the event object
            IntegerEventRequestDataContractSerializer eventDcs;
            eventDcs = new IntegerEventRequestDataContractSerializer("IntegerEventRequest", "http://schemas.example.org/EventingService");
            msg.Serializer = eventDcs;

            // Fire event
            Dpws.Device.Device.SubscriptionManager.FireEvent(this, eventSource, msg);
        }
    }
}
