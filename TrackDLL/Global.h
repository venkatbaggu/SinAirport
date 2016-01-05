#ifndef GLOBAL_H
#define GLOBAL_H

#include "DataStructure.h"
#include <opencv.hpp>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////
/// ȫ���㷨����
/**< �ص�����ָ�� */
extern TrackCallback gCallback;

/**< ���ƾ�� */
extern void* gDC;

/**< Ĭ��������С */
extern size_t gDefaultVectorLength;

/**< ����Ŀ�����������ֵ */
extern double gUncertainTargetAreaRatioThreshold;

/**< ����Ŀ��ʱ�������ֵ */
extern size_t gUncertainTargetFramesCount;

/**< Ŀ�걻��Ϊͣ����֡�� */
extern size_t gHaltFrameCount;

/**< Ŀ�걻��Ϊͣ���ľ�����ֵ */
extern unsigned int gHaltThreshold;

/**< ȷ��Ŀ���˶������֡�� */
extern unsigned int gMoveDirectionFrameDifference;

/**< �Ƕ���ֵ */
extern double gAngleThreshold;

/**< �������ROIʱ����չ������ */
extern unsigned int gExternedPixels;

/**< ��ʾ��������X */
extern int gROIX;

/**< ��ʾ��������Y */
extern int gROIY;

/**< ��ʾ���������� */
extern int gROIWidth;

/**< ��ʾ��������߶� */
extern int gROIHeight;

/**< ��ʾ������� */
extern cv::Rect gWholeRect;

/**< ǰ��Ŀ����ɫ */
const cv::Scalar gForegroundColor = cv::Scalar(255);

/**< ��һ֡ͼ�� */
extern cv::Mat gLastFrame;

/**< ��ǰ֡ͼ�� */
extern cv::Mat gCurFrame;

////////////////////////////////////////////////////////////////////////////
/// ȫ�ָ�������

/**< ��ȡ�㷨���� */
bool readGlobal(const std::string& file);

/**< �����ϲ����� */
std::vector<cv::Rect> merge(const std::vector<cv::Rect>& base,
						const std::vector<cv::Rect>& merged);

/**< ���Ŀ��ϲ� */
std::vector<cv::Rect> mergeAll(const std::vector<std::vector<cv::Rect>>& cur);

/**< ��ʼ��������Ĥ */
void initMask(cv::Mat& mask, const std::vector<cv::Point>& points);

/**< Ŀ������ת���� */
std::vector<cv::Rect> contoursToRects(
	const std::vector<std::vector<cv::Point> >& contours);

std::vector<cv::Rect> contoursToRects(const cv::Mat& mask);

/**< ����Ŀ��ල */
double angle(const double x, const double y);

#endif // GLOBAL_H