#ifndef IWARNING_H
#define IWARNING_H

#include <list>
using std::list;

#include "DataStructure.h"

class IWarning
{
public:
	/**
	* @brief Constructor for IDetector.
	*
	* Detailed description.
	* @param[in] rt Defaults to None.  告警类型
	* @param[in] ri Defaults to rt. 告警规则ID
	*/
	IWarning(const RuleType rt = None, const RuleID ri = -1) :
		ruleInfo(rt, ri)
	{
		assert(None != rt);
		assert(0 < ri);
	}

	/**
	* @brief getRuleInfo
	*
	* 获取规则信息
	* @return const RuleInfo  规则信息
	*/
	const RuleInfo getRuleInfo()
	{
		return ruleInfo;
	}

	/**
	* @brief boundingRect
	*
	* 获取该规则所监控的区域外接矩形
	* @return const cv::Rect  该规则所监控的区域外接矩形
	*/
	virtual const Rect boundingRect() const = 0;

	/**
	* @brief
	*
	* 检测警告
	* @return std::list<ID> detect  告警目标列表
	* @warning	派生类必须实现该函数
	*/
	virtual list<ID> detect() = 0;

	virtual cv::Rect getRect() = 0;

	/**
	* @brief Destructor for IDetector.
	*
	* 虚析构函数
	*/
	virtual ~IWarning(){};

protected:
	const RuleInfo ruleInfo;
};

#endif // IWARNING_H