#include "ConflictTrajectory.h"
#include "TargetManager.h"
#include "Global.h"

/**
* @brief Constructor for ConflictTrajectory::ConflictTrajectory.
*
* 构造函数
* @param[in] ri  规则ID
* @param[in] targetVertexes  目标区域多边形顶点
* @param[in] detectVertexes  检测区域多边形顶点
* @param[in] itv  目标运动角度是否有效
* @param[in] ta   目标运动角度
* @param[in] das 检测区域角度
*/
ConflictTrajectory::ConflictTrajectory(const RuleID ri
	, const vector<cv::Point>& targetVertexes
	, const vector<cv::Point>& detectVertexes
	, const bool itv
	, const int ta
	, const vector<int>& das)
	: IWarning(RuleType::ConflictTrajectoryType, ri)
	, isTargetAngleValid(itv)
	, targetAngle(ta)
	, detectAngles(das)
{
	initMask(targetMask, targetVertexes);
	auto targetRects(std::move(contoursToRects(targetMask)));
	assert(1 == targetRects.size());
	targetRect = targetRects.front();

	initMask(detectMask, detectVertexes);
	auto detectRects(std::move(contoursToRects(detectMask)));
	assert(1 == detectRects.size());
	detectRect = detectRects.front();

	rect = targetRect | detectRect;
}

/**
* @brief ConflictTrajectory::boundingRect
*
* 实现了void IDetector::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect ConflictTrajectory::boundingRect() const
{
	return rect;
}

/**
* @brief ConflictTrajectory::isInTargetRegion
*
* 判断目标是否在TargetRegion区域
* @param[in] r  目标所在区域
* @return bool  如果目标在TargetRegion区域则返回true，否则返回false
*/
bool ConflictTrajectory::isInTargetRegion(const cv::Rect& r) const
{
	const cv::Point p(r.x + r.width / 2 - 1, r.y + r.height - 1);
	if (true == targetRect.contains(p))
	{
		return true;
	}
	return false;
}

/**
* @brief ConflictTrajectory::isInDetectRegion
*
* 判断目标是否在DetectRegion区域
* @param[in] r  目标所在区域
* @return bool  如果目标在DetectRegion区域则返回true，否则返回false
*/
bool ConflictTrajectory::isInDetectRegion(const cv::Rect& r) const
{
	const cv::Point p(r.x + r.width / 2 - 1, r.y + r.height - 1);
	if (true == detectRect.contains(p))
	{
		return true;
	}
	return false;
}

/**
* @brief ConflictTrajectory::detect
*
* 实现了std::list<ID> IDetector::detect ( )
* @return std::list<ID>
*/
list<ID> ConflictTrajectory::detect()
{
	list<ID> idList;

	const auto curTargets(std::move(getTargets()));
	if (true == curTargets.empty())
	{
		lastTargets.clear();
		lastIllegalTargets.clear();
		return idList;
	}

	const auto curIllegalTargets(std::move(getIllegalTargets()));
	if (true == curIllegalTargets.empty())
	{
		lastTargets.clear();
		lastIllegalTargets.clear();
		return idList;
	}

	list<ID> output;
	set_difference(curTargets.begin(), curTargets.end(),
		lastTargets.begin(), lastTargets.end(),
		std::back_insert_iterator<decltype(output)>(output));

	if (false == output.empty())
	{
		idList.insert(idList.end()
			, curIllegalTargets.begin(), curIllegalTargets.end());
	}
	else
	{
		output.clear();
		set_difference(curIllegalTargets.begin(), curIllegalTargets.end(),
			lastIllegalTargets.begin(), lastIllegalTargets.end(),
			std::back_insert_iterator<decltype(output)>(output));
		idList.swap(output);
	};
	lastTargets = std::move(curTargets);
	lastIllegalTargets = std::move(curIllegalTargets);
	return idList;
}

/**
* @brief ConflictTrajectory::checkAngle
*
* 判断目标运动方向是否与设定方向一致
* @param[in] trajectory  目标运动轨迹
* @param[in] moveAngle  运动方向
* @return bool  如果目标与设定方向一致则返回true，否则返回false
*/
bool ConflictTrajectory::checkAngle(const Trajectory& trajectory
	, const int moveAngle)
{
	const auto& endRect = trajectory.rects.back();

	const auto size = trajectory.rects.size();
	int index = 0;
	if (gMoveDirectionFrameDifference < (size - 1))
		index = size - 1 - gMoveDirectionFrameDifference;

	const auto& startRect = trajectory.rects[index];

	const double xOffset = (endRect.x + endRect.width / 2)
		- (startRect.x + startRect.width / 2);
	const double yOffset = -((endRect.y + endRect.height)
		- (startRect.y + startRect.height));

	if (0 == xOffset && 0 == yOffset)
	{
		return true;
	}

	const double diff = fabs(angle(xOffset, yOffset) - moveAngle);
	if (diff <= gAngleThreshold)
	{
		return true;
	}
	return false;
}

/**
* @brief ConflictTrajectory::getTargets
*
* 获取targetMask内且方向一致的运动目标
* @return std::set<ID>  有效的运动目标ID列表
*/
std::set<ID> ConflictTrajectory::getTargets()
{
	std::set<ID> targetSet;
	const auto& trajectories = TargetManager::instance()->getTrajectory();
	const auto endTargetIter = trajectories.cend();
	for (auto targetIter = trajectories.cbegin()
		; endTargetIter != targetIter
		; ++targetIter)
	{
		const auto& t = targetIter->second;
		if (true == isInTargetRegion(t.lastRect()))
		{
			if (false == isTargetAngleValid ||
				(true == isTargetAngleValid
				&& true == checkAngle(t, targetAngle)))
				targetSet.insert(targetIter->first);
		}
	}
	return targetSet;
}

/**
* @brief ConflictTrajectory::getIllegalTargets
*
* 获取detectMask内且方向一致的违规运动目标
* @return std::set<ID>  违规的运动目标ID列表
*/
std::set<ID> ConflictTrajectory::getIllegalTargets()
{
	static const auto& endAngleIter = detectAngles.cend();
	static bool isAnglesEmpty = detectAngles.empty();

	std::set<ID> illegalTargetSet;

	const auto& trajectories = TargetManager::instance()->getTrajectory();
	const auto endTargetIter = trajectories.cend();
	for (auto targetIter = trajectories.cbegin()
		; endTargetIter != targetIter
		; ++targetIter)
	{
		const auto& t = targetIter->second;
		if (true == isInDetectRegion(t.lastRect()))
		{
			const auto id = targetIter->first;
			if (true == isAnglesEmpty)
			{
				illegalTargetSet.insert(id);
			}
			else
			{
				for (auto angleIter = detectAngles.cbegin()
					; endAngleIter != angleIter
					; ++angleIter)
				{
					if (true == checkAngle(t, *angleIter))
					{
						illegalTargetSet.insert(id);
						break;
					}
				}
			}
		}
	}
	return illegalTargetSet;
}

cv::Rect ConflictTrajectory::getRect()
{
	return rect;
}