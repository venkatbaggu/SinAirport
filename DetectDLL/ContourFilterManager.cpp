#include "ContourFilterManager.h"

/**
* @brief ContourFilterManager::ContourFilterManager
*
* ���캯��
*/
ContourFilterManager::ContourFilterManager()
{
}

/**
* @brief ContourFilterManager::filter
*
* ��������
* @param[in] contours ��������
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
* ��ʼ������������
* @return void
*/
void ContourFilterManager::init()
{
	contourFilters.emplace_back(new ContourAreaFilter);
}