#ifndef TRACKER_MANAGER_H
#define TRACKER_MANAGER_H

#include <map>
using std::map;
using std::pair;

#include "Global.h"
#include "Singleton.h"
#include "KalmanFilterTracker.h"

/**
* @class TrackerManager
* @brief
*
* �������еĿ�����������
*/
class TrackerManager : public Singleton<TrackerManager>
{
	friend class Singleton<TrackerManager>;
	friend class TargetManager;

public:
	void predict(vector< pair<ID, cv::Rect> >& predictedRects);
	void correct(const vector< pair<ID, cv::Rect> >& matchedRects
		, map<ID, cv::Rect>& correctedRects);

private:
	TrackerManager(void);
	void addTracker(ID id, const cv::Rect& rect);
	void removeTrackers(const vector<ID>& lostTargets);
	void removeAllTracker(void);
	void release(void);

private:
	map<ID, KalmanFilterTracker> trackers;	/**< ������������ */
	map<ID, size_t> lostTimes;				/**< Ŀ��ID�Ķ�ʧ���� */
};

#endif // TRACKER_MANAGER_H