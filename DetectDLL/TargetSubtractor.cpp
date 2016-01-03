#include "TargetSubtractor.h"

/**
* @brief TargetSubtractor::TargetSubtractor
*
* ���캯��
*/
TargetSubtractor::TargetSubtractor(const cv::Rect& r) :
	rect(r)
{
	pBackgroundSubtractor = 
		cv::cuda::createBackgroundSubtractorMOG2(gMOG2HistoryLength,
						gMOG2VarThreshold, gShadowDetection);

	pDilateFilter1 =
		cv::cuda::createMorphologyFilter(
		cv::MORPH_DILATE, CV_8UC1, cv::Mat(), cv::Point(-1, -1), 2);

	pDilateFilter2 =
		cv::cuda::createMorphologyFilter(
		cv::MORPH_DILATE, CV_8UC1, cv::Mat(), cv::Point(-1, -1), 1);

	pErodeFilter =
		cv::cuda::createMorphologyFilter(
		cv::MORPH_ERODE, CV_8UC1, cv::Mat(), cv::Point(-1, -1), 3);
}

/**
* @brief TargetSubtractor::getContours
*
* ��ȡ����
* @param[in] mask  ǰ����Ĥ���˶�Ŀ�꣩
* @param[in] contours  ��⵽��Ŀ������
* @return void
*/
void TargetSubtractor::getContours(GpuMat& mask,
					vector<vector<Point>>& contours) const
{
	pDilateFilter2->apply(mask, mask);
	pErodeFilter->apply(mask, mask);
	pDilateFilter1->apply(mask, mask);

	cv::Mat tmp;
	mask.download(tmp);

	contours.clear();
	findContours(tmp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
}

/**
* @brief TargetSubtractor::update
*
* ������ģ
* @param[in] frame  ��ǰ֡
* @param[in] mask  ǰ����Ĥ���˶�Ŀ�꣩
* @return void
*/
void TargetSubtractor::update(GpuMat& frame, GpuMat& mask)
{
	assert(nullptr != pBackgroundSubtractor);

	pBackgroundSubtractor->apply(frame(rect), mask(rect), gMOG2LearningRate);
	//pBackgroundSubtractor->apply(frame, mask, gMOG2LearningRate);
	if (true == gShadowDetection)
	{
		cv::threshold(mask, mask, gMOG2MidValue, gForegroundColorValue, cv::THRESH_BINARY);
	}
}