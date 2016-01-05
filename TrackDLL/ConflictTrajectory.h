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
* 负责检查轨迹冲突
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
	cv::Rect targetRect;					/**< 运动目标所在区域 */
	cv::Mat targetMask;						/**< 运动目标所在区域掩模 */

	cv::Rect detectRect;					/**< 检测区域 */
	cv::Mat detectMask;						/**< 检测区域掩模 */

	cv::Rect rect;							/**< 告警区 */
	bool isTargetAngleValid;				/**< 目标运动角度是否有效 */
	const int targetAngle;					/**< 目标运动角度 */

	const vector<int> detectAngles;	/**< 检测区目标运动角度 */
	set<ID> lastTargets;				/**< 上一帧的目标ID */
	set<ID> lastIllegalTargets;		/**< 上一帧的违规目标ID */
};

#endif // CONFLICT_TRAJECTORY_H