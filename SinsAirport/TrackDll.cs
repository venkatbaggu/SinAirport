using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sins.Airport.Mat
{
    using TData=Sins.Client.Service;
    #region  点数据结构
    /// <summary>
    /// 点数据结构
    /// </summary>
    [Serializable]
    public struct CPoint
    {
        /// <summary>
        ///  X坐标
        /// </summary>
        public int X; 
        /// <summary>
        ///  坐标
        /// </summary>
        public int Y; 
    }
    #endregion

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
        public Int32 X;
        /// <summary>
        ///   Y坐标
        /// </summary>
        public Int32 Y;
        /// <summary>
        ///  矩形长
        /// </summary>
        public Int32 Width;
        /// <summary>
        ///  矩形宽
        /// </summary>
        public Int32 Height;
    }
    #endregion

    #region  入侵检测规则
    /// <summary>
    /// 入侵检测规则结构
    /// </summary>
    [Serializable]
    public struct InvadeRule
    {
        /// <summary>
        ///  规则ID
        /// </summary>
        public int ID; 
        /// <summary>
        ///  入侵检测区域
        /// </summary>
        public CPoint[] vertexes; 
    }
    #endregion

    #region 停机检测规则
    /// <summary>
    /// 停机检测规则
    /// </summary>
    [Serializable]
    public struct HaltRule
    {
        /// <summary>
        ///  规则ID
        /// </summary>
        public int ID; 
        /// <summary>
        ///  停机检测区域
        /// </summary>
        public CPoint[] vertexes; 
    };
    #endregion

    #region 错误轨迹规则
    /// <summary>
    ///  错误轨迹规则
    /// </summary>
    [Serializable]
    public struct WrongTrajectoryRule
    {
        /// <summary>
        /// 规则ID
        /// </summary>
        public int ID; 
        /// <summary>
        /// 角度是否有效
        /// </summary>
        public bool isValid; 	
        /// <summary>
        /// 角度
        /// </summary>
        public int angle; 
        /// <summary>
        ///  入口区域
        /// </summary>
        public CPoint[] entryVertexes; 
        /// <summary>
        /// 出口区域
        /// </summary>
        public CPoint[] exitVertexes; 
    }
    #endregion

    #region 轨迹冲突规则
    /// <summary>
    /// 轨迹冲突规则
    /// </summary>
    [Serializable]
    public struct ConflictTrajectoryRule
    {
        /// <summary>
        ///  规则ID
        /// </summary>
        public int ID; 
        /// <summary>
        /// 角度是否有效
        /// </summary>
        public bool isValid; 
        /// <summary>
        /// 目标运动角度
        /// </summary>
        public int targetAngle; 
        /// <summary>
        /// 检测角度
        /// </summary>
        public int[] detectAngles; 
        /// <summary>
        /// 目标区域
        /// </summary>
        public CPoint[] targetVertexes; 
        /// <summary>
        /// 检测区域
        /// </summary>
        public CPoint[] detectVertexes; 
    }
    #endregion

    #region 回调函数
    /// <summary>
    /// 回调函数
    /// </summary>
    /// <param name="results">返回结果</param>
    /// <param name="nums">返回数组长度</param>
    public unsafe delegate void TrackResultCallBack(CRect* results, int nums);
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
        public static extern int TrackInit(IntPtr Handle, CameraInfo data);

        /// <summary>
        ///  设置回调委托
        /// </summary>
        /// <param name="callback">设置回调函数</param>
        /// <returns>eturn  成功返回值0，不成功则返回出错代码（<0）</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setTrackCallback(TrackResultCallBack callback);


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
        public static extern void run(CRect[][] detections);

        /// <summary>
        ///  更新入侵检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int updateInvadeRule(HaltRule[] rules);


        /// <summary>
        ///  更新停机检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern 
            int updateHaltRule(HaltRule* rules, int size);

        /// <summary>
        ///  更新错误轨迹检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern 
            int updateWrongTrajectoryRule(WrongTrajectoryRule* rules);


        /// <summary>
        ///  更新错误轨迹检测规则
        /// </summary>
        /// <param name="rules">规则</param>
        /// <returns>更新成功返回true，不成功返回false。</returns>
        [DllImport(@"TrackDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int updateConflictTrajectoryRule(ConflictTrajectoryRule[] rules);
    }
    #endregion
}
