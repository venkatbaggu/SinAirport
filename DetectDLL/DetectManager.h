#ifndef DETECT_MANAGER_H
#define DETECT_MANAGER_H

#include <future>
#include <boost/thread.hpp>

#include <core/cuda.hpp>
#include <cudawarping.hpp>
#include <cudaimgproc.hpp>

#include "Global.h"
#include "Singleton.h"
#include "Conversion.h"
#include "TargetSubtractor.h"
#include "ContourFilterManager.h"

/**
* @class DetectManager
* @brief
*
* 负责整个检测过程
*/
class DetectManager : public Singleton<DetectManager>
{
	friend class Singleton<DetectManager>;
	DetectManager(void);

public:
	void update(cv::Mat mat);

	void setRect(const vector<cv::Rect>& rects);

public:
	boost::mutex data;

private:
	vector<cv::Rect> contoursToRects(
					const vector<vector<cv::Point>>& contours);

	bool send(void);

private:
	cv::cuda::GpuMat m_cur;

	boost::mutex m_runMutex;
	unique_ptr<TargetSubtractor> m_pTargetSubtractor;
	vector<vector<cv::Point> > m_contours;
	vector<cv::Rect> m_contourRects;
};

#endif // DETECT_MANAGER_H

