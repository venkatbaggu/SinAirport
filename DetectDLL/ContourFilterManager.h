#ifndef CONTOUR_FILTER_MANAGER_H
#define CONTOUR_FILTER_MANAGER_H

#include <memory>
using std::unique_ptr;

#include "Singleton.h"
#include "ContourFilter.h"
#include "ContourAreaFilter.h"

/**
* @class ContourFilterManager
* @brief
*
* ����ά����������������
*/
class ContourFilterManager : public Singleton<ContourFilterManager>
{
	friend class Singleton<ContourFilterManager>;
	ContourFilterManager();

public:
	void init();
	void filter(vector<vector<Point>>& contours);

private:
	vector< unique_ptr<ContourFilter> > contourFilters;
};

#endif // CONTOUR_FILTER_MANAGER_H

