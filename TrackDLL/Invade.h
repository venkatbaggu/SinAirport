#ifndef INVADE_H
#define INVADE_H

#include <set>
using std::set;

#include "IWarning.h"
#include "Trajectory.h"

/**
* @class Invade
* @brief
*
* 检测目标入侵警戒区
*/
class Invade : public IWarning
{
public:
	Invade(const RuleID ri, const vector<Point>& vertexes);
	virtual const Rect boundingRect() const;
	virtual list<ID> detect();
	virtual Rect getRect();

private:
	bool isInRegion(const Rect& r) const;

private:
	Rect rect;				/**< 告警区 */
	Mat mask;				/**< 告警区掩模 */
	set<ID> ids;			/**< 已经报警过的目标ID列表 */
};

#endif // INVADE_H