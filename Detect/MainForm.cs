using System;
using System.Windows.Forms;
using System.Configuration;

namespace Sins.Airport.Detect
{
    using Sins.Client;//通信客户端
    using Sins.Client.Binary;
    public partial class MainForm : Form
    {
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

       
        #region 构造
        public MainForm()
        {
            InitializeComponent();
        }
        #endregion
        #region 窗体加载
        private void MainForm_Load(object sender, EventArgs e)
        {
            this.Resize += (ss, ee) => { this.Size = new System.Drawing.Size(475, 400); };

            if (!this.GetSettints()) return;//获取参数
            this.client = new ClientHandle(user);//创建通信客户端
            this.client.OnDisconnencted += (ok) => { this.runInfo.BeginInvoke((MethodInvoker)(() => { this.runInfo.AppendText("系统连接断开。\r\n"); })); };
            this.client.OnEndLogin += (r, m) => { this.runInfo.BeginInvoke((MethodInvoker)(() => { this.runInfo.AppendText(r ? "系统已经登录。\r\n" : "系统登录失败。\r\n"); })); };
            client.Login();

            //this.DetectInit();//初始化检测客户端
        }
         #endregion
        #region 初始化视频检测
        private unsafe void DetectInit()
        {
            Detect.init(this.fileName, new Camera {  ip=this.cip, port=this.cport, userName=this.cuser, password=this.cpassword});
            Detect.setDetectCallback(new DetectCallBack(this.CallBack));
            Detect.start();
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
        #region 回调事件，向跟踪段发送跟踪检测数据
        /// <summary>
        /// 检测回调函数
        /// </summary>
        /// <param name="data">CRect数组</param>
        /// <param name="len">数组长度</param>
        private unsafe void CallBack(CRect* data,int len)
        {
            try
            {
                CRect[] temp = new CRect[len];
                for (int i = 0; i < len; i++) { CRect rect = data[i]; temp[i] = rect; }
                if (this.client != null) this.client.BroadcastBin(1, this.DetectId, "D", BinData.GetBin<CRect[]>(temp));
            }
            catch
            {

            }
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
