#include "Conversion.h"

/**
* @brief Conversion::Conversion
*
* 构造函数
*/
Conversion::Conversion(void) {

}

/**
* @brief Conversion::init
*
* 用于初始化坐标转换矩阵
* @param[in] param 保存转换矩阵的文件名
* @return bool  成功则返回true，否则返回false
*/
bool Conversion::init(const string& param)
{
	cv::FileStorage params(param, cv::FileStorage::Mode::READ);

	if (params["Parameters"].empty())
		return false;
	params["Parameters"] >> m_trans;

	if (params["Offset"].empty())
		return false;
	params["Offset"] >> m_offset;

	params.release();
	return true;
}

/**
* @brief Conversion::run
*
* 坐标转换函数，将单摄像头目标坐标转换为拼接后目标坐标
* @param[in] objects 运动目标集合
* @return bool  成功则返回true，否则返回false
*/
bool Conversion::run(vector<cv::Rect>& objects)
{
	if (objects.empty())
		return false;

	const auto end = objects.end();
	for (auto iter = objects.begin(); end != iter; ++iter)
	{
		cv::Mat tmp = rect2Mat(*iter);
		cv::Mat result = m_trans * tmp;
		*iter = mat2Rect(result);
		//add offset.
		iter->x += m_offset.x;
		iter->y += m_offset.y;
	}

	return true;
}

/**
* @brief Conversion::rect2Mat
*
* 将目标由矩形表示转换为点表示
* @param[in] obj 目标矩形
* @return Mat（3*4） 不成功全为1，
* 成功格式如下：
* x1 x2 x3 x4    (x1, y1):左上
* y1 y2 y3 y4    (x2, y2):右上
*  1  1  1  1    (x3, y3):左下
*                (x4, y4):右下
*/
cv::Mat Conversion::rect2Mat(const cv::Rect& obj)
{
	cv::Mat result = cv::Mat::ones(3, 4, CV_64F);

	//put (x, y) in the right position of result.
	for (int c = 0; c < result.cols; ++c) {
		result.at<double>(0, c) = obj.x + (c % 2 ? obj.width : 0);
		result.at<double>(1, c) = obj.y + (c / 2 ? obj.height : 0);
	}

	return result;
}

/**
* @brief Conversion::mat2Rect
*
* 将目标由点表示转换为矩形表示
* @param[in] obj 目标点（3*4）格式如下：
* x1 x2 x3 x4    (x1, y1):左上
* y1 y2 y3 y4    (x2, y2):右上
*  1  1  1  1    (x3, y3):左下
*                (x4, y4):右下
* @return Rect 不成功全为0
*/
cv::Rect Conversion::mat2Rect(const cv::Mat& obj) {
	vector<cv::Point> points;
	for (int c = 0; c < obj.cols; ++c) {
		points.push_back(
			cv::Point(obj.at<double>(0, c) / obj.at<double>(2, c),
			obj.at<double>(1, c) / obj.at<double>(2, c)));
	}

	//minimal external rect.
	cv::Rect result = cv::boundingRect(points);

	return result;
}