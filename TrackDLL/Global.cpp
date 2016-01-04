#include "Global.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

////////////////////////////////////////////////////////////////////////////
/// ȫ���㷨����
/**< �ص�����ָ�� */
TrackCallback gCallback = nullptr;

/**< ���ƾ�� */
void* gDC = nullptr;

/**< Ĭ��������С */
size_t gDefaultVectorLength = 64;

/**< ����Ŀ�����������ֵ */
double gUncertainTargetAreaRatioThreshold = 0.1;

/**< ����Ŀ��ʱ�������ֵ */
size_t gUncertainTargetFramesCount = 3;

/**< Ŀ�걻��Ϊͣ����֡�� */
size_t gHaltFrameCount = 15;

/**< Ŀ�걻��Ϊͣ���ľ�����ֵ */
uint gHaltThreshold = 1;

/**< ȷ��Ŀ���˶������֡�� */
uint gMoveDirectionFrameDifference = 25;

/**< �Ƕ���ֵ */
double gAngleThreshold = 30;

/**< �������ROIʱ����չ������ */
uint gExternedPixels = 80;

/**< ��ʾ��������X */
int gROIX = 0;

/**< ��ʾ��������Y */
int gROIY = 0;

/**< ��ʾ���������� */
int gROIWidth = 5000;

/**< ��ʾ��������߶� */
int gROIHeight = 1080;

/**< ��ʾ������� */
Rect gWholeRect;

Mat gLastFrame = Mat::zeros(5000, 5000, CV_8U);

Mat gCurFrame = Mat::zeros(5000, 5000, CV_8U);

////////////////////////////////////////////////////////////////////////////
/// ȫ�ָ�������
/**< ��ȡ�㷨���� */
bool readGlobal(const string& file) {

	if (false == boost::filesystem::exists(file)) {
		return false;
	}

	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(file, pt);

	gUncertainTargetAreaRatioThreshold =
		pt.get("Algorithm.UncertainTargetAreaRationThreshold=0.1", 
								gUncertainTargetAreaRatioThreshold);

	gUncertainTargetFramesCount =
		pt.get("Algorithm.gUncertainTargetFramesCount", 
								gUncertainTargetFramesCount);

	gMoveDirectionFrameDifference =
		pt.get("Algorithm.MoveDirectionFrameDifference", 
								gMoveDirectionFrameDifference);

	gExternedPixels = pt.get("Algorithm.ExternedPixels", gExternedPixels);

	gROIX = pt.get("Video.ImageX", gROIX);

	gROIY = pt.get("Video.ImageY", gROIY);

	gROIWidth = pt.get("Video.ImageWidth", gROIWidth);

	gROIHeight = pt.get("Video.ImageHeight", gROIHeight);

	gDefaultVectorLength =
		pt.get("Optimization.VectorLength", gDefaultVectorLength);

	gWholeRect = cv::Rect(0, 0, gROIWidth, gROIHeight);

	return true;
}

/**< �����ϲ����� */
vector<cv::Rect> merge(const vector<cv::Rect>& base,
	const vector<cv::Rect>& merged)
{
	if (base.empty()) {
		return merged;
	}
	else if (merged.empty()) {
		return base;
	}
	else {
		vector<cv::Rect> result(base);

		for (auto cur = merged.begin(); merged.end() != cur; ++cur) {
			bool find = false;

			for (auto iter = result.begin(); result.end() != iter; ++iter) {
				if ((*cur & *iter).area() > 0) {
					*iter |= *cur;
					find = true;
					break;
				}
			}

			if (!find)
				result.push_back(*cur);
		}

		return result;
	}
}

/**< ���Ŀ��ϲ� */
vector<cv::Rect> mergeAll(const vector<vector<cv::Rect>>& cur)
{
	vector<cv::Rect> result;

	if (cur.size() == 3) {
		if (cur[1].empty()) {
			result.insert(result.begin(), cur[0].begin(), cur[0].end());
			result.insert(result.begin(), cur[2].begin(), cur[2].end());
		}
		else {
			result = merge(merge(cur[1], cur[0]), cur[2]);
		}
	}

	return result;
}

/**< ����Ŀ��Ƕ� */
double angle(const double x, const double y)
{
	static const double PI = 3.14159265;
	if (0 == x)
	{
		return y > 0 ? 90 : 270;
	}

	double a = atan(y / x) * 180.0 / PI;
	if (x < 0)
		a += 180;
	else if (x > 0 && y < 0)
		a += 360;
	return a;
}

/**< Ŀ������ת���� */
vector<Rect> contoursToRects(
		const vector<vector<Point>>& contours)
{
	vector<Rect> contourRects;
	contourRects.reserve(contours.size());
	const auto endIter = contours.cend();
	for (auto iter = contours.cbegin();
		endIter != iter;
		++iter)
	{
		contourRects.push_back(boundingRect(*iter) & gWholeRect);
	}
	return contourRects;
}

vector<Rect> contoursToRects(const Mat& mask)
{
	vector< vector<Point> > contours;
	Mat tmp = mask.clone();

	cv::findContours(tmp, contours, 
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	return contoursToRects(contours);
}

/**< ��ʼ��������Ĥ */
void initMask(Mat& mask, const vector<Point>& points)
{
	mask = Mat::zeros(gROIHeight, gROIWidth, CV_8U);
	vector<vector<Point>> contours;
	contours.push_back(points);
	cv::drawContours(mask,
		contours,
		-1,
		gForegroundColor,
		CV_FILLED, CV_AA);
}