#include "TrackerManager.h"

/**
* @brief TrackerManager::TrackerManager.
*
* 构造函数
*/
TrackerManager::TrackerManager(void)
{
}

/**
* @brief TrackerManager::predict
*
* 预测目标运动位置
* @param[in] predictedRects  预测的运动位置
* @return void
*/
void TrackerManager::predict(vector< pair<ID, cv::Rect> >& predictedRects)
{
	const auto endIter = trackers.end();
	for (auto iter = trackers.begin()
		; endIter != iter
		; ++iter)
	{
		predictedRects.push_back(
			std::make_pair(iter->first, iter->second.predict()));
	}
}

/**
* @brief TrackerManager::correct
*
* 根据检测结果校正目标运动位置
* @param[in] matchedRects  需要校正的目标
* @param[in] correctedRects  校正后的结果
* @return void
*/
void TrackerManager::correct(const vector< pair<ID, cv::Rect> >& matchedRects
	, map<ID, cv::Rect>& correctedRects)
{
	/*	assert( trackers.size() == matchedRects.size() );*/
	const auto endIter = matchedRects.cend();
	for (auto iter = matchedRects.cbegin()
		; endIter != iter
		; ++iter)
	{
		const auto correctedIter = correctedRects.find(iter->first);
		if (correctedRects.cend() == correctedRects.find(iter->first))
		{
			auto trackerIter = trackers.find(iter->first);
			assert(trackers.cend() != trackerIter);
			correctedRects.insert(
				std::make_pair(iter->first, trackerIter->second.correct(iter->second)));
		}
	}
}

/**
* @brief TrackerManager::removeTrackers
*
* 移除丢失目标的跟踪器
* @param[in] lostTargets  需要移除跟踪器的目标
* @return void
*/
void TrackerManager::removeTrackers(const vector<ID>& lostTargets)
{
	const auto endIter = lostTargets.cend();
	for (auto iter = lostTargets.cbegin()
		; endIter != iter
		; ++iter)
	{
		assert(trackers.end() != trackers.find(*iter));
		trackers.erase(*iter);
	}
}

/**
* @brief TrackerManager::addTracker
*
* 增加跟踪器
* @param[in] id  需要跟踪的目标ID
* @param[in] rect  目标id的起始位置
* @return void
*/
void TrackerManager::addTracker(ID id, const cv::Rect& rect)
{
	assert(0 != id && trackers.end() == trackers.find(id));
	trackers[id].init(rect);
}

/**
* @brief TrackerManager::release
*
* 释放资源
* @param[in] void
* @return void
*/
void TrackerManager::release(void)
{
	trackers.clear();
	lostTimes.clear();
}

/**
* @brief TrackerManager::removeAllTracker
*
* 移除所有跟踪器
* @param[in] void
* @return void
*/
void TrackerManager::removeAllTracker(void)
{
	release();
}

