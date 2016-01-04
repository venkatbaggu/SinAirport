#ifndef WRONG_TRAJECTORY_H
#define WRONG_TRAJECTORY_H

#include <set>
using std::set;

#include "IWarning.h"
#include "Trajectory.h"

/**
* @class WrongTrajectory
* @brief
*
* ������켣
*/
class WrongTrajectory : public IWarning
{
public:
	WrongTrajectory(const RuleID ri,
		const vector<Point>& entryVertexes,
		const vector<Point>& exitVertexes,
		const bool iv,
		const int a);
	virtual const Rect boundingRect() const;
	virtual list<ID> detect();
	virtual Rect getRect();

private:
	bool isInEntryRegion(const Rect& r);
	bool isInExitRegion(const Rect& r);
	bool checkAngle(const Trajectory& trajectory);

private:
	Rect entryRect;			/**< ������� */
	Mat entryMask;			/**< ���������ģ */
	Rect exitRect;			/**< �������� */
	Mat exitMask;			/**< ����������ģ */
	Rect rect;				/**< �澯�� */
	const bool isValid;			/**< ���������˶������Ƿ���Ч */
	const double moveAngle;		/**< ���������˶����� */
	set<ID> ids;				/**< �Ѿ���������Ŀ��ID�б� */
	set<ID> entryIDs;			/**< ������������Ŀ��ID�б� */
};

#endif // WRONG_TRAJECTORY_H