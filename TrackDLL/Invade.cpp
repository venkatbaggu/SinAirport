#include "Invade.h"
#include "Global.h"
#include "TargetManager.h"

/**
* @brief Constructor for InvadeDetector::InvadeDetector.
*
* ���캯��
* @param[in] ri  �澯����ID
* @param[in] vertexes  ��������򶥵�
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
* ʵ����void Invade::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect Invade::boundingRect() const
{
	return rect;
}

/**
* @brief Invade::detect
*
* ʵ����std::list<ID> IDetector::detect ( )
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
* �жϸù켣�Ƿ��ڼ��������
* @param[in] r  Ŀ����������
* @return bool  ���Ŀ�����������򷵻�true�����򷵻�false
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

