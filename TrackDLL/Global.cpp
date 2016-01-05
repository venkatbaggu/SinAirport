#include "Global.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

////////////////////////////////////////////////////////////////////////////
/// 全局算法参数
/**< 回调函数指针 */
TrackCallback gCallback = nullptr;

/**< 绘制句柄 */
void* gDC = nullptr;

/**< 默认容器大小 */
size_t gDefaultVectorLength = 64;

/**< 疑似目标面积过滤阈值 */
double gUncertainTargetAreaRatioThreshold = 0.1;

/**< 疑似目标时间过滤阈值 */
size_t gUncertainTargetFramesCount = 3;

/**< 目标被认为停机的帧数 */
size_t gHaltFrameCount = 15;

/**< 目标被认为停机的距离阈值 */
uint gHaltThreshold = 1;

/**< 确定目标运动方向的帧差 */
uint gMoveDirectionFrameDifference = 25;

/**< 角度阈值 */
double gAngleThreshold = 30;

/**< 构建监控ROI时的扩展像素数 */
uint gExternedPixels = 80;

/**< 显示区域坐标X */
int gROIX = 0;

/**< 显示区域坐标Y */
int gROIY = 0;

/**< 显示区域坐标宽度 */
int gROIWidth = 5000;

/**< 显示区域坐标高度 */
int gROIHeight = 1080;

/**< 显示区域矩形 */
cv::Rect gWholeRect;

cv::Mat gLastFrame = cv::Mat::zeros(5000, 5000, CV_8U);

cv::Mat gCurFrame = cv::Mat::zeros(5000, 5000, CV_8U);

////////////////////////////////////////////////////////////////////////////
/// 全局辅助函数
/**< 读取算法参数 */
bool readGlobal(const std::string& file) {

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

/**< 辅助合并函数 */
std::vector<cv::Rect> merge(const std::vector<cv::Rect>& base,
	const std::vector<cv::Rect>& merged)
{
	if (base.empty()) {
		return merged;
	}
	else if (merged.empty()) {
		return base;
	}
	else {
		std::vector<cv::Rect> result(base);

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

/**< 检测目标合并 */
std::vector<cv::Rect> mergeAll(const std::vector<std::vector<cv::Rect>>& cur)
{
	std::vector<cv::Rect> result;

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

/**< 计算目标角度 */
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

/**< 目标轮廓转矩形 */
std::vector<cv::Rect> contoursToRects(
	const std::vector<std::vector<cv::Point>>& contours)
{
	std::vector<cv::Rect> contourRects;
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

std::vector<cv::Rect> contoursToRects(const cv::Mat& mask)
{
	std::vector< std::vector<cv::Point> > contours;
	cv::Mat tmp = mask.clone();

	cv::findContours(tmp, contours, 
		CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	return contoursToRects(contours);
}

/**< 初始化背景掩膜 */
void initMask(cv::Mat& mask, const std::vector<cv::Point>& points)
{
	mask = cv::Mat::zeros(gROIHeight, gROIWidth, CV_8U);
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(points);
	cv::drawContours(mask,
		contours,
		-1,
		gForegroundColor,
		CV_FILLED, CV_AA);
}