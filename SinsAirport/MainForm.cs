using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Configuration;

namespace Sins.Airport.Mat
{
    using Sins.Client;
    using Sins.Client.Binary;
    public partial class MainForm : Form
    {
        const int DetecChannel = 3;//检测摄像机个数
        #region 字段属性
        /// <summary>
        /// 单点监视视频图像
        /// </summary>
        private List<VideoBox> pointVideos = new List<VideoBox>();
        /// <summary>
        /// 单点图像显示数量
        /// </summary>
        private int pointVideoCount = 8;
        /// <summary>
        ///  单点监视视频图像放置行数
        /// </summary>
        private int pointVideoRows = 2;
        /// <summary>
        ///   单点监视视频图像放置X方向间隔
        /// </summary>
        private int xSpace = 1;
        /// <summary>
        ///   单点监视视频图像放置Y方向间隔
        /// </summary>
        private int ySpace = 1;
        /// <summary>
        /// 单点播放视频
        /// </summary>
        private Sins.Video.Camera.Cameras cameras = null;
        /// <summary>
        /// 通信服务的客户端
        /// </summary>
        private ClientHandle Client;
        /// <summary>
        /// 数据库
        /// </summary>
        private Sins.Data.MySql.SQLHelper mysql = new Sins.Data.MySql.SQLHelper();

        private Action<int,int,byte[]> DetectAction = DoDetectData;
        #endregion

        #region 构造函数
        /// <summary>
        /// 构造函数
        /// </summary>
        public MainForm()
        {
            InitializeComponent();
        }
        #endregion  

        #region 窗体初始化
        /// <summary>
        ///  窗体初始化
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            //获取屏幕大小
            this.Location = new Point(2, 1);
            this.Size = new Size(Screen.PrimaryScreen.Bounds.Width+5,Screen.PrimaryScreen.Bounds.Height);
            //拼接显示
            this.MatVideo.Size = new Size(this.Size.Width, this.Size.Height / 2);
            //显示设置
            this.Setting_Btn.Location = new Point(this.Size.Width - 41, this.Size.Height / 2);
            this.Setting_Btn.Click += (s, ee) => { this.Visible = false; SettingForm sform = new SettingForm(this.Size, this.MatVideo.Size, this.GetMatVideoImage()); sform.ShowDialog(); this.Visible=true; };
            //关闭窗体
            this.Close_Btn.Location = new Point(this.Size.Width - 20, this.Size.Height/2);
            this.Close_Btn.Click += (s, ee) => { this.Close(); };
            //网络连接状态显示
            this.status_pan.Location = new Point(this.Size.Width-90,4);
            //单点监视窗体
            this.InitPointVodes();
            //测试窗体
           // this.LoadImage(this.MatVideo,"test.bmp");
            //初始化单点播放视频服务器
            // cameras=new Video.Camera.Cameras(this.pointVideos);
            //初始化跟踪拼接系统
            //this.InitTrackMat();
        }
        #endregion
        #region 初始化点监控图像
        /// <summary>
        ///  初始化点监控图像
        /// </summary>
        private void InitPointVodes()
        {
            int count = this.pointVideoCount / this.pointVideoRows;
            int width = (this.Size.Width / count)+((this.Size.Width%count)==0?0:1) - count * this.xSpace;
            int heigth = ((this.Size.Height / 2) / this.pointVideoRows) - this.pointVideoRows * this.ySpace;
            int x = 1; int y = this.Height / this.pointVideoRows + 5; int index = 0;
            for (int row = 0; row < this.pointVideoRows; row++)
            {
                for (int idx = 0; idx < count; idx++)
                {
                    if (idx == 0) x = 1; else x += width + xSpace;
                    VideoBox vbx = new VideoBox { Location = new Point(x, y), Width = width, Height = heigth, Index = index++, Margin=new Padding(0,0,0,0) };
                   // this.LoadImage(vbx, "test.bmp");
                    this.pointVideos.Add(vbx);
                }
                y += heigth + this.ySpace;
            }
            this.Controls.AddRange(this.pointVideos.ToArray());
        }
        #endregion
        #region 加载初始化图像
        private void LoadImage(dynamic pbx,string path)
        {
            Image curimg = Image.FromFile(path);
            pbx.Image = curimg;
        }
        #endregion
        #region 加载初始化图像
        /// <summary>
        /// 获取设置图像
        /// </summary>
        /// <returns></returns>
        private Image GetMatVideoImage()
        {
            if (this.MatVideo.Image == null) return null;
            MemoryStream stream=new MemoryStream();
            this.MatVideo.Image.Save(stream, this.MatVideo.Image.RawFormat);
            Image img = Image.FromStream(stream);
            return img;
        }
        #endregion

        #region 连接通信服务器
        private void ConnectServer()
        {
            try
            {
                string server=ConfigurationSettings.AppSettings["server"];
                string port=ConfigurationSettings.AppSettings["port"];
                string user=ConfigurationSettings.AppSettings["user"];
                Client = new ClientHandle(user);
              //  Client.OnRecvSendDetectData += this.RecvDetectData;
              //  Client.OnRecvBroadBin += this.RecvUpdateRule;
                Client.Login();
            }
            catch
            {

            }
        }
        #endregion
        #region  收到检测数据的处理方法
        /// <summary>
        /// 收到检测数据
        /// </summary>
        /// <param name="userName">用户名</param>
        /// <param name="dataType">检测类型 1：检测数据 2：规则数据</param>
        /// <param name="size">数据类型 dataType=1{1...n 检测端编号}  dataType=2{1:入侵检测规则2:停机检测规则3:错误轨迹规则4：轨迹冲突规则}</param>
        /// <param name="note">说明</param>
        /// <param name="data">数据流</param>
        private void RecvDetectData(string userName, int dataType, long size, string note, byte[] data)
        {
            if (!userName.Contains("detect")) return;
            if (dataType == 1 && data != null) BeginInvoke(this.DetectAction,new object[]{dataType,size,data});
        }
        /// <summary>
        /// 处理接收到的检测数据
        /// </summary>
        /// <param name="dataType"></param>
        /// <param name="size"></param>
        /// <param name="data"></param>
        private static  void  DoDetectData(int dataType,int size,byte[] data)
        {
            try
            {
                var temp = BinData.GetData<CRect[]>(data);
                CRect[][] rundata = new CRect[DetecChannel][];
                rundata[size - 1] = temp;
                TrackMat.run(rundata);
            }
            catch
            {

            }
        }
        #endregion
        


        #region 初始化拼接和跟踪
        /// <summary>
        ///  初始化拼接和跟踪
        /// </summary>
        /// <param name="results">回调结果ram>
        /// <param name="nums">数组长度ram>
        private unsafe void InitTrackMat()
        {
            TrackMat.init(this.MatVideo.Handle);
            TrackMat.setCallback(new TrackResultCallBack(this.TrackCallBackFuncult));
            TrackMat.startMat();
        }
        #endregion
        #region 跟踪回调 保存数据
        /// <summary>
        ///  跟踪回调
        /// </summary>
        /// <param name="results">回调结果ram>
        /// <param name="nums">数组长度ram>
        private unsafe void TrackCallBackFuncult(CRect* results, int nums)
        {
            for (int i = 0; i < nums;i++ )
            {
                CRect rect = results[i];
             //   this.mysql.GetCommand("SaveTrackData", new string[] { "ID", "ruleID", "position", "cur", "trajectories" }, new object[] { rect.ID, rect.position, rect.ruleID, rect.cur, rect.trajectories }, true).ExecuteNonQuery();
            }
        }
        #endregion

        #region 检查网络连接
        private void  StartCheckNet()
        {
            if(this.Client!=null)
            {
                Task task = new Task(() => {
                    this.Client.BroadcastMsg(1, null);
                });
            }
        }
        #endregion

        #region 窗体关闭
        /// <summary>
        ///  窗体关闭
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try { this.cameras.CleanUp(); } catch { }
        }
        #endregion

    }
}
