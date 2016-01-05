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
extern "C" int __declspec(dllexport) 
			DetectInit(CameraInfo camera, char* param);

/*
* @brief setDetectCallback
*
* ���ûص�����
* @param[in] ptr �ص�����
* @return bool
* �ɹ�����ֵtrue�����ɹ��򷵻�false.
*/
extern "C" int __declspec(dllexport) 
			setDetectCallback(DetectCallback ptr);

/*
* @brief setDetectCallback
*
* ���ûص�����
* @param[in] void 
* @return bool
* �ɹ�����ֵtrue�����ɹ��򷵻�false.
*/
extern "C" int __declspec(dllexport) DetectStart();

/*
* @brief DetectRelease
*
* �ͷ���Դ
* @param[in] void
* @return void
*/
extern "C" void __declspec(dllexport) DetectRelease();

#endif // DETECT_H