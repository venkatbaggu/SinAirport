#include "Global.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

/**< 回调函数指针 */
DetectCallback gCallback = nullptr;

/**< 默认图像区域 */
cv::Rect gWholeRect(0, 0, 1920, 1080);

/**< 默认容器大小 */
size_t gDefaultVectorLength = 64;

/**< 背景建模方式是否使用阴影检测 */
bool gShadowDetection = false;

/**< BackgroundSubtractorMOG2历史帧数 */
int gMOG2HistoryLength = 500;

/**< BackgroundSubtractorMOG2阈值 */
float gMOG2VarThreshold = 16;

/**< BackgroundSubtractorMOG2背景更新率 */
double gMOG2LearningRate = 0.01;

/**< BackgroundSubtractorMOG2二值化中使用的中值 */
unsigned int gMOG2MidValue = 127;

/**< 目标轮廓面积阈值 */
unsigned int gContourAreaThreshold = 32;

/**< 构建监控ROI时的扩展像素数 */
unsigned int gExternedPixels = 80;

/**< 前景颜色值 */
const unsigned char gForegroundColorValue = 255;

/**< 边界像素宽度 */
const unsigned int gEdgePixel = 10;

/**< 视频宽 */
int gWidth = 1920;

/**< 视频高 */
int gHeight = 1080;

/**< 缩放比 */
int gRatio = 1;

/**< 读取全局参数 */
bool readGlobal(const string& file) {

	if (false == boost::filesystem::exists(file)) {
		return false;
	}

	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(file, pt);

	gShadowDetection =
		pt.get("Algorithm.ShadowDetection", gShadowDetection);

	gMOG2HistoryLength =
		pt.get("Algorithm.MOG2HistoryLength", gMOG2HistoryLength);

	gMOG2VarThreshold =
		pt.get("Algorithm.MOG2VarThreshold", gMOG2VarThreshold);

	gMOG2LearningRate =
		pt.get("Algorithm.MOG2LearningRate", gMOG2LearningRate);

	gMOG2MidValue =
		pt.get("Algorithm.MOG2MidValue", gMOG2MidValue);

	gContourAreaThreshold =
		pt.get("Algorithm.ContourAreaThreshold", gContourAreaThreshold);

	gExternedPixels =
		pt.get("Algorithm.ExternedPixels", gExternedPixels);

	gWidth =
		pt.get("Video.ImageWidth", gWidth);

	gHeight =
		pt.get("Video.ImageHeight", gHeight);

	gRatio =
		pt.get("Video.ImageRatio", gRatio);

	gDefaultVectorLength =
		pt.get("Optimization.VectorLength", gDefaultVectorLength);

	gWholeRect = cv::Rect(gEdgePixel, gEdgePixel,
		gWidth / gRatio - 2 * gEdgePixel,
		gHeight / gRatio - 2 * gEdgePixel);

	return true;
}