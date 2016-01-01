#ifndef DETECT_H
#define DETECT_H

#include "DataStructure.h"

/*
* @brief init
*
* Ŀ�����ʼ��
* @param[in] param �㷨����
* @param[in] camera ����ͷ��Ϣ
* @return int
* �ɹ�����ֵ0�����ɹ��򷵻س�����루<0��
*/
int __declspec(dllexport) DetectInit(
				const CameraInfo& camera, const string& param);

/*
* @brief setDetectCallback
*
* ���ûص�����
* @param[in] ptr �ص�����
* @return bool
* �ɹ�����ֵtrue�����ɹ��򷵻�false.
*/
bool __declspec(dllexport) setDetectCallback(DetectCallback ptr);

/*
* @brief setDetectCallback
*
* ���ûص�����
* @param[in] void 
* @return bool
* �ɹ�����ֵtrue�����ɹ��򷵻�false.
*/
bool __declspec(dllexport) DetectStart(void);

/*
* @brief DetectRelease
*
* �ͷ���Դ
* @param[in] void
* @return void
*/
void __declspec(dllexport) DetectRelease(void);

#endif // DETECT_H