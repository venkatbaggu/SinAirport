
#include "CU_utility.h"
#include <iostream>
//#include <windows.h>

////////////////////////////////////////////////////
/// 1.将透视变换转成gpu
/// 2.计算重叠区域的加权值
/// 3.增加自动参数获取功能

CvMat* cuFindAffineMat(const CvPoint2D32f* src,
	const CvPoint2D32f* dst,
	CvMat* map_matrix,
	int ptnum)
{
	CvMat mA, mX, mB;
	float *A = new float[ptnum * 6 * 2];
	float *B = new float[ptnum * 2];
	float x[6];
	int i;

	cvInitMatHeader(&mA, ptnum * 2, 6, CV_32F, A);
	cvInitMatHeader(&mB, ptnum * 2, 1, CV_32F, B);
	cvInitMatHeader(&mX, 6, 1, CV_32F, x);

	// 	if( !src || !dst || !map_matrix )
	// 		CV_ERROR( CV_StsNullPtr, "" );

	for (i = 0; i < ptnum; i++)
	{
		int j = i * 12;
		int k = i * 12 + 6;
		A[j] = A[k + 3] = src[i].x;
		A[j + 1] = A[k + 4] = src[i].y;
		A[j + 2] = A[k + 5] = 1;
		A[j + 3] = A[j + 4] = A[j + 5] = 0;
		A[k] = A[k + 1] = A[k + 2] = 0;
		B[i * 2] = dst[i].x;
		B[i * 2 + 1] = dst[i].y;
	}
	cvSolve(&mA, &mB, &mX, CV_SVD);

	mX = cvMat(2, 3, CV_32FC1, x);
	cvConvert(&mX, map_matrix);

	delete[]A;
	delete[]B;

	return map_matrix;

}

void cuWarpAffine(const IplImage* src,
	IplImage** dst,
	const CvMat* map_matrix,
	CvPoint2D32f bnd_pts[4],
	int flags)
{
	float *a = new float[3 * 4];
	float *b = new float[2 * 4];
	CvMat A, B;
	cvInitMatHeader(&A, 3, 4, CV_32F, a);
	cvInitMatHeader(&B, 2, 4, CV_32F, b);
	a[0] = a[3] = a[4] = a[5] = 0;
	a[1] = a[2] = src->width - 1;
	a[6] = a[7] = src->height - 1;
	a[8] = a[9] = a[10] = a[11] = 1;

	cvMatMul(map_matrix, &A, &B);

	int dst_w = max(b[0], max(b[1], max(b[2], b[3])));
	int dst_h = max(b[4], max(b[5], max(b[6], b[7])));

	if (*dst == NULL)
		*dst = cvCreateImage(cvSize(dst_w, dst_h), src->depth, src->nChannels);
	cvZero(*dst);

	bnd_pts[0] = cvPoint2D32f(b[0], b[4]);
	bnd_pts[1] = cvPoint2D32f(b[1], b[5]);
	bnd_pts[2] = cvPoint2D32f(b[2], b[6]);
	bnd_pts[3] = cvPoint2D32f(b[3], b[7]);

	cvWarpAffine(src, *dst, map_matrix, flags);
}

cv::Size cuCalcBoundPts(const cv::Mat& src,
	const cv::Mat& map_matrix,
	vector<cv::Point2f>& bnd_pts)
{
	float a[] = { 0, src.cols - 1, src.cols - 1, 0,
		0, 0, src.rows - 1, src.rows - 1,
		1, 1, 1, 1 };
	Mat A = Mat(3, 4, CV_32F, a).clone();
	Mat B = Mat(3, 4, CV_32F);

	cv::gemm(map_matrix, A, 1, Mat(), 0, B);

	int dst_w = max(B.at<float>(0, 0) / B.at<float>(2, 0),
		max(B.at<float>(0, 1) / B.at<float>(2, 1),
		max(B.at<float>(0, 2) / B.at<float>(2, 2),
		B.at<float>(0, 3) / B.at<float>(2, 3))));

	int dst_h = max(B.at<float>(1, 0) / B.at<float>(2, 0),
		max(B.at<float>(1, 1) / B.at<float>(2, 1),
		max(B.at<float>(1, 2) / B.at<float>(2, 2),
		B.at<float>(1, 3) / B.at<float>(2, 3))));


	bnd_pts[0] = Point2f(B.at<float>(0, 0) / B.at<float>(2, 0),
		B.at<float>(1, 0) / B.at<float>(2, 0));

	bnd_pts[1] = Point2f(B.at<float>(0, 1) / B.at<float>(2, 1),
		B.at<float>(1, 1) / B.at<float>(2, 1));

	bnd_pts[2] = Point2f(B.at<float>(0, 2) / B.at<float>(2, 2),
		B.at<float>(1, 2) / B.at<float>(2, 2));

	bnd_pts[3] = Point2f(B.at<float>(0, 3) / B.at<float>(2, 3),
		B.at<float>(1, 3) / B.at<float>(2, 3));

	return Size(dst_w, dst_h);
}

void cuWarpPerspective(const cv::cuda::GpuMat& src,
	cv::cuda::GpuMat& dst,
	const cv::Mat& map_matrix,
	vector<Point2f>& bnd_pts,
	Size dst_size)
{
	cv::cuda::warpPerspective(src, dst, map_matrix, dst_size);

	///为后面的融合消除锯齿
	// 	if(bnd_pts[1].x >= bnd_pts[2].x)
	// 	{
	// 		bnd_pts[1].x = bnd_pts[2].x;
	// 		bnd_pts[1].y = max(bnd_pts[0].y, bnd_pts[1].y);
	// 
	// 		if(bnd_pts[1].y>0 && bnd_pts[1].y<dst.rows)
	// 			dst(Rect(0,0, dst.cols, bnd_pts[1].y+1)).setTo(0);
	// 
	// 		if( dst.cols - bnd_pts[2].x > 0 && 
	// 			dst.cols - bnd_pts[2].x < dst.cols)
	// 			dst(Rect(bnd_pts[2].x,0, dst.cols-bnd_pts[2].x+1, dst.rows)).setTo(0);
	// 	}else
	// 	{
	// 		bnd_pts[1].y = max(bnd_pts[0].y, bnd_pts[1].y);
	// 
	// 		if(bnd_pts[1].y > 0 && bnd_pts[1].y < dst.rows)
	// 			dst(Rect(0,0,dst.cols,bnd_pts[1].y+1)).setTo(0);
	// 
	// 		if( dst.cols - bnd_pts[2].x > 0 && 
	// 			dst.cols - bnd_pts[2].x < dst.cols)
	// 			dst(Rect(bnd_pts[1].x,0, dst.cols-bnd_pts[2].x+1,dst.rows)).setTo(0);
	// 	}
}

/*
* / x0 y0  1  0  0  0 -x0*u0 -y0*u0 \ /c00\ /u0\
* | x1 y1  1  0  0  0 -x1*u1 -y1*u1 | |c01| |u1|
* | x2 y2  1  0  0  0 -x2*u2 -y2*u2 | |c02| |u2|
* | x3 y3  1  0  0  0 -x3*u3 -y3*u3 |.|c10|=|u3|,
* |  0  0  0 x0 y0  1 -x0*v0 -y0*v0 | |c11| |v0|
* |  0  0  0 x1 y1  1 -x1*v1 -y1*v1 | |c12| |v1|
* |  0  0  0 x2 y2  1 -x2*v2 -y2*v2 | |c20| |v2|
* \  0  0  0 x3 y3  1 -x3*v3 -y3*v3 / \c21/ \v3/
*/
CvMat* cuFindPerspectiveMat(const CvPoint2D32f* src,
	const CvPoint2D32f* dst,
	CvMat* map_matrix, int ptnum)
{
	float *a = new float[ptnum * 2 * 8];
	float *b = new float[ptnum * 2];
	float x[9];

	//     CvMat A = cvMat( ptnum*2, 8, CV_32FC1, a );
	//     CvMat B = cvMat( ptnum*2, 1, CV_32FC1, b );
	//     CvMat X = cvMat( 8, 1, CV_32FC1, x );

	CvMat A, B, X;
	cvInitMatHeader(&A, ptnum * 2, 8, CV_32F, a);
	cvInitMatHeader(&B, ptnum * 2, 1, CV_32F, b);
	cvInitMatHeader(&X, 8, 1, CV_32F, x);

	int i;

	//     if( !src || !dst || !map_matrix )
	//         CV_ERROR( CV_StsNullPtr, "" );

	for (i = 0; i < ptnum; ++i)
	{
		a[i * 8 + 0] = a[(i + ptnum) * 8 + 3] = src[i].x;
		a[i * 8 + 1] = a[(i + ptnum) * 8 + 4] = src[i].y;
		a[i * 8 + 2] = a[(i + ptnum) * 8 + 5] = 1;
		a[i * 8 + 3] = a[i * 8 + 4] = a[i * 8 + 5] =
			a[(i + ptnum) * 8 + 0] = a[(i + ptnum) * 8 + 1] = a[(i + ptnum) * 8 + 2] = 0;
		a[i * 8 + 6] = -src[i].x*dst[i].x;
		a[i * 8 + 7] = -src[i].y*dst[i].x;
		a[(i + ptnum) * 8 + 6] = -src[i].x*dst[i].y;
		a[(i + ptnum) * 8 + 7] = -src[i].y*dst[i].y;
		b[i] = dst[i].x;
		b[i + ptnum] = dst[i].y;
	}

	cvSolve(&A, &B, &X, CV_SVD);
	x[8] = 1;

	X = cvMat(3, 3, CV_32FC1, x);
	cvConvert(&X, map_matrix);

	delete[]a;
	delete[]b;

	return map_matrix;
}

void cuSaveTransMat(const char *filename, const CvMat *mat, int trans_type)
{
	CvFileStorage* storage = cvOpenFileStorage(filename, NULL, CV_STORAGE_APPEND);

	//     if(!storage)
	//       CV_ERROR(CV_StsNullPtr,"");
	// 
	//     if(!CV_IS_MAT(mat))
	//        CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	int i, j;
	float val;

	// 变换类型
	char strType[100];
	int Type;
	switch (trans_type)
	{
	case VES_TRANS_TYPE_AFFINE:
		//sprintf(strType, VES_STR_TRANS_TYPE_AFFINE);
		sprintf_s(strType, VES_STR_TRANS_TYPE_AFFINE);
		Type = VES_TRANS_TYPE_AFFINE;
		break;

	case VES_TRANS_TYPE_PERSPEC:
		//sprintf(strType, VES_STR_TRANS_TYPE_PERSPEC);
		sprintf_s(strType, VES_STR_TRANS_TYPE_PERSPEC);
		Type = VES_TRANS_TYPE_PERSPEC;
		break;
	}

	cvStartWriteStruct(storage, "Description", CV_NODE_MAP);
	cvWriteString(storage, "Des0", "Original Image:I0(x0,y0),Transformed Image: I1(x1,y1)");
	cvWriteString(storage, "Des1", "(x1,y1) = TransMat*(x0,y0,1)");
	cvEndWriteStruct(storage);

	cvStartWriteStruct(storage, "TransType", CV_NODE_MAP);
	cvWriteString(storage, "Type", strType);
	cvWriteInt(storage, "TypeID", Type);
	cvEndWriteStruct(storage);

	cvStartWriteStruct(storage, "Parameters", CV_NODE_SEQ | CV_NODE_MAP);

	CvSize size = cvGetSize(mat);
	for (i = 0; i<size.height; i++)
		for (j = 0; j<size.width; j++)
		{
			val = cvGetReal2D(mat, i, j);
			cvWriteRawData(storage, &val, 1, "d");
		}
	cvEndWriteStruct(storage);
	cvReleaseFileStorage(&storage);
}

bool cuReadTransMat(const string& filename, Mat& mat, int param_index)
{
	cv::FileStorage storage(filename, cv::FileStorage::READ);

	//     if(!storage)
	//       CV_ERROR(CV_StsNullPtr,"");
	// 
	// 	if(!CV_IS_MAT(mat))
	//        CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	cv::String node = cv::format("Parameters_%01d", param_index);

	if (storage.isOpened())
		storage[node] >> mat;
	else
		return false;

	storage.release();
	return true;
}

int cuAffineRansac(CvMat* mat, CvPoint2D32f* base, CvPoint2D32f* reg, int pt_num, int *pIndex, \
	int samplesize, float thresold, float prob, int maxiter)
{
	int minK = 10000;
	int i, k = 0;
	int inlier = 0, bestinlier = 0;
	float frac = 1.0;

	CvRNG rng_state = cvRNG(0xffffffff);
	CvMat* rmat = cvCloneMat(mat);

	int *idx = new int[samplesize];
	CvPoint2D32f *tmp_reg = new CvPoint2D32f[samplesize];
	CvPoint2D32f *tmp_base = new CvPoint2D32f[samplesize];

	int *tmp_pIndex = new int[pt_num];

	while (minK > k)
	{
		//   	   if(k>maxiter)
		// 		   break;

		//随机采样samplesize个点
		for (i = 0; i<samplesize; i++)
		{
			idx[i] = abs((int)(cvRandInt(&rng_state) % pt_num));
			tmp_reg[i] = reg[idx[i]];
			tmp_base[i] = base[idx[i]];
		}

		//计算变换模型
		cuFindAffineMat(tmp_reg, tmp_base, rmat, samplesize);

		//计算内点的个数
		inlier = 0;
		for (i = 0; i<pt_num; i++)
		{
			float dist = cuAffinePtDist(&reg[i], &base[i], rmat);
			if (dist < thresold)
			{
				tmp_pIndex[inlier] = i;
				inlier++;
			}
		}

		//如果优于当前模型，更新之，
		//并重新计算下一轮迭代次数
		if (inlier>bestinlier)
		{
			bestinlier = inlier;
			cvCopy(rmat, mat);
			memcpy(pIndex, tmp_pIndex, sizeof(int)*pt_num);
			frac = (float)inlier / (float)pt_num;
			minK = cvCeil(log(1 - prob) / log(1 - pow(frac, samplesize)));
			k = 0;
		}
		else
		{
			k++;
		}
	}

	delete[]idx;
	delete[]tmp_reg;
	delete[]tmp_base;
	delete[]tmp_pIndex;

	cvReleaseMat(&rmat);

	return bestinlier;
}

int cuPerspectiveRansac(CvMat* mat, CvPoint2D32f* base, CvPoint2D32f* reg, int pt_num, int *pIndex, \
	int samplesize, float thresold, float prob, int maxiter)
{
	int minK = 10000;
	int i, k = 0;
	int inlier = 0, bestinlier = 0;
	float frac = 1.0;

	CvRNG rng_state = cvRNG(0xffffffff);
	CvMat* rmat = cvCloneMat(mat);

	int *idx = new int[samplesize];
	CvPoint2D32f *tmp_reg = new CvPoint2D32f[samplesize];
	CvPoint2D32f *tmp_base = new CvPoint2D32f[samplesize];

	int *tmp_pIndex = new int[pt_num];

	while (minK > k)
	{
		if (k>maxiter)
			break;

		//随机采样samplesize个点
		for (i = 0; i<samplesize; i++)
		{
			idx[i] = abs((int)(cvRandInt(&rng_state) % pt_num));
			tmp_reg[i] = reg[idx[i]];
			tmp_base[i] = base[idx[i]];
		}

		//计算变换模型
		cuFindPerspectiveMat(tmp_reg, tmp_base, rmat, samplesize);

		//计算内点的个数
		inlier = 0;
		for (i = 0; i<pt_num; i++)
		{
			float dist = cuPerspectivePtDist(&reg[i], &base[i], rmat);
			if (dist < thresold)
			{
				tmp_pIndex[inlier] = i;
				inlier++;
			}
		}

		//如果优于当前模型，更新之，
		//并重新计算下一轮迭代次数
		if (inlier>bestinlier)
		{
			bestinlier = inlier;
			cvCopy(rmat, mat);
			memcpy(pIndex, tmp_pIndex, sizeof(int)*pt_num);
			frac = (float)inlier / (float)pt_num;
			minK = cvCeil(log(1 - prob) / log(1 - pow(frac, samplesize)));
			k = 0;
		}
		else
		{
			k++;
		}
	}

	delete[]idx;
	delete[]tmp_reg;
	delete[]tmp_base;
	delete[]tmp_pIndex;
	cvReleaseMat(&rmat);

	return bestinlier;
}

float cuAffinePtDist(CvPoint2D32f* src, CvPoint2D32f* dst, CvMat* trans_mat)
{
	float distance;

	float scr_pt[3] = { src->x, src->y, 1.0f };
	float dst_pt[2] = { 0, 0 };
	float dst_pt_1[2] = { dst->x, dst->y };

	// 	if(!CV_IS_MAT(trans_mat))
	//        CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	CvMat scr_mat = cvMat(3, 1, CV_32F, scr_pt);
	CvMat dst_mat = cvMat(2, 1, CV_32F, dst_pt);
	CvMat dst_mat_1 = cvMat(2, 1, CV_32F, dst_pt_1);

	cvMatMul(trans_mat, &scr_mat, &dst_mat);
	cvSub(&dst_mat, &dst_mat_1, &dst_mat);
	distance = cvNorm(&dst_mat);

	return distance;
}

/*
c00*xi + c01*yi + c02
* ui = ---------------------
*      c20*xi + c21*yi + c22
*
*      c10*xi + c11*yi + c12
* vi = ---------------------
*      c20*xi + c21*yi + c22

*/
float cuPerspectivePtDist(CvPoint2D32f* src, CvPoint2D32f* dst, CvMat* trans_mat)
{

	float distance;

	float scr_pt[3] = { src->x, src->y, 1.0f };
	float dst_pt[2] = { 0, 0 };
	float dst_pt_1[2] = { dst->x, dst->y };

	// 	if(!CV_IS_MAT(trans_mat))
	// 		CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	CvMat dst_mat = cvMat(2, 1, CV_32F, dst_pt);
	CvMat dst_mat_1 = cvMat(2, 1, CV_32F, dst_pt_1);

	float m00, m01, m02, m10, m11, m12, m20, m21, m22;

	m00 = cvGetReal2D(trans_mat, 0, 0);
	m01 = cvGetReal2D(trans_mat, 0, 1);
	m02 = cvGetReal2D(trans_mat, 0, 2);
	m10 = cvGetReal2D(trans_mat, 1, 0);
	m11 = cvGetReal2D(trans_mat, 1, 1);
	m12 = cvGetReal2D(trans_mat, 1, 2);
	m20 = cvGetReal2D(trans_mat, 2, 0);
	m21 = cvGetReal2D(trans_mat, 2, 1);
	m22 = cvGetReal2D(trans_mat, 2, 2);

	dst_pt[0] = ((m00*src->x + m01*src->y + m02) / (m20*src->x + m21*src->y + m22));
	dst_pt[1] = ((m10*src->x + m11*src->y + m12) / (m20*src->x + m21*src->y + m22));

	cvSub(&dst_mat, &dst_mat_1, &dst_mat);
	distance = cvNorm(&dst_mat);

	return distance;
}

cv::Rect cuCreateWeightsMatrix(const cv::Mat& src_base,
	const cv::Mat& src_reg,
	const cv::Size reg_size,
	const std::vector<cv::Point2f>& reg_bndpts,
	cv::Mat& baseWeights,
	cv::Mat& regWeights,
	int left_ex) //基准图像中左边扩展部分
{
	Mat base, reg;
	Mat pBaseMask, pOverlap, pRegTmp;
	Point pts[4];
	int nw=0, nh=0;
	//设置基准图像、配准图像以及重叠部分的标记
	int base_flag = 1, reg_flag = 2, overlap_flag = 3;

	try{
		cv::cvtColor(src_base, base, CV_RGB2GRAY);
		cv::cvtColor(src_reg, reg, CV_RGB2GRAY);

		nw = max(base.cols, reg_size.width);
		nh = max(base.rows, reg_size.height);

		baseWeights.setTo(0);
		regWeights.setTo(0);

		//建立基准图像
		pBaseMask = Mat(Size(nw, nh), CV_8UC1).clone();
		pBaseMask.setTo(0);

		//基准图像权值
		baseWeights = Mat(Size(nw, nh),
			MOSAIC_WEIGHTS_CV_TYPE).clone();
		baseWeights.setTo(0);

		//在基准图像中，将基准图像区域，置为base_flag
		pBaseMask(Rect(left_ex, 0, base.cols - left_ex, base.rows)).setTo(base_flag);
		baseWeights(Rect(left_ex, 0, base.cols -
			left_ex, base.rows)).setTo(MOSAIC_WEIGHTS_RATIO);

		//建立重叠图像
		pOverlap= Mat(Size(nw, nh), CV_8UC1).clone();
		pOverlap.setTo(0);

		//配准图像的4个边界点，向外扩展extend个像素
		int extend = 0;
		pts[0] = Point(reg_bndpts[0]);
		pts[0] += Point(-extend, -extend);
		pts[1] = Point(reg_bndpts[1]);
		pts[1] += Point(extend, -extend);
		pts[2] = Point(reg_bndpts[2]);
		pts[2] += Point(extend, extend);
		pts[3] = Point(reg_bndpts[3]);
		pts[3] += Point(-extend, extend);
	}
	catch (...)
	{
		printf("aaaaaaaaaaaaa!!!!\n");
	}

	try{
		//配准图像权值
		Mat tmp_regWeights = pOverlap.clone();
		fillConvexPoly(tmp_regWeights, pts, 4, MOSAIC_WEIGHTS_RATIO);
		tmp_regWeights.convertTo(regWeights, MOSAIC_WEIGHTS_CV_TYPE);

		//配准图像部分填充为reg_flag
		fillConvexPoly(pOverlap, pts, 4, cvScalarAll(reg_flag));

		//通过求和，将重叠部分置为overlap_flag=3
		pOverlap = pOverlap + pBaseMask;
		base.copyTo(pBaseMask(Rect(0, 0, base.cols, base.rows)));

		//建立临时拼接图像
		pRegTmp = Mat(Size(nw, nh), CV_8UC1).clone();
		pRegTmp.setTo(0);
		reg.copyTo(pRegTmp(Rect(0, 0, reg.cols, reg.rows)));
	}
	catch (...)
	{
		printf("bbbbbbbbbbbbb!!!!\n");
	}

	int min_bd = INT_MAX, max_bd = 0;

	try{
		//找出重叠区域的边界
		//int *lb = new int[pRegTmp.rows]; //每一行的左边界
		//int *ub = new int[pRegTmp.rows]; //每一行的右边界
		vector<int>lb(pRegTmp.rows);
		vector<int>ub(pRegTmp.rows);

		bool findlb = false;
		for (int i = 0; i < pOverlap.rows; i++)
		{
			findlb = false;
			lb[i] = 0, ub[i] = pOverlap.cols - 1;
			for (int j = 0; j < pOverlap.cols; j++)
			{
				if (!findlb && pOverlap.at<uchar>(i, j) == overlap_flag)
				{
					findlb = true;
					lb[i] = j;
				}
				if (findlb && pOverlap.at<uchar>(i, j) != overlap_flag)
				{
					ub[i] = j;
					break;
				}
			}
		}
				
		//加权融合
		for (int i = 0; i < pOverlap.rows; i++)
		{
			for (int j = 0; j < pOverlap.cols; j++)
			{
				if (pOverlap.at<uchar>(i, j) == overlap_flag)
				{
					min_bd = min_bd > lb[i] ? lb[i] : min_bd;
					max_bd = max_bd < ub[i] ? ub[i] : max_bd;

					float ratio = (j - lb[i]) / (ub[i] - lb[i] + 0.00001f);

					if (left_ex > 0)
						ratio = 1.0 - ratio;

					//if(pRegTmp.at<uchar>(i,j)==0)
					//	ratio = 0;
					//if(pBaseMask.at<uchar>(i,j)==0)
					//	ratio = 1;

					baseWeights.at<MOSAIC_WEIGHTS_DATATYPE>(i, j) =
						(1.0 - ratio) * MOSAIC_WEIGHTS_RATIO;
					regWeights.at<MOSAIC_WEIGHTS_DATATYPE>(i, j) =
						ratio * MOSAIC_WEIGHTS_RATIO;
				}
			}
		}
	}
	catch (...)
	{
		printf("cccccccccccccc!!!!\n");
	}

	try
	{
		Mat _tmp = baseWeights.clone();
		/*Mat _show;
		baseWeights.convertTo(_show, CV_8UC1);
		cv::imshow("ss", _show);
		cv::waitKey();*/

		baseWeights.release();
		baseWeights.create(Size(nw, nh), CV_16UC3);
		for (int y = 1; y < _tmp.rows - 1; y++)
		{
			for (int x = 0; x < _tmp.cols; x++)
				for (int c = 0; c < 3; c++)
					baseWeights.at<cv::Vec3w>(y, x)[c]
					= _tmp.at<ushort>(y, x);
		}

		/*vector<mat> b_merge;
		b_merge.push_back(baseweights);
		b_merge.push_back(baseweights);
		b_merge.push_back(baseweights);
		baseweights.release();
		baseweights.create(size(nw, nh), cv_16uc3);
		cv::merge(b_merge, baseweights);
		b_merge.clear();*/
	}
	catch (...)
	{
		printf("dddddddd!!!!\n");
	}

	try
	{
		Mat _tmp = regWeights.clone();
		regWeights.release();
		regWeights.create(Size(nw, nh), CV_16UC3);
		for (int y = 1; y < _tmp.rows - 1; y++)
		{
			for (int x = 0; x < _tmp.cols; x++)
				for (int c = 0; c < 3; c++)
					regWeights.at<cv::Vec3w>(y, x)[c] 
									= _tmp.at<ushort>(y, x);
		}

		/*vector<Mat> B_merge1;
		B_merge1.push_back(regWeights);
		B_merge1.push_back(regWeights);
		B_merge1.push_back(regWeights);
		regWeights.release();
		regWeights.create(Size(nw, nh), CV_16UC3);
		cv::merge(B_merge1, regWeights);*/

	}
	catch (...)
	{
		printf("eeeeeeeee!!!!\n");
	}
	//delete[]lb;
	//delete[]ub;

	return Rect(min_bd, 0, max_bd - min_bd, baseWeights.rows);
}


void cuImageFusionSimpleProcess(const GpuMat& base,
	const GpuMat& reg,
	const GpuMat& baseWeights,
	const GpuMat& regWeights,
	GpuMat& dst)
{
	//TIMER_BEGIN(3);
	GpuMat fbase = base;
	GpuMat freg = reg;
	//TIMER_END(3, "Type Converted Time");

	int ex_rows = baseWeights.rows - fbase.rows;
	int ex_cols = baseWeights.cols - fbase.cols;
	if (ex_rows>0 || ex_cols>0)
		cv::cuda::copyMakeBorder(fbase, fbase, 0, ex_rows, 0, ex_cols,
		cv::BORDER_CONSTANT, 0);

	ex_rows = regWeights.rows - freg.rows;
	ex_cols = regWeights.cols - freg.cols;
	if (ex_rows>0 || ex_cols>0)
		cv::cuda::copyMakeBorder(freg, freg, 0, ex_rows, 0, ex_cols,
		cv::BORDER_CONSTANT, 0);

	cv::cuda::divide(fbase, Scalar(MOSAIC_WEIGHTS_RATIO,
		MOSAIC_WEIGHTS_RATIO, MOSAIC_WEIGHTS_RATIO), fbase);
	cv::cuda::divide(freg, Scalar(MOSAIC_WEIGHTS_RATIO,
		MOSAIC_WEIGHTS_RATIO, MOSAIC_WEIGHTS_RATIO), freg);

	cv::cuda::multiply(fbase, baseWeights, fbase);
	cv::cuda::multiply(freg, regWeights, freg);
	cv::cuda::add(fbase, freg, dst);
}

//MEMORYSTATUS mem;
//void CoutMemStatus()
//{
//	int mb = 1024 * 1024;
//	GlobalMemoryStatus(&mem);
//	std::cout << mem.dwTotalPhys / mb << " " << mem.dwAvailPhys / mb << std::endl
//		<< mem.dwTotalPageFile / mb << " " << mem.dwAvailPageFile / mb << std::endl
//		<< mem.dwTotalVirtual / mb << " " << mem.dwAvailVirtual / mb << std::endl;
//}

void cuImageFusionProcess(const GpuMat& base,
	const GpuMat& reg,
	const GpuMat& baseWeights,
	const GpuMat& regWeights,
	GpuMat& dst)
{
	//TIMER_BEGIN(3);
	GpuMat fbase, freg;
	base.convertTo(fbase, CV_16UC3);
	reg.convertTo(freg, CV_16UC3);
	//TIMER_END(3, "Type Converted Time");

	int ex_rows = baseWeights.rows - fbase.rows;
	int ex_cols = baseWeights.cols - fbase.cols;
	if (ex_rows>0 || ex_cols>0)
		cv::cuda::copyMakeBorder(fbase, fbase, 0, ex_rows, 0, ex_cols,
		cv::BORDER_CONSTANT, 0);

	ex_rows = regWeights.rows - freg.rows;
	ex_cols = regWeights.cols - freg.cols;
	if (ex_rows>0 || ex_cols>0)
		cv::cuda::copyMakeBorder(freg, freg, 0, ex_rows, 0, ex_cols,
		cv::BORDER_CONSTANT, 0);

	//TIMER_BEGIN(5);
	cv::cuda::multiply(fbase, baseWeights, fbase);
	cv::cuda::multiply(freg, regWeights, freg);
	//TIMER_END(5, "multiply Time");

	//TIMER_BEGIN(6);
	cv::cuda::add(fbase, freg, freg);
	cv::cuda::divide(freg, Scalar(MOSAIC_WEIGHTS_RATIO,
		MOSAIC_WEIGHTS_RATIO, MOSAIC_WEIGHTS_RATIO), freg);

	if (dst.empty())
		dst.create(Size(freg.cols, freg.rows), CV_8UC3);

	freg.convertTo(dst, CV_8UC3);
	//std::cout << "isCon:" << freg.isContinuous() << std::endl;
	//CoutMemStatus();
	//TIMER_END(6, "add cvted time");

}

void cuCreateMatMargin(const Mat& src, Mat& dst, int top, int bottom, int left, int right, Scalar color)
{
	if (!src.data)
	{
		printf("Mat data is NULL.\n");
		return;
	}

	int ex_rows = src.rows + top + bottom;
	int ex_cols = src.cols + left + right;

	if (ex_rows <= 0 || ex_cols <= 0)
		return;

	Mat extend(ex_rows, ex_cols, src.type());
	extend.setTo(color);

	Mat _tmp = extend(Rect(left, top, src.cols, src.rows));
	src.copyTo(_tmp);
	dst = extend.clone();
}

//#define TRANS_TYPE_AFFINE
void cuCalcTransformParam(const Mat& baseImg, const Mat& regImg, const char *param_filename)
{

}

void cuImageMosaic_TwoCam(const Mat& left,
	const Mat& right,
	const Mat& param,
	Mat& m_pMosaicImage,
	vector<Point2f>& bnd_pts,
	int left_ex)
{
	// 	Mat pTmp;
	// 	if(!param.empty()) 
	// 	{
	// #ifdef TRANS_TYPE_AFFINE
	// 		cuWarpAffine(&m_pRegImage,&pTmp,param,bnd_pts, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
	// #else
	// 	 	cuWarpPerspective(right, pTmp, param, bnd_pts);
	// #endif
	// 	}
	// 
	// 	//TIMER_BEGIN(1)
	// 	cuImageFusionProcess(left,pTmp,bnd_pts,m_pMosaicImage,left_ex);
	// 	//TIMER_END(1,"ImageMosaic_fusion");

}