using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace System.Windows.Forms
{


    public class VideoBox : System.Windows.Forms.PictureBox
    {
        private int idx = 0;
        private int xPos=0;
        private int yPos=0;

        /// <summary>
        /// 图像序号
        /// </summary>
        public int Index { set { this.idx = value; } get { return this.idx; } }
        /// <summary>
        /// X方向位置
        /// </summary>
        public int XPos { set { this.xPos = value; } get { return this.xPos; } }
        /// <summary>
        /// Y方向位置
        /// </summary>
        public int YPos { set { this.yPos = value; } get { return this.yPos; } }
        /// <summary>
        /// 播放句柄
        /// </summary>
        public int PlayHandle { set; get; }
        /// <summary>
        /// 摄像机
        /// </summary>
        public Sins.Video.Camera.Camera Camera { set; get; } 
    }
}
