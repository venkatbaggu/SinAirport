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
* ���Ŀ��Ƿ�ͣ��
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
	cv::Rect rect;				/**< �澯�� */
	cv::Mat mask;				/**< �澯����ģ */
	std::set<ID> ids;			/**< �Ѿ���������Ŀ��ID�б� */
};

#endif // HALT_H