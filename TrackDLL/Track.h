#ifndef TRACK_H
#define TRACK_H

#include "DataStructure.h"

/*
* @brief init
*
* 目标跟踪初始化
* @param[in] cameras 摄像头信息
* @return int
* 成功返回值0，不成功则返回出错代码（<0）
*/
extern "C" int __declspec(dllexport) 
	TrackInit(void* pDC, CameraInfo* cameras, int size);

/*
* @brief setTrackCallback
*
* 设置回调函数
* @param[in] ptr 回调函数
* @return int
* 成功返回值0，不成功则返回出错代码（<0）
*/
extern "C" int __declspec(dllexport) 
			setTrackCallback(TrackCallback ptr);

/*
* @brief TrackRun
*
* 输入拼接数据
* @param[in] detections 检测结果
* detections[0]:左摄像头检测数据
* detections[1]:中摄像头检测数据
* detections[2]:右摄像头检测数据
* @return void
*/
extern "C" void __declspec(dllexport) 
		TrackRun(CRect** objs, int row, int col);

/*
* @brief TrackStart
*
* 开始拼接、显示数据
* @param[in] void
* @return bool
* 启动成功返回true，不成功返回false。
*/
extern "C" int __declspec(dllexport) TrackStart(void);

/**
* @brief updateInvadeRule
*
* 更新入侵检测规则
* @param[in] rules 入侵检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
extern "C" int __declspec(dllexport) 
		updateInvadeRule(InvadeRule* rules, int size);

/**
* @brief updateHaltRule
*
* 更新停机检测规则
* @param[in] rules 停机检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
extern "C" int __declspec(dllexport)
		updateHaltRule(HaltRule* rules, int size);

/**
* @brief updateWrongTrajectoryRule
*
* 更新错误轨迹检测规则
* @param[in] rules 错误轨迹检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
extern "C" int __declspec(dllexport) 
		updateWrongTrajectoryRule(
				WrongTrajectoryRule* rules, int size);

/**
* @brief updateConflictTrajectoryRule
*
* 更新错误轨迹检测规则
* @param[in] rules 轨迹冲突检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
extern "C" int __declspec(dllexport) 
		updateConflictTrajectoryRule(
				ConflictTrajectoryRule* rules, int size);

extern "C" void __declspec(dllexport) clearRules(void);

/**
* @brief TrackRelease
*
* 释放资源
* @param[in] void
* @return void
*/
extern "C" void __declspec(dllexport) TrackRelease(void);

#endif // TRACK_H