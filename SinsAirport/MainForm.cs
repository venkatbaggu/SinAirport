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
    using Sins.Net;
    using Sins.Client;
    using Sins.Client.Binary;
    public partial class MainForm : Form
    {
        private static TrackResultCallBack callback;
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
        private ClientHandle client;
        /// <summary>
        /// 数据库
        /// </summary>
        private Sins.Data.MySql.SQLHelper mysql = new Sins.Data.MySql.SQLHelper();

        //private Action<int,int,byte[]> DetectAction = DoDetectData;
        private Action<CRect[], int> DetectAction = DoDetectData;
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
            //连接通信服务器
            this.ConnectServer();
            //初始化跟踪拼接系统
            this.InitTrackMat();

            CPoint[] rules = new CPoint[5];

            rules[0].X = 22;
            rules[0].Y = 22;
            rules[1].X = 100;
            rules[1].Y = 100;
            rules[2].X = 1000;
            rules[2].Y = 100;
            rules[3].X = 100;
            rules[3].Y = 1000;
            rules[4].X = 1000;
            rules[4].Y = 1000;
            //rules[0].vertexes = new CPoint[4];
            //rules[0].vertexes[0].X = 100;
            //rules[0].vertexes[0].Y = 100;
            //rules[0].vertexes[1].X = 100;
            //rules[0].vertexes[1].Y = 1000;
            //rules[0].vertexes[2].X = 400;
            //rules[0].vertexes[2].Y = 100;
            //rules[0].vertexes[3].X = 1000;
            //rules[0].vertexes[3].Y = 400;
            //rules[0].size = 4;

            //rules[1].ID = 32;
            //rules[1].vertexes = new CPoint[4];
            //rules[1].size = rules[1].vertexes.Length;

            TrackMat.updateInvadeRule(rules, 5);
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
                this.client = new ClientHandle(user);
                this.client.OnReceieveCRect += this.RecvCRect;
              //  Client.OnRecvBroadBin += this.RecvUpdateRule;
                this.client.Login();
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
        private void RecvCRect(string userName, int dataType, long size, string note, CRect[] data)
        {
            if (!userName.Contains("detect")) 
                return;
            if (data != null) 
            {
                DetectAction.BeginInvoke(
                    data, dataType, null,new Object[] { data });
            }
        }
        /// <summary>
        /// 处理接收到的检测数据
        /// </summary>
        /// <param name="dataType"></param>
        /// <param name="size"></param>
        /// <param name="data"></param>
        private static  void  DoDetectData(CRect[] data, int cur)
        {
            try
            {
                //var temp = BinData.GetData<CRect[]>(data);
                //CRect[][] rundata = new CRect[DetecChannel][];
                //rundata[size - 1] = data;
                TrackMat.TrackRun(data, data.Length, cur);
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
        private void InitTrackMat()
        {
            CameraInfo[] cameras = new CameraInfo[3];

            cameras[0].ip = "192.168.1.64";
            cameras[0].port = 8000;
            cameras[0].userName = "admin";
            cameras[0].password = "shiyanshi236";

            cameras[1].ip = "192.168.1.65";
            cameras[1].port = 8000;
            cameras[1].userName = "admin";
            cameras[1].password = "shiyanshi236";

            cameras[2].ip = "192.168.1.66";
            cameras[2].port = 8000;
            cameras[2].userName = "admin";
            cameras[2].password = "shiyanshi236";

            TrackMat.TrackInit(this.MatVideo.Handle, cameras, 3);
            callback = new TrackResultCallBack(this.TrackCallBackFuncult);
            TrackMat.setTrackCallback(callback);
            TrackMat.TrackStart();
        }
        #endregion
        #region 跟踪回调 保存数据
        /// <summary>
        ///  跟踪回调
        /// </summary>
        /// <param name="results">回调结果ram>
        /// <param name="nums">数组长度ram>
        private void TrackCallBackFuncult(IntPtr results, int nums)
        {
            for (int i = 0; i < nums;i++ )
            {
                CRect[] temp = BinData.PtrToArray<CRect>(results, nums);
                //test code
                this.Invoke((MethodInvoker)(() =>
                {
                    this.warningInfo.Clear();
                    int count = 1;
                    this.warningInfo.AppendText("预警数据:\r\n");

                    foreach (CRect rect in temp)
                    {
                        this.warningInfo.AppendText(string.Format("[{0}]：X:{1},Y:{2};Heigth:{3},Width:{4}.\r\n", count++.ToString().PadLeft(2), rect.X.ToString().PadLeft(2), rect.Y.ToString().PadLeft(3), rect.Height.ToString().PadLeft(3), rect.Width.ToString().PadLeft(3)));
                    }
                }));
             //   this.mysql.GetCommand("SaveTrackData", new string[] { "ID", "ruleID", "position", "cur", "trajectories" }, new object[] { rect.ID, rect.position, rect.ruleID, rect.cur, rect.trajectories }, true).ExecuteNonQuery();
            }
        }
        #endregion

        #region 检查网络连接
        private void  StartCheckNet()
        {
            if(this.client!=null)
            {
                Task task = new Task(() => {
                    this.client.BroadcastMsg(1, null);
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

        private void MatVideo_Paint(object sender, PaintEventArgs e)
        {
            int i = 0;
        }

        private void MainForm_Paint(object sender, PaintEventArgs e)
        {
            this.MatVideo.Invalidate();
            int i = 0;
        }

    }
}
