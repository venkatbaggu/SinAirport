#ifndef CONVERSION_H
#define CONVERSION_H

#include "Singleton.h"
#include "Global.h"
#include <vector>
using std::vector;

/**
* @class Conversion
* @brief
*
* ����ת���࣬���ڼ��Ŀ�������ת��
*/
class Conversion : public Singleton<Conversion>
{
	friend class Singleton<Conversion>;
	Conversion(void);

public:
	bool init(const string& param);
	bool run(vector<cv::Rect>& objects);

private:
	cv::Mat m_trans;
	cv::Point m_offset;

private:
	cv::Mat rect2Mat(const cv::Rect& rect);
	cv::Rect mat2Rect(const cv::Mat& mat);
};

#endif // CONVERSION_H

