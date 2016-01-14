using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sins.Airport.Detect
{
    using Sins.Net;

    #region  回调函数
    /// <summary>
    ///  回调函数
    /// </summary>
    /// <param name="data">传回的CRect数组</param>
    /// <param name="len">数组长度</param>
    public delegate void DetectCallBack(IntPtr data, int len);
    #endregion

    #region  摄像头数据结构
    /// <summary>
    /// 摄像头数据结构
    /// </summary>
    public struct CameraInfo
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
        [DllImport(@"DetectDLL.dll",
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int DetectInit(CameraInfo cameras, string fileName);

        /// <summary>
        /// 开始检测
        /// </summary>
        /// <returns>成功返回值0，不成功则返回出错代码（<0）</returns>
        [DllImport(@"DetectDLL.dll", 
            CallingConvention = CallingConvention.Cdecl)]
        public static extern int DetectStart();

        /// <summary>
        ///  设置回调函数
        /// </summary>
        /// <param name="callback">成功返回值0，不成功则返回出错代码（<0）</param>
        [DllImport(@"DetectDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setDetectCallback(DetectCallBack callback);

        /// <summary>
        ///  释放DLL资源
        /// </summary>
        /// <param name="callback">无返回值</param>
        [DllImport(@"DetectDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DetectRelease(DetectCallBack callback);
    }
    #endregion
}