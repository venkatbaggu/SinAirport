#if !defined UTILITY_H
#define UTILITY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <algorithm>
using std::max;
using std::min;

#include <core/core.hpp>
#include <core/utility.hpp>
#include <imgproc/imgproc.hpp>
#include <highgui/highgui.hpp>
#include <imgproc/imgproc_c.h>
#include <features2d/features2d.hpp>
using cv::Mat;
using cv::Rect;
using cv::Scalar;

#define VES_TRANS_TYPE_RIGID   0 //刚性变换 
#define VES_TRANS_TYPE_AFFINE  1 //仿射变换 
#define VES_TRANS_TYPE_PERSPEC 2 //投影变换 

#define VES_STR_TRANS_TYPE_RIGID   "Rigid_Transform"
#define VES_STR_TRANS_TYPE_AFFINE  "Affine_Transform"
#define VES_STR_TRANS_TYPE_PERSPEC "Perspective_Transform"


/*计时器*/
#define TIMER_BEGIN(id) double timer_##id = (double)cvGetTickCount();
#define TIMER_END(id,func_name)											 \
	timer_##id = (double)cvGetTickCount() - timer_##id;				 \
	printf( "%s _[time] = %gms \n", func_name, timer_##id/(cvGetTickFrequency()*1000.));
  

/* 利用最小二乘法计算仿射矩阵*/
CvMat* veFindAffineMat(const CvPoint2D32f* src,
					 const CvPoint2D32f* dst,
                     CvMat* map_matrix,int ptnum );

/* 先计算变换后的尺寸，再进行仿射变换*/
void veWarpAffine(const IplImage* src, 
				  IplImage** dst, 
				  const CvMat* map_matrix,
				  CvPoint2D32f bnd_pts[4],
				  int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));

/* 先计算变换后的尺寸，再进行透视变换*/
void veWarpPerspective(const IplImage* src, 
					   IplImage** dst, 
					   const CvMat* map_matrix,
					   CvPoint2D32f bnd_pts[4],
					   int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));

void veImageFusionProcess(const IplImage* base, 
						   const IplImage* reg,
						   CvPoint2D32f reg_bndpts[4],
						   IplImage** dst,
						   int left_ex = 0);

/* 利用最小二乘法计算投影变换矩阵*/
CvMat* veFindPerspectiveMat(const CvPoint2D32f* src,
					 const CvPoint2D32f* dst,
                     CvMat* map_matrix,int ptnum);


/* 保存变换矩阵至文件*/
void veSaveTransMat( const char *filename,
					 const CvMat *mat,
					 int TransType);
 
/* 读取变换矩阵*/
bool veReadTransMat( const char *filename,CvMat *mat);

/* 计算点src经过trans_mat变换后，到点dst的距离 */
double veAffinePtDist(CvPoint2D32f* src,CvPoint2D32f* dst,CvMat* trans_mat);
double vePerspectivePtDist(CvPoint2D32f* src,CvPoint2D32f* dst,CvMat* trans_mat);
 
/* 利用RANSAC剔除外点,然后计算仿射变换矩阵*/
/* pt_num:点对个数; pIndex:保存好点的索引;samplesize:样本尺寸
   threshold:判断为好点的阈值; prob:概率阈值; maxiter:最大迭代次数*/
int veAffineRansac(CvMat* mat,CvPoint2D32f* base,CvPoint2D32f* reg,int pt_num,int *pIndex,\
				   int samplesize=3,double thresold=12,float prob=0.9,int maxiter=2000);

/* 利用RANSAC剔除外点,然后计算投影变换矩阵*/
int vePerspectiveRansac(CvMat* mat,CvPoint2D32f* base,CvPoint2D32f* reg,int pt_num,int *pIndex, \
				   int samplesize=4,double thresold=12,float prob=0.9,int maxiter=2000);

void veCreateMatMargin(const cv::Mat& src, cv::Mat& dst, int top, int bottom, int left, int right, cv::Scalar color);

void veCalcTransformParam(const cv::Mat& baseImg, const cv::Mat& regImg, const char *param_filename = NULL);

//对两个摄像机图像进行拼接，最后一个参数left_ex是指mid.jpg左边扩展的黑色区域的宽度
void veImageMosaic_TwoCam(const cv::Mat& left, const cv::Mat& right, const CvMat* param, IplImage** m_pMosaicImage,int left_ex = 0);

//图像显示
void my_imshow(const char* wndName, const Mat& img);
int my_waikKey(int delay);

#endif 