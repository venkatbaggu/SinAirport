#ifndef DETECT_CONF_H
#define DETECT_CONF_H

#include "DataStructure.h"
#include <opencv.hpp>

/**< �ص����� */
extern DetectCallback gCallback;

/**< Ĭ��ͼ������ */
extern cv::Rect gWholeRect;

/**< Ĭ��������С */
extern size_t gDefaultVectorLength;

/**< ������ģ��ʽ�Ƿ�ʹ����Ӱ��� */
extern bool gShadowDetection;

/**< BackgroundSubtractorMOG2��ʷ֡�� */
extern int gMOG2HistoryLength;

/**< BackgroundSubtractorMOG2��ֵ */
extern float gMOG2VarThreshold;

/**< BackgroundSubtractorMOG2���������� */
extern double gMOG2LearningRate;

/**< BackgroundSubtractorMOG2��ֵ����ʹ�õ���ֵ */
extern unsigned int	gMOG2MidValue;

/**< Ŀ�����������ֵ */
extern unsigned int gContourAreaThreshold;

/**< �������ROIʱ����չ������ */
extern unsigned int gExternedPixels;

/**< ǰ����ɫֵ */
extern const unsigned char gForegroundColorValue;

/**< �߽����ؿ�� */
extern const unsigned int gEdgePixel;

/**< ��Ƶ�� */
extern int gWidth;

/**< ��Ƶ�� */
extern int gHeight;

/**< ���ű� */
extern int gRatio;

/**< ����ͷ��� */
extern unsigned char gCameraID;

/**< ��ȡȫ�ֲ��� */
bool readGlobal(const string& file);

#endif // DETECTCONF_H