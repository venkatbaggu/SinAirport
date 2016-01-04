#ifndef TRACK_H
#define TRACK_H

#include "DataStructure.h"

/*
* @brief init
*
* Ŀ����ٳ�ʼ��
* @param[in] cameras ����ͷ��Ϣ
* @return int
* �ɹ�����ֵ0�����ɹ��򷵻س�����루<0��
*/
extern "C" int __declspec(dllexport) 
	TrackInit(void* pDC, const vector<CameraInfo>& cameras);

/*
* @brief setTrackCallback
*
* ���ûص�����
* @param[in] ptr �ص�����
* @return int
* �ɹ�����ֵ0�����ɹ��򷵻س�����루<0��
*/
extern "C" int __declspec(dllexport) 
			setTrackCallback(TrackCallback ptr);

/*
* @brief TrackRun
*
* ����ƴ������
* @param[in] detections �����
* detections[0]:������ͷ�������
* detections[1]:������ͷ�������
* detections[2]:������ͷ�������
* @return void
*/
extern "C" void __declspec(dllexport) 
		TrackRun(vector<vector<cv::Rect>> objs);

/*
* @brief TrackStart
*
* ��ʼƴ�ӡ���ʾ����
* @param[in] void
* @return bool
* �����ɹ�����true�����ɹ�����false��
*/
extern "C" bool __declspec(dllexport) TrackStart(void);

/**
* @brief updateInvadeRule
*
* �������ּ�����
* @param[in] rules ���ּ�����
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" bool __declspec(dllexport) 
		updateInvadeRule(vector<HaltRule> rules);

/**
* @brief updateHaltRule
*
* ����ͣ��������
* @param[in] rules ͣ��������
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" bool __declspec(dllexport)
		updateHaltRule(vector<HaltRule> rules);

/**
* @brief updateWrongTrajectoryRule
*
* ���´���켣������
* @param[in] rules ����켣������
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" bool __declspec(dllexport) 
		updateWrongTrajectoryRule(
				vector<WrongTrajectoryRule> rules);

/**
* @brief updateConflictTrajectoryRule
*
* ���´���켣������
* @param[in] rules �켣��ͻ������
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" bool __declspec(dllexport) 
		updateConflictTrajectoryRule(
				vector<ConflictTrajectoryRule> rules);

extern "C" void __declspec(dllexport) clearRules(void);

/**
* @brief TrackRelease
*
* �ͷ���Դ
* @param[in] void
* @return void
*/
extern "C" void __declspec(dllexport) TrackRelease(void);

#endif // TRACK_H