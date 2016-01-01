#include "ContourFilterManager.h"

/**
* @brief ContourFilterManager::ContourFilterManager
*
* 构造函数
*/
ContourFilterManager::ContourFilterManager()
{
}

/**
* @brief ContourFilterManager::filter
*
* 过滤轮廓
* @param[in] contours 轮廓集合
* @return void
*/
void ContourFilterManager::filter(vector<vector<Point>>& contours)
{
	const auto endIter = contourFilters.end();
	for (auto iter = contourFilters.begin();
		endIter != iter;
		++iter)
	{
		iter->get()->filter(contours);
	}
	return;
}

/**
* @brief ContourFilterManager::init
*
* 初始化过滤器集合
* @return void
*/
void ContourFilterManager::init()
{
	contourFilters.emplace_back(new ContourAreaFilter);
}