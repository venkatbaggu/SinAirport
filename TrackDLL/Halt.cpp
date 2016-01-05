#include "Halt.h"
#include "Global.h"
#include "TargetManager.h"

/**
* @brief Constructor for Halt::Halt.
*
* 构造函数
* @param[in] ri   告警规则ID
* @param[in] vertexes  多边形区域顶点
*/
Halt::Halt(const RuleID ri, const vector<cv::Point>& vertexes) :
IWarning(RuleType::HaltType, ri)
{
	initMask(mask, vertexes);
	auto inRects(std::move(contoursToRects(mask)));
	assert(1 == inRects.size());
	rect = inRects.front();
}

/**
* @brief Halt::boundingRect
*
* 实现了void IDetector::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect Halt::boundingRect() const
{
	return rect;
}

/**
* @brief Halt::detect
*
* 实现了std::list<ID> IDetector::detect ( )
* @return std::list<ID>
*/
list<ID> Halt::detect()
{
	const auto tmp(std::move(ids));

	list<ID> idList;

	const auto endIDIter = tmp.end();

	auto& trajectories = TargetManager::instance()->getTrajectory();
	const auto endTargetIter = trajectories.cend();

	for (auto targetIter = trajectories.cbegin()
		; endTargetIter != targetIter
		; ++targetIter)
	{
		if (false == isInRegion(targetIter->second.lastRect()))
			continue;
		const auto id = targetIter->first;
		if (endIDIter == tmp.find(id))
		{
			if (false == detect(targetIter->second))
				continue;
			idList.push_back(id);
		}
		ids.insert(id);
	}
	return idList;
}

/**
* @brief Halt::isInRegion
*
* 判断该轨迹是否在监控区域内
* @param[in] r  目标所在区域
* @return bool  如果目标在区域内则返回true，否则返回false
*/
bool Halt::isInRegion(const cv::Rect& r) const
{
	const cv::Point p(r.x + r.width / 2 - 1, r.y + r.height - 1);
	if (true == rect.contains(p))
	{
		return true;
	}
	return false;
}

/**
* @brief Halt::detect
*
* 检测该轨迹是否发生停机事件
* @param[in] trajectory  需要检测的轨迹
* @return bool  如果该轨迹表明目标发生停机事件则返回true，否则false
* @overload	void HaltDetector::detect()
*/
bool Halt::detect(const Trajectory& trajectory) const
{
	const auto& fragments = trajectory.rects;
	const size_t size = fragments.size();
	if (gHaltFrameCount > size || 0 == size)
	{
		return false;
	}
	const cv::Rect& curRect = fragments[size - 1];

	for (size_t i = 0; i != gHaltFrameCount; ++i)
	{
		const auto& r = fragments[size - 1 - i];
		if (fabs(static_cast<double>(curRect.x - r.x)) > gHaltThreshold ||
			fabs(static_cast<double>(curRect.y - r.y)) > gHaltThreshold ||
			fabs(static_cast<double>(curRect.width - r.width)) > gHaltThreshold ||
			fabs(static_cast<double>(curRect.height - r.height)) > gHaltThreshold)
		{
			return false;
		}
	}
	return true;
}

cv::Rect Halt::getRect()
{
	return rect;
}