#ifndef DETECT_CONF_H
#define DETECT_CONF_H

#include "DataStructure.h"
#include <opencv.hpp>

/**< 回调函数 */
extern DetectCallback gCallback;

/**< 默认图像区域 */
extern cv::Rect gWholeRect;

/**< 默认容器大小 */
extern size_t gDefaultVectorLength;

/**< 背景建模方式是否使用阴影检测 */
extern bool gShadowDetection;

/**< BackgroundSubtractorMOG2历史帧数 */
extern int gMOG2HistoryLength;

/**< BackgroundSubtractorMOG2阈值 */
extern float gMOG2VarThreshold;

/**< BackgroundSubtractorMOG2背景更新率 */
extern double gMOG2LearningRate;

/**< BackgroundSubtractorMOG2二值化中使用的中值 */
extern unsigned int	gMOG2MidValue;

/**< 目标轮廓面积阈值 */
extern unsigned int gContourAreaThreshold;

/**< 构建监控ROI时的扩展像素数 */
extern unsigned int gExternedPixels;

/**< 前景颜色值 */
extern const unsigned char gForegroundColorValue;

/**< 边界像素宽度 */
extern const unsigned int gEdgePixel;

/**< 视频宽 */
extern int gWidth;

/**< 视频高 */
extern int gHeight;

/**< 缩放比 */
extern int gRatio;

/**< 摄像头编号 */
extern unsigned char gCameraID;

/**< 读取全局参数 */
bool readGlobal(const string& file);

#endif // DETECTCONF_H