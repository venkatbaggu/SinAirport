#include "TrackerManager.h"

/**
* @brief TrackerManager::TrackerManager.
*
* ���캯��
*/
TrackerManager::TrackerManager(void)
{
}

/**
* @brief TrackerManager::predict
*
* Ԥ��Ŀ���˶�λ��
* @param[in] predictedRects  Ԥ����˶�λ��
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
* ���ݼ����У��Ŀ���˶�λ��
* @param[in] matchedRects  ��ҪУ����Ŀ��
* @param[in] correctedRects  У����Ľ��
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
* �Ƴ���ʧĿ��ĸ�����
* @param[in] lostTargets  ��Ҫ�Ƴ���������Ŀ��
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
* ���Ӹ�����
* @param[in] id  ��Ҫ���ٵ�Ŀ��ID
* @param[in] rect  Ŀ��id����ʼλ��
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
* �ͷ���Դ
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
* �Ƴ����и�����
* @param[in] void
* @return void
*/
void TrackerManager::removeAllTracker(void)
{
	release();
}

