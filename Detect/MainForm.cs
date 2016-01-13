using System;
using System.Windows.Forms;
using System.Configuration;

namespace Sins.Airport.Detect
{
    using Sins.Client;//通信客户端
    using Sins.Client.Binary;
    public partial class MainForm : Form
    {
        private Action<CRect[]> work = null;
        private int DetectId = 1;//检测机器编号
        #region 通信服务器配置
        private string server = "";
        private int port = 0;
        private string user = "";
        private ClientHandle client = null;
        #endregion 
        #region 摄像机配置
        private string cip = "";
        private int cport = 0;
        private string cuser = "";
        private string cpassword = "";
        #endregion 
        #region 检测配置
        private string fileName = "";
        #endregion 

        private static DetectCallBack callback;
       
        #region 构造
        public MainForm()
        {
            InitializeComponent();
        }
        #endregion
        #region 窗体加载
        private void MainForm_Load(object sender, EventArgs e)
        {
            this.work = this.send;
            this.Resize += (ss, ee) => { this.Size = new System.Drawing.Size(475, 400); };
            //获取参数
            if (!this.GetSettints()) 
                return;
            //创建通信客户端
            this.client = new ClientHandle(user);
            //this.client = new ClientHandle("");

            this.client.OnReceieveBin += this.recv;

            this.client.OnDisconnencted += (ok) => { 
                this.runInfo.BeginInvoke((MethodInvoker)(() => { 
                    this.runInfo.AppendText("系统连接断开。\r\n"); })); };

            this.client.OnEndLogin += (r, m) => { 
                this.runInfo.BeginInvoke((MethodInvoker)(() => { 
                    this.runInfo.AppendText(r ? 
                        "系统已经登录。\r\n" : "系统登录失败。\r\n"); })); };

            client.Login();

            callback = new DetectCallBack(this.CallBack);

            this.DetectInit();//初始化检测客户端
        }
         #endregion
        #region 初始化视频检测
        private void DetectInit()
        {
            Detect.DetectInit(new CameraInfo {  
                ip=this.cip, 
                port=this.cport, 
                userName=this.cuser, 
                password=this.cpassword},
                this.fileName);

            Detect.setDetectCallback(callback);

            Detect.DetectStart();
        }
        #endregion
        #region 获取配置
        private bool GetSettints()
        {
            bool ok = false;
            try
            {
                this.DetectId = int.Parse(ConfigurationSettings.AppSettings["index"]);
                this.server = ConfigurationSettings.AppSettings["server"];
                this.port = int.Parse(ConfigurationSettings.AppSettings["port"]);
                this.user = string.Format("{0}_{1}", ConfigurationSettings.AppSettings["user"], DetectId);
                this.fileName = ConfigurationSettings.AppSettings["file"];
                this.cip = ConfigurationSettings.AppSettings["cip"];
                this.cport = int.Parse(ConfigurationSettings.AppSettings["cport"]);
                this.cuser = ConfigurationSettings.AppSettings["cuser"];
                this.cpassword = ConfigurationSettings.AppSettings["cpassword"];
                this.Flag.Text = string.Format("检测客户端{0}", this.DetectId);
                ok = true;
            }
            catch
            {
                this.runInfo.AppendText("加载参数失败！系统没有启动！\r\n");
            }
            return ok;
        }
        #endregion
        #region 检测回调事件
        /// <summary>
        /// 检测回调函数
        /// </summary>
        /// <param name="data">CRect数组</param>
        /// <param name="len">数组长度</param>
        private void CallBack(IntPtr data, int len) {
            CRect[] temp = BinData.PtrToArray<CRect>(data, len);
            work.BeginInvoke(temp, null, new Object[] { temp });
        }
        #endregion

        #region 发送检测数据到跟踪端
        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="data">CRect数组</param>
        private void send(CRect[] data)
        {
            try {
                this.Invoke((MethodInvoker)(() =>
                {
                    this.runInfo.Clear();
                    int count = 1;
                    this.runInfo.AppendText("回调获得的数组:\r\n");
                    foreach (CRect rect in data)
                    {
                        this.runInfo.AppendText(
                            string.Format("[{0}]：X:{1},Y:{2};Heigth:{3},Width:{4}.\r\n",
                            count++.ToString().PadLeft(2),
                            rect.X.ToString().PadLeft(2),
                            rect.Y.ToString().PadLeft(3),
                            rect.Height.ToString().PadLeft(3),
                            rect.Width.ToString().PadLeft(3)));
                    }
                  
                    //发送数据
                    this.client.SendBin("tracker", 1, 1, "D", 
                        BinData.GetBin<CRect[]>(data));
                }));
            } catch {

            }
        }
        #endregion

        #region 接收网络数据
        /// <summary>
        /// 接收数据函数
        /// </summary>
        /// <param name="user">发送数据用户名</param>
        /// <param name="dataType">数据类型</param>
        /// <param name="size">数据大小</param>
        /// <param name="note">CRect数组</param>
        /// <param name="data">接收数据</param>
        private void recv(string user, int dataType, 
            long size, string note, byte[] data)
        {
            CRect[] temp = BinData.GetData<CRect[]>(data);
            this.Invoke((MethodInvoker)(() =>
            {
                this.runInfo.Clear();
                int count = 1;
                this.runInfo.AppendText("网络接收到的数组:\r\n");
                foreach (CRect rect in temp)
                {
                    this.runInfo.AppendText(
                        string.Format("[{0}]：X:{1},Y:{2};Heigth:{3},Width:{4}.\r\n", 
                        count++.ToString().PadLeft(2), 
                        rect.X.ToString().PadLeft(2), 
                        rect.Y.ToString().PadLeft(3), 
                        rect.Height.ToString().PadLeft(3), 
                        rect.Width.ToString().PadLeft(3)));
                }
            }));
        }
         #endregion

        #region 检测消息
        /// <summary>
        ///  检测连接
        /// </summary>
        /// <param name="userName">用户</param>
        /// <param name="dataType">类型1：查询 2：回复</param>
        /// <param name="msg">附加消息</param>
        private void GetConnectOk(string userName,int dataType,string msg)
        {
            if (dataType == 1)
                this.client.SendMsg(userName, 2, null);
        }
        #endregion
    }
}
