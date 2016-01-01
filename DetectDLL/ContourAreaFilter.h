#ifndef CONTOUR_AREA_FILTER_H
#define CONTOUR_AREA_FILTER_H

#include "Global.h"
#include "ContourFilter.h"

/**
* @class ContourAreaFilter
* @brief
*
* ������˲������������������
*/
class ContourAreaFilter : public ContourFilter
{
protected:
	virtual bool ifFiltered(const vector<Point>& contour);
};

#endif // CONTOUR_AREA_FILTER_H
