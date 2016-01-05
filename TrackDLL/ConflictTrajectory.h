#ifndef CONFLICT_TRAJECTORY_H
#define CONFLICT_TRAJECTORY_H

#include <set>
using std::set;

#include "IWarning.h"
#include "Trajectory.h"

/**
* @class ConflictTrajectory
* @brief
*
* ������켣��ͻ
*/
class ConflictTrajectory : public IWarning
{
public:
	ConflictTrajectory(const RuleID ri,
		const vector<cv::Point>& targetVertexes,
		const vector<cv::Point>& detectVertexes,
		const bool itv,
		const int ta,
		const vector<int>& das);
	virtual const cv::Rect boundingRect() const;
	virtual list<ID> detect();
	virtual cv::Rect getRect();

private:
	bool isInTargetRegion(const cv::Rect& r) const;
	bool isInDetectRegion(const cv::Rect& r) const;

	set<ID> getTargets();
	set<ID> getIllegalTargets();

	bool checkAngle(const Trajectory& trajectory
		, const int moveAngle);

private:
	cv::Rect targetRect;					/**< �˶�Ŀ���������� */
	cv::Mat targetMask;						/**< �˶�Ŀ������������ģ */

	cv::Rect detectRect;					/**< ������� */
	cv::Mat detectMask;						/**< ���������ģ */

	cv::Rect rect;							/**< �澯�� */
	bool isTargetAngleValid;				/**< Ŀ���˶��Ƕ��Ƿ���Ч */
	const int targetAngle;					/**< Ŀ���˶��Ƕ� */

	const vector<int> detectAngles;	/**< �����Ŀ���˶��Ƕ� */
	set<ID> lastTargets;				/**< ��һ֡��Ŀ��ID */
	set<ID> lastIllegalTargets;		/**< ��һ֡��Υ��Ŀ��ID */
};

#endif // CONFLICT_TRAJECTORY_H