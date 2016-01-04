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
		const vector<Point>& targetVertexes,
		const vector<Point>& detectVertexes,
		const bool itv,
		const int ta,
		const vector<int>& das);
	virtual const Rect boundingRect() const;
	virtual list<ID> detect();
	virtual Rect getRect();

private:
	bool isInTargetRegion(const Rect& r) const;
	bool isInDetectRegion(const Rect& r) const;

	set<ID> getTargets();
	set<ID> getIllegalTargets();

	bool checkAngle(const Trajectory& trajectory
		, const int moveAngle);

private:
	Rect targetRect;					/**< �˶�Ŀ���������� */
	Mat targetMask;						/**< �˶�Ŀ������������ģ */

	Rect detectRect;					/**< ������� */
	Mat detectMask;						/**< ���������ģ */

	Rect rect;							/**< �澯�� */
	bool isTargetAngleValid;				/**< Ŀ���˶��Ƕ��Ƿ���Ч */
	const int targetAngle;					/**< Ŀ���˶��Ƕ� */

	const vector<int> detectAngles;	/**< �����Ŀ���˶��Ƕ� */
	set<ID> lastTargets;				/**< ��һ֡��Ŀ��ID */
	set<ID> lastIllegalTargets;		/**< ��һ֡��Υ��Ŀ��ID */
};

#endif // CONFLICT_TRAJECTORY_H