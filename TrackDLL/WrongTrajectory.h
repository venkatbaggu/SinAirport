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
* 检测错误轨迹
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
	cv::Rect entryRect;			/**< 入口区域 */
	cv::Mat entryMask;			/**< 入口区域掩模 */
	cv::Rect exitRect;			/**< 出口区域 */
	cv::Mat exitMask;			/**< 出口区域掩模 */
	cv::Rect rect;				/**< 告警区 */
	const bool isValid;			/**< 出口区域运动方向是否有效 */
	const double moveAngle;		/**< 出口区域运动方向 */
	set<ID> ids;				/**< 已经报警过的目标ID列表 */
	set<ID> entryIDs;			/**< 经过入口区域的目标ID列表 */
};

#endif // WRONG_TRAJECTORY_H