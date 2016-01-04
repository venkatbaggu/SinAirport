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
int __declspec(dllexport) TrackInit(void* pDC, 
						const vector<CameraInfo>& cameras);

/*
* @brief setTrackCallback
*
* 设置回调函数
* @param[in] ptr 回调函数
* @return int
* 成功返回值0，不成功则返回出错代码（<0）
*/
int __declspec(dllexport) setTrackCallback(TrackCallback ptr);

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
void __declspec(dllexport) TrackRun(vector<vector<cv::Rect>> objs);

/*
* @brief TrackStart
*
* 开始拼接、显示数据
* @param[in] void
* @return bool
* 启动成功返回true，不成功返回false。
*/
bool __declspec(dllexport) TrackStart(void);

/**
* @brief updateInvadeRule
*
* 更新入侵检测规则
* @param[in] rules 入侵检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
bool __declspec(dllexport) updateInvadeRule(vector<HaltRule> rules);

/**
* @brief updateHaltRule
*
* 更新停机检测规则
* @param[in] rules 停机检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
bool __declspec(dllexport) updateHaltRule(vector<HaltRule> rules);

/**
* @brief updateWrongTrajectoryRule
*
* 更新错误轨迹检测规则
* @param[in] rules 错误轨迹检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
bool __declspec(dllexport) updateWrongTrajectoryRule(
	vector<WrongTrajectoryRule> rules);

/**
* @brief updateConflictTrajectoryRule
*
* 更新错误轨迹检测规则
* @param[in] rules 轨迹冲突检测规则
* @return bool
* 更新成功返回true，不成功返回false。
*/
bool __declspec(dllexport) updateConflictTrajectoryRule(
	vector<ConflictTrajectoryRule> rules);

void __declspec(dllexport) clearRules(void);

/**
* @brief TrackRelease
*
* 释放资源
* @param[in] void
* @return void
*/
void __declspec(dllexport) TrackRelease(void);

#endif // TRACK_H