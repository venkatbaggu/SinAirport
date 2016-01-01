using System;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Sins.Airport.Mat
{
    /// <summary>
    /// 基类
    /// </summary>
    public abstract class Shape
    {
            public abstract void Draw(Graphics g);
            protected Rectangle bounding;
            protected Point[] pointlist;
            public abstract void pointchange(Point[] p);
            public virtual GraphicsPath pathChange(GraphicsPath path, bool pick)
            {
                return null;
            }
            protected Color penColor;
            protected float penWidth;
    }
    /// <summary>
    /// 填充接口
    /// </summary>
    public interface IFillable
    {
        void Fill(Graphics g);
        Color FillBrushColor
        {
            get;
            set;
        }
    }
    /// <summary>
    /// 画多边形
    /// </summary>
    public class Polygon : Shape
    {
        public Polygon(Point[] p, Color penColor, float penWidth)
        {
            this.pointchange(p);
            this.penColor = penColor;
            this.penWidth = penWidth;
        }
        public override void pointchange(Point[] p)
        {
            pointlist = new Point[p.Length];
            int i = 0;
            foreach (Point tempPoint in p)
            {
                pointlist[i++] = tempPoint;
            }
        }
        public override void Draw(Graphics g)
        {
            using (Pen p = new Pen(penColor, penWidth))
            {
                g.DrawPolygon(p, pointlist);
            }
        }
    }
}
