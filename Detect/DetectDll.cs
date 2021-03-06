﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sins.Airport.Detect
{
    using Sins.Client.Service;
    
    //回调函数
    public delegate void DetectCallBack(Rect[] objs);

    //摄像头数据
    public struct Camera
    {
        public string ip; //摄像头IP地址
        public int port; //摄像头端口
        public string userName;  //摄像头用户名
        public string password;  //摄像头密码
    }
    public class Detect
    {
        /*
        * @brief init
        *
        * 目标检测初始化
        * @param[in] params 算法参数
        * @param[in] cameras 摄像头信息
        * @return int 
        * 成功返回值0，不成功则返回出错代码（<0）
        */
        [DllImport(@"Detect.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int init(string fileName, Camera cameras);

        /*
        * @brief init
        *
        * 开始检测
        * @return int 
        * 成功返回值0，不成功则返回出错代码（<0）
        */
        [DllImport(@"Detect.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int start();


        /*
        * @brief setDetectCallback
        *
        * 设置回调函数
        * @param[in] ptr 回调函数
        * @return int
        * 成功返回值0，不成功则返回出错代码（<0）
        */

        [DllImport(@"Detect.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void setDetectCallback(DetectCallBack callback);
    }

}