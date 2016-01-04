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

#define VES_TRANS_TYPE_RIGID   0 //���Ա任 
#define VES_TRANS_TYPE_AFFINE  1 //����任 
#define VES_TRANS_TYPE_PERSPEC 2 //ͶӰ�任 

#define VES_STR_TRANS_TYPE_RIGID   "Rigid_Transform"
#define VES_STR_TRANS_TYPE_AFFINE  "Affine_Transform"
#define VES_STR_TRANS_TYPE_PERSPEC "Perspective_Transform"


/*��ʱ��*/
#define TIMER_BEGIN(id) double timer_##id = (double)cvGetTickCount();
#define TIMER_END(id,func_name)											 \
	timer_##id = (double)cvGetTickCount() - timer_##id;				 \
	printf( "%s _[time] = %gms \n", func_name, timer_##id/(cvGetTickFrequency()*1000.));
  

/* ������С���˷�����������*/
CvMat* veFindAffineMat(const CvPoint2D32f* src,
					 const CvPoint2D32f* dst,
                     CvMat* map_matrix,int ptnum );

/* �ȼ���任��ĳߴ磬�ٽ��з���任*/
void veWarpAffine(const IplImage* src, 
				  IplImage** dst, 
				  const CvMat* map_matrix,
				  CvPoint2D32f bnd_pts[4],
				  int flags CV_DEFAULT(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS));

/* �ȼ���任��ĳߴ磬�ٽ���͸�ӱ任*/
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

/* ������С���˷�����ͶӰ�任����*/
CvMat* veFindPerspectiveMat(const CvPoint2D32f* src,
					 const CvPoint2D32f* dst,
                     CvMat* map_matrix,int ptnum);


/* ����任�������ļ�*/
void veSaveTransMat( const char *filename,
					 const CvMat *mat,
					 int TransType);
 
/* ��ȡ�任����*/
bool veReadTransMat( const char *filename,CvMat *mat);

/* �����src����trans_mat�任�󣬵���dst�ľ��� */
double veAffinePtDist(CvPoint2D32f* src,CvPoint2D32f* dst,CvMat* trans_mat);
double vePerspectivePtDist(CvPoint2D32f* src,CvPoint2D32f* dst,CvMat* trans_mat);
 
/* ����RANSAC�޳����,Ȼ��������任����*/
/* pt_num:��Ը���; pIndex:����õ������;samplesize:�����ߴ�
   threshold:�ж�Ϊ�õ����ֵ; prob:������ֵ; maxiter:����������*/
int veAffineRansac(CvMat* mat,CvPoint2D32f* base,CvPoint2D32f* reg,int pt_num,int *pIndex,\
				   int samplesize=3,double thresold=12,float prob=0.9,int maxiter=2000);

/* ����RANSAC�޳����,Ȼ�����ͶӰ�任����*/
int vePerspectiveRansac(CvMat* mat,CvPoint2D32f* base,CvPoint2D32f* reg,int pt_num,int *pIndex, \
				   int samplesize=4,double thresold=12,float prob=0.9,int maxiter=2000);

void veCreateMatMargin(const cv::Mat& src, cv::Mat& dst, int top, int bottom, int left, int right, cv::Scalar color);

void veCalcTransformParam(const cv::Mat& baseImg, const cv::Mat& regImg, const char *param_filename = NULL);

//�����������ͼ�����ƴ�ӣ����һ������left_ex��ָmid.jpg�����չ�ĺ�ɫ����Ŀ��
void veImageMosaic_TwoCam(const cv::Mat& left, const cv::Mat& right, const CvMat* param, IplImage** m_pMosaicImage,int left_ex = 0);

//ͼ����ʾ
void my_imshow(const char* wndName, const Mat& img);
int my_waikKey(int delay);

#endif 