using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sins.Video.Camera
{
    using System.Windows.Forms;

    public class CameraServer
    {
        #region 字段属性
        private uint lastError = 0;//运行中错误数量
        /// <summary>
        /// 获取错误代码
        /// </summary>
        public uint LastError { get { return this.lastError; } }
        private Int32 userID = -1; //Login设备获得用户连接好，大于0连接成功否则未连接或失败
        public Int32 UserID { set { this.userID = -1; } get { return this.userID; } }
        /// <summary>
        /// 是否登录设备
        /// </summary>
        public bool IsLogin { get { return this.userID > -1 ? true : false; } }
        private CHCNetSDK.NET_DVR_DEVICEINFO_V30 DeviceInfo = new CHCNetSDK.NET_DVR_DEVICEINFO_V30();//设备信息

        private List<System.Windows.Forms.VideoBox> videos = new List<VideoBox>();
        /// <summary>
        /// 实时播放窗口
        /// </summary>
        public List<System.Windows.Forms.VideoBox> Videos { get { return this.videos; } set { this.videos = value; } }
        /// <summary>
        /// 系统配置
        /// </summary>
        public Sins.Helper.CameraSettings Setting { set; get; }
       

        #endregion
        public CameraServer()
        {

        }
        public CameraServer(Sins.Helper.CameraSettings setting)
        {
            this.Setting = setting;
        }
        /// <summary>
        /// 登录设备
        /// </summary>
        /// <returns>1000:成功，1001:其他错误代码,1000以下错误代码</returns>
        public uint Login()
        {
            this.userID = CHCNetSDK.NET_DVR_Login_V30(this.Setting.IP, this.Setting.Port, this.Setting.UserName, this.Setting.Password, ref DeviceInfo);//登录
            this.lastError = CHCNetSDK.NET_DVR_GetLastError();
            return this.lastError;
        }

        /// <summary>
        /// 设备登出
        /// </summary>
        /// <returns>1000:成功，1001:其他错误代码,1000以下错误代码</returns>
        public void Logout()
        {
            if (this.userID < 0) return;
            foreach (VideoBox vbx in Videos) if(vbx.Camera!=null )vbx.Camera.StopPlay();
        }

        /// <summary>
        /// 播放图像
        /// </summary>
        /// <param name="channel">通道</param>
        /// <param name="boxHandle">图像显示控件</param>
        public bool Play()
        {

            if(this.userID<-1)this.Login();
            if (this.userID < -1) return false;
            foreach (VideoBox vbx in this.Videos) vbx.Camera.Play(this.userID);
            return true;
        }

        /// <summary>
        /// 停止播放
        /// </summary>
        /// <param name="boxHandle">图像显示控件</param>
        public void StopPlay()
        {
            foreach (VideoBox vbx in this.Videos) vbx.Camera.StopPlay();
        }

        /// <summary>
        /// 实时播放回调方法
        /// </summary>
        /// <param name="realHandle">播放句柄</param>
        /// <param name="dwDataType">数据类型</param>
        /// <param name="pBuffer">视频流</param>
        /// <param name="dwBufSize">视频流大小</param>
        /// <param name="pUser">用户</param>
        public void RealDataCallBack(Int32 realHandle, UInt32 dwDataType, ref byte pBuffer, UInt32 dwBufSize, IntPtr pUser)
        {

        }

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="boxHandle">图像</param>
        /// <param name="type">控制类型</param>
        /// <param name="speed">速度</param>
        /// <param name="startOrStop">速度</param>
        public void CameraContol(VideoBox vbx,ControlType type,uint speed,uint startOrStop)
        {
            var video = Videos.Where(c => c == vbx); if (video.Count() == 0 || video.First().Camera==null) return;
            video.First().Camera.CameraContol(type, startOrStop, speed);
        }
        /// <summary>
        /// 清除资源
        /// </summary>
        public void CleanUp()
        {
            foreach (VideoBox vbx in Videos) if (vbx != null && vbx.Camera != null) vbx.Camera.CleanUP();
            this.userID = -1;
        }
    }
}
