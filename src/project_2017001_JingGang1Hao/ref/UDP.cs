using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using DBFile.Block;
using System.Diagnostics;


namespace DBFile.UDP
{
    public delegate void RecvDataHandler(RecvDataBlock block);
    public delegate void RecvCmdHandler(RecvCmdBlock block);
    public delegate void RecvTimeHandler(DateTime time);
    public delegate void RecvCommHandler(DateTime time, string host, int commType);
    public delegate void RecvLogHandler(string msg, string func);

    public class StateObject 
    { 
        public Socket workSocket = null; 
        public const int BufferSize = 1024; 
        public byte[] buffer = new byte[BufferSize];
        public UDPBus bus = null;
    } 

    public class UDPBus: IDisposable
    {
        private Thread threadAsync;
        Socket s1, s2, s3, s4;
        IAsyncResult sr1, sr2, sr3, sr4;
        Boolean sc1 = false, sc2 = false, sc3 = false, sc4 = false;

        private string m_GroupHost1, m_GroupHost2;
        private int m_GroupPort1, m_GroupPort2;
        private string m_LocalHost1, m_LocalHost2;
        private int m_LocalPort1, m_LocalPort2;

        public UDPBus(string grouphost1, int groupport1, string grouphost2, int groupport2, 
            string localhost1, int localport1, string localhost2, int localport2)
        {
            m_GroupHost1 = grouphost1;
            m_GroupHost2 = grouphost2;
            m_GroupPort1 = groupport1;
            m_GroupPort2 = groupport2;
            m_LocalHost1 = localhost1;
            m_LocalHost2 = localhost2;
            m_LocalPort1 = localport1;
            m_LocalPort2 = localport2;
        }

        public UDPBus()
        {
        }

        public void Dispose()
        {
            Stop();
        }
        //启动
        public void Start()
        {
            Trace.WriteLine("UDP AsyncThread Start...");
            threadAsync = new Thread(new ThreadStart(AsyncRecieve));
            threadAsync.Start();
        }
        //停止
        public void Stop()
        {
            try
            {
                if (threadAsync != null)
                {
                    threadAsync.Abort();
                }
            }
            catch (System.Exception e)
            {
                if (OnLog != null)
                    OnLog(e.Message, "UDPBus.Stop");
            }
        }

        public event RecvDataHandler OnRecv;
        public event RecvCmdHandler OnCmd;
        public event RecvCommHandler OnComm;
        public event RecvTimeHandler OnTime;
        public event RecvLogHandler OnLog;

        private Boolean GetTime(byte[] bytes, ref DateTime time)
        {
            try
            {
                time = new DateTime(bytes[0] * 100 + bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6]);
                return true;
            }
            catch
            {
                return false;
            }
        }

        //数据接收，解析
        private void Recv(byte[] bytes, int count)
        {
            if ((bytes[0] == 0x53) && (count >= 16) && (bytes[count - 1] == 0x0D) && ((bytes[7] * 256 + bytes[6]) + 7 == count))
            {
                //以太网帧校验
                byte check = 0;
                for (int i = 8; i < count - 2; i++)
                    check = (byte)(check + bytes[i]);
                if (check != bytes[count - 2])
                {
                    return;
                }

                byte[] ip = new byte[4];
                ip[0] = bytes[1]; ip[1] = bytes[2]; ip[2] = bytes[3]; ip[3] = bytes[4];
                IPAddress host = new IPAddress(ip);//数据来源 IP 地址

                try
                {
                    int dt = bytes[5];//数据类型
                    int p = 8;
                    DateTime time = DateTime.Now;

                    switch (dt)
                    {
                        case 0x64://网络确认请求
                            if (OnComm != null)
                                OnComm(DateTime.Now, host.ToString(), 0);
                            break;
                        case 0x65://网络确认响应
                            if (OnComm != null)
                                OnComm(DateTime.Now, host.ToString(), 1);
                            break;
                        case 0x12://网络校时
                            if (GetTime(new byte[] { bytes[p+1], bytes[p], bytes[p + 2], bytes[p + 3], bytes[p + 4], bytes[p + 5], bytes[p + 6] }, ref time))
                            {
                                if (OnTime != null)
                                    OnTime(time);
                            }
                            break;
                        case 0x11://广播数据
                            if (GetTime(new byte[] { bytes[p+1], bytes[p], bytes[p + 2], bytes[p + 3], bytes[p + 4], bytes[p + 5], bytes[p + 6] }, ref time))
                            {
                                p = p + 7;
                                RecvDataBlock recvBlock = new RecvDataBlock(time);
                                while ((p + 12) < count)
                                {
                                    //CAN帧校验
                                    //if (bytes[p + 11] == (byte)(bytes[p + 0] + bytes[p + 1] + bytes[p + 2] + bytes[p + 3] + bytes[p + 4] + bytes[p + 5] + bytes[p + 6] + bytes[p + 7] + bytes[p + 8] + bytes[p + 9] + bytes[p + 10]))
                                    //{
                                    int id = (bytes[p + 0] << 24) + (bytes[p + 1] << 16) + (bytes[p + 2] << 8) + bytes[p + 3];
                                    Int16 value = (Int16)((bytes[p + 6] << 8) + bytes[p + 5]);
                                    Int16 alarmType = (Int16)((bytes[p + 8] << 8) + bytes[p + 7]);
                                    Int16 limitValue = (Int16)((bytes[p + 10] << 8) + bytes[p + 9]);

                                    recvBlock.Add(new DataValue(id, value, alarmType, limitValue));
                                    //}

                                    p = p + 12;
                                }
                                if (OnRecv != null)
                                    OnRecv(recvBlock);
                            }
                            break;
                        case 0x13://车令
                            if (GetTime(new byte[] { bytes[p+1], bytes[p], bytes[p + 2], bytes[p + 3], bytes[p + 4], bytes[p + 5], bytes[p + 6] }, ref time))
                            {
                                UInt16 shij = (UInt16)((bytes[p + 10] << 8) + bytes[p + 9]);
                                Int16 jiasbwz = (Int16)((bytes[p + 12] << 8) + bytes[p + 11]);
                                Int16 jisbwz = (Int16)((bytes[p + 14] << 8) + bytes[p + 13]);
                                Int16 luoj = (Int16)((bytes[p + 16] << 8) + bytes[p + 15]);
                                Int16 zhouxzs = (Int16)((bytes[p + 18] << 8) + bytes[p + 17]);
                                byte jiasbwzJm = bytes[p + 19];
                                byte jisbwzJm = bytes[p + 20];
                                byte luojJm = bytes[p + 21];
                                byte zhouxzsJm = bytes[p + 22];

                                RecvCmdBlock cmdBlock = new RecvCmdBlock(time, shij, jiasbwz, jisbwz, luoj, zhouxzs, jiasbwzJm, jisbwzJm, luojJm, zhouxzsJm);
                                //车令测试，测试完成后删除
                                //Trace.WriteLine(string.Format("time:{0}, shiji:{1}",time, shij));
                                if (OnCmd != null)
                                    OnCmd(cmdBlock);
                            }
                            break;
                    }
                }

                catch (Exception e)
                {
                    Trace.WriteLine("UDP.Recv Exception:");
                    Trace.Indent();
                    Trace.WriteLine(e.Message, "Message");
                    Trace.WriteLine(e.Source, "Source");
                    Trace.WriteLine(e.StackTrace, "StackTrace");
                    Trace.WriteLine(e.TargetSite, "TargetSite");
                    Trace.Unindent();
                }
            }
        }


        //广播通讯确认数据
        public void Send(int socketIndex, string host, int port, byte[] bytes)
        {
            try
            {
                switch (socketIndex)
                {
                    case 3:
                        if (sc3)
                        {
                            IPAddress ip = IPAddress.Parse(host);
                            IPEndPoint ipep = new IPEndPoint(ip, port);
                            s3.SendTo(bytes, ipep);
                        }
                        break;
                    case 4:
                        if (sc4)
                        {
                            IPAddress ip = IPAddress.Parse(host);
                            IPEndPoint ipep = new IPEndPoint(ip, port);
                            s4.SendTo(bytes, ipep);
                        }
                        break;
                }
            }
            catch { }
        }

        //异步接收线程
        private void AsyncRecieve()
        {
            while (Thread.CurrentThread.ThreadState.Equals(System.Threading.ThreadState.Running))
            {
                if (!sc1)
                    StartRecieve1();
                if (!sc2)
                    StartRecieve2();
                if(!sc3)
                    StartRecieve3();
                if (!sc4)
                    StartRecieve4();
                Thread.Sleep(1000);
            }
            if (sc1) StopRecieve1();
            if (sc2) StopRecieve2();
            if (sc3) StopRecieve3();
            if (sc4) StopRecieve4();
        }
        //启动网络组播数据接收1
        private void StartRecieve1()
        {
            try
            {
                s1 = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
                s1.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true); //允许地址重用
                IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(m_LocalHost1), m_GroupPort1);
                s1.Bind(ipep);
                //加入组播
                MulticastOption multicastOption = new MulticastOption(IPAddress.Parse(m_GroupHost1), IPAddress.Parse(m_LocalHost1));
                s1.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, multicastOption);
                StateObject state = new StateObject();
                state.workSocket = s1;
                state.bus = this;
                //异步数据接收
                sr1=s1.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                sc1 = true;
                Trace.WriteLine("UDP Recv1 Start...");
            }
            catch {}
        }
        //停止网络组播数据接收1
        private void StopRecieve1()
        {
            s1.EndReceive(sr1);
            s1.Close();
            sc1 = false;
        }
        //启动网络组播数据接收2
        private void StartRecieve2()
        {
            try
            {
                s2 = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
                s2.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true); //允许地址重用
                IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(m_LocalHost2), m_GroupPort2);
                s2.Bind(ipep);
                //加入组播
                MulticastOption multicastOption = new MulticastOption(IPAddress.Parse(m_GroupHost2), IPAddress.Parse(m_LocalHost2));
                s2.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.AddMembership, multicastOption);
                StateObject state = new StateObject();
                state.workSocket = s2;
                state.bus = this;
                //异步数据接收
                sr2=s2.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                sc2 = true;
                Trace.WriteLine("UDP Recv2 Start...");
            }
            catch {}
        }
        //停止网络组播数据接收2
        private void StopRecieve2()
        {
            s2.EndReceive(sr2);
            s2.Close();
            sc2 = false;
        }

        //启动网络数据接收3
        private void StartRecieve3()
        {
            try
            {
                s3 = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
                s3.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true); //允许地址重用
                IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(m_LocalHost1), m_LocalPort1);
                s3.Bind(ipep);
                StateObject state = new StateObject();
                state.workSocket = s3;
                state.bus = this;
                //异步数据接收
                sr3 = s3.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                sc3 = true;
                Trace.WriteLine("UDP Recv3 Start...");
            }
            catch { }
        }

        //停止网络数据接收3
        private void StopRecieve3()
        {
            s3.EndReceive(sr3);
            s3.Close();
            sc3 = false;
        }

        //启动网络数据接收4
        private void StartRecieve4()
        {
            try
            {
                s4 = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.IP);
                s4.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true); //允许地址重用
                IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(m_LocalHost2), m_LocalPort2);
                s4.Bind(ipep);
                StateObject state = new StateObject();
                state.workSocket = s4;
                state.bus = this;
                //异步数据接收
                sr4 = s4.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                sc4 = true;
                Trace.WriteLine("UDP Recv4 Start...");
            }
            catch { }
        }
        //停止网络数据接收4
        private void StopRecieve4()
        {
            s4.EndReceive(sr4);
            s4.Close();
            sc4 = false;
        }


        //接收数据回调函数
        public void ReceiveCallBack(IAsyncResult ar)
        {
            StateObject state = (StateObject)ar.AsyncState;
            Socket handler = state.workSocket;
            UDPBus bus = state.bus;

            int bytesRead = 0;
            try
            {
                bytesRead = handler.EndReceive(ar);
            }
            catch
            {
                Thread.CurrentThread.Abort();
                Thread.CurrentThread.Join();
            }

            if (bytesRead > 0)
            {
                bus.Recv(state.buffer, bytesRead);
                //继续进行异步数据接收
                try
                {
                    if (handler == s1)
                        sr1 = handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                    else if (handler==s2)
                        sr2 = handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                    else if(handler==s3)
                        sr3 = handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                    else
                        sr4 = handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, SocketFlags.None, new AsyncCallback(ReceiveCallBack), state);
                }
                catch(Exception e)
                {
                    Trace.WriteLine("UDP.ReceiveCallBack Exception");
                    Trace.Indent();
                    Trace.WriteLine(e.Message, "Message");
                    Trace.WriteLine(e.Source, "Source");
                    Trace.WriteLine(e.StackTrace, "StackTrace");
                    Trace.WriteLine(e.TargetSite, "TargetSite");
                    Trace.Unindent();
                }

            }
        }
    }

    public class RecvDataBlock : BlockArray
    {
        private DateTime m_Time;
        public DateTime Time
        {
            get { return m_Time; }
            set { m_Time = value; }
        }

        public RecvDataBlock(DateTime time)
            : base(null)
        {
            m_Time = time;
        }
    }

    public class RecvCmdBlock : Block.Block
    {
        private DateTime m_Time;
        public DateTime Time
        {
            get { return m_Time; }
            set { m_Time = value; }
        }

        private UInt16 m_Shij;

        public UInt16 Shij
        {
            get { return m_Shij; }
            set { m_Shij = value; }
        }


        private int m_Jiasbwz;

        public int Jiasbwz
        {
            get { return m_Jiasbwz; }
            set { m_Jiasbwz = value; }
        }

        private int m_Jisbwz;

        public int Jisbwz
        {
            get { return m_Jisbwz; }
            set { m_Jisbwz = value; }
        }

        private int m_Luoj;

        public int Luoj
        {
            get { return m_Luoj; }
            set { m_Luoj = value; }
        }

        private int m_Zhouxzs;

        public int Zhouxzs
        {
            get { return m_Zhouxzs; }
            set { m_Zhouxzs = value; }
        }

        private byte m_JiasbwzJm;

        public byte JiasbwzJm
        {
            get { return m_JiasbwzJm; }
            set { m_JiasbwzJm = value; }
        }

        private byte m_JisbwzJm;

        public byte JisbwzJm
        {
            get { return m_JisbwzJm; }
            set { m_JisbwzJm = value; }
        }

        private byte m_LuojJm;

        public byte LuojJm
        {
            get { return m_LuojJm; }
            set { m_LuojJm = value; }
        }

        private byte m_ZhouxzsJm;

        public byte ZhouxzsJm
        {
            get { return m_ZhouxzsJm; }
            set { m_ZhouxzsJm = value; }
        }


        public RecvCmdBlock(DateTime time, UInt16 shij, int jiasbwz, int jisbwz, int luoj, int zhouxzs, byte jiasbwzJm, byte jisbwzJm, byte luojJm, byte zhouxzsJm)
            : base(null)
        {
            m_Time = time;
            m_Shij = shij;
            m_Jiasbwz = jiasbwz;
            m_Jisbwz = jisbwz;
            m_Luoj = luoj;
            m_Zhouxzs = zhouxzs;
            m_JiasbwzJm = jiasbwzJm;
            m_JisbwzJm = jisbwzJm;
            m_LuojJm = luojJm;
            m_ZhouxzsJm = zhouxzsJm;
        }
    }
}