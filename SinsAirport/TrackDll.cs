using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sins.Airport.Mat
{
    using Sins.Client.Service;

    //入侵检测规则
    public struct InvadeRule
    {
        public int ID; //规则ID
        public Point[] vertexes; //入侵检测区域
    }

    //停机检测规则
    public struct HaltRule
    {
        public int ID; //规则ID
        public Point[] vertexes; //停机检测区域
    };

    //错误轨迹规则
    public struct WrongTrajectoryRule
    {
        public int ID; //规则ID
        public bool isValid; //角度是否有效	
        public int angle; //角度
        public Point[] entryVertexes; //入口区域
        public Point[] exitVertexes; //出口区域
    }

    //轨迹冲突规则
    public struct ConflictTrajectoryRule
    {
        public int ID; //规则ID
        public bool isValid; //角度是否有效
        public int targetAngle; //目标运动角度
        public int[] detectAngles; //检测角度
        public Point[] targetVertexes; //目标区域
        public Point[] detectVertexes; //检测区域
    }

    //回调函数
    public delegate void TrackResultCallBack(Result[] results, int nums);

    public class TrackMat
    {
        /*
        * @brief init
        *
        * 运行目标跟踪算法
        * @return int 
        * 成功返回值0，不成功则返回出错代码（<0）
        */

        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int init(IntPtr Handle);
        /*
        * @brief setCallback
        *
        * 设置回调函数
        * @return  成功返回值0，不成功则返回出错代码（<0）
        */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int setCallback(TrackResultCallBack callback);


        /*
       * @brief run
       *
       * 开始运行拼接
       * @return void
       */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void startMat();



        /*
        * @brief run
        *
        * 输入拼接数据
        * detections[0]:左摄像头检测数据
        * detections[1]:中摄像头检测数据
        * detections[2]:右摄像头检测数据
        * @return void
        */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void run(Rect[][] detections);

        /**
        * @brief updateInvadeRule
        *
        * 更新入侵检测规则
        * @param[in] rules 入侵检测规则
        * @return bool
        * 更新成功返回true，不成功返回false。
        */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool updateInvadeRule(HaltRule[] rules);

        /**
        * @brief updateHaltRule
        *
        * 更新停机检测规则
        * @param[in] rules 停机检测规则
        * @return bool
        * 更新成功返回true，不成功返回false。
        */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool updateHaltRule(HaltRule[] rules);

        /**
        * @brief updateWrongTrajectoryRule
        *
        * 更新错误轨迹检测规则
        * @param[in] rules 错误轨迹检测规则
        * @return bool
        * 更新成功返回true，不成功返回false。
        */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool updateWrongTrajectoryRule(WrongTrajectoryRule[] rules);

        /**
        * @brief updateConflictTrajectoryRule
        *
        * 更新错误轨迹检测规则
        * @param[in] rules 轨迹冲突检测规则
        * @return bool
        * 更新成功返回true，不成功返回false。
        */
        [DllImport(@"TrackMat.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool updateConflictTrajectoryRule(ConflictTrajectoryRule[] rules);
    }
}
