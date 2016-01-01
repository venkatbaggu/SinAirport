#include "ContourFilter.h"

/**
* @brief ContourFilter::filter
*
* 过滤轮廓
* @param[in] contours 需要被过滤的轮廓
* @return void
*/
void ContourFilter::filter(vector<vector<Point> >& contours)
{
	auto iter = contours.begin();
	auto endIter = contours.end();
	while (iter != endIter)
	{
		if (ifFiltered(*iter))
		{
			iter = contours.erase(iter);
			endIter = contours.end();
		}
		else
		{
			iter++;
		}
	}
}
