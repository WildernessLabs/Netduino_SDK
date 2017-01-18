////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Wireless.IEEE_802_15_4;
using Microsoft.SPOT.Wireless.IEEE_802_15_4.Network;

namespace IEEE_802_15_4_Sample
{
    public class Program
    {
        class Monitor : IMonitor
        {
            public void Print(string s)
            {
                Microsoft.SPOT.Wireless.IEEE_802_15_4.Trace.Print(s);
            }

            public void Status(NodeStatus[] status)
            {
                Print("Status update");
                if (status != null)
                {
                    for (int i = 0; i < status.Length; i++)
                    {
                        Print("0x" + HexConverter.ConvertUintToHex(status[i].addr, 4) + ":");

                        if (status[i].txBps > 0)
                        {
                            Print("  TX: " + status[i].txBps.ToString() + " bytes/s");
                        }

                        if (status[i].neighbors == null || status[i].neighbors.Length == 0)
                        {
                            Print("  no neighbors");
                        }
                        else
                        {
                            for (int j = 0; j < status[i].neighbors.Length; j++)
                            {
                                Print("  neighbor 0x" + HexConverter.ConvertUintToHex(status[i].neighbors[j].addr, 4) +
                                    ": LQI " + status[i].neighbors[j].lqi);
                            }
                        }

                        if (status[i].traffic == null || status[i].traffic.Length == 0)
                        {
                            Print("  no traffic");
                        }
                        else
                        {
                            for (int j = 0; j < status[i].traffic.Length; j++)
                            {
                                Print("  traffic from 0x" + HexConverter.ConvertUintToHex(status[i].traffic[j].addr, 4) +
                                    ": " + status[i].traffic[j].rxBps + " bytes/sec");
                            }
                        }
                    }
                }
            }
        }

        public static void Main()
        {
            NetworkLayer net = Singleton.Instance;
            if (net == null)
            {
                Debug.Print("Failed to initialize network layer");
                return;
            }

            Monitor monitor = new Monitor();

            NetworkTest test = new NetworkTest(net, monitor);
            test.Run(null);
        }
    }
}


