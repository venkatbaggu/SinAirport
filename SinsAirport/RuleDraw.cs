using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;

using System.Linq;

namespace System.Windows.Forms
{
    class RuleDraw : PictureBox
    {
        /// <summary>
        /// 画笔宽度
        /// </summary>
        public float PenWidth { set { this.pen.Width = value; } get { return this.pen.Width; } }
        private Pen pen = new Pen(System.Drawing.Color.Red);
        /// <summary>
        /// 画笔颜色
        /// </summary>
        public Color PenCOlor { set { this.pen.Color = value; } get { return this.pen.Color; } }

        private List<Point> points = new List<Point>();
        private System.IO.Stream stream=null;
        /// <summary>
        /// 绘制多边形的点
        /// </summary>
        public List<Point> Points { set { this.points = value; } get { return this.points; } }
        /// <summary>
        /// 绘制是否结束
        /// </summary>
        private bool changepoint = false;
        private int selectedIdx = -1;
 

        /// <summary>
        /// 构造
        /// </summary>
        public RuleDraw():base()
        {

        }

        /// <summary>
        /// 图像重绘
        /// </summary>
        /// <param name="pe"></param>
        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            this.DrawRule();
        }
        /// <summary>
        /// 鼠标右键按下事件
        /// </summary>
        /// <param name="e"></param>
        protected override void OnMouseDown(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                base.OnMouseDown(e);
                this.points.Add(e.Location);
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (selectedIdx < 0) this.selectedIdx = this.GetNearIdx(e.Location); else this.changepoint = true;
            }
            this.Invalidate();
        }
        /// <summary>
        /// 鼠标键释放
        /// </summary>
        /// <param name="e"></param>
        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            if (e.Button == MouseButtons.Left)
            {
                this.changepoint = false;
                
            }
            else if(e.Button==MouseButtons.Right && this.selectedIdx>-1 && this.changepoint)
            {
                this.changepoint = false;
                this.points[selectedIdx] = e.Location;
                this.selectedIdx = -1;
            }
            this.DrawRule();
        }
        /// <summary>
        /// 鼠标移动
        /// </summary>
        /// <param name="e"></param>
        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

        }
        /// <summary>
        /// 重绘
        /// </summary>
        private void DrawRule()
        {
            if (this.points.Count < 2) return;
            Graphics dc = this.CreateGraphics();
            if (this.points.Count == 2) { dc.DrawLine(this.pen, this.points[0], this.points[1]); return; }
            dc.DrawPolygon(this.pen, this.points.ToArray());
            dc.Dispose();
        }
        /// <summary>
        /// 获取最近的点
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private int GetNearIdx(Point p)
        {
            int temp=-1;
            for(int idx=0;idx<this.points.Count-1;idx++)
            {
                if (this.GetLong(p, this.points[idx]) < this.GetLong(p, this.points[idx + 1])) temp = idx; else temp = idx + 1;
            }
            return temp;
        }
        /// <summary>
        ///  获取点的距离
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
       private double GetLong(Point a,Point b)
        {
            return Math.Pow(a.X - b.X, 2) + Math.Pow(a.Y - b.Y, 2);
        }
    }
}
