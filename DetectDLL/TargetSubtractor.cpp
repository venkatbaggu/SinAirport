#include "TargetSubtractor.h"

/**
* @brief TargetSubtractor::TargetSubtractor
*
* 构造函数
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
* 获取轮廓
* @param[in] mask  前景掩膜（运动目标）
* @param[in] contours  检测到的目标轮廓
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
* 背景建模
* @param[in] frame  当前帧
* @param[in] mask  前景掩膜（运动目标）
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