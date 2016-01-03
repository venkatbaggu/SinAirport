using System;
using System.Windows.Forms;
using System.Configuration;

namespace Sins.Airport.Detect
{
    using Sins.Client;//通信客户端
    using Data=Sins.Client.Service;//通信客户端数据
    public partial class MainForm : Form
    {
        #region 通信服务器配置
        private int dataType = 0;
        private string server = "";
        private int port = 0;
        private string user = "";
        private ClientHandle client = null;
        #endregion 
        #region 摄像机配置
        private string cip = "192.168.1.64";
        private int cport = 8000;
        private string cuser = "admin";
        private string cpassword = "shiyanshi236";
        #endregion 
        #region 检测配置
        private string fileName = "left.xml";
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
            //client.Login();

            this.DetectInit();//初始化检测客户端
        }
         #endregion
        #region 初始化视频检测
        private void DetectInit()
        {
            Detect.DetectInit(
                new CameraInfo {ip=this.cip, port=this.cport, 
                    userName=this.cuser, password=this.cpassword}, 
                    this.fileName);
            Detect.setDetectCallback(new DetectCallBack(this.CallBack));
            Detect.DetectStart();
        }
        #endregion
        #region 获取配置
        private bool GetSettints()
        {
            bool ok = false;
            try
            {
                this.dataType = int.Parse(ConfigurationSettings.AppSettings["index"]);
                this.server = ConfigurationSettings.AppSettings["server"];
                this.port = int.Parse(ConfigurationSettings.AppSettings["port"]);
                this.user = string.Format("{0}_{1}", ConfigurationSettings.AppSettings["user"], dataType);
                this.fileName = ConfigurationSettings.AppSettings["file"];
                this.cip = ConfigurationSettings.AppSettings["cip"];
                this.cport = int.Parse(ConfigurationSettings.AppSettings["cport"]);
                this.cuser = ConfigurationSettings.AppSettings["cuser"];
                this.cpassword = ConfigurationSettings.AppSettings["cpassword"];
                this.Flag.Text = string.Format("检测客户端{0}", this.dataType);
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
        /// <param name="data"></param>
        private void CallBack(Data.Rect[] data)
        {
            if (this.client != null) 
                this.client.SendDetectData("tracker",this.dataType, data);
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
