#include "ContourAreaFilter.h"

/**
* @brief ContourAreaFilter::ifFiltered
*
* 判断轮廓是否负责被过滤的条件
* @param[in] contour 需要判断的轮廓
* @return bool  如果需要过滤该轮廓则返回true，否则返回false
*/
bool ContourAreaFilter::ifFiltered(const vector<Point>& contour)
{
	return contourArea(contour) < gContourAreaThreshold;
}

