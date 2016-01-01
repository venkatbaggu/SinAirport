#ifndef CONTOUR_FILTER_H
#define CONTOUR_FILTER_H

#include <vector>
using std::vector;

#include "opencv.hpp"
using cv::Point;

/**
* @class ContourFilter
* @brief
*
* 负责过滤不需要的轮廓
*/
class ContourFilter
{
public:
	void filter(vector<vector<Point> >& contours);

protected:
	virtual bool ifFiltered(const vector<Point>& contour) = 0;
};

#endif // CONTOUR_FILTER_H
