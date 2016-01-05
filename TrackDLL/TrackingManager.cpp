#include "TrackingManager.h"

#include <core/cuda.hpp>
#include <cudawarping.hpp>

#include "Global.h"
#include "TargetManager.h"
#include "TemplateMatch.h"
#include "TrackerManager.h"
#include "AffinityMatrix.h"
#include "WarningManager.h"

/**
* @brief TrackingManager::TrackingManager.
*
* 构造函数
* @param[in] void  状态向量维数
*/
TrackingManager::TrackingManager(void):
	regionDetect(600, 100, 1000, 300)
{
	contourRects.reserve(gDefaultVectorLength);
	predictedRects.reserve(gDefaultVectorLength);
	lostTargets.reserve(gDefaultVectorLength);
	matchedTargets.reserve(gDefaultVectorLength);
	splitTargets.reserve(gDefaultVectorLength);
	occludedTargets.reserve(gDefaultVectorLength);
	newTargets.reserve(gDefaultVectorLength);
	trajectoryID = 0;
}

/**
* @brief TrackingManager::update.
*
* 更新当前跟踪结果
* @param[in] objs  当前检测结果
* @return map<ID, vector<cv::Rect>>  所有跟踪目标的ID及历史信息
*/
void TrackingManager::update(vector<cv::Rect>& objs)
{
	static const vector< pair<ID, vector<cv::Rect> > > emptySplitTargets;
	static const vector< pair<cv::Rect, vector<ID> > > emptyOccludedTargets;

	if (!runMutex.try_lock())
	{
		return ;
	}

	TrackerManager::instance()->predict(predictedRects);
	
	//当前检测结果为空
	if (true == objs.empty())
	{
		TargetManager::instance()->removeAllTarget();
		TargetManager::instance()->updateTemplates(emptySplitTargets, emptyOccludedTargets);
	}
	else if (true == predictedRects.empty() && false == objs.empty())
	{
		TargetManager::instance()->removeAllTarget();
		TargetManager::instance()->addTarget(objs);
		TargetManager::instance()->updateTemplates(emptySplitTargets, emptyOccludedTargets);
	}
	else if (false == predictedRects.empty() && false == objs.empty())
	{
		AffinityMatrix::instance()->build(predictedRects, objs);

		// 1. 消失的目标
		lostTargets.clear();
		AffinityMatrix::instance()->lostTarget(lostTargets);
		TargetManager::instance()->removeTarget(lostTargets);

		// 2. 新目标
		newTargets.clear();
		AffinityMatrix::instance()->newTarget(newTargets);

		// 3. 匹配的目标
		matchedTargets.clear();
		AffinityMatrix::instance()->matchedTarget(matchedTargets);

		// 4. 分裂的目标
		//splitTargets.clear();
		//AffinityMatrix::instance()->splitTarget(splitTargets);

		// 5. 遮挡的目标
		//occludedTargets.clear();
		//AffinityMatrix::instance()->occludedTarget(occludedTargets);

		//TargetManager::instance()->updateTemplates(splitTargets, occludedTargets);
		//TemplateMatch::instance()->match(splitTargets, matchedTargets);
		//TemplateMatch::instance()->match(occludedTargets, matchedTargets);
		correctedRects.clear();
		TrackerManager::instance()->correct(matchedTargets, correctedRects);
		TargetManager::instance()->append(correctedRects);

		// 最后添加新目标
		TargetManager::instance()->addTarget(newTargets);
	}


	predictedRects.clear();
	contourRects.clear();
	runMutex.unlock();

	return ;
}

/**
* @brief TrackingManager::setRect
*
* 设置监控区域
* @param[in] rects  区域列表
*/
void TrackingManager::setRect(const vector<cv::Rect>& rects) {
	if (0 == rects.size())
	{
		return;
	}

	cv::Rect rect = rects.front();

	auto iter = rects.begin();
	const auto endIter = rects.end();
	for (; endIter != iter; iter++)
		rect |= *iter;

	rect -= cv::Point(gExternedPixels, gExternedPixels);
	rect += cv::Size(2 * gExternedPixels, 2 * gExternedPixels);

	rect &= gWholeRect;
}