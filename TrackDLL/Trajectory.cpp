#include "Trajectory.h"

/**
* @brief Trajectory::append
*
* �����µ��˶��켣Ƭ��
* @param[in] rect  Ŀ�����ڵ�λ������
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
* ��ȡĿ�����ʱ�����ڵ�λ��
* @param[in] void
* @return const cv::Rect  Ŀ�����ʱ�����ڵ�λ��
*/
const cv::Rect Trajectory::lastRect(void) const
{
	assert(false == rects.empty());
	return rects.back();
}

/**
* @brief Trajectory::points
*
* ��ȡ����Ŀ����˶��켣���㹹�ɣ�
* @param[in] void
* @return const cv::points  Ŀ����˶��켣
*/
const vector<cv::Point> Trajectory::trajects() const
{
	return points;
}

