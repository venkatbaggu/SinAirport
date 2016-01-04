#include "Trajectory.h"

/**
* @brief Trajectory::append
*
* 增加新的运动轨迹片段
* @param[in] rect  目标所在的位置区域
* @return void
*/
void Trajectory::append(const cv::Rect& rect)
{
	rects.push_back(rect);

	points.push_back(cv::Point(
		rect.x + rect.width / 2, rect.y + rect.height));
}

/**
* @brief Trajectory::lastRect
*
* 获取目标最后时刻所在的位置
* @param[in] void
* @return const cv::Rect  目标最后时刻所在的位置
*/
const cv::Rect Trajectory::lastRect(void) const
{
	assert(false == rects.empty());
	return rects.back();
}

/**
* @brief Trajectory::points
*
* 获取跟踪目标的运动轨迹（点构成）
* @param[in] void
* @return const cv::points  目标的运动轨迹
*/
const vector<cv::Point> Trajectory::trajects() const
{
	return points;
}

