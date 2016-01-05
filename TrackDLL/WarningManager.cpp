#include "WarningManager.h"
#include "TrackingManager.h"

/**
* @brief Constructor for WarningManager::WarningManager.
*
* ���캯��
*/
WarningManager::WarningManager()
{
}

/**
* @brief WarningManager::reset
*
* ����µľ�������
* @param[in] dts  �µľ�������
* @warning	��������������new�ڶ�������
*/
void WarningManager::reset(list<unique_ptr<IWarning>>& dts)
{
	boost::mutex::scoped_lock sl(warningMutex);
	detectors = std::move(dts);

	TrackingManager::instance()->setRect(std::move(boundingRects()));
}

/**
* @brief WarningManager::release
*
* ������еľ�������
*/
void WarningManager::release()
{
	boost::mutex::scoped_lock sl(warningMutex);
	detectors.clear();
}

/**
* @brief WarningManager::boundingRects
*
* ��ȡ���й���ļ������
* @return vector<cv::Rect>  ���й���ļ������
*/
vector<cv::Rect> WarningManager::boundingRects() const
{
	vector<cv::Rect> rects;
	const auto endIter = detectors.end();
	for (auto beginIter = detectors.begin();
		endIter != beginIter;
		beginIter++)
	{
		rects.push_back((*beginIter)->boundingRect());
	}
	return rects;
}

/**
* @brief WarningManager::warning
*
* ����Ƿ�������
*/
void WarningManager::warning(map<RuleInfo, set<ID>>& warnigs)
{
	boost::mutex::scoped_lock sl(warningMutex);

	const auto endIter = detectors.end();
	for (auto iter = detectors.begin(); endIter != iter; ++iter)
	{
		std::list<ID> ids(std::move((*iter)->detect()));
		if (false == ids.empty())
		{
			const RuleInfo ruleInfo = (*iter)->getRuleInfo();
			const auto endIdIter = ids.end();
			auto beginIdIter = ids.begin();
			for (; endIdIter != beginIdIter; beginIdIter++)
				warnigs[ruleInfo].insert(*beginIdIter);
		}
	}

	return;
}

//void WarningManager::add(list<unique_ptr<IWarning>>& dts) {
//	boost::mutex::scoped_lock sl(warningMutex);
//
//	const auto end = dts.end();
//	for (auto iter = dts.begin();
//		end != iter; ++iter) {
//		detectors.emplace_back(*iter);
//	}
//
//	return;
//}