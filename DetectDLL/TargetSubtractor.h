#ifndef TARGET_SUBTRACTOR_H
#define TARGET_SUBTRACTOR_H

#include <opencv2/cudabgsegm.hpp>
#include <opencv2/cudafilters.hpp>
using cv::Ptr;
using cv::BackgroundSubtractor;
using cv::cuda::GpuMat;
using cv::cuda::Filter;

#include "Global.h"
#include "ContourFilterManager.h"

/**
* @class TargetSubtractor
* @brief
*
* 负责提取目标轮廓
*/
class TargetSubtractor
{
public:
	TargetSubtractor(const cv::Rect& r);

	void update(GpuMat& frame, GpuMat& mask);

	void getContours(GpuMat& mask,
		vector<vector<Point>>& contours) const;

private:
	/**< 混合高斯背景建模 */
	Ptr<BackgroundSubtractor> pBackgroundSubtractor;	
	/**< 监控区域 */
	cv::Rect rect;										
	double learningRate;

	Ptr<Filter> pDilateFilter1, pDilateFilter2, pErodeFilter;
};

#endif // TARGET_SUBTRACTOR_H

