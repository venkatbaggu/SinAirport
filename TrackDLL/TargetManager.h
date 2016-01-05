#ifndef TARGET_MANAGER_H
#define TARGET_MANAGER_H

#include <map>
using std::map;
using std::pair;

#include "DataStructure.h"
#include "Singleton.h"
#include "Trajectory.h"

/**
* @class TargetManager
* @brief
*
* 负责管理目标
*/
class TargetManager : public Singleton<TargetManager>
{
	friend class Singleton<TargetManager>;
public:
	const Trajectory& getTrajectory(const ID id);
	const map<ID, Trajectory>& getTrajectory();

	bool contains(const ID id) const;

	void append(const map<ID, cv::Rect>& fragments);
	void getLastFragments(vector< pair<ID, cv::Rect> >& fragments) const;

	void addTarget(const vector<cv::Rect>& rects);
	void removeTarget(const vector<ID>& ids);
	void removeAllTarget(void);
	void updateTemplates(const vector< pair<ID, vector<cv::Rect> > >& splitTargets
		, const vector< pair<cv::Rect, vector<ID> > >& occludedTargets);

	const cv::Mat getTemplate(const ID id) const;
	void release(void);

private:
	TargetManager(void);

	void getSplitIds(const vector< pair<ID, vector<cv::Rect> > >& splitTargets
		, vector<ID>& splitIds) const;
	void getOccludedIds(const vector< pair<cv::Rect, vector<ID> > >& occludedTargets
		, vector<ID>& occludedIds) const;

	void getTemplates(map<ID, cv::Mat>& templatesClone, const vector<ID>& ids);

private:
	map<ID, Trajectory> trajectories;			/**< 目标轨迹 */
	map<ID, cv::Mat> templates;						/**< 目标模板 */
	vector< pair<cv::Rect, size_t> > probTargets;	/**< 可能的新目标 */
};

#endif // TARGET_MANAGER_H


