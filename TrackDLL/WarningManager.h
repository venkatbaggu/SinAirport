#ifndef WARNING_MANAGER_H
#define WARNING_MANAGER_H

#include <map>
#include <set>
#include <list>
#include <vector>
#include <memory>
using std::map;
using std::set;
using std::list;
using std::unique_ptr;
using std::vector;

#include <boost/thread.hpp>

#include "IWarning.h"
#include "Singleton.h"
#include "DataStructure.h"

class WarningManager : public Singleton<WarningManager>
{
	friend class Singleton<WarningManager>;
	WarningManager(void);

public:
	void reset(list<unique_ptr<IWarning>>& dts);
	vector<cv::Rect> boundingRects() const;
	void warning(map<RuleInfo, set<ID>>& warnigs);
	vector<cv::Rect> getRects();
	/*void add(list<unique_ptr<IWarning>>& dts);*/

	void release();

private:
	boost::mutex warningMutex;
	std::list<unique_ptr<IWarning>> detectors;	/**< ¾¯¸æ¼ì²âÆ÷ÁÐ±í */
};

#endif // WARNING_MANAGER_H