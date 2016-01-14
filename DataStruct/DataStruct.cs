using System;
using System.Runtime.InteropServices;

namespace Sins.Net
{
    #region  摄像头信息
    /// <summary>
    /// 摄像头信息
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

    #region  点数据结构
    /// <summary>
    /// 点数据结构
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct CPoint
    {
        /// <summary>
        ///  X坐标
        /// </summary>
        public int X;
        /// <summary>
        ///  Y坐标
        /// </summary>
        public int Y;
    }
    #endregion

    #region  矩形结构体
    /// <summary>
    /// 矩形结构体
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct CRect
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

    #region  入侵检测规则
    /// <summary>
    /// 入侵检测规则结构
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
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
        /// <summary>
        ///  数组长度
        /// </summary>
        public int size;
    }
    #endregion

    #region 停机检测规则
    /// <summary>
    /// 停机检测规则
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
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
        /// <summary>
        ///  数组长度
        /// </summary>
        public int size;
    };
    #endregion

    #region 错误轨迹规则
    /// <summary>
    ///  错误轨迹规则
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
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
        ///  入口区域数组长度
        /// </summary>
        public int entrySize;
        /// <summary>
        /// 出口区域
        /// </summary>
        public CPoint[] exitVertexes;
        /// <summary>
        /// 出口区域数组长度
        /// </summary>
        public int exitSize;
    }
    #endregion

    #region 轨迹冲突规则
    /// <summary>
    /// 轨迹冲突规则
    /// </summary>
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
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
        /// 检测角度数组长度
        /// </summary>
        public int angleSize;
        /// <summary>
        /// 目标区域
        /// </summary>
        public CPoint[] targetVertexes;
        /// <summary>
        /// 目标区域数组长度
        /// </summary>
        public int targetSize;
        /// <summary>
        /// 检测区域
        /// </summary>
        public CPoint[] detectVertexes;
        /// <summary>
        /// 检测区域数组长度
        /// </summary>
        public int detectSize;
    }
    #endregion
}
