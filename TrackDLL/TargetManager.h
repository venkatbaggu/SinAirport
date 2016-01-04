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
* �������Ŀ��
*/
class TargetManager : public Singleton<TargetManager>
{
	friend class Singleton<TargetManager>;
public:
	const Trajectory& getTrajectory(const ID id);
	const map<ID, Trajectory>& getTrajectory();

	bool contains(const ID id) const;

	void append(const map<ID, Rect>& fragments);
	void getLastFragments(vector< pair<ID, Rect> >& fragments) const;

	void addTarget(const vector<cv::Rect>& rects);
	void removeTarget(const vector<ID>& ids);
	void removeAllTarget(void);
	void updateTemplates(const vector< pair<ID, vector<Rect> > >& splitTargets
		, const vector< pair<Rect, vector<ID> > >& occludedTargets);

	const Mat getTemplate(const ID id) const;
	void release(void);

private:
	TargetManager(void);

	void getSplitIds(const vector< pair<ID, vector<Rect> > >& splitTargets
		, vector<ID>& splitIds) const;
	void getOccludedIds(const vector< pair<Rect, vector<ID> > >& occludedTargets
		, vector<ID>& occludedIds) const;

	void getTemplates(map<ID, Mat>& templatesClone, const vector<ID>& ids);

private:
	map<ID, Trajectory> trajectories;			/**< Ŀ��켣 */
	map<ID, Mat> templates;						/**< Ŀ��ģ�� */
	vector< pair<Rect, size_t> > probTargets;	/**< ���ܵ���Ŀ�� */
};

#endif // TARGET_MANAGER_H


