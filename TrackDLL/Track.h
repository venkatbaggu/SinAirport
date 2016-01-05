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
	TrackInit(void* pDC, CameraInfo* cameras, int size);

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
		TrackRun(CRect** objs, int row, int col);

/*
* @brief TrackStart
*
* ��ʼƴ�ӡ���ʾ����
* @param[in] void
* @return bool
* �����ɹ�����true�����ɹ�����false��
*/
extern "C" int __declspec(dllexport) TrackStart(void);

/**
* @brief updateInvadeRule
*
* �������ּ�����
* @param[in] rules ���ּ�����
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" int __declspec(dllexport) 
		updateInvadeRule(InvadeRule* rules, int size);

/**
* @brief updateHaltRule
*
* ����ͣ��������
* @param[in] rules ͣ��������
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" int __declspec(dllexport)
		updateHaltRule(HaltRule* rules, int size);

/**
* @brief updateWrongTrajectoryRule
*
* ���´���켣������
* @param[in] rules ����켣������
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" int __declspec(dllexport) 
		updateWrongTrajectoryRule(
				WrongTrajectoryRule* rules, int size);

/**
* @brief updateConflictTrajectoryRule
*
* ���´���켣������
* @param[in] rules �켣��ͻ������
* @return bool
* ���³ɹ�����true�����ɹ�����false��
*/
extern "C" int __declspec(dllexport) 
		updateConflictTrajectoryRule(
				ConflictTrajectoryRule* rules, int size);

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