#ifndef TRACKING_MANAGER_H
#define TRACKING_MANAGER_H

#include "DataStructure.h"
#include "Singleton.h"

#include <map>
#include <set>
#include <vector>
using std::map;
using std::pair;
using std::set;
using std::vector;

#include <boost/thread.hpp>

#include <opencv.hpp>

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
	void update(vector<cv::Rect>& objs);

	void setRect(const vector<cv::Rect>& rects);

private:
	boost::mutex runMutex;
	vector<cv::Rect> contourRects;
	vector<pair<ID, cv::Rect>> predictedRects;
	vector<ID> lostTargets;
	vector<pair<ID, cv::Rect> > matchedTargets;
	vector<pair<ID, vector<cv::Rect> > > splitTargets;
	vector<pair<cv::Rect, vector<ID> > > occludedTargets;
	vector<cv::Rect> newTargets;
	map<ID, cv::Rect> correctedRects;
	cv::Rect regionDetect;
	unsigned int trajectoryID;
	set<ID> ids;
};

#endif // TRACKING_MANAGER_H
