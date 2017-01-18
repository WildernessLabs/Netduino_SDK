using System;
using System.Collections;
using System.Text;
using System.Threading;
using System.IO;
using System.Xml;
using Dpws.Device;
using Dpws.Device.Services;
using Ws.Services.Utilities;
using Ws.Services.WsaAddressing;

using System.Ext.Xml;
using Ws.Services;
using Microsoft.SPOT;
using System.Ext;

namespace schemas.example.org.EventingService
{
    /// <summary>
    /// Defines an event simulator.
    /// </summary>
    class EventSimulator : IDisposable
    {

        // Temporary event simulator declarations
        static Timer m_stateTimer = null;
        DpwsHostedService m_hostedService = null;
        object m_threadLock = new object();
        bool rentryFlag = false;
        static int s_cnt = 0;

        public EventSimulator(DpwsHostedService hostedService)
        {
            m_hostedService = hostedService;
        }

        // Disposer stops and kills Event Simulator timers
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        //
        // Summary:
        //     Releases the unmanaged resources used by the System.Net.Sockets.UdpClient
        //     and optionally releases the managed resources.
        //
        // Parameters:
        //   disposing:
        //     true to release both managed and unmanaged resources; false to release only
        //     unmanaged resources.
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                StopEventSimulator();
                m_stateTimer = null;
            }
        }

        static byte[] s_val = new byte[1024];

        // This method is called by the timer delegate.
        public void SendEvent(Object stateInfo)
        {
            if (rentryFlag == true)
                return;
            rentryFlag = true;

            // Fire SimpleEvent
            try
            {
                SimpleEventRequest req = new SimpleEventRequest();
                req.Param = s_val;

                ((EventingService)m_hostedService).SimpleEvent(req);
                System.Ext.Console.Write("");
                System.Ext.Console.Write("FireEvent called SimpleEvent!");
                System.Ext.Console.Write("");
            }
            catch (Exception e)
            {
                System.Ext.Console.Write("");
                System.Ext.Console.Write("SimpleEvent FireEvent failed: " + e.Message);
                System.Ext.Console.Write("");
                rentryFlag = false;
            }

            // Fire IntegerEvent
            try
            {
                IntegerEventRequest eventReq = new IntegerEventRequest();
                eventReq.Param = s_cnt++;
                ((EventingService)m_hostedService).IntegerEvent(eventReq);
                System.Ext.Console.Write("");
                System.Ext.Console.Write("FireEvent called IntegerEvent!!! " + eventReq.Param.ToString());
                System.Ext.Console.Write("");
            }
            catch (Exception e)
            {
                System.Ext.Console.Write("");
                System.Ext.Console.Write("IntegerEvent FireEvent failed: " + e.Message);
                System.Ext.Console.Write("");
                rentryFlag = false;
            }

            rentryFlag = false;
        }

        // Initialize and start event simulator
        public void StartEventSimulator()
        {

            // Create the delegate that invokes methods for the timer.
            TimerCallback timerDelegate = new TimerCallback(this.SendEvent);

            m_stateTimer = new Timer(timerDelegate, null, 10000, 20000);
            return;
        }

        // Stop event simulator
        public void StopEventSimulator()
        {
            if (m_stateTimer != null)
            {
                m_stateTimer.Dispose();
                m_stateTimer = null;
            }
            return;
        }
    }
}
