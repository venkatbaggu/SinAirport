#ifndef GLOBAL_H
#define GLOBAL_H

#include "DataStructure.h"

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
extern uint gHaltThreshold;

/**< ȷ��Ŀ���˶������֡�� */
extern uint gMoveDirectionFrameDifference;

/**< �Ƕ���ֵ */
extern double gAngleThreshold;

/**< �������ROIʱ����չ������ */
extern uint gExternedPixels;

/**< ��ʾ��������X */
extern int gROIX;

/**< ��ʾ��������Y */
extern int gROIY;

/**< ��ʾ���������� */
extern int gROIWidth;

/**< ��ʾ��������߶� */
extern int gROIHeight;

/**< ��ʾ������� */
extern Rect gWholeRect;

/**< ǰ��Ŀ����ɫ */
const cv::Scalar gForegroundColor = cv::Scalar(255);

/**< ��һ֡ͼ�� */
extern Mat gLastFrame;

/**< ��ǰ֡ͼ�� */
extern Mat gCurFrame;

////////////////////////////////////////////////////////////////////////////
/// ȫ�ָ�������

/**< ��ȡ�㷨���� */
bool readGlobal(const string& file);

/**< �����ϲ����� */
vector<Rect> merge(const vector<Rect>& base,
						const vector<Rect>& merged);

/**< ���Ŀ��ϲ� */
vector<Rect> mergeAll(const vector<vector<Rect>>& cur);

/**< ��ʼ��������Ĥ */
void initMask(Mat& mask, const vector<Point>& points);

/**< Ŀ������ת���� */
vector<Rect> contoursToRects(
		const vector<vector<Point> >& contours);

vector<Rect> contoursToRects(const Mat& mask);

/**< ����Ŀ��ල */
double angle(const double x, const double y);

#endif // GLOBAL_H