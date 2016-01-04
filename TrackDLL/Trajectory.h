#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <deque>
#include <vector>
using std::deque;
using std::vector;

#include "opencv.hpp"

/**
* @class Trajectory
* @brief
*
* 描述了目标的运动轨迹特征
*/
class Trajectory
{
public:
	void append(const cv::Rect& rect);
	const cv::Rect lastRect(void) const;
	const vector<cv::Point> trajects(void) const;

public:
	deque<cv::Rect> rects;	/**< 目标轨迹片段链表 */

private:
	vector<cv::Point> points;
};


#endif // TRAJECTORY_H

