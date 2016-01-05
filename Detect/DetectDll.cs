using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sins.Airport.Detect
{
    #region  矩形结构体
    /// <summary>
    /// 矩形结构体
    /// </summary>
    [Serializable]
    public unsafe struct CRect
    {
        /// <summary>
        /// X坐标
        /// </summary>
        public int X;
        /// <summary>
        ///   Y坐标
        /// </summary>
        public int Y;
        /// <summary>
        ///  矩形长
        /// </summary>
        public int Width;
        /// <summary>
        ///  矩形宽
        /// </summary>
        public int Height;
    }
    #endregion

    #region  回调函数
    /// <summary>
    ///  回调函数
    /// </summary>
    /// <param name="objs">传回的CRect数组</param>
    /// <param name="len">数组长度</param>
    public unsafe delegate void DetectCallBack(CRect* objs);
    #endregion

    #region  摄像头数据结构
    /// <summary>
    /// 摄像头数据结构
    /// </summary>
    public struct Camera
    {
        /// <summary>
        ///  摄像头IP地址
        /// </summary>
        public string ip; 
        /// <summary>
        ///  摄像头端口
        /// </summary>
        public int port; 
        /// <summary>
        ///  摄像头用户名
        /// </summary>
        public string userName; 
        /// <summary>
        ///  摄像头密码
        /// </summary>
        public string password; 
    }
    #endregion

    #region  检测接口类型
    /// <summary>
    ///  检测接口类型
    /// </summary>
    public class Detect
    {
        /// <summary>
        ///  目标检测初始化
        /// </summary>
        /// <param name="fileName">param[in] params 算法参数</param>
        /// <param name="cameras">param[in] cameras 摄像头信息</param>
        /// <returns>成功返回值0，不成功则返回出错代码（<0）</returns>
        [DllImport(@"Detect.dll", CallingConvention = CallingConvention.ThisCall, CharSet=CharSet.Unicode)]
        public static extern int init(string fileName, Camera cameras);

        /// <summary>
        /// 开始检测
        /// </summary>
        /// <returns>成功返回值0，不成功则返回出错代码（<0）</returns>
        [DllImport(@"Detect.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int start();

        /// <summary>
        ///  设置回调函数
        /// </summary>
        /// <param name="callback">成功返回值0，不成功则返回出错代码（<0）</param>
        [DllImport(@"Detect.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setDetectCallback(DetectCallBack callback);
    }
    #endregion
}