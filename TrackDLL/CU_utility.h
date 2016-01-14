/*
* cu_utility.h
*
*  Created on: Nov. 11, 2015
*      Author: GL
*/

#ifndef CU_UTILITY_H_
#define CU_UTILITY_H_

#include "Utility.h"
using cv::Point;
using cv::Size;
using cv::Point2f;
using cv::cuda::GpuMat;

//#define _SIMPLE_FUSION_

#ifdef _SIMPLE_FUSION_
#define MOSAIC_WEIGHTS_CV_TYPE CV_8UC1
#define MOSAIC_WEIGHTS_DATATYPE uchar
#define MOSAIC_WEIGHTS_RATIO 20.0
#else
#define MOSAIC_WEIGHTS_CV_TYPE CV_16UC1
#define MOSAIC_WEIGHTS_DATATYPE ushort
#define MOSAIC_WEIGHTS_RATIO 100.0
#endif

/* ������С���˷�����������*/
CvMat* cuFindAffineMat(const CvPoint2D32f* src,
	const CvPoint2D32f* dst,
	CvMat* map_matrix, int ptnum);

/* �ȼ���任��ĳߴ磬�ٽ��з���任*/
void cuWarpAffine(const IplImage* src,
	IplImage** dst,
	const CvMat* map_matrix,
	CvPoint2D32f bnd_pts[4],
	int flags CV_DEFAULT(CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS));

/* �ȼ���任��ĳߴ磬�ٽ���͸�ӱ任*/
void cuWarpPerspective(const cv::cuda::GpuMat& src,
	cv::cuda::GpuMat& dst,
	const cv::Mat& map_matrix,
	std::vector<cv::Point2f>& bnd_pts,
	cv::Size dst_size);

/* ������С���˷�����ͶӰ�任����*/
CvMat* cuFindPerspectiveMat(const CvPoint2D32f* src,
	const CvPoint2D32f* dst,
	CvMat* map_matrix, int ptnum);


/* ����任�������ļ�*/
void cuSaveTransMat(const char *filename,
	const CvMat *mat,
	int TransType);

/* ��ȡ�任����*/
bool cuReadTransMat(const std::string& filename,
	cv::Mat& mat,
	int param_index);

/* �����src����trans_mat�任�󣬵���dst�ľ��� */
float cuAffinePtDist(CvPoint2D32f* src,
	CvPoint2D32f* dst,
	CvMat* trans_mat);
float cuPerspectivePtDist(CvPoint2D32f* src,
	CvPoint2D32f* dst,
	CvMat* trans_mat);

/* ����RANSAC�޳����,Ȼ��������任����*/
/* pt_num:��Ը���; pIndex:����õ������;samplesize:�����ߴ�
threshold:�ж�Ϊ�õ����ֵ; prob:������ֵ; maxiter:����������*/
int cuAffineRansac(CvMat* mat, CvPoint2D32f* base,
	CvPoint2D32f* reg, int pt_num, int *pIndex,
	int samplesize = 3, float thresold = 12,
	float prob = 0.9, int maxiter = 2000);

/* ����RANSAC�޳����,Ȼ�����ͶӰ�任����*/
int cuPerspectiveRansac(CvMat* mat, CvPoint2D32f* base,
	CvPoint2D32f* reg, int pt_num, int *pIndex,
	int samplesize = 4, float thresold = 12,
	float prob = 0.9, int maxiter = 2000);

void cuCreateMatMargin(const cv::Mat& src,
	cv::Mat& dst,
	int top, int bottom, int left, int right,
	cv::Scalar color);

void cuCalcTransformParam(const cv::Mat& baseImg,
	const cv::Mat& regImg,
	const char *param_filename = NULL);

//�����������ͼ�����ƴ�ӣ����һ������left_ex��ָmid.jpg�����չ�ĺ�ɫ����Ŀ��
void cuImageMosaic_TwoCam(const cv::Mat& left,
	const cv::Mat& right,
	const CvMat* param,
	IplImage** m_pMosaicImage,
	int left_ex = 0);


cv::Size cuCalcBoundPts(const cv::Mat& src,
	const cv::Mat& map_matrix,
	std::vector<cv::Point2f>& bnd_pts);

cv::Rect cuCreateWeightsMatrix(const cv::Mat& base,
	const cv::Mat& reg,
	const cv::Size reg_size,
	const std::vector<cv::Point2f>& reg_bndpts,
	cv::Mat& baseWeights,
	cv::Mat& regWeights,
	int left_ex = 0); //��׼ͼ���������չ����

void cuImageFusionProcess(const cv::cuda::GpuMat& base,
	const cv::cuda::GpuMat& reg,
	const cv::cuda::GpuMat& baseWeights,
	const cv::cuda::GpuMat& regWeights,
	cv::cuda::GpuMat& dst);
void cuImageFusionSimpleProcess(const cv::cuda::GpuMat& base,
	const cv::cuda::GpuMat& reg,
	const cv::cuda::GpuMat& baseWeights,
	const cv::cuda::GpuMat& regWeights,
	cv::cuda::GpuMat& dst);

void calcTransParams_3Cams(const cv::Mat& left,
	const cv::Mat& mid,
	const cv::Mat& right,
	bool initial = true);

void calcTransParams(int CamsGroupIdx, bool initial = true);

#endif /* CU_UTILITY_H_*/