#include "Conversion.h"

/**
* @brief Conversion::Conversion
*
* ���캯��
*/
Conversion::Conversion(void) {

}

/**
* @brief Conversion::init
*
* ���ڳ�ʼ������ת������
* @param[in] param ����ת��������ļ���
* @return bool  �ɹ��򷵻�true�����򷵻�false
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
* ����ת����������������ͷĿ������ת��Ϊƴ�Ӻ�Ŀ������
* @param[in] objects �˶�Ŀ�꼯��
* @return bool  �ɹ��򷵻�true�����򷵻�false
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
* ��Ŀ���ɾ��α�ʾת��Ϊ���ʾ
* @param[in] obj Ŀ�����
* @return Mat��3*4�� ���ɹ�ȫΪ1��
* �ɹ���ʽ���£�
* x1 x2 x3 x4    (x1, y1):����
* y1 y2 y3 y4    (x2, y2):����
*  1  1  1  1    (x3, y3):����
*                (x4, y4):����
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
* ��Ŀ���ɵ��ʾת��Ϊ���α�ʾ
* @param[in] obj Ŀ��㣨3*4����ʽ���£�
* x1 x2 x3 x4    (x1, y1):����
* y1 y2 y3 y4    (x2, y2):����
*  1  1  1  1    (x3, y3):����
*                (x4, y4):����
* @return Rect ���ɹ�ȫΪ0
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