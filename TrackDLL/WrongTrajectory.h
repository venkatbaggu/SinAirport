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
		const vector<cv::Point>& entryVertexes,
		const vector<cv::Point>& exitVertexes,
		const bool iv,
		const int a);
	virtual const cv::Rect boundingRect() const;
	virtual list<ID> detect();
	virtual cv::Rect getRect();

private:
	bool isInEntryRegion(const cv::Rect& r);
	bool isInExitRegion(const cv::Rect& r);
	bool checkAngle(const Trajectory& trajectory);

private:
	cv::Rect entryRect;			/**< ������� */
	cv::Mat entryMask;			/**< ���������ģ */
	cv::Rect exitRect;			/**< �������� */
	cv::Mat exitMask;			/**< ����������ģ */
	cv::Rect rect;				/**< �澯�� */
	const bool isValid;			/**< ���������˶������Ƿ���Ч */
	const double moveAngle;		/**< ���������˶����� */
	set<ID> ids;				/**< �Ѿ���������Ŀ��ID�б� */
	set<ID> entryIDs;			/**< ������������Ŀ��ID�б� */
};

#endif // WRONG_TRAJECTORY_H