#include "ContourFilter.h"

/**
* @brief ContourFilter::filter
*
* ��������
* @param[in] contours ��Ҫ�����˵�����
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
