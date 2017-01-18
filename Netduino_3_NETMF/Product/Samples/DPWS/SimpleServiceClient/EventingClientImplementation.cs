using System;

#if !Windows && !WindowsCE
using Microsoft.SPOT;
using System.Ext;
#endif

namespace schemas.example.org.EventingService
{
    public class EventingClientImplementation : IEventingServiceCallback
    {
        public void SimpleEvent(SimpleEventRequest req)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("!SimpleEvent received. Size: " + (req.Param != null ? req.Param.Length.ToString() : "null"));
        }

        public void IntegerEvent(IntegerEventRequest req)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("!!IntegerEvent received. Value = " + req.Param.ToString());
        }
    }
}
