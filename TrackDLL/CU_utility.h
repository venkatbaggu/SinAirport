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

/* 利用最小二乘法计算仿射矩阵*/
CvMat* cuFindAffineMat(const CvPoint2D32f* src,
	const CvPoint2D32f* dst,
	CvMat* map_matrix, int ptnum);

/* 先计算变换后的尺寸，再进行仿射变换*/
void cuWarpAffine(const IplImage* src,
	IplImage** dst,
	const CvMat* map_matrix,
	CvPoint2D32f bnd_pts[4],
	int flags CV_DEFAULT(CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS));

/* 先计算变换后的尺寸，再进行透视变换*/
void cuWarpPerspective(const cv::cuda::GpuMat& src,
	cv::cuda::GpuMat& dst,
	const cv::Mat& map_matrix,
	std::vector<cv::Point2f>& bnd_pts,
	cv::Size dst_size);

/* 利用最小二乘法计算投影变换矩阵*/
CvMat* cuFindPerspectiveMat(const CvPoint2D32f* src,
	const CvPoint2D32f* dst,
	CvMat* map_matrix, int ptnum);


/* 保存变换矩阵至文件*/
void cuSaveTransMat(const char *filename,
	const CvMat *mat,
	int TransType);

/* 读取变换矩阵*/
bool cuReadTransMat(const std::string& filename,
	cv::Mat& mat,
	int param_index);

/* 计算点src经过trans_mat变换后，到点dst的距离 */
float cuAffinePtDist(CvPoint2D32f* src,
	CvPoint2D32f* dst,
	CvMat* trans_mat);
float cuPerspectivePtDist(CvPoint2D32f* src,
	CvPoint2D32f* dst,
	CvMat* trans_mat);

/* 利用RANSAC剔除外点,然后计算仿射变换矩阵*/
/* pt_num:点对个数; pIndex:保存好点的索引;samplesize:样本尺寸
threshold:判断为好点的阈值; prob:概率阈值; maxiter:最大迭代次数*/
int cuAffineRansac(CvMat* mat, CvPoint2D32f* base,
	CvPoint2D32f* reg, int pt_num, int *pIndex,
	int samplesize = 3, float thresold = 12,
	float prob = 0.9, int maxiter = 2000);

/* 利用RANSAC剔除外点,然后计算投影变换矩阵*/
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

//对两个摄像机图像进行拼接，最后一个参数left_ex是指mid.jpg左边扩展的黑色区域的宽度
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
	int left_ex = 0); //基准图像中左边扩展部分

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