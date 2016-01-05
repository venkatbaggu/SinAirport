#ifndef TEMPLATE_MATCH_H
#define TEMPLATE_MATCH_H

#include "DataStructure.h"
#include "Singleton.h"

#include <opencv.hpp>

#include <utility>
#include <vector>
using std::pair;
using std::vector;

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
	void match(const std::vector< pair<ID, vector<cv::Rect> > >& splitTargets
		, vector< pair<ID, cv::Rect> >& matchedTargets) const;

	void match(const std::vector< pair<cv::Rect, vector<ID> > >& occludedTarget
		, vector< pair<ID, cv::Rect> >& matchedTargets) const;

private:
	const cv::Rect boundingRect(const std::vector<cv::Rect>& rects) const;
	const cv::Rect matchedRect(const ID id, cv::Rect rect) const;
};

#endif // TEMPLATE_MATCH_H

