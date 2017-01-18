////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MICROFRAMEWORK

using System;
using System.Collections;
using System.Threading;
#if MICROFRAMEWORK
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
#endif
using Microsoft.SPOT.Wireless.IEEE_802_15_4;  // Microsoft.SPOT.Wireless.IEEE_802_15_4.Network
using Microsoft.SPOT.Wireless.IEEE_802_15_4.Network;

namespace IEEE_802_15_4_Sample
{
    public struct NeighborStatus
    {
        public UInt16 addr;
        public Byte lqi;
    }

    public struct TrafficStatus
    {
        public UInt16 addr;
        public UInt32 rxBps;
    }

    public struct NodeStatus
    {
        public UInt16 addr;
        public UInt32 txBps;
        public NeighborStatus[] neighbors;
        public TrafficStatus[] traffic;
    }

    interface IMonitor
    {
        void Print(string s);
        void Status(NodeStatus[] status);
    }

    enum Message : byte
    {
        Data,
        Report,
        Neighbors
    }

    class MsgReport
    {
        public struct Node
        {
            public UInt16 addr;
            public UInt32 rxBps; // bytes
        }

        public Node[] nodes;
        public int nodeCount;

        public int Length()
        {
            return 1 + nodeCount * (2 + 4);
        }

        public bool WriteToFrame(Frame frame)
        {
            int len = Length();
            if (frame == null || frame.LengthDataAvail < len)
                return false;
            frame.AllocBack(len);

            int offset = 0;
            frame.Write(offset, (byte)Message.Report);
            offset++;

            for (int i = 0; i < nodeCount; i++)
            {
                frame.Write(offset, nodes[i].addr);
                offset += 2;
                frame.Write(offset, nodes[i].rxBps);
                offset += 4;
            }

            return true;
        }

        public bool ReadFromFrame(Frame frame)
        {
            if (frame == null || frame.LengthDataUsed < 1)
                return false;
            byte header = frame.ReadByte(0);
            if (header != (byte)Message.Report)
                return false;
            nodeCount = (frame.LengthDataUsed - 1) / 6;
            if (frame.LengthDataUsed != Length())
                return false;
            frame.DeleteFromFront(1);
            nodes = new Node[nodeCount];
            for (int i = 0; i < nodeCount; i++)
            {
                nodes[i].addr = frame.ReadUInt16(0);
                frame.DeleteFromFront(2);
                nodes[i].rxBps = frame.ReadUInt32(0);
                frame.DeleteFromFront(4);
            }

            return true;
        }
    }

    class MsgNeighbors
    {
        public Neighbor[] neighbors;
        public int neighborCount;

        public int Length()
        {
            return 1 + neighborCount * (3);
        }

        public bool WriteToFrame(Frame frame)
        {
            int len = Length();
            if (frame == null || frame.LengthDataAvail < len)
                return false;
            frame.AllocBack(len);

            int offset = 0;
            frame.Write(offset, (byte)Message.Neighbors);
            offset++;

            for (int i = 0; i < neighborCount; i++)
            {
                frame.Write(offset, neighbors[i].shortAdr);
                offset += 2;
                frame.Write(offset, neighbors[i].lqi);
                offset += 1;
            }

            return true;
        }

        public bool ReadFromFrame(Frame frame)
        {
            if (frame == null || frame.LengthDataUsed < 1)
                return false;
            byte header = frame.ReadByte(0);
            if (header != (byte)Message.Neighbors)
                return false;
            neighborCount = (frame.LengthDataUsed - 1) / 3;
            if (frame.LengthDataUsed != Length())
                return false;
            frame.DeleteFromFront(1);
            neighbors = new Neighbor[neighborCount];
            for (int i = 0; i < neighborCount; i++)
            {
                neighbors[i].shortAdr = frame.ReadUInt16(0);
                frame.DeleteFromFront(2);
                neighbors[i].lqi = frame.ReadByte(0);
                frame.DeleteFromFront(1);
            }

            return true;
        }
    }

    class NetworkTest
    {
        INetworkLayer _net;
        IMonitor _monitor;
        int _mtu, _head, _tail; // GetMtuSize
        NodeStatus[] _status;
        object _statusLock;

        public static int cReportIntervalDefault = 3000; // milliseconds
        int _reportInterval;
        Timer _timerReport;
        MsgReport _msgReport;
        bool _testMode;
        bool _coordinator;

        public NetworkTest(INetworkLayer net, IMonitor monitor)
        {
            _net = net;
            _net.GetMtuSize(out _mtu, out _head, out _tail);
            _monitor = monitor;
            _statusLock = new object();
            _reportInterval = cReportIntervalDefault;
        }

        #region status
        private void StatusInit(UInt16[] nodes)
        {
            lock (_statusLock)
            {
                int len = 0;
                if (nodes != null)
                    len = nodes.Length;
                _status = new NodeStatus[len];
                for (int i = 0; i < len; i++)
                {
                    _status[i] = new NodeStatus();
                    _status[i].addr = nodes[i];
                }

                StatusGetNode(0, true, false); // ensure that local node is present
            }
        }

        private int StatusGetNode(UInt16 addr, bool add, bool delete)
        {
            int index = -1; ;
            for (int i = 0; i < _status.Length; i++)
            {
                if (_status[i].addr == addr)
                {
                    index = i;
                    break;
                }
            }

            if (index == -1 && add)
            {
                // add to array
                NodeStatus[] res = new NodeStatus[_status.Length + 1];
                index = _status.Length;
                for (int i = 0; i < _status.Length; i++)
                    res[i] = _status[i];
                res[index] = new NodeStatus();
                res[index].addr = addr;
                _status = res;
            }

            if (index != -1 && delete)
            {
                // remove from array
                NodeStatus[] res = new NodeStatus[_status.Length - 1];
                for (int i = 0; i < res.Length; i++)
                    res[i] = _status[i];
                if (index != res.Length)
                    res[index] = _status[res.Length];
                _status = res;
                index = -1;
            }

            return index;
        }

        private void StatusHandleNodeChanged(UInt16 node, bool isAvailable)
        {
            lock (_statusLock)
            {
                if (isAvailable)
                {
                    // node joined
                    StatusGetNode(node, true, false);
                }
                else
                {
                    // node left
                    StatusGetNode(node, false, true);
                }
            }
        }

        private void StatusHandleReport(UInt16 source, MsgReport msg)
        {
            // update status
            lock (_statusLock)
            {
                int index = StatusGetNode(source, true, false);
                _status[index].traffic = new TrafficStatus[msg.nodeCount];
                for (int i = 0; i < msg.nodeCount; i++)
                {
                    _status[index].traffic[i].addr = msg.nodes[i].addr;
                    _status[index].traffic[i].rxBps = msg.nodes[i].rxBps;
                }
            }
        }

        private void StatusHandleNeighbors(UInt16 source, MsgNeighbors msg)
        {
            // update status
            lock (_statusLock)
            {
                int index = StatusGetNode(source, true, false);
                _status[index].neighbors = new NeighborStatus[msg.neighborCount];
                for (int i = 0; i < msg.neighborCount; i++)
                {
                    _status[index].neighbors[i].addr = msg.neighbors[i].shortAdr;
                    _status[index].neighbors[i].lqi = msg.neighbors[i].lqi;
                }
            }
        }

        #endregion

        #region data sink
        public void StartSink()
        {
            StopSink();
            _msgReport = new MsgReport();
            _msgReport.nodes = new MsgReport.Node[16];
            _net.DataIndication = DataIndicationHandler;
            _net.NodeChangedIndication = NodeChangedHandler;
            if (_coordinator)
                _reportInterval = cReportIntervalDefault * 2; // report only every second interval to ensure that we have the reports from all nodes
            _timerReport = new Timer(ReportCallback, null, _reportInterval, _reportInterval);
            _status = new NodeStatus[0];
        }

        public void StopSink()
        {
            if (_timerReport != null)
            {
                _timerReport.Dispose();
                _timerReport = null;
            }

            _net.DataIndication = null;
            _net.NodeChangedIndication = null;
        }

        private void DataIndicationHandler(
            object sender,
            UInt16 source,
            UInt16 targetShortAddr,
            Frame frame)
        {
            lock (_msgReport)
            {
                Message msg = (Message)frame.ReadByte(0);
                switch (msg)
                {
                    case Message.Data:
                        {
                            if (_testMode)
                            {
                                _monitor.Print("received frame from 0x" + HexConverter.ConvertUintToHex(source, 4) +
                                    ", sduHandle=" + frame.ReadByte(1) +
                                    ", len=" + frame.LengthDataUsed + " bytes");
                            }

                            for (int i = 0; i < _msgReport.nodeCount; i++)
                            {
                                if (_msgReport.nodes[i].addr == source)
                                {
                                    _msgReport.nodes[i].rxBps += (UInt32)frame.LengthDataUsed;
                                    return;
                                }
                            }

                            if (_msgReport.nodeCount < _msgReport.nodes.Length)
                            {
                                _msgReport.nodes[_msgReport.nodeCount].addr = source;
                                _msgReport.nodes[_msgReport.nodeCount].rxBps = (UInt32)frame.LengthDataUsed;
                                _msgReport.nodeCount++;
                            }
                            break;
                        }
                    case Message.Report:
                        {
                            MsgReport rep = new MsgReport();
                            if (rep.ReadFromFrame(frame))
                            {
                                if (_testMode)
                                {
                                    _monitor.Print("Report from node 0x" + HexConverter.ConvertUintToHex(source, 4) + ":");
                                    for (int i = 0; i < rep.nodeCount; i++)
                                    {
                                        _monitor.Print(" " + rep.nodes[i].rxBps + " bytes per second from 0x" + HexConverter.ConvertUintToHex(rep.nodes[i].addr, 4));
                                    }
                                }

                                if (_coordinator)
                                {
                                    StatusHandleReport(source, rep);
                                }
                            }
                            break;
                        }
                    case Message.Neighbors:
                        {
                            MsgNeighbors neigh = new MsgNeighbors();
                            if (neigh.ReadFromFrame(frame))
                            {
                                if (_testMode)
                                {
                                    _monitor.Print("Neighbors from node 0x" + HexConverter.ConvertUintToHex(source, 4) + ":");
                                    for (int i = 0; i < neigh.neighborCount; i++)
                                    {
                                        _monitor.Print(" 0x" + HexConverter.ConvertUintToHex(neigh.neighbors[i].shortAdr, 4) +
                                            ": " + neigh.neighbors[i].lqi);
                                    }
                                }

                                if (_coordinator)
                                {
                                    StatusHandleNeighbors(source, neigh);
                                }
                            }
                            break;
                        }
                }
            }

            Frame.Release(ref frame);
        }

        private void NodeChangedHandler(
            object sender,
            UInt16 node,
            bool isAvailable)
        {
            string s = "NodeChangedHandler: node " + node;
            if (isAvailable)
                s += " joined";
            else
                s += " left";
            _monitor.Print(s);

            StatusHandleNodeChanged(node, isAvailable);
            UpdateTarget();
        }

        private void ReportCallback(Object state)
        {
            Frame frame = null;
            lock (_msgReport)
            {
                for (int i = 0; i < _msgReport.nodeCount; i++)
                    _msgReport.nodes[i].rxBps = (_msgReport.nodes[i].rxBps * 1000) / (UInt32)_reportInterval;

                frame = Frame.GetFrame(_head, _msgReport.Length() + _tail);
                if (_msgReport.WriteToFrame(frame))
                {
                    _net.DataRequest(0, ref frame, 0, null);
                }

                Frame.Release(ref frame);
                _msgReport.nodeCount = 0;
            }

            _net.NeighborsRequest(NeighborConfirmHandler);
        }

        private void NeighborConfirmHandler(
            object sender,
            Status status,
            Neighbor[] neighbors)
        {
            MsgNeighbors msg = new MsgNeighbors();
            msg.neighbors = neighbors;
            msg.neighborCount = 0;
            if (neighbors != null)
                msg.neighborCount = neighbors.Length;

            Frame frame = null;
            if (msg.neighborCount > 0)
            {
                frame = Frame.GetFrame(_head, msg.Length() + _tail);
                if (msg.WriteToFrame(frame))
                {
                    _net.DataRequest(0, ref frame, 0, null);
                }

                Frame.Release(ref frame);
            }

            if (_coordinator)
            {
                lock (_statusLock)
                {
                    for (int i = 0; i < _status.Length; i++)
                        _status[i].txBps = (_status[i].txBps * 1000) / (UInt32)_reportInterval;

                    _monitor.Status(_status);
                    // reset status
                    NodeStatus[] res = new NodeStatus[_status.Length];
                    for (int i = 0; i < _status.Length; i++)
                    {
                        res[i].addr = _status[i].addr;
                    }

                    _status = res;
                }
            }
        }

        #endregion

        #region data source
        UInt16 target; // target for data, 0==none
        int _cntPending; // current outstanding messages
        object _sendLock; // lock for concurrent access
        byte _sduHandle;
        bool _stop;
        AutoResetEvent _sendEvent;
        int _size;

        public void StopSource()
        {
            _net.NodeChangedIndication = null;
            _stop = true;
        }

        public void StartSource()
        {
            _sendLock = new object();
            _sendEvent = new AutoResetEvent(false);
            target = 0;

            _net.NodeDiscoveryRequest(NodeDiscoveryConfirmHandler);
            _stop = false;
            _size = 50;// _mtu;

            Thread thread = new Thread(SendThread);
            thread.Start();
        }

        private void NodeDiscoveryConfirmHandler(
            object sender,
            Status status,
            UInt16[] nodes)
        {
            if (status == Status.Success)
            {
                StatusInit(nodes);
            }

            UpdateTarget();
        }

        private void UpdateTarget()
        {
            string s = "Current nodes: ";
            target = 0;
            UInt16 targetNode = 1;

            lock (_statusLock)
            {
                for (int i = 0; i < _status.Length; i++)
                {
                    if (i > 0)
                        s += ", ";
                    s += "0x" + HexConverter.ConvertUintToHex(_status[i].addr, 4);
                    if (_status[i].addr == targetNode)
                        target = _status[i].addr;
                }
            }

            _monitor.Print(s);
            if (target == 0)
            {
                _monitor.Print("Not sending any data");
            }
            else
            {
                _monitor.Print("Sending data to 0x" + HexConverter.ConvertUintToHex(target, 4));
            }
        }

        bool once = false;

        private void SendData()
        {
            if (target == 0)
            { // no target
                return;
            }

            if (once)
                return;
            else
            {
                once = true;
            }

            int size = _size;
            _size = _mtu - 128;

            Frame frame = Frame.GetFrame(_head, _size + _tail);
            frame.AllocBack(_size);
            for (int i = 0; i < _size; i++)
                frame.Write(i, (byte)i);

            _size = size;
            frame.Write(0, (byte)Message.Data);
            frame.Write(1, _sduHandle);
            _net.DataRequest(target, ref frame, _sduHandle, DataConfirmHandler);

            lock (_sendLock)
            {
                _cntPending++;
                _sduHandle++;
            }

        }

        public void SendThread()
        {
            _cntPending = 0;

            if (_testMode)
            {
                int interval = 100;
                while (!_stop)
                {
                    if (target != 0)
                    { // no target
                        SendData();
                    }

                    Thread.Sleep(interval);
                }
            }
            else
            {
                while (!_stop)
                {
                    int n;
                    lock (_sendLock)
                    {
                        n = 4 - _cntPending;
                    }

                    while (n > 0)
                    {
                        SendData();
                        n--;
                    }

                    _sendEvent.WaitOne(100, false);
                }
            }
        }

        private void DataConfirmHandler(
            object sender,
            Byte sduHandle,
            Status status)
        {
            if (status == Status.Success)
            {
                lock (_statusLock)
                {
                    int i = StatusGetNode(0, true, false);
                    _status[i].txBps += (UInt32)_size;
                }
            }

            lock (_sendLock)
            {
                _cntPending--;
                if (_testMode)
                {
                    _monitor.Print("DataConfirmHandler: sduHandle=" + sduHandle + ", status=" + status);
                }
            }

            _sendEvent.Set();
        }

        #endregion

        public void Run(AutoResetEvent stopEvent)
        {
            UInt64 extAddr;
            _net.GetDeviceAddress(out extAddr);
            _monitor.Print("Device address is 0x" + HexConverter.ConvertUint64ToHex(extAddr, 16));

            _monitor.Print("Frame structure: mtu=" + _mtu +
                ", head=" + _head +
                ", tail=" + _tail);

            UInt16 panId = 0x6366;

            AutoResetEvent callback = new AutoResetEvent(false);
            bool started = false;
            UInt16 shortAddr = 0;
            Byte logicalChannel = 0;
            Byte channelPage = 0;

#if MICROFRAMEWORK
            _coordinator = false;
#else
            _coordinator = true;
#endif

            _monitor.Print("attempting to start network layer");
            while (!started)
            {
                if (_coordinator)
                {
                    _monitor.Print("StartRequest for PanId=0x" + HexConverter.ConvertUint64ToHex(panId, 4));
                    _net.StartRequest(panId, delegate(
                        object sender,
                        Status status,
                        UInt16 _shortAddr,
                        Byte _logicalChannel,
                        Byte _channelPage)
                    {
                        _monitor.Print("StartConfirm: " + status.ToString());
                        if (status == Status.Success)
                        {
                            shortAddr = _shortAddr;
                            logicalChannel = _logicalChannel;
                            channelPage = _channelPage;
                            started = true;
                        }

                        callback.Set();
                    });
                }
                else
                {
                    _monitor.Print("JoinRequest for PanId=0x" + HexConverter.ConvertUint64ToHex(panId, 4));
                    _net.JoinRequest(panId, delegate(
                        object sender,
                        Status status,
                        UInt16 _shortAddr,
                        Byte _logicalChannel,
                        Byte _channelPage)
                    {
                        _monitor.Print("JoinConfirm: " + status.ToString());
                        if (status == Status.Success)
                        {
                            shortAddr = _shortAddr;
                            logicalChannel = _logicalChannel;
                            channelPage = _channelPage;
                            started = true;
                        }

                        callback.Set();
                    });
                }

                callback.WaitOne();
            }

            _monitor.Print("network is running, logicalChannel=" + logicalChannel +
                ", channelPage=" + channelPage +
                ", shortAddr=0x" + HexConverter.ConvertUintToHex(shortAddr, 4));
            _testMode = true;

            StartSink();
            if (_coordinator)
                StartSource();

            if (stopEvent != null)
            {
                stopEvent.WaitOne();
            }
            else
            {
                for (; ; )
                {
                    Thread.Sleep(60 * 1000);
                }
            }

            StopSource();
            StopSink();
        }
    }
}


