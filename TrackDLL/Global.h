#ifndef GLOBAL_H
#define GLOBAL_H

#include "DataStructure.h"

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
extern uint gHaltThreshold;

/**< 确定目标运动方向的帧差 */
extern uint gMoveDirectionFrameDifference;

/**< 角度阈值 */
extern double gAngleThreshold;

/**< 构建监控ROI时的扩展像素数 */
extern uint gExternedPixels;

/**< 显示区域坐标X */
extern int gROIX;

/**< 显示区域坐标Y */
extern int gROIY;

/**< 显示区域坐标宽度 */
extern int gROIWidth;

/**< 显示区域坐标高度 */
extern int gROIHeight;

/**< 显示区域矩形 */
extern Rect gWholeRect;

/**< 前景目标颜色 */
const cv::Scalar gForegroundColor = cv::Scalar(255);

/**< 上一帧图像 */
extern Mat gLastFrame;

/**< 当前帧图像 */
extern Mat gCurFrame;

////////////////////////////////////////////////////////////////////////////
/// 全局辅助函数

/**< 读取算法参数 */
bool readGlobal(const string& file);

/**< 辅助合并函数 */
vector<Rect> merge(const vector<Rect>& base,
						const vector<Rect>& merged);

/**< 检测目标合并 */
vector<Rect> mergeAll(const vector<vector<Rect>>& cur);

/**< 初始化背景掩膜 */
void initMask(Mat& mask, const vector<Point>& points);

/**< 目标轮廓转矩形 */
vector<Rect> contoursToRects(
		const vector<vector<Point> >& contours);

vector<Rect> contoursToRects(const Mat& mask);

/**< 计算目标监督 */
double angle(const double x, const double y);

#endif // GLOBAL_H