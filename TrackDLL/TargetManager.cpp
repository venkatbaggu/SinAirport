#include "TargetManager.h"
#include "IDManager.h"
#include "Global.h"
#include "TrackerManager.h"

/**
* @brief TargetManager::TargetManager.
*
* ���캯��
*/
TargetManager::TargetManager(void)
{
	probTargets.reserve(gDefaultVectorLength);
}

/**
* @brief TargetManager::release
*
* @param[in] void
* @return void
*/
void TargetManager::release(void)
{
	trajectories.clear();
	templates.clear();
	probTargets.clear();
	TrackerManager::instance()->release();
	IDManager::instance()->reset();
}

/**
* @brief TargetManager::append
*
* ׷����Ŀ��
* @param[in] fragments  Ŀ��켣Ƭ��
* @return void
*/
void TargetManager::append(const map<ID, cv::Rect>& fragments)
{
	const auto endIter = trajectories.end();
	const auto endFragmentIter = fragments.cend();
	for (auto iter = fragments.cbegin(); endFragmentIter != iter; ++iter)
	{
		const auto trajectoryIter = trajectories.find(iter->first);
		assert(endIter != trajectoryIter);
		trajectoryIter->second.append(iter->second);
	}
}

/**
* @brief TargetManager::getLastFragments
*
* ��ȡ��һ֡��Ŀ��켣
* @param[in] fragments  ��һ֡��Ŀ��켣�ᱻ���������
* @return void
*/
void TargetManager::getLastFragments(vector< pair<ID, cv::Rect> >& fragments) const
{
	const auto endIter = trajectories.cend();
	for (auto iter = trajectories.cbegin()
		; endIter != iter
		; ++iter)
	{
		fragments.push_back(
			std::make_pair(iter->first, iter->second.lastRect()));
	}
}

/**
* @brief TargetManager::addTarget
*
* ����Ŀ��
* @param[in] rects  ��ǰĿ������
* @return void
*/
void TargetManager::addTarget(const vector<cv::Rect>& rects)
{
	if (rects.empty())
	{
		probTargets.clear();
		return;
	}
	if (probTargets.empty())
	{
		const size_t size = rects.size();
		for (size_t i = 0; size != i; ++i)
		{
			probTargets.push_back(
				std::make_pair(rects[i], 1));
		}
		return;
	}
	vector< pair<cv::Rect, size_t> > temp;
	temp.swap(probTargets);
	probTargets.clear();

	const size_t curSize = rects.size();
	const size_t lastSize = temp.size();

	for (size_t i = 0; curSize != i; ++i)
	{
		double maxAreaRation = 0;
		size_t index = 0;

		for (size_t j = 0; lastSize != j; ++j)
		{
			const cv::Rect inter = rects[i] & temp[j].first;
			if (0 == inter.width || 0 == inter.height)
				continue;
			const double ratio = static_cast<double>(inter.area())
				/ temp[j].first.area();

			if (maxAreaRation < ratio)
			{
				maxAreaRation = ratio;
				index = j;
			}
		}
		if (maxAreaRation > gUncertainTargetAreaRatioThreshold
			&& gUncertainTargetFramesCount == temp[index].second)
		{
			const ID id = IDManager::instance()->nextID();
			trajectories[id].append(rects[i]);
			TrackerManager::instance()->addTracker(id, rects[i]);
		}
		else if (maxAreaRation > gUncertainTargetAreaRatioThreshold)
		{
			probTargets.push_back(
				std::make_pair(rects[i], temp[index].second + 1));
		}
		else
		{
			probTargets.push_back(
				std::make_pair(rects[i], 1));
		}
	}
}

/**
* @brief TargetManager::removeTarget
*
* �Ƴ���ʧ��Ŀ��
* @param[in] ids  ��ʧ��Ŀ��ID
* @return void
*/
void TargetManager::removeTarget(const vector<ID>& ids)
{
	const size_t size = ids.size();
	for (size_t i = 0; size != i; ++i)
	{
		auto iter = trajectories.find(ids[i]);
		assert(trajectories.cend() != iter);
		trajectories.erase(iter);
	}

	TrackerManager::instance()->removeTrackers(ids);
}

/**
* @brief TargetManager::getTemplates
*
* ��ȡĿ��ģ��
* @param[in] templatesClone  Ŀ��ģ��
* @param[in] ids  ��Ҫʹ��ģ���Ŀ��ID
* @return void
*/
void TargetManager::getTemplates(map<ID, cv::Mat>& templatesClone
	, const vector<ID>& ids)
{
	const auto endIter = templates.end();

	const size_t size = ids.size();
	for (size_t i = 0; size != i; ++i)
	{
		if (templatesClone.end() != templatesClone.find(ids[i]))
			continue;

		const auto iter = templates.find(ids[i]);
		cv::Mat roi;
		if (endIter != iter)
		{
			roi = iter->second.clone();
		}
		else
		{
			cv::Rect tmp = trajectories[ids[i]].lastRect();
			roi = gLastFrame(tmp).clone();
		}
		templatesClone.insert(std::make_pair(ids[i], roi));
	}
}

/**
* @brief TargetManager::getSplitIds
*
* ��ȡ���ѵ�Ŀ��ID
* @param[in] splitTargets  ���ѵ�Ŀ��ID������
* @param[in] splitIds  ���ѵ�Ŀ��ID
* @return void
*/
void TargetManager::getSplitIds(const vector< pair<ID, vector<cv::Rect> > >& splitTargets
	, vector<ID>& splitIds) const
{
	const auto endIter = splitTargets.cend();
	for (auto iter = splitTargets.cbegin(); endIter != iter; ++iter)
	{
		splitIds.push_back(iter->first);
	}
}

/**
* @brief TargetManager::getOccludedIds
*
* ��ȡ�ڵ���Ŀ��ID
* @param[in] occludedTargets  ���ѵ�Ŀ��ID������
* @param[in] occludedIds  ���ѵ�Ŀ��
* @return void
*/
void TargetManager::getOccludedIds(const vector< pair<cv::Rect, vector<ID> > >& occludedTargets
	, vector<ID>& occludedIds) const
{
	const auto endIter = occludedTargets.cend();
	for (auto iter = occludedTargets.cbegin(); endIter != iter; ++iter)
	{
		occludedIds.insert(occludedIds.end(), iter->second.begin(), iter->second.end());
	}
}

/**
* @brief TargetManager::updateTemplates
*
* ����Ŀ��ģ��
* @param[in] splitTargets  ���ѵ�Ŀ��ID������
* @param[in] occludedTargets  �ڵ���Ŀ��ID������
* @return void
*/
void TargetManager::updateTemplates(const vector< pair<ID, vector<cv::Rect> > >& splitTargets
	, const vector< pair<cv::Rect, vector<ID> > >& occludedTargets)
{
	map<ID, cv::Mat> templatesClone;
	if (false == splitTargets.empty())
	{
		vector<ID> splitIds;
		splitIds.reserve(gDefaultVectorLength);
		getSplitIds(splitTargets, splitIds);
		getTemplates(templatesClone, splitIds);
	}

	if (false == occludedTargets.empty())
	{
		vector<ID> occludedIds;
		occludedIds.reserve(gDefaultVectorLength);
		getOccludedIds(occludedTargets, occludedIds);
		getTemplates(templatesClone, occludedIds);
	}
	templates.swap(templatesClone);
}

/**
* @brief TargetManager::getTemplate
*
* ��ȡĿ��ģ��
* @param[in] id  ��Ҫģ���Ŀ��ID
* @return const cv::Mat  Ŀ��id��ģ��
*/
const cv::Mat TargetManager::getTemplate(const ID id) const
{
	const auto iter = templates.find(id);
	assert(templates.end() != iter);
	return iter->second;
}

/**
* @brief TargetManager::removeAllTarget
*
* �Ƴ�����Ŀ��
* @param[in] void
* @return void
*/
void TargetManager::removeAllTarget(void)
{
	trajectories.clear();
	templates.clear();
	TrackerManager::instance()->removeAllTracker();
}

/**
* @brief TargetManager::getTrajectory
*
* ��ȡĿ��켣
* @param[in] id  ��Ҫ�켣��Ŀ��ID
* @return const Trajectory&  Ŀ��id�Ĺ켣
*/
const Trajectory& TargetManager::getTrajectory(const ID id)
{
	assert(trajectories.end() != trajectories.find(id));
	return trajectories[id];
}

/**
* @brief TargetManager::getTrajectory
*
* ��ȡ����Ŀ��켣
* @param[in] void
* @return const map<ID,Trajectory>&  ����Ŀ��켣
*/
const map<ID, Trajectory>& TargetManager::getTrajectory(void)
{
	return trajectories;
}

/**
* @brief TargetManager::contains
*
* �ж��Ƿ����Ŀ��
* @param[in] id  Ŀ��ID
* @return bool  ������Ŀ��ID���򷵻�true������false
*/
bool TargetManager::contains(const ID id) const
{
	return trajectories.end() != trajectories.find(id);
}