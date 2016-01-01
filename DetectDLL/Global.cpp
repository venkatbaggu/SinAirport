#include "Global.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

/**< �ص�����ָ�� */
DetectCallback gCallback = nullptr;

/**< Ĭ��ͼ������ */
cv::Rect gWholeRect(0, 0, 1920, 1080);

/**< Ĭ��������С */
size_t gDefaultVectorLength = 64;

/**< ������ģ��ʽ�Ƿ�ʹ����Ӱ��� */
bool gShadowDetection = false;

/**< BackgroundSubtractorMOG2��ʷ֡�� */
int gMOG2HistoryLength = 500;

/**< BackgroundSubtractorMOG2��ֵ */
float gMOG2VarThreshold = 16;

/**< BackgroundSubtractorMOG2���������� */
double gMOG2LearningRate = 0.01;

/**< BackgroundSubtractorMOG2��ֵ����ʹ�õ���ֵ */
unsigned int gMOG2MidValue = 127;

/**< Ŀ�����������ֵ */
unsigned int gContourAreaThreshold = 32;

/**< �������ROIʱ����չ������ */
unsigned int gExternedPixels = 80;

/**< ǰ����ɫֵ */
const unsigned char gForegroundColorValue = 255;

/**< �߽����ؿ�� */
const unsigned int gEdgePixel = 10;

/**< ��Ƶ�� */
int gWidth = 1920;

/**< ��Ƶ�� */
int gHeight = 1080;

/**< ���ű� */
int gRatio = 1;

/**< ��ȡȫ�ֲ��� */
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