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
* ������Ŀ����˶��켣����
*/
class Trajectory
{
public:
	void append(const cv::Rect& rect);
	const cv::Rect lastRect(void) const;
	const vector<cv::Point> trajects(void) const;

public:
	deque<cv::Rect> rects;	/**< Ŀ��켣Ƭ������ */

private:
	vector<cv::Point> points;
};


#endif // TRAJECTORY_H

