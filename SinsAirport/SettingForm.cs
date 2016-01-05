using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Sins.Airport.Mat
{
   
    public partial class SettingForm : Form
    {
        
        /// <summary>
        /// 绘制何种图形 0：不绘制 1：矩形 2：多边形
        /// </summary>
        private int drawSharp = 0;
        /// <summary>
        /// 绘制颜色
        /// </summary>
        private Color penColor=Color.Red;
        /// <summary>
        /// 画的点
        /// </summary>
        private List<Point> Points = new List<Point>();
        /// <summary>
        /// 方向
        /// </summary>
        private Point[] directtion = new Point[2];
        /// <summary>
        ///  构造
        /// </summary>
        public SettingForm()
        {
            InitializeComponent();
            
        }
        /// <summary>
        ///  构造
        /// </summary>
        /// <param name="size">屏的大小</param>
        /// <param name="videoSize">图像大小</param>
        /// <param name="img">加载的图片</param>
        public SettingForm(Size size,Size videoSize,Image img)
        {
            this.Location = new Point(0, 0);
            InitializeComponent();
            this.Size = new Size(size.Width, videoSize.Height + 360);
            this.VideoCopy.Location=new Point(2, 1);
            this.VideoCopy.Size = videoSize;
            this.VideoCopy.Image = img;
            this.GBX.Location = new Point(this.GBX.Location.X, videoSize.Height + 5);
            this.StartPosition = FormStartPosition.CenterScreen;
        }
        /// <summary>
        ///  窗体加载
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SettingForm_Load(object sender, EventArgs e)
        {
            this.Close_Btn.Click += (s, ee) => { this.Close(); };
        }
        /// <summary>
        /// 选择规则图形
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SelectRulesGraphic(object sender, EventArgs e)
        {
            ToolStripButton btn = sender as ToolStripButton;
            this.drawSharp = int.Parse(btn.Name.Replace("rule",""));
            switch(this.drawSharp)
            {
                case 4: using (ColorDialog cor = new ColorDialog()) { if (cor.ShowDialog() == DialogResult.OK)this.penColor = cor.Color; } break;
                case 5: this.Close(); break;
                default: break;
            }
        }

        /// <summary>
        ///  图像鼠标按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void VideoCopy_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button != MouseButtons.Left) return;//不是鼠标左键按下

                switch(this.drawSharp)
                {
                    case 1:
                        this.Draw(this.drawSharp, e.Location);
                        this.Points.Add(e.Location);
                        break;
                    case 2:

                        break;
                    case 3:

                        break;
                    case 4:
                        break;
                    default: return;
                }
        }

        private void VideoCopy_MouseMove(object sender, MouseEventArgs e)
        {
           this.Draw(this.drawSharp, e.Location);
        }

        private void VideoCopy_MouseUp(object sender, MouseEventArgs e)
        {
            this.Draw(this.drawSharp, e.Location);
        }
        private void Draw(int sharp,Point curpos)
        {
            if (this.Points.Count == 0) return;
            Graphics dc = this.VideoCopy.CreateGraphics();
            Pen pen = new Pen(this.penColor);
            switch(sharp)
            {
                case 1:
                    if (this.Points.Count > 1) dc.DrawLines(pen,this.Points.ToArray());
                    dc.DrawLine(pen,this.Points.Last(), curpos);
                    dc.Dispose();
                    this.VideoCopy.Refresh();
                    break;
                case 2:

                    break;
                case 3:

                    break;
                case 4:
                    break;
                default: return;
            }
           
        }
    }
}
