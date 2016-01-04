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
* ���Ŀ�����־�����
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
	Rect rect;				/**< �澯�� */
	Mat mask;				/**< �澯����ģ */
	set<ID> ids;			/**< �Ѿ���������Ŀ��ID�б� */
};

#endif // INVADE_H