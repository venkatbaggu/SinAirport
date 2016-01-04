#ifndef TRACKING_MANAGER_H
#define TRACKING_MANAGER_H

#include <map>
#include <set>
using std::map;
using std::pair;
using std::set;

#include <boost/thread.hpp>

#include "DataStructure.h"
#include "Singleton.h"

/**
* @class TrackingManager
* @brief
*
* 负责整个跟踪过程
*/
class TrackingManager : public Singleton<TrackingManager>
{
	friend class Singleton<TrackingManager>;
	TrackingManager(void);

public:
	void update(vector<Rect>& objs);

	void setRect(const vector<Rect>& rects);

private:
	boost::mutex runMutex;
	vector<Rect> contourRects;
	vector<pair<ID, Rect>> predictedRects;
	vector<ID> lostTargets;
	vector<pair<ID, Rect> > matchedTargets;
	vector<pair<ID, vector<Rect> > > splitTargets;
	vector<pair<Rect, vector<ID> > > occludedTargets;
	vector<Rect> newTargets;
	map<ID, Rect> correctedRects;
	Rect regionDetect;
	unsigned int trajectoryID;
	set<ID> ids;
};

#endif // TRACKING_MANAGER_H
