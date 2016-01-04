#ifndef STITCH_H
#define STITCH_H

#include "Mosaic.h"
#include "Singleton.h"

/**
* @class Stitch
* @brief
*
* ƴ���࣬����ƴ������ͷ
* ����˳��init->iniLogging->run
* ʹ�����о�̬������ʽʵ��
*/
class Stitch : public Singleton<Stitch> {
	friend class Singleton<Stitch>;
	Stitch(void);

public:
	/*
	* @brief init
	*
	* ��ʼ��ƴ�Ӳ���
	* @param[in] parameters ƴ�Ӳ����ļ�
	* @return int �ɹ�����ֵ0�����ɹ��򷵻س�����루<0��
	*/
	int init(const string& parameters);

	/*
	* @brief run
	*
	* ����Ŀ�����㷨
	* @param[in] left ��ǰ֡��������ͷ��
	* @param[in] mid ��ǰ֡��������ͷ��
	* @param[in] right ��ǰ֡��������ͷ��
	* @return mat �ɹ�����ƴ�ӽ�������ɹ�����ֵΪ��
	*/
	//cv::Mat run(const cv::Mat& left, 
	//				const cv::Mat& mid, const cv::Mat& right);

	void run(void);

private:
	CuStitcher stitcher;
	cv::Mat m_cur;
};

#endif // STITCH_H