#include "WrongTrajectory.h"
#include "TargetManager.h"
#include "Global.h"

/**
* @brief Constructor for WrongTrajectory::WrongTrajectory.
*
* ���캯��
* @param[in] ri  ����ID
* @param[in] entryVertexes  ����������ζ���
* @param[in] exitVertexes  �����������ζ���
* @param[in] iv  Ŀ���˶��Ƕ��Ƿ���Ч
* @param[in] a   Ŀ���˶��Ƕ�
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
* ʵ����void IDetector::boundingRect() const
* @return const cv::Rect
*/
const cv::Rect WrongTrajectory::boundingRect() const
{
	return rect;
}

/**
* @brief WrongTrajectory::isInEntryRegion
*
* �ж�Ŀ���Ƿ����������
* @param[in] r  Ŀ����������
* @return bool  ���Ŀ��������������򷵻�true�����򷵻�false
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
* �ж�Ŀ���Ƿ��ڳ�������
* @param[in] r  Ŀ����������
* @return bool  ���Ŀ���ڳ����������򷵻�true�����򷵻�false
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
* �ж�Ŀ���˶������Ƿ����趨����һ��
* @param[in] trajectory  Ŀ���˶��켣
* @return bool  ���Ŀ�����趨����һ���򷵻�true�����򷵻�false
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
* ʵ����std::list<ID> IDetector::detect ( )
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