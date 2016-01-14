using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sins.Airport.Mat
{
    using TData=Sins.Client.Service;
    using Sins.Net;
    

    #region 回调函数
    /// <summary>
    /// 回调函数
    /// </summary>
    /// <param name="data">返回结果</param>
    /// <param name="len">返回数组长度</param>
    public delegate void TrackResultCallBack(IntPtr data, int len);
    #endregion

    #region 跟踪接口类
    /// <summary>
    ///  跟踪接口类
    /// </summary>
    public class TrackMat
    {

        /// <summary>
        ///  初始化
        /// </summary>
        /// <param name="Handle">显示窗口</param>
        /// <returns>成功返回值0，不成功则返回出错代码（<0）</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int TrackInit(IntPtr Handle, 
            CameraInfo[] cameras, int size);

        /// <summary>
        ///  设置回调委托
        /// </summary>
        /// <param name="callback">设置回调函数</param>
        /// <returns>eturn  成功返回值0，不成功则返回出错代码（<0）</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setTrackCallback(
            TrackResultCallBack callback);


        /// <summary>
        ///  开始运行拼接
        /// </summary>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int TrackStart();

        /// <summary>
        ///  输入拼接数据
        /// </summary>
        /// <param name="detections">detections[0]:左摄像头检测数据 detections[1]:中摄像头检测数据  detections[2]:右摄像头检测数据 </param>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void TrackRun(CRect[][] detections);

        /// <summary>
        ///  更新入侵检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int updateInvadeRule(HaltRule[] rules, int size);


        /// <summary>
        ///  更新停机检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern 
            int updateHaltRule(HaltRule[] rules, int size);

        /// <summary>
        ///  更新错误轨迹检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern
            int updateWrongTrajectoryRule(HaltRule rules, int size);


        /// <summary>
        ///  更新错误轨迹检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern 
            int updateConflictTrajectoryRule(
            ConflictTrajectoryRule[] rules, int size);
    }
    #endregion
}
