#ifndef HALT_H
#define HALT_H

#include <set>
using std::set;

#include "IWarning.h"
#include "Trajectory.h"

/**
* @class Halt
* @brief
*
* 检测目标非法停机
*/
class Halt : public IWarning
{
public:
	Halt(const RuleID ri, const vector<Point>& vertexes);
	virtual const Rect boundingRect() const;
	virtual list<ID> detect();
	virtual Rect getRect();
private:
	bool isInRegion(const Rect& r) const;
	bool detect(const Trajectory& trajectory) const;

private:
	cv::Rect rect;				/**< 告警区 */
	cv::Mat mask;				/**< 告警区掩模 */
	std::set<ID> ids;			/**< 已经报警过的目标ID列表 */
};

#endif // HALT_H