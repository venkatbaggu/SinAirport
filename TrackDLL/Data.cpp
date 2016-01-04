#include "Data.h"

/**
* @brief Data::Data()
*
* 构造函数
*/
Data::Data() :
m_left(1), m_mid(1), m_right(1)
{

}

/**
* @brief Data::writeLeft
*
* @param[in] cur 当前数据
* @return void
*/
void Data::writeLeft(const cv::Mat& cur)
{
	if (!m_left.full()) {
		m_left.push_back(cur);
	} else {
		m_left.back() = cur;
	}

	return;
}

/**
* @brief Data::writeMid
*
* @param[in] cur 当前数据
* @return void
*/
void Data::writeMid(const cv::Mat& cur)
{
	if (!m_mid.full()) {
		m_mid.push_back(cur);
	} else {
		m_mid.back() = cur;
	}

	return;
}

/**
* @brief Data::writeRight
*
* @param[in] cur 当前数据
* @return void
*/
void Data::writeRight(const cv::Mat& cur)
{
	if (!m_right.full()) {
		m_right.push_back(cur);
	} else {
		m_right.back() = cur;
	}

	return;
}

/**
* @brief Data::read
*
* @param[in] void
* @return vector<cv::Rect>
* 当前三帧数据（顺序：左，中，右）
*/
std::vector<cv::Mat> Data::read(void)
{
	std::vector<cv::Mat> result(3);

	if (!m_left.empty()) {
		result[0] = m_left.front();
		if (m_left.size() > 1)
			m_left.pop_front();
	}

	if (!m_mid.empty()) {
		result[1] = m_mid.front();
		if (m_mid.size() > 1)
			m_mid.pop_front();
	}

	if (!m_right.empty()) {
		result[2] = m_right.front();
		if (m_right.size() > 1)
			m_right.pop_front();
	}

	return result;
}
