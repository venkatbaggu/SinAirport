using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sins.Video.Camera
{
    using System.Windows.Forms;
    public class Cameras
    {

        // <summary>
        /// 视频服务器
        /// </summary>
        CameraServerList VideoServers = new CameraServerList();

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="vbxs"></param>
        public Cameras(List<VideoBox> vbxs )
        {
            Sins.Helper.XmlReader.GetCameraInfo();
            this.VideoServers.InitServers(Sins.Helper.XmlReader.CamerasInfo.Values.ToList(), vbxs);
            this.VideoServers.SDKInit();
            this.VideoServers.Play();
        }
        /// <summary>
        /// 清除播放资源
        /// </summary>
        public void CleanUp()
        {
            this.VideoServers.CleanUp();
        }
    }
}
