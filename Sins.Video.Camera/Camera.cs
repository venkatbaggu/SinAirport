using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sins.Video.Camera
{
    public class Camera
    {
        private uint lastError = 0;//运行中错误数量
        /// <summary>
        /// 获取错误代码
        /// </summary>
        public uint LastError { get { return this.lastError; } }
        private Int32 userID = -1; //Login设备获得用户连接好，大于0连接成功否则未连接或失败

        /// <summary>
        /// 设备是否被初始化
        /// </summary>
        //private bool Record = false;//是否开启录像

        private int Channel;//通道
        private int realHandle = -1;//播放ID
        private IntPtr videoBoxHandle=IntPtr.Zero;//播放窗体
        /// <summary>
        /// 图像实时播放窗体
        /// </summary>
        public IntPtr VideoBoxHandle { get { return this.videoBoxHandle; } }
   


        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="channel">通道</param>
        /// <param name="userID">登录ID</param>
        /// <param name="videoBox">显示窗口</param>
        public Camera(int channel, int userID, IntPtr videoBox)
        {
            this.Channel = channel;
            this.userID = userID;
            this.videoBoxHandle = videoBox;
        }


        /// <summary>
        /// 播放图像
        /// </summary>
        /// <param name="channel">通道</param>
        /// <param name="boxHandle">图像显示控件</param>
        /// <returns>1:用户未登录，2:传入显示窗口为空，3:通道数大于2，4:图像已经播放，100:播放成功</returns>
        public uint Play(int userId)
        {
            if (this.userID < 0) this.userID = userId;
            if (this.userID < 0) return 1001;
            if (this.videoBoxHandle == IntPtr.Zero) return 2;
            if (this.Channel > 1002) return 1003;
            if (this.realHandle > -1) return 1004;
            CHCNetSDK.NET_DVR_PREVIEWINFO lpPreviewInfo = new CHCNetSDK.NET_DVR_PREVIEWINFO();
            lpPreviewInfo.hPlayWnd = this.videoBoxHandle;//预览窗口
            lpPreviewInfo.lChannel = Channel;//预te览的设备通道
            lpPreviewInfo.dwStreamType = 0;//码流类型：0-主码流，1-子码流，2-码流3，3-码流4，以此类推
            lpPreviewInfo.dwLinkMode = 0;//连接方式：0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP 
            lpPreviewInfo.bBlocked = true; //0- 非阻塞取流，1- 阻塞取流

            IntPtr pUser = IntPtr.Zero;//用户数据 user data

  

            //打开预览 Start live view 
            this.realHandle = CHCNetSDK.NET_DVR_RealPlay_V40(userID, ref lpPreviewInfo, null/*RealData*/, pUser);
            if (this.realHandle < 0) return 1000;
            this.lastError = CHCNetSDK.NET_DVR_GetLastError();
            return this.lastError;

        }

        /// <summary>
        /// 停止播放
        /// </summary>
        /// <param name="boxHandle">图像显示控件</param>
        public uint StopPlay()
        {
            if (realHandle < -1) return 1;
            if (CHCNetSDK.NET_DVR_StopRealPlay(this.realHandle)) { this.realHandle = -1; return 1000; }
            this.lastError = CHCNetSDK.NET_DVR_GetLastError(); 
            return this.lastError; 
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
        /// <param name="startOrStop">速度</param>
        /// <param name="speed">速度</param>
        /// <returns></returns>
        public uint CameraContol(ControlType type, uint startOrStop, uint speed)
        {
            if (CHCNetSDK.NET_DVR_PTZControlWithSpeed(this.realHandle, (uint)type, startOrStop, speed)) return 1000;
            this.lastError = CHCNetSDK.NET_DVR_GetLastError();
            return this.lastError;
        }
        /// <summary>
        /// 清除资源
        /// </summary>
        public void CleanUP()
        {
            if (this.realHandle > -1) 
            {
                this.StopPlay();
                this.userID=-1;
            }
        }
    }
    public enum ControlType
    {
        /// <summary>
        /// 无操作
        /// </summary>
        DEFAULT = 0,
        /// <summary>
        /// 云台上仰
        /// </summary>
        TILT_UP = 21,
        /// <summary>
        /// 云台下俯
        /// </summary>
        TILT_DOWN = 22,
        /// <summary>
        /// 云台左转
        /// </summary>
        PAN_LEFT = 23,
        /// <summary>
        /// 云台右转
        /// </summary>
        PAN_RIGHT = 24,
        /// <summary>
        ///  焦距变大
        /// </summary>
        ZOOM_IN = 11,
        /// <summary>
        /// 焦距变小
        /// </summary>
        ZOOM_OUT = 12,
        /// <summary>
        /// 焦点前调 
        /// </summary>
        FOCUS_NEAR = 13,
        /// <summary>
        /// 焦点后调
        /// </summary>
        FOCUS_FAR = 14
    }
}
