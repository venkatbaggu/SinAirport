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
* ���캯��
* @param[in] void  ״̬����ά��
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
* ���µ�ǰ���ٽ��
* @param[in] objs  ��ǰ�����
* @return map<ID, vector<cv::Rect>>  ���и���Ŀ���ID����ʷ��Ϣ
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
	
	//��ǰ�����Ϊ��
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

		// 1. ��ʧ��Ŀ��
		lostTargets.clear();
		AffinityMatrix::instance()->lostTarget(lostTargets);
		TargetManager::instance()->removeTarget(lostTargets);

		// 2. ��Ŀ��
		newTargets.clear();
		AffinityMatrix::instance()->newTarget(newTargets);

		// 3. ƥ���Ŀ��
		matchedTargets.clear();
		AffinityMatrix::instance()->matchedTarget(matchedTargets);

		// 4. ���ѵ�Ŀ��
		//splitTargets.clear();
		//AffinityMatrix::instance()->splitTarget(splitTargets);

		// 5. �ڵ���Ŀ��
		//occludedTargets.clear();
		//AffinityMatrix::instance()->occludedTarget(occludedTargets);

		//TargetManager::instance()->updateTemplates(splitTargets, occludedTargets);
		//TemplateMatch::instance()->match(splitTargets, matchedTargets);
		//TemplateMatch::instance()->match(occludedTargets, matchedTargets);
		correctedRects.clear();
		TrackerManager::instance()->correct(matchedTargets, correctedRects);
		TargetManager::instance()->append(correctedRects);

		// ��������Ŀ��
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
* ���ü������
* @param[in] rects  �����б�
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