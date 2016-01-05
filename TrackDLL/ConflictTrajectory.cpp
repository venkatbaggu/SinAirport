#include "ConflictTrajectory.h"
#include "TargetManager.h"
#include "Global.h"

/**
* @brief Constructor for ConflictTrajectory::ConflictTrajectory.
*
* ���캯��
* @param[in] ri  ����ID
* @param[in] targetVertexes  Ŀ���������ζ���
* @param[in] detectVertexes  ����������ζ���
* @param[in] itv  Ŀ���˶��Ƕ��Ƿ���Ч
* @param[in] ta   Ŀ���˶��Ƕ�
* @param[in] das �������Ƕ�
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
* ʵ����void IDetector::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect ConflictTrajectory::boundingRect() const
{
	return rect;
}

/**
* @brief ConflictTrajectory::isInTargetRegion
*
* �ж�Ŀ���Ƿ���TargetRegion����
* @param[in] r  Ŀ����������
* @return bool  ���Ŀ����TargetRegion�����򷵻�true�����򷵻�false
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
* �ж�Ŀ���Ƿ���DetectRegion����
* @param[in] r  Ŀ����������
* @return bool  ���Ŀ����DetectRegion�����򷵻�true�����򷵻�false
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
* ʵ����std::list<ID> IDetector::detect ( )
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
* �ж�Ŀ���˶������Ƿ����趨����һ��
* @param[in] trajectory  Ŀ���˶��켣
* @param[in] moveAngle  �˶�����
* @return bool  ���Ŀ�����趨����һ���򷵻�true�����򷵻�false
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
* ��ȡtargetMask���ҷ���һ�µ��˶�Ŀ��
* @return std::set<ID>  ��Ч���˶�Ŀ��ID�б�
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
* ��ȡdetectMask���ҷ���һ�µ�Υ���˶�Ŀ��
* @return std::set<ID>  Υ����˶�Ŀ��ID�б�
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