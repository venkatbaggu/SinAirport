using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sins.Video.Camera
{
    class CameraServerList
    {
        private uint lastError = 0;//运行中错误数量
        private bool initSDK = false;//SDK初始化
        /// <summary>
        /// 设备是否被初始化
        /// </summary>
        public bool InitSDK { set { this.initSDK = value; } get { return this.initSDK; } }
        private List<CameraServer> servers = new List<CameraServer>();

        /// <summary>
        /// 构造
        /// </summary>
        public CameraServerList()
        {

        }

        /// <summary>
        /// 初始化设备 整个应用只需初始化一次
        /// </summary>
        /// <returns>1000:成功，其他错误代码</returns>
        public uint SDKInit()
        {
            if (CHCNetSDK.NET_DVR_Init()) { this.initSDK = true; return 1000; }
            this.lastError = CHCNetSDK.NET_DVR_GetLastError();
            return this.lastError;
        }

        /// <summary>
        ///  播放
        /// </summary>
        public void Play()
        {
            foreach (CameraServer server in servers) server.Play();
        }


        /// <summary>
        ///  释放设备资源
        /// </summary>
        public uint CleanUp()
        {
            this.initSDK = false;
            foreach (CameraServer server in servers) server.CleanUp();
            if (CHCNetSDK.NET_DVR_Cleanup()) return 1000;
            this.lastError = CHCNetSDK.NET_DVR_GetLastError();
            return this.lastError;
        }

        /// <summary>
        /// 添加初始化视频
        /// </summary>
        /// <param name="setting">设置</param>
        /// <param name="vbx">设置</param>
        public void InitServers(List<Sins.Helper.CameraSettings> setting, List<System.Windows.Forms.VideoBox> vbx)
        {
            int count = 0;
            foreach (Sins.Helper.CameraSettings set in setting)
            {
                if (count >= vbx.Count) break;
                CameraServer server = new CameraServer(set);
                foreach (int x in server.Setting.Channels) {  vbx[count ].Camera = new Camera(x, server.UserID, vbx[count++].Handle); server.Videos.Add(vbx[count - 1]); if (count > vbx.Count) break; }
                this.servers.Add(server);
            }
        }
    }
}
