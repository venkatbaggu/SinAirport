#include "Utility.h"
#include <iostream>

///////////////////手工选取图像中的矩形/////////////////////////////////



CvMat* veFindAffineMat(const CvPoint2D32f* src,
					 const CvPoint2D32f* dst,
                     CvMat* map_matrix,int ptnum)
{
    CvMat mA, mX, mB;
    double *A = new double[ptnum*6*2];
    double *B = new double[ptnum*2];
	double x[6];
    int i;

    cvInitMatHeader(&mA, ptnum*2, 6, CV_64F, A);
    cvInitMatHeader(&mB, ptnum*2, 1, CV_64F, B);
	cvInitMatHeader(&mX, 6, 1, CV_64F, x);

// 	if( !src || !dst || !map_matrix )
// 		CV_ERROR( CV_StsNullPtr, "" );

    for( i = 0; i < ptnum; i++ )
    {
        int j = i*12;
        int k = i*12+6;
        A[j] = A[k+3] = src[i].x;
        A[j+1] = A[k+4] = src[i].y;
        A[j+2] = A[k+5] = 1;
        A[j+3] = A[j+4] = A[j+5] = 0;
        A[k] = A[k+1] = A[k+2] = 0;
        B[i*2] = dst[i].x;
        B[i*2+1] = dst[i].y;
    }
    cvSolve(&mA, &mB, &mX,CV_SVD);

    mX = cvMat( 2, 3, CV_64FC1, x );
	cvConvert( &mX, map_matrix );

	delete []A;
	delete []B;

    return map_matrix;
 
}

void veWarpAffine(const IplImage* src, 
				  IplImage** dst, 
				  const CvMat* map_matrix,
				  CvPoint2D32f bnd_pts[4],
				  int flags)
{
	double *a = new double[3*4];
	double *b = new double[2*4];
	CvMat A,B;
	cvInitMatHeader(&A, 3, 4, CV_64F, a);
	cvInitMatHeader(&B, 2, 4, CV_64F, b);
	a[0]=a[3]=a[4]=a[5]=0;
	a[1]=a[2]=src->width-1;
	a[6]=a[7]=src->height-1;
	a[8]=a[9]=a[10]=a[11]=1;

	cvMatMul(map_matrix,&A,&B);

	int dst_w = max(b[0],max(b[1],max(b[2],b[3])));
	int dst_h = max(b[4],max(b[5],max(b[6],b[7])));

	if(*dst == NULL)
		*dst = cvCreateImage(cvSize(dst_w,dst_h),src->depth,src->nChannels);
	cvZero(*dst);

	bnd_pts[0] = cvPoint2D32f(b[0],b[4]);
	bnd_pts[1] = cvPoint2D32f(b[1],b[5]);
	bnd_pts[2] = cvPoint2D32f(b[2],b[6]);
	bnd_pts[3] = cvPoint2D32f(b[3],b[7]);

	cvWarpAffine(src,*dst,map_matrix,flags);
}

void veWarpPerspective(const IplImage* src, 
					   IplImage** dst, 
					   const CvMat* map_matrix,
					   CvPoint2D32f bnd_pts[4],
					   int flags)
{
	double *a = new double[3*4];
	double *b = new double[3*4];
	CvMat A,B;
	cvInitMatHeader(&A, 3, 4, CV_64F, a);
	cvInitMatHeader(&B, 3, 4, CV_64F, b);
	a[0]=a[3]=a[4]=a[5]=0;
	a[1]=a[2]=src->width-1;
	a[6]=a[7]=src->height-1;
	a[8]=a[9]=a[10]=a[11]=1;
	
	cvMatMul(map_matrix,&A,&B);
	
	int dst_w = max(b[0]/b[8],max(b[1]/b[9],max(b[2]/b[10],b[3]/b[11])));
	int dst_h = max(b[4]/b[8],max(b[5]/b[9],max(b[6]/b[10],b[7]/b[11])));
	
	if(*dst == NULL)
		*dst = cvCreateImage(cvSize(dst_w,dst_h),src->depth,src->nChannels);
	cvZero(*dst);
	
	bnd_pts[0] = cvPoint2D32f(b[0]/b[8],b[4]/b[8]);
	bnd_pts[1] = cvPoint2D32f(b[1]/b[9],b[5]/b[9]);
	bnd_pts[2] = cvPoint2D32f(b[2]/b[10],b[6]/b[10]);
	bnd_pts[3] = cvPoint2D32f(b[3]/b[11],b[7]/b[11]);

	cvWarpPerspective(src,*dst,map_matrix,flags);

	///为后面的融合消除锯齿
	if(bnd_pts[1].x > bnd_pts[2].x)
	{
		bnd_pts[1].x = bnd_pts[2].x;
		bnd_pts[1].y = max(bnd_pts[0].y, bnd_pts[1].y);

		if(bnd_pts[1].y>0 && bnd_pts[1].y<(*dst)->height)
		{
			cvSetImageROI(*dst,cvRect(0,0,
				(*dst)->width,bnd_pts[1].y+1));
			cvSet(*dst,cvScalarAll(0));
			cvResetImageROI(*dst);
		}

		if((*dst)->width-bnd_pts[2].x>0 && 
			(*dst)->width-bnd_pts[2].x < (*dst)->width)
		{
			cvSetImageROI(*dst,cvRect(bnd_pts[2].x,0,
				(*dst)->width-bnd_pts[2].x+1,(*dst)->height));
			cvSet(*dst,cvScalarAll(0));
			cvResetImageROI(*dst);
		}
		
	}else
	{
		bnd_pts[1].y = max(bnd_pts[0].y, bnd_pts[1].y);

		if(bnd_pts[1].y>0 && bnd_pts[1].y<(*dst)->height)
		{
			cvSetImageROI(*dst,cvRect(0,0,
				(*dst)->width,bnd_pts[1].y+1));
			cvSet(*dst,cvScalarAll(0));
			cvResetImageROI(*dst);
		}
		
		if((*dst)->width-bnd_pts[2].x>0 && 
			(*dst)->width-bnd_pts[2].x < (*dst)->width)
		{
			cvSetImageROI(*dst,cvRect(bnd_pts[1].x,0,
				(*dst)->width-bnd_pts[2].x+1,(*dst)->height));
			cvSet(*dst,cvScalarAll(0));
			cvResetImageROI(*dst);
		}
		
	}

	///
	delete[] a;
	delete[] b;
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
CvMat* veFindPerspectiveMat(const CvPoint2D32f* src,
					 const CvPoint2D32f* dst,
                     CvMat* map_matrix,int ptnum)
{
    double *a = new double[ptnum*2*8];
    double *b = new double[ptnum*2];
	double x[9];

//     CvMat A = cvMat( ptnum*2, 8, CV_64FC1, a );
//     CvMat B = cvMat( ptnum*2, 1, CV_64FC1, b );
//     CvMat X = cvMat( 8, 1, CV_64FC1, x );

	CvMat A,B,X;
	cvInitMatHeader(&A, ptnum*2, 8, CV_64F, a);
    cvInitMatHeader(&B, ptnum*2, 1, CV_64F, b);
	cvInitMatHeader(&X, 8, 1, CV_64F, x);

    int i;

//     if( !src || !dst || !map_matrix )
//         CV_ERROR( CV_StsNullPtr, "" );

    for( i = 0; i < ptnum; ++i )
    {
        a[i*8+0] = a[(i+ptnum)*8+3] = src[i].x;
        a[i*8+1] = a[(i+ptnum)*8+4] = src[i].y;
        a[i*8+2] = a[(i+ptnum)*8+5] = 1;
        a[i*8+3] = a[i*8+4] = a[i*8+5] =
        a[(i+ptnum)*8+0] = a[(i+ptnum)*8+1] = a[(i+ptnum)*8+2] = 0;
        a[i*8+6] = -src[i].x*dst[i].x;
        a[i*8+7] = -src[i].y*dst[i].x;
        a[(i+ptnum)*8+6] = -src[i].x*dst[i].y;
        a[(i+ptnum)*8+7] = -src[i].y*dst[i].y;
        b[i] = dst[i].x;
        b[i+ptnum] = dst[i].y;
    }

    cvSolve( &A, &B, &X, CV_SVD);
    x[8] = 1;
    
    X = cvMat( 3, 3, CV_64FC1, x );
    cvConvert( &X, map_matrix );

  	delete []a;
  	delete []b;

    return map_matrix;
}

void veSaveTransMat( const char *filename,const CvMat *mat,int trans_type)
{
	CvFileStorage* storage = cvOpenFileStorage(filename,NULL,CV_STORAGE_APPEND);

//     if(!storage)
//       CV_ERROR(CV_StsNullPtr,"");
// 
//     if(!CV_IS_MAT(mat))
//        CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	int i,j;
	double val;

	// 变换类型
    char strType[100];
	int Type;
	switch(trans_type)
	{
	case VES_TRANS_TYPE_AFFINE :
         //sprintf(strType,VES_STR_TRANS_TYPE_AFFINE);
		 sprintf_s(strType, VES_STR_TRANS_TYPE_AFFINE);
		 Type = VES_TRANS_TYPE_AFFINE;
		 break;
	
	case VES_TRANS_TYPE_PERSPEC :
         //sprintf(strType,VES_STR_TRANS_TYPE_PERSPEC);
		 sprintf_s(strType, VES_STR_TRANS_TYPE_AFFINE);
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

	cvStartWriteStruct(storage, "Parameters", CV_NODE_SEQ|CV_NODE_MAP);

	CvSize size =  cvGetSize(mat);
	for (i=0;i<size.height;i++)
	   for (j=0;j<size.width;j++)
	   {
		 val = cvGetReal2D(mat,i,j);
         cvWriteRawData(storage,&val,1,"d");
	   }
	cvEndWriteStruct(storage);
    cvReleaseFileStorage(&storage);
}

bool veReadTransMat( const char *filename,CvMat *mat)
{
	CvFileStorage* storage = cvOpenFileStorage(filename,NULL,CV_STORAGE_READ);

//     if(!storage)
//       CV_ERROR(CV_StsNullPtr,"");
// 
// 	if(!CV_IS_MAT(mat))
//        CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	CvSize size = cvGetSize(mat);
	int i,j;
	CvFileNode* trans_type;
	double *dst;

	i=size.height;
	j=size.width;
	dst = new double[i*j];

	trans_type= cvGetFileNodeByName( storage, 0, "Parameters");
 
    cvReadRawData( storage, trans_type, dst, "d");

	for (i=0;i<size.height;i++)
	   for (j=0;j<size.width;j++)
          cvSetReal2D(mat,i,j,dst[i*size.width+j]);

    cvReleaseFileStorage(&storage);
	delete []dst;
	return true;
}

int veAffineRansac(CvMat* mat,CvPoint2D32f* base,CvPoint2D32f* reg,int pt_num,int *pIndex, \
				   int samplesize,double thresold,float prob,int maxiter)
{
	int minK = 10000;
    int i,k=0;
	int inlier = 0,bestinlier=0;
	double frac=1.0; 
  	
  	CvRNG rng_state = cvRNG(0xffffffff);
	CvMat* rmat = cvCloneMat(mat);

	int *idx = new int[samplesize];
    CvPoint2D32f *tmp_reg = new CvPoint2D32f[samplesize];
    CvPoint2D32f *tmp_base = new CvPoint2D32f[samplesize];

	int *tmp_pIndex = new int[pt_num];
  
 	while(minK > k)
	{
//   	   if(k>maxiter)
// 		   break;

       //随机采样samplesize个点
	   for (i=0;i<samplesize;i++)
	   {
	      idx[i] = abs((int)(cvRandInt(&rng_state )%pt_num));
	      tmp_reg[i] = reg[idx[i]];
          tmp_base[i] = base[idx[i]];
	   }

	   //计算变换模型
	   veFindAffineMat(tmp_reg,tmp_base,rmat,samplesize);
 
	   //计算内点的个数
	   inlier = 0;
	   for (i=0;i<pt_num;i++)
	   {
		   double dist = veAffinePtDist(&reg[i],&base[i],rmat);
		   if(dist < thresold)
		   {
		       tmp_pIndex[inlier]=i;
			   inlier++;
		   }
	   }
       
	   //如果优于当前模型，更新之，
	   //并重新计算下一轮迭代次数
	   if(inlier>bestinlier)
	   {
		   bestinlier = inlier;
 		   cvCopy(rmat,mat);
		   memcpy(pIndex,tmp_pIndex,sizeof(int)*pt_num);
		   frac = (double)inlier/(double)pt_num;
		   minK = cvCeil(log(1-prob)/log(1-pow(frac,samplesize)));
		   k = 0;
	   }
	   else
	   {
		   k++;
	   }
	}

	delete []idx;
	delete []tmp_reg;
	delete []tmp_base;
	delete []tmp_pIndex;

	cvReleaseMat(&rmat);

	return bestinlier;
}

int vePerspectiveRansac(CvMat* mat,CvPoint2D32f* base,CvPoint2D32f* reg,int pt_num,int *pIndex, \
				   int samplesize,double thresold,float prob,int maxiter)
{
	int minK = 10000;
    int i,k=0;
	int inlier = 0,bestinlier=0;
	double frac=1.0; 
  	
  	CvRNG rng_state = cvRNG(0xffffffff);
	CvMat* rmat = cvCloneMat(mat);

	int *idx = new int[samplesize];
    CvPoint2D32f *tmp_reg = new CvPoint2D32f[samplesize];
    CvPoint2D32f *tmp_base = new CvPoint2D32f[samplesize];
  
	int *tmp_pIndex = new int[pt_num];

 	while(minK > k)
	{
    	   if(k>maxiter)
  		   break;

       //随机采样samplesize个点
	   for (i=0;i<samplesize;i++)
	   {
	      idx[i] = abs((int)(cvRandInt( &rng_state )%pt_num));
	      tmp_reg[i] = reg[idx[i]];
          tmp_base[i] = base[idx[i]];
	   }

	   //计算变换模型
	   veFindPerspectiveMat(tmp_reg,tmp_base,rmat,samplesize);
 
	   //计算内点的个数
	   inlier = 0;
	   for (i=0;i<pt_num;i++)
	   {
		   double dist = vePerspectivePtDist(&reg[i],&base[i],rmat);
		   if(dist < thresold)
		   {
		       tmp_pIndex[inlier]=i;
			   inlier++;
		   }
	   }
       
	   //如果优于当前模型，更新之，
	   //并重新计算下一轮迭代次数
	   if(inlier>bestinlier)
	   {
		   bestinlier = inlier;
 		   cvCopy(rmat,mat);
   		   memcpy(pIndex,tmp_pIndex,sizeof(int)*pt_num);
		   frac = (double)inlier/(double)pt_num;
		   minK = cvCeil(log(1-prob)/log(1-pow(frac,samplesize)));
		   k = 0;
	   }
	   else
	   {
		   k++;
	   }
	}

	delete []idx;
	delete []tmp_reg;
	delete []tmp_base;
	delete []tmp_pIndex;
	cvReleaseMat(&rmat);

	return bestinlier;
}

double veAffinePtDist(CvPoint2D32f* src,CvPoint2D32f* dst,CvMat* trans_mat)
{
	double distance;

	double scr_pt[3] = {src->x,src->y,1.0f};
	double dst_pt[2] = {0,0};
	double dst_pt_1[2] = {dst->x,dst->y};

// 	if(!CV_IS_MAT(trans_mat))
//        CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");

	CvMat scr_mat  = cvMat(3,1,CV_64F,scr_pt);
 	CvMat dst_mat  = cvMat(2,1,CV_64F,dst_pt);
	CvMat dst_mat_1  = cvMat(2,1,CV_64F,dst_pt_1);
 	
	cvMatMul(trans_mat,&scr_mat,&dst_mat);
	cvSub(&dst_mat,&dst_mat_1,&dst_mat);
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
double vePerspectivePtDist(CvPoint2D32f* src,CvPoint2D32f* dst,CvMat* trans_mat)
{
    
	double distance;
	
	double scr_pt[3] = {src->x,src->y,1.0f};
	double dst_pt[2] = {0,0};
	double dst_pt_1[2] = {dst->x,dst->y};
	
// 	if(!CV_IS_MAT(trans_mat))
// 		CV_ERROR(CV_StsUnsupportedFormat, "Input parameter mat must be a matrix");
	
	CvMat dst_mat  = cvMat(2,1,CV_64F,dst_pt);
	CvMat dst_mat_1  = cvMat(2,1,CV_64F,dst_pt_1);
	
	double m00,m01,m02,m10,m11,m12,m20,m21,m22;
	
	m00 = cvGetReal2D(trans_mat,0,0);
	m01 = cvGetReal2D(trans_mat,0,1);
	m02 = cvGetReal2D(trans_mat,0,2);
	m10 = cvGetReal2D(trans_mat,1,0);
	m11 = cvGetReal2D(trans_mat,1,1);
	m12 = cvGetReal2D(trans_mat,1,2);
	m20 = cvGetReal2D(trans_mat,2,0);
	m21 = cvGetReal2D(trans_mat,2,1);
	m22 = cvGetReal2D(trans_mat,2,2);
	
	dst_pt[0] =  ((m00*src->x+m01*src->y+m02) / (m20*src->x+m21*src->y+m22));
	dst_pt[1] =  ((m10*src->x+m11*src->y+m12) / (m20*src->x+m21*src->y+m22));
	
	cvSub(&dst_mat,&dst_mat_1,&dst_mat);
 	distance = cvNorm(&dst_mat);

 	return distance;
}

void veImageFusionProcess(const IplImage* base, 
						   const IplImage* reg,
						   CvPoint2D32f reg_bndpts[4],
						   IplImage** dst,
						   int left_ex)
{
	int nw = max(base->width,reg->width);
	int nh = max(base->height,reg->height);

	int base_flag = 1, reg_flag = 2, overlap_flag = 3;
	
	IplImage* pBaseMask = cvCreateImage(cvSize(nw,nh),base->depth,base->nChannels);
	cvZero(pBaseMask);
	cvSetImageROI(pBaseMask,cvRect(left_ex,0,base->width-left_ex,base->height));
	cvSet(pBaseMask,cvScalarAll(base_flag));
    cvResetImageROI(pBaseMask);

	IplImage* pOverlap = cvCreateImage(cvSize(nw,nh),reg->depth,reg->nChannels);
	cvZero(pOverlap);
	CvPoint pts[4];
	int extend = 0;
	pts[0] = cvPointFrom32f(reg_bndpts[0]);
	pts[0].x-=extend, pts[0].y-=extend;
	pts[1] = cvPointFrom32f(reg_bndpts[1]);
	pts[1].x+=extend, pts[1].y-=extend;
	pts[2] = cvPointFrom32f(reg_bndpts[2]);
	pts[2].x+=extend, pts[2].y+=extend;
	pts[3] = cvPointFrom32f(reg_bndpts[3]);
	pts[3].x-=extend, pts[3].y+=extend;
	cvFillConvexPoly(pOverlap,pts,4,cvScalarAll(reg_flag));
	cvAdd(pOverlap,pBaseMask,pOverlap);

	cvSetImageROI(pBaseMask,cvRect(0,0,base->width,base->height));
	cvCopy(base,pBaseMask);
    cvResetImageROI(pBaseMask);

	IplImage* pRegTmp = cvCreateImage(cvSize(nw,nh),reg->depth,reg->nChannels);
	cvZero(pRegTmp);
	cvSetImageROI(pRegTmp,cvRect(0,0,reg->width,reg->height));
	cvCopy(reg,pRegTmp);
    cvResetImageROI(pRegTmp);

	//if(*dst)
	//	cvReleaseImage(dst);
	if(*dst==NULL)
		*dst = cvCreateImage(cvSize(nw,nh),base->depth,base->nChannels);
	cvAdd(pBaseMask,pRegTmp,*dst);
	
	int *lb = new int[pRegTmp->height];
	int *ub = new int[pRegTmp->height];
	bool findlb;
	uchar* ptr = NULL;
	for (int i=0; i<pOverlap->height; i++)
	{
		findlb=false;
		lb[i] = 0, ub[i] = pOverlap->width-1;
		ptr = (uchar*) (pOverlap->imageData+i*pOverlap->widthStep);
		for (int j = 0; j < pOverlap->width; j++)
		{
		     if(!findlb && ptr[3*j]==overlap_flag)
			 {
				 findlb = true;
				 lb[i] = j;
			 }
			 if(findlb && ptr[3*j]!=3)
			 {
				 ub[i] = j;
				 break;
			 }
		}
	}

	uchar *dst_ptr,*base_ptr,*reg_ptr;
	for (int i=0; i<pOverlap->height; i++)
	{
		ptr = (uchar*) (pOverlap->imageData+i*pOverlap->widthStep);
		dst_ptr = (uchar*) ((*dst)->imageData+i*(*dst)->widthStep);
		base_ptr = (uchar*) (pBaseMask->imageData+i*pBaseMask->widthStep);
		reg_ptr = (uchar*) (pRegTmp->imageData+i*pRegTmp->widthStep);

		for (int j = 0; j < pOverlap->width; j++)
		{
			if(ptr[3*j]==overlap_flag)
			{
				double ratio=(j-lb[i])/(ub[i]-lb[i]+0.00001f);

				//if(j>=637 && j<=642 && i>=39 && i<=45)  //637,39,6,7)
				   // std::cout << i << " " << ratio << " " << 1.0-ratio << endl;
				
				if(left_ex>0)
					ratio = 1.0 - ratio;

				if(reg_ptr[3*j]==0 && reg_ptr[3*j+1]==0 && reg_ptr[3*j+2]==0)
					ratio = 0;
				if(base_ptr[3*j]==0 && base_ptr[3*j+1]==0 && base_ptr[3*j+2]==0)
					ratio = 1;

				dst_ptr[3*j] = reg_ptr[3*j]*ratio + base_ptr[3*j]*(1-ratio);
				dst_ptr[3*j+1] = reg_ptr[3*j+1]*ratio + base_ptr[3*j+1]*(1-ratio);
				dst_ptr[3*j+2] = reg_ptr[3*j+2]*ratio + base_ptr[3*j+2]*(1-ratio);
			}
		}
	}

	cvReleaseImage(&pBaseMask);
	cvReleaseImage(&pOverlap);
	cvReleaseImage(&pRegTmp);
	delete []lb;
	delete []ub;
}

void veCreateMatMargin(const Mat& src, Mat& dst, int top, int bottom, int left, int right, Scalar color)
{
	if(!src.data)
	{
		printf("Mat data is NULL.\n");
		return ;
	}

	int ex_rows = src.rows+top+bottom;
	int ex_cols = src.cols+left+right;

	if(ex_rows <= 0 || ex_cols <= 0)
		return;

	Mat extend(ex_rows,ex_cols,src.type());
	extend.setTo(color);

	Mat _tmp = extend(Rect(left,top,src.cols,src.rows));
	src.copyTo(_tmp);
	dst = extend.clone();
}

//#define TRANS_TYPE_AFFINE
void veCalcTransformParam(const Mat& baseImg, const Mat& regImg, const char *param_filename)
{
	

}

void veImageMosaic_TwoCam(const Mat& left,  
						  const Mat& right,  
						  const CvMat* param,
						  IplImage** m_pMosaicImage,
						  int left_ex)
{
	IplImage m_pRegImage = right;
	IplImage m_pBaseImage = left;
	IplImage *pTmp = NULL;
	//IplImage *m_pMosaicImage=NULL;
	CvPoint2D32f bnd_pts[4];

	if(param != NULL) 
	{
#ifdef TRANS_TYPE_AFFINE
		veWarpAffine(&m_pRegImage,&pTmp,param,bnd_pts, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
#else
		veWarpPerspective(&m_pRegImage,&pTmp,param,bnd_pts, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
#endif
	}

	//TIMER_BEGIN(1)
	veImageFusionProcess(&m_pBaseImage,pTmp,bnd_pts,m_pMosaicImage,left_ex);
	//TIMER_END(1,"ImageMosaic_fusion");

	cvReleaseImage(&pTmp);
}

#include "windows.h"

static void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void my_imshow(const char* wndName, const Mat& image)
{
	BITMAPINFO bitmapinfo;
	FillBitmapInfo(&bitmapinfo, image.cols, image.rows, image.channels() * 8, 0);

	HWND hWnd = (HWND)cvGetWindowHandle(wndName);

	if (hWnd == NULL)
		cv::namedWindow(wndName);

	PAINTSTRUCT ptStr;
	HDC hDC = BeginPaint(hWnd, &ptStr);   // 开始绘图   
	StretchDIBits(hDC, 0, 0, image.cols + 14, image.rows + 35, 0, 0, \
		image.cols, image.rows, image.data, &bitmapinfo, DIB_RGB_COLORS, SRCCOPY);
	EndPaint(hWnd, &ptStr); // 结束绘图 

	RECT rct;
	rct.left = 0; rct.right = image.cols + 13;
	rct.top = 0; rct.bottom = image.rows + 34;
	InvalidateRect(hWnd, &rct, 0);
}

int my_waikKey(int delay)
{
	int c = 0;
	while (c<delay)
	{
		c++;
		MSG message;
		int is_processed = 0;

		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE)
		{
			Sleep(1);
			continue;
		}

		switch (message.message)
		{
		case WM_DESTROY:
		case WM_CHAR:
			DispatchMessage(&message);
			return (int)message.wParam;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			TranslateMessage(&message);
			if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
				message.wParam == VK_HOME || message.wParam == VK_END ||
				message.wParam == VK_UP || message.wParam == VK_DOWN ||
				message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
				message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
				message.wParam == VK_PRIOR || message.wParam == VK_NEXT)
			{
				DispatchMessage(&message);
				is_processed = 1;
				return (int)(message.wParam << 16);
			}

		default:
			DispatchMessage(&message);
			break;
		}

		if (!is_processed)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////