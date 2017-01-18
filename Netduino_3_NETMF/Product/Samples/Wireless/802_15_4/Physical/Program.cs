using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Wireless.IEEE_802_15_4;
using Microsoft.SPOT.Wireless.IEEE_802_15_4.Phy;

namespace IEEE_802_15_4_Sample
{

    public class Program
    {
        IPhy phy;

        static void TestPins()
        {
            Cpu.Pin[] pins = new Cpu.Pin[] {    /*(Cpu.Pin)63, (Cpu.Pin)62, (Cpu.Pin)60, (Cpu.Pin)61,
                                                (Cpu.Pin)46, (Cpu.Pin)51, (Cpu.Pin)48, (Cpu.Pin)49*/

                                                (Cpu.Pin)44, (Cpu.Pin)42, (Cpu.Pin)48, (Cpu.Pin)43, (Cpu.Pin)40,
                                            };
            for (int i = 0; i < pins.Length; i++)
            {
                try
                {
                    using (OutputPort port = new OutputPort(pins[i], false))
                    {
                        port.Write(true);
                        port.Write(false);
                        port.Write(true);
                    }
                }
                catch (Exception e)
                {
                    Debug.Print(e.ToString());
                }
            }
        }

        void TestSPI()
        {
            SPI spi = new SPI(new SPI.Configuration((Cpu.Pin)28, false, 0, 0, false, false, 1, SPI.SPI_module.SPI1));
            byte[] write = new byte[] { 0, 0, 0 };
            byte[] read = new byte[] { 0, 0, 0 };

            spi.WriteRead(write, read, 2);
            spi.WriteRead(write, read, 2);
        }

        public Program()
        {
            phy = new CC2420(CC2420PinConfig.DefaultiMXS());
        }

        private static void Assert(bool val)
        {
            if (!val)
                throw new SystemException();
        }

        public void TestCommon()
        {
            // GetFrameHeaders
            {
                int mtu, head, tail;
                phy.GetMtuSize(out mtu, out head, out tail);
                Assert(mtu == 127);
                Assert(head == 2);
                Assert(tail == 0);
            }

            // Capabilities
            {
                bool res;
                phy.IsCapabilitySupported(Capabilities.AddressFilter, out res);
                Assert(res == true);
                phy.IsCapabilitySupported(Capabilities.AutoAck, out res);
                Assert(res == true);
                phy.IsCapabilitySupported(Capabilities.AutoFcs, out res);
                Assert(res == true);
                phy.IsCapabilitySupported(Capabilities.PowerOff, out res);
                Assert(res == true);
            }

            phy.SetPower(true);

            // GetDeviceAddress
            {
                UInt64 mac;
                phy.GetDeviceAddress(out mac);
                String s = String.Empty;
                for (int i = 0; i < 8; i++)
                {
                    if (i > 0)
                        s += ":";
                    s += HexConverter.ConvertUintToHex((UInt32)((mac >> i * 8) & 0xFF), 2);
                }

                Debug.Print(s);
            }

            // GetRequest
            {
                Status status;
                PibValue value;

                // constants
                phy.GetRequest(PibAttribute.phyChannelsSupported, out status, out value);
                Assert(status == Status.Success);
                int[] res = value.IntArray;
                Assert(res != null);
                Assert(res.Length == 1);
                Assert((res[0] & 0xF8000000) == 0); // page 0
                for (int i = 0; i <= 27; i++)
                {
                    bool val = (res[0] & (1 << i)) != 0;
                    bool expected = (i >= 11 && i <= 26);
                    Assert(val == expected);
                }

                phy.GetRequest(PibAttribute.phyCCAMode, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == 3);

                phy.GetRequest(PibAttribute.phyCurrentPage, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == 0);

                phy.GetRequest(PibAttribute.phyMaxFrameDuration, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == 1064);

                phy.GetRequest(PibAttribute.phySHRDuration, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == 40);

                phy.GetRequest(PibAttribute.phySymbolsPerOctet, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Float == 8);

                // defaults
                phy.GetRequest(PibAttribute.phyCurrentChannel, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == 11);

                phy.GetRequest(PibAttribute.phyTransmitPower, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == -1);
            }

            // SetRequest
            {
                // constants
                Status status;
                PibValue value = new PibValue();

                int[] res = new int[1];
                res[0] = 256;
                value.IntArray = res;
                phy.SetRequest(PibAttribute.phyChannelsSupported, value, out status);
                Assert(status == Status.ReadOnly);

                value.Int = 0;
                phy.SetRequest(PibAttribute.phyCCAMode, value, out status);
                Assert(status == Status.ReadOnly);

                phy.SetRequest(PibAttribute.phyMaxFrameDuration, value, out status);
                Assert(status == Status.ReadOnly);

                phy.SetRequest(PibAttribute.phySHRDuration, value, out status);
                Assert(status == Status.ReadOnly);

                phy.SetRequest(PibAttribute.phySymbolsPerOctet, value, out status);
                Assert(status == Status.ReadOnly);

                // page
                phy.SetRequest(PibAttribute.phyCurrentPage, value, out status);
                Assert(status == Status.Success);

                value.Int = 1;
                phy.SetRequest(PibAttribute.phyCurrentPage, value, out status);
                Assert(status == Status.InvalidParam);

                // channel
                for (int i = 0; i < 27; i++)
                {
                    value.Int = i;
                    phy.SetRequest(PibAttribute.phyCurrentChannel, value, out status);
                    if (i >= 11 && i <= 26)
                    {
                        Assert(status == Status.Success);
                        phy.GetRequest(PibAttribute.phyCurrentChannel, out status, out value);
                        Assert(status == Status.Success);
                        Assert(value.Int == i);
                    }
                    else
                    {
                        Assert(status == Status.InvalidParam);
                    }
                }

                // power
                value.Int = -100;
                phy.SetRequest(PibAttribute.phyTransmitPower, value, out status);
                Assert(status == Status.InvalidParam);

                value.Int = -32;
                phy.SetRequest(PibAttribute.phyTransmitPower, value, out status);
                Assert(status == Status.Success);
                phy.GetRequest(PibAttribute.phyTransmitPower, out status, out value);
                Assert(status == Status.Success);
                value.Int = 1;
                phy.SetRequest(PibAttribute.phyTransmitPower, value, out status);
                Assert(status == Status.Success);
                phy.GetRequest(PibAttribute.phyTransmitPower, out status, out value);
                Assert(status == Status.Success);
                Assert(value.Int == 0); // max CC tx power

                value.Int = 64;
                phy.SetRequest(PibAttribute.phyTransmitPower, value, out status);
                Assert(status == Status.InvalidParam);
            }

            phy.SetPower(false);
        }

        Frame m_frame = Frame.GetFrame(128);

        public void TestSend(bool bSendData)
        {
            phy.SetPower(true);
            phy.SetAutoFCS(true);
            int mtu, head, tail;
            phy.GetMtuSize(out mtu, out head, out tail);

            PibValue value = new PibValue();
            Status status;
            value.Int = 11;
            phy.SetRequest(PibAttribute.phyCurrentChannel, value, out status);
            Assert(status == Status.Success);

            value.Int = 0;
            phy.SetRequest(PibAttribute.phyTransmitPower, value, out status);
            Assert(status == Status.Success);

            if (bSendData)
            {   // Sends data continously. Dummy processing.
                phy.DataIndication = phy_DataIndicationDummy;
            }
            else
            {   // Does not send data. During receive pings data back.
                phy.DataIndication = phy_DataIndicationAqc;
            }

            phy.SetTrxStateRequest(State.RxOn, out status);
            byte[] packetData = new byte[100];

            DateTime prevTime = DateTime.Now;
            byte[] b = new byte[128];

            Thread.Sleep(10);

            if (!bSendData)
            {
                return;
            }

            int sleepTime = 1000;

            for (int i = 0; ; ++i)
            {
                // build custom frame

                m_frame.ReserveHeader(head);
                int cInd = -1;

                ushort u = (ushort)(1 << 0) | // m_frame type: data
                    (0 << 3) | // security: none
                    (0 << 4) | // m_frame pending: none
                    (1 << 5) | // ack request: true
                    (1 << 6) | // pan id compression: true
                    (2 << 10) | // dst addr: short addr
                    (2 << 14);  // src addr: short addr
                b[++cInd] = (byte)(u & 0xFF);
                b[++cInd] = (byte)(u >> 8);
                // Append sequence number.
                b[++cInd] = (byte)i;    // seq no
                u = 1; // pan id
                b[++cInd] = (byte)(u & 0xFF);
                b[++cInd] = (byte)(u >> 8);
                u = 2;
                b[++cInd] = (byte)(u & 0xFF);
                b[++cInd] = (byte)(u >> 8);
                u = 3;
                b[++cInd] = (byte)(u & 0xFF);
                b[++cInd] = (byte)(u >> 8);
                b[++cInd] = 0xDE;
                b[++cInd] = 0xAD;
                b[++cInd] = 0xBE;
                b[++cInd] = 0xEF; // dead beef
                m_frame.AppendToBack(b, 0, cInd + 1 + 100); // payload
                do
                {
                    phy.DataRequest(m_frame, out status);
                } while (status != Status.Success);
                DateTime curTime = DateTime.Now;
                Debug.Print("Frame " + (byte)i + " is sent at " + curTime.Second + ":" + curTime.Millisecond);

                Thread.Sleep( sleepTime );
            }
        }

        void phy_DataIndicationDummy(IPhyDataSap sender, Frame frame, byte linkQuality)
        {
            Frame.Release(ref frame);
        }

        Frame m_RespFrame = Frame.GetFrame(128);

        void phy_DataIndicationAqc(IPhyDataSap sender, Frame frame, byte linkQuality)
        {
            byte[] bResp = new byte[128];

            int i = frame.buf[4];
            int mtu, head, tail;
            phy.GetMtuSize(out mtu, out head, out tail);
            Status status;

            m_RespFrame.ReserveHeader(head);
            int cInd = -1;

            ushort u = (ushort)(1 << 0) | // m_RespFrame type: data
                (0 << 3) | // security: none
                (0 << 4) | // m_RespFrame pending: none
                (1 << 5) | // ack request: true
                (1 << 6) | // pan id compression: true
                (2 << 10) | // dst addr: short addr
                (2 << 14);  // src addr: short addr
            bResp[++cInd] = (byte)(u & 0xFF);
            bResp[++cInd] = (byte)(u >> 8);
            // Append sequence number.
            bResp[++cInd] = (byte)i;    // seq no

            u = 1; // pan id
            bResp[++cInd] = (byte)(u & 0xFF);
            bResp[++cInd] = (byte)(u >> 8);
            u = 2;
            bResp[++cInd] = (byte)(u & 0xFF);
            bResp[++cInd] = (byte)(u >> 8);
            u = 3;
            bResp[++cInd] = (byte)(u & 0xFF);
            bResp[++cInd] = (byte)(u >> 8);
            bResp[++cInd] = 0xBE;
            bResp[++cInd] = 0xEF;
            bResp[++cInd] = 0xDE;
            bResp[++cInd] = 0xAD; // dead beef
            m_RespFrame.AppendToBack(bResp, 0, cInd + 1 + 100); // payload
            do
            {
                phy.DataRequest(m_RespFrame, out status);
            }

            while (status != Status.Success);

            DateTime curTime = DateTime.Now;
            Debug.Print("Response " + i + " is sent at " + curTime.Second + ":" + curTime.Millisecond);
            TimeSpan printTime = DateTime.Now - curTime;
            Debug.Print("Print Takes " + printTime);

            Frame.Release(ref frame);
        }

        void phy_DataIndicationFlash(IPhyDataSap sender, Frame frame, byte linkQuality)
        {
            byte[] b = new byte[2];
            frame.ReadBytes(b, 0, frame.LengthDataUsed - 2, 2);
            uint u = ((uint)(b[0] << 8)) | b[1];
            Frame.Release(ref frame);
        }

        public void TestReceive()
        {
            phy.SetPower(true);

            phy.DataIndication = phy_DataIndicationDummy;

            // EDRequest
            {
                Debug.Print("EDRequest");
                Status status;
                Byte energyLevel;

                phy.SetTrxStateRequest(State.TRxOff, out status);
                Assert(status == Status.TRxOff);

                phy.EDRequest(out status, out energyLevel);
                Assert(status == Status.TRxOff);

                phy.SetTrxStateRequest(State.RxOn, out status);
                Assert(status == Status.RxOn);
                for (int i = 0; i < 5; i++)
                {
                    for (int channel = 11; channel <= 26; channel++)
                    {
                        PibValue value = new PibValue();
                        value.Int = channel;
                        phy.SetRequest(PibAttribute.phyCurrentChannel, value, out status);
                        Assert(status == Status.Success);

                        phy.EDRequest(out status, out energyLevel);
                        if (status == Status.Success)
                        {
                            String s = String.Empty;
                            s += "ED: " + energyLevel + " on " + channel;
                            Debug.Print(s);
                        }
                    }

                    System.Threading.Thread.Sleep(100);
                }
            }

            // CCARequest
            {
                Debug.Print("CCA");
                PibValue value = new PibValue();
                value.Int = 11;
                Status status;
                phy.SetRequest(PibAttribute.phyCurrentChannel, value, out status);
                Assert(status == Status.Success);

                phy.SetTrxStateRequest(State.RxOn, out status);
                Assert(status == Status.RxOn);

                DateTime start = DateTime.Now;
                TimeSpan duration = new TimeSpan(0, 0, 5);
                while (DateTime.Now - start < duration)
                {
                    phy.CCARequest(out status);
                }

            }

            // DataIndication
            phy.SetAutoFCS(true);
            phy.SetAddressFilter(true, 2, 1, false); // ok, no recv for other short addr/pan id
            phy.SetAutoAck(true);

            phy.DataIndication = phy_DataIndicationFlash;
            {
                Status status;
                phy.SetTrxStateRequest(State.RxOn, out status);
                Assert(status == Status.RxOn);

                Thread.Sleep(5000);
            }
        }

        void phy_DataIndicationDump(IPhyDataSap sender, Frame frame, byte linkQuality)
        {
            string s = "recv: ";
            for (int i = 0; i < frame.LengthDataUsed; i++)
            {
                byte b = frame.ReadByte(i);
                s += HexConverter.ConvertUintToHex(b, 2) + " ";
            }

            Debug.Print(s);
            Frame.Release(ref frame);
        }

        public void TestReceiveDump()
        {
            phy.SetPower(true);

            // page 0
            Status status;
            PibValue value = new PibValue();
            value.Int = 0;
            phy.SetRequest(PibAttribute.phyCurrentPage, value, out status);
            Assert(status == Status.Success);

            // channel 11
            value.Int = 11;
            phy.SetRequest(PibAttribute.phyCurrentChannel, value, out status);
            Assert(status == Status.Success);

            // sniff
            phy.DataIndication = phy_DataIndicationDump;
            phy.SetTrxStateRequest(State.RxOn, out status);
            Assert(status == Status.RxOn);

            for (; ; )
                Thread.Sleep(5000);
        }

        public static void Main(string[] args)
        {
            Program prg = new Program();
            prg.TestSend(true);
        }

    }
}


