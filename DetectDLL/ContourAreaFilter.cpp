#include "ContourAreaFilter.h"

/**
* @brief ContourAreaFilter::ifFiltered
*
* �ж������Ƿ��𱻹��˵�����
* @param[in] contour ��Ҫ�жϵ�����
* @return bool  �����Ҫ���˸������򷵻�true�����򷵻�false
*/
bool ContourAreaFilter::ifFiltered(const vector<Point>& contour)
{
	return contourArea(contour) < gContourAreaThreshold;
}

