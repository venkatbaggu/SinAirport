#include "Invade.h"
#include "Global.h"
#include "TargetManager.h"

/**
* @brief Constructor for InvadeDetector::InvadeDetector.
*
* 构造函数
* @param[in] ri  告警规则ID
* @param[in] vertexes  多边形区域顶点
*/
Invade::Invade(const RuleID ri, const vector<cv::Point>& vertexes) :
IWarning(RuleType::InvadeType, ri)
{
	initMask(mask, vertexes);
	auto inRects(std::move(contoursToRects(mask)));
	assert(1 == inRects.size());
	rect = inRects.front();
}

/****************************************************************************/
//*	@implements	void Invade::boundingRect() const
/****************************************************************************/

/**
* @brief Invade::boundingRect
*
* 实现了void Invade::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect Invade::boundingRect() const
{
	return rect;
}

/**
* @brief Invade::detect
*
* 实现了std::list<ID> IDetector::detect ( )
* @return std::list<ID>
*/
list<ID> Invade::detect()
{
	const auto tmp(std::move(ids));

	std::list<ID> idList;

	const auto endIDIter = tmp.end();

	auto& trajectories = TargetManager::instance()->getTrajectory();
	const auto endTargetIter = trajectories.cend();
	for (auto targetIter = trajectories.cbegin()
		; endTargetIter != targetIter
		; ++targetIter)
	{
		const auto id = targetIter->first;
		if (false == isInRegion(targetIter->second.lastRect()))
			continue;
		if (endIDIter == tmp.find(id))
		{
			idList.push_back(id);
		}
		ids.insert(id);
	}
	return idList;
}

/**
* @brief Invade::isInRegion
*
* 判断该轨迹是否在监控区域内
* @param[in] r  目标所在区域
* @return bool  如果目标在区域内则返回true，否则返回false
*/
bool Invade::isInRegion(const cv::Rect& r) const
{
	const cv::Point p(r.x + r.width / 2 - 1, r.y + r.height - 1);
	if (true == rect.contains(p))
	{
		return true;
	}
	return false;
}

cv::Rect Invade::getRect()
{
	return rect;
}

