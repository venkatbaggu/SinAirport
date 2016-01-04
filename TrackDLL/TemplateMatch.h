#ifndef TEMPLATE_MATCH_H
#define TEMPLATE_MATCH_H

#include <utility>
using std::pair;

#include "DataStructure.h"
#include "Singleton.h"

/**
* @class TemplateMatch
* @brief
*
* 负责处理模板匹配
*/
class TemplateMatch : public Singleton<TemplateMatch>
{
	friend class Singleton<TemplateMatch>;

public:
	void match(const vector< pair<ID, vector<Rect> > >& splitTargets
		, vector< pair<ID, Rect> >& matchedTargets) const;

	void match(const vector< pair<Rect, vector<ID> > >& occludedTarget
		, vector< pair<ID, Rect> >& matchedTargets) const;

private:
	const Rect boundingRect(const vector<Rect>& rects) const;
	const Rect matchedRect(const ID id, Rect rect) const;
};

#endif // TEMPLATE_MATCH_H

