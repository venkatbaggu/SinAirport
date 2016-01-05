#ifndef DETECT_H
#define DETECT_H

#include "DataStructure.h"

/*
* @brief init
*
* 目标检测初始化
* @param[in] param 算法参数
* @param[in] camera 摄像头信息
* @return int
* 成功返回值0，不成功则返回出错代码（<0）
*/
extern "C" int __declspec(dllexport) 
			DetectInit(CameraInfo camera, char* param);

/*
* @brief setDetectCallback
*
* 设置回调函数
* @param[in] ptr 回调函数
* @return bool
* 成功返回值true，不成功则返回false.
*/
extern "C" int __declspec(dllexport) 
			setDetectCallback(DetectCallback ptr);

/*
* @brief setDetectCallback
*
* 设置回调函数
* @param[in] void 
* @return bool
* 成功返回值true，不成功则返回false.
*/
extern "C" int __declspec(dllexport) DetectStart();

/*
* @brief DetectRelease
*
* 释放资源
* @param[in] void
* @return void
*/
extern "C" void __declspec(dllexport) DetectRelease();

#endif // DETECT_H