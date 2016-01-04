#include "TargetManager.h"
#include "IDManager.h"
#include "Global.h"
#include "TrackerManager.h"

/**
* @brief TargetManager::TargetManager.
*
* 构造函数
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
* 追加新目标
* @param[in] fragments  目标轨迹片段
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
* 获取上一帧的目标轨迹
* @param[in] fragments  上一帧的目标轨迹会被加入该其中
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
* 增加目标
* @param[in] rects  当前目标区域
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
* 移除消失的目标
* @param[in] ids  消失的目标ID
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
* 获取目标模板
* @param[in] templatesClone  目标模板
* @param[in] ids  需要使用模板的目标ID
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
* 获取分裂的目标ID
* @param[in] splitTargets  分裂的目标ID及区域
* @param[in] splitIds  分裂的目标ID
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
* 获取遮挡的目标ID
* @param[in] occludedTargets  分裂的目标ID及区域
* @param[in] occludedIds  分裂的目标
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
* 更新目标模板
* @param[in] splitTargets  分裂的目标ID及区域
* @param[in] occludedTargets  遮挡的目标ID及区域
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
* 获取目标模板
* @param[in] id  需要模板的目标ID
* @return const cv::Mat  目标id的模板
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
* 移除所有目标
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
* 获取目标轨迹
* @param[in] id  需要轨迹的目标ID
* @return const Trajectory&  目标id的轨迹
*/
const Trajectory& TargetManager::getTrajectory(const ID id)
{
	assert(trajectories.end() != trajectories.find(id));
	return trajectories[id];
}

/**
* @brief TargetManager::getTrajectory
*
* 获取所有目标轨迹
* @param[in] void
* @return const map<ID,Trajectory>&  所有目标轨迹
*/
const map<ID, Trajectory>& TargetManager::getTrajectory(void)
{
	return trajectories;
}

/**
* @brief TargetManager::contains
*
* 判断是否包含目标
* @param[in] id  目标ID
* @return bool  若包含目标ID，则返回true，否则false
*/
bool TargetManager::contains(const ID id) const
{
	return trajectories.end() != trajectories.find(id);
}