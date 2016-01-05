#ifndef GLOBAL_H
#define GLOBAL_H

#include "DataStructure.h"
#include <opencv.hpp>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////
/// 全局算法参数
/**< 回调函数指针 */
extern TrackCallback gCallback;

/**< 绘制句柄 */
extern void* gDC;

/**< 默认容器大小 */
extern size_t gDefaultVectorLength;

/**< 疑似目标面积过滤阈值 */
extern double gUncertainTargetAreaRatioThreshold;

/**< 疑似目标时间过滤阈值 */
extern size_t gUncertainTargetFramesCount;

/**< 目标被认为停机的帧数 */
extern size_t gHaltFrameCount;

/**< 目标被认为停机的距离阈值 */
extern unsigned int gHaltThreshold;

/**< 确定目标运动方向的帧差 */
extern unsigned int gMoveDirectionFrameDifference;

/**< 角度阈值 */
extern double gAngleThreshold;

/**< 构建监控ROI时的扩展像素数 */
extern unsigned int gExternedPixels;

/**< 显示区域坐标X */
extern int gROIX;

/**< 显示区域坐标Y */
extern int gROIY;

/**< 显示区域坐标宽度 */
extern int gROIWidth;

/**< 显示区域坐标高度 */
extern int gROIHeight;

/**< 显示区域矩形 */
extern cv::Rect gWholeRect;

/**< 前景目标颜色 */
const cv::Scalar gForegroundColor = cv::Scalar(255);

/**< 上一帧图像 */
extern cv::Mat gLastFrame;

/**< 当前帧图像 */
extern cv::Mat gCurFrame;

////////////////////////////////////////////////////////////////////////////
/// 全局辅助函数

/**< 读取算法参数 */
bool readGlobal(const std::string& file);

/**< 辅助合并函数 */
std::vector<cv::Rect> merge(const std::vector<cv::Rect>& base,
						const std::vector<cv::Rect>& merged);

/**< 检测目标合并 */
std::vector<cv::Rect> mergeAll(const std::vector<std::vector<cv::Rect>>& cur);

/**< 初始化背景掩膜 */
void initMask(cv::Mat& mask, const std::vector<cv::Point>& points);

/**< 目标轮廓转矩形 */
std::vector<cv::Rect> contoursToRects(
	const std::vector<std::vector<cv::Point> >& contours);

std::vector<cv::Rect> contoursToRects(const cv::Mat& mask);

/**< 计算目标监督 */
double angle(const double x, const double y);

#endif // GLOBAL_H