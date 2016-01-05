#include "WrongTrajectory.h"
#include "TargetManager.h"
#include "Global.h"

/**
* @brief Constructor for WrongTrajectory::WrongTrajectory.
*
* 构造函数
* @param[in] ri  规则ID
* @param[in] entryVertexes  入口区域多边形顶点
* @param[in] exitVertexes  出口区域多边形顶点
* @param[in] iv  目标运动角度是否有效
* @param[in] a   目标运动角度
*/
WrongTrajectory::WrongTrajectory(const RuleID ri,
	const vector<cv::Point>& entryVertexes, 
	const vector<cv::Point>& exitVertexes, 
	const bool iv, const int a):
	IWarning(RuleType::WrongTrajectoryType, ri), 
	isValid(iv), moveAngle(a)
{
	initMask(entryMask, entryVertexes);
	auto inRects(std::move(contoursToRects(entryMask)));
	assert(1 == inRects.size());
	entryRect = inRects.front();

	initMask(exitMask, exitVertexes);
	auto outRects(std::move(contoursToRects(exitMask)));
	assert(1 == outRects.size());
	exitRect = outRects.front();

	rect = entryRect | exitRect;
}

/**
* @brief WrongTrajectory::boundingRect
*
* 实现了void IDetector::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect WrongTrajectory::boundingRect() const
{
	return rect;
}

/**
* @brief WrongTrajectory::isInEntryRegion
*
* 判断目标是否在入口区域
* @param[in] r  目标所在区域
* @return bool  如果目标在入口区域内则返回true，否则返回false
*/
bool WrongTrajectory::isInEntryRegion(const cv::Rect& r)
{
	const cv::Point p(r.x + r.width / 2 - 1, r.y + r.height - 1);
	if (true == entryRect.contains(p))
	{
		return true;
	}
	return false;
}

/**
* @brief WrongTrajectory::isInExitRegion
*
* 判断目标是否在出口区域
* @param[in] r  目标所在区域
* @return bool  如果目标在出口区域内则返回true，否则返回false
*/
bool WrongTrajectory::isInExitRegion(const cv::Rect& r)
{
	const cv::Point p(r.x + r.width / 2 - 1, r.y + r.height - 1);
	if (true == exitRect.contains(p))
	{
		return true;
	}
	return false;
}

/**
* @brief WrongTrajectory::checkAngle
*
* 判断目标运动方向是否与设定方向一致
* @param[in] trajectory  目标运动轨迹
* @return bool  如果目标与设定方向一致则返回true，否则返回false
*/
bool WrongTrajectory::checkAngle(const Trajectory& trajectory)
{
	assert(true == isValid);
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
* @brief WrongTrajectory::detect
*
* 实现了std::list<ID> IDetector::detect ( )
* @return std::list<ID>
*/
list<ID> WrongTrajectory::detect()
{
	const auto tmpIDs(std::move(ids));
	const auto endIDIter = tmpIDs.end();

	const auto tmpEntryIDs(std::move(entryIDs));
	const auto endEntryIDIter = tmpEntryIDs.end();

	std::list<ID> idList;

	const auto& trajectories = TargetManager::instance()->getTrajectory();
	const auto endTargetIter = trajectories.cend();
	for (auto targetIter = trajectories.cbegin()
		; endTargetIter != targetIter
		; ++targetIter)
	{
		const auto id = targetIter->first;

		if (endIDIter != tmpIDs.find(id))
		{
			ids.insert(id);
			entryIDs.insert(id);
			continue;
		}

		const auto& trajectory = targetIter->second;
		const auto lastRect = trajectory.lastRect();

		if (endEntryIDIter != tmpEntryIDs.find(id)
			|| true == isInEntryRegion(lastRect))
		{
			entryIDs.insert(id);
		}
		else
			continue;

		if (false == isInExitRegion(lastRect))
			continue;

		if (false == isValid
			|| (true == isValid && true == checkAngle(trajectory)))
		{
			ids.insert(id);
			idList.push_back(id);
		}
	}
	return idList;
}

cv::Rect WrongTrajectory::getRect()
{
	return rect;
}