#include "DetectManager.h"

/**
* @brief DetectManager::DetectManager
*
* 构造函数
*/
DetectManager::DetectManager(void) :
	m_pTargetSubtractor(nullptr)
{
	m_contours.reserve(gDefaultVectorLength);
	m_contourRects.reserve(gDefaultVectorLength);
}

/**
* @brief DetectManager::setRect
*
* 设置监控区域
* @param[in] rects  区域列表
* @return void
*/
void DetectManager::setRect(const vector<Rect>& rects)
{
	if (0 == rects.size())
	{
		m_pTargetSubtractor.release();
		return;
	}

	Rect rect = rects.front();

	for (const auto& r : rects)
	{
		rect |= r;
	}

	rect -= cv::Point(gExternedPixels, gExternedPixels);
	rect += cv::Size(2 * gExternedPixels, 2 * gExternedPixels);

	rect &= gWholeRect;

	m_pTargetSubtractor.reset(new TargetSubtractor(rect));
}

/**
* @brief DetectManager::update
*
* 更新检测结果
* @param[in] cur 当前帧图像（灰度图）
* @return vector<cv::Rect>  检测结果（最小外接矩形）
*/
void DetectManager::update(Mat mat)
{
	if (true == mat.empty() || false == m_pTargetSubtractor) {
		return ;
	}

	if (false == m_runMutex.try_lock()) {
		return ;
	}

	m_cur.upload(mat);
	cv::cuda::resize(m_cur, m_cur, cv::Size(m_cur.cols / gRatio, m_cur.rows / gRatio));
	GpuMat mask(m_cur.rows, m_cur.cols, CV_8U, cv::Scalar::all(0));

	//detect(tested)
	m_pTargetSubtractor->update(m_cur, mask);
	m_pTargetSubtractor->getContours(mask, m_contours);

	//filter(tested)
	ContourFilterManager::instance()->filter(m_contours);
	m_contourRects = move(contoursToRects(m_contours));

	//call DetetCallback
	(*gCallback)(m_contourRects);

	m_contours.clear();
	m_contourRects.clear();
	m_runMutex.unlock();

	return ;
}

/**
* @brief DetectManager::contoursToRects
*
* 将检测结果由轮廓转换为最小外接矩形
* @param[in] contours 当前检测结果（点组成的轮廓）
* @return vector<cv::Rect>  检测结果（最小外接矩形）
*/
vector<Rect> DetectManager::contoursToRects(
	const vector<vector<Point>>& contours)
{
	vector<Rect> contourRects;
	contourRects.reserve(contours.size());
	const auto endIter = contours.cend();
	for (auto iter = contours.cbegin();
		endIter != iter;
		++iter)
	{
		contourRects.push_back(boundingRect(*iter) & gWholeRect);
	}

	return contourRects;
}