#include "TemplateMatch.h"
#include "TargetManager.h"
#include "Global.h"

/**
* @brief TemplateMatch::boundingRect
*
* �����������Ӿ���
* @param[in] rects  ��Ҫ������Ӿ��ε�����
* @return const cv::Rect  ��Ӿ���
*/
const cv::Rect TemplateMatch::boundingRect(const vector<cv::Rect>& rects) const
{
	const size_t size = rects.size();
	assert(0 != size);

	cv::Rect rect = rects[0];
	for (size_t i = 1; size != i; ++i)
	{
		rect |= rects[i];
	}
	return rect;
}

/**
* @brief TemplateMatch::matchedRect
*
* Ѱ����ģ��ƥ�������
* @param[in] id  ��ҪѰ��ƥ�������Ŀ��ID
* @param[in] rect  Ŀ��ID���ڵ�����
* @return const cv::Rect  ��Ŀ��IDƥ�������
*/
const cv::Rect TemplateMatch::matchedRect(const ID id, cv::Rect rect) const
{
	const cv::Mat& srcTemplate = TargetManager::instance()->getTemplate(id);

	cv::Mat tpl;

	bool isWidthSmall = rect.width <= srcTemplate.cols;
	bool isHeightSmall = rect.height <= srcTemplate.rows;

	if (isWidthSmall && isHeightSmall)
	{
		return rect;
	}
	if (isWidthSmall || isHeightSmall)
	{
		cv::Size dSize(0, 0);
		if (isWidthSmall)
		{
			double ratio = static_cast<double>(rect.width) / srcTemplate.cols;
			dSize.width = rect.width;
			dSize.height = static_cast<int>(srcTemplate.rows * ratio) - 1;
			if (dSize.height <= 0)
				dSize.height = 1;
		}
		else
		{
			double ratio = static_cast<double>(rect.height) / srcTemplate.rows;
			dSize.height = rect.height;
			dSize.width = static_cast<int>(srcTemplate.cols * ratio) - 1;
			if (dSize.width <= 0)
				dSize.width = 1;
		}
		resize(srcTemplate, tpl, dSize);
	}
	else
		tpl = srcTemplate;

	const cv::Mat src = gCurFrame(rect);

	double maxValue;
	cv::Point maxPoint;
	cv::Mat result;

	cv::matchTemplate(src, tpl, result, CV_TM_CCOEFF);
	cv::minMaxLoc(result, nullptr, &maxValue, nullptr, &maxPoint);

	return cv::Rect(maxPoint.x + rect.x, maxPoint.y + rect.y, tpl.cols, tpl.rows)
		& gWholeRect;
}

/**
* @brief TemplateMatch::match
*
* Ѱ������ѵ�Ŀ��ƥ�������
* @param[in] splitTargets  ���ѵ�Ŀ��
* @param[in] matchedTargets  ����ѵ�Ŀ��ƥ�������
* @return void
*/
void TemplateMatch::match(const vector< pair<ID, vector<cv::Rect> > >& splitTargets
	, vector< pair<ID, cv::Rect> >& matchedTargets) const
{
	const size_t size = splitTargets.size();
	for (size_t i = 0; size != i; ++i)
	{
		const ID id = splitTargets[i].first;
		matchedTargets.push_back(
			std::make_pair(id, matchedRect(id, boundingRect(splitTargets[i].second))));
	}
}

/**
* @brief TemplateMatch::match
*
* Ѱ�����ڵ���Ŀ��ƥ�������
* @param[in] occludedTarget  �ڵ���Ŀ��
* @param[in] matchedTargets  ���ڵ���Ŀ��ƥ�������
* @return void
*/
void TemplateMatch::match(const vector< pair<cv::Rect, vector<ID> > >& occludedTarget
	, vector< pair<ID, cv::Rect> >& matchedTargets)const
{
	cv::Mat result;
	cv::Point maxPoint;

	const size_t size = occludedTarget.size();
	for (size_t s = 0; size != s; ++s)
	{
		const vector<ID>& ids = occludedTarget[s].second;
		const cv::Rect rect = occludedTarget[s].first;
		const size_t idSize = ids.size();
		for (size_t i = 0; idSize != i; ++i)
		{
			const ID id = ids[i];
			matchedTargets.push_back(
				std::make_pair(id, matchedRect(id, rect)));
		}
	}
}