#include "Halt.h"
#include "Global.h"
#include "TargetManager.h"

/**
* @brief Constructor for Halt::Halt.
*
* ���캯��
* @param[in] ri   �澯����ID
* @param[in] vertexes  ��������򶥵�
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
* ʵ����void IDetector::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect Halt::boundingRect() const
{
	return rect;
}

/**
* @brief Halt::detect
*
* ʵ����std::list<ID> IDetector::detect ( )
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
* �жϸù켣�Ƿ��ڼ��������
* @param[in] r  Ŀ����������
* @return bool  ���Ŀ�����������򷵻�true�����򷵻�false
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
* ���ù켣�Ƿ���ͣ���¼�
* @param[in] trajectory  ��Ҫ���Ĺ켣
* @return bool  ����ù켣����Ŀ�귢��ͣ���¼��򷵻�true������false
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