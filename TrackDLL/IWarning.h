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
	* @param[in] rt Defaults to None.  �澯����
	* @param[in] ri Defaults to rt. �澯����ID
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
	* ��ȡ������Ϣ
	* @return const RuleInfo  ������Ϣ
	*/
	const RuleInfo getRuleInfo()
	{
		return ruleInfo;
	}

	/**
	* @brief boundingRect
	*
	* ��ȡ�ù�������ص�������Ӿ���
	* @return const cv::Rect  �ù�������ص�������Ӿ���
	*/
	virtual const Rect boundingRect() const = 0;

	/**
	* @brief
	*
	* ��⾯��
	* @return std::list<ID> detect  �澯Ŀ���б�
	* @warning	���������ʵ�ָú���
	*/
	virtual list<ID> detect() = 0;

	virtual cv::Rect getRect() = 0;

	/**
	* @brief Destructor for IDetector.
	*
	* ����������
	*/
	virtual ~IWarning(){};

protected:
	const RuleInfo ruleInfo;
};

#endif // IWARNING_H