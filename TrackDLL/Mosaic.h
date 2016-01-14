#ifndef MOSAIC_H
#define MOSAIC_H

#include "CU_utility.h"

#include <core/cuda.hpp>
using cv::cuda::GpuMat;

#include <string>
#include <vector>
using std::vector;
using std::string;

/** Image Mosaic */
class __declspec(dllexport) CuMosaic {
public:
	enum { TRANS_TYPE_PERSPEC, TRANS_TYPE_AFFINE };

	/** constructor */
	CuMosaic(const string& param_file = string(),
		int param_index = 0) :
		m_zoom(1.0),
		m_trans_type(TRANS_TYPE_PERSPEC),
		m_paramfile(param_file),
		m_initialized(false)
	{
		if (m_trans_type == TRANS_TYPE_PERSPEC)
		{
			m_param = Mat(3, 3, CV_32F).clone();
		}
		else
		{
			m_param = Mat(2, 3, CV_32F).clone();
		}

		if (param_file.empty())
			m_left_ex_cols = 0;
		else
			loadParam(param_file, param_index);
	}

	int getOrignalRows()
	{
		return m_mosaic.rows;
	}

	/** Destructor */
	virtual ~CuMosaic(void);

	/**
	* Stitch two image
	* @param  base_img   [description]
	* @param  reg_img  [description]
	* @return			  [result]
	*/
	int process(const Mat& base_img,
		const Mat& reg_img);

	int initialize(const Mat& base_img,
		const Mat& reg_img);

	int imageMosaic(const Mat& base_img,
		const Mat& reg_img,
		bool getGpuData = false);

	void setTransType() {};

	void loadParam(const string& file, int param_index = 0);

	const Mat& getPanorama(void) const { return m_mosaic; }
	void getPanorama(Mat& pano) const { pano = m_mosaic; }

	Size getFinalSize(void) const { return m_final_dsize; }

	int		m_left_ex_cols;

private:
	float 	m_zoom;
	int		m_trans_type;

	Mat		m_mosaic;
	GpuMat	mg_mosaic;

	Mat		m_param;
	GpuMat  mg_param;

	string  m_paramfile;

	Mat		m_base_img, m_reg_img, m_base_ovlap;
	GpuMat  mg_base_img, mg_reg_img;
	GpuMat  mg_warp_reg_img;

	Mat     m_baseWeights, m_regWeights;
	GpuMat  mg_baseWeights, mg_regWeights, mg_mosaic_ovlp;
	vector<cv::Point2f> m_bndPts;

	bool    m_initialized;
	Size    m_reg_dsize;
	Size    m_final_dsize;

	Rect	m_overlap_rct;
};

/** Image Stitcher */
class __declspec(dllexport) CuStitcher {
public:

	/** constructor */
	CuStitcher()
	{};

	/** Destructor */
	virtual ~CuStitcher(void){};

	/**
	* Stitch two image
	* @param images   [sorted from left to right]
	* @return			[result]
	*/
	int process2cam(const Mat& left_img,
		const Mat& right_img);

	/**
	* Stitch three image
	* @param  images   [sorted from left to right]
	* @return			[result]
	*/
	int process3cam(const Mat& left_img,
		const Mat& mid_img,
		const Mat& right_img);

	/**
	* Stitch four image
	* @param  images   [sorted from left to right]
	* @return			[result]
	*/
	int process4cam(const Mat& image0,
		const Mat& image1,
		const Mat& image2,
		const Mat& image3);

	int initialize(const std::string& param_files);

	const Mat& getPanorama(void) const { return m_panorama; }
	void getPanorama(Mat& pano) const { pano = m_panorama; }

	void getPanoramaRoi(Mat& pano)
	{
		pano = m_panorama(m_roi_rct);
	}

	Rect getRoiRect(void) const { return m_roi_rct; }
	void setRoiRect(Rect rct) { m_roi_rct = rct; }

private:
	CuMosaic m_mosaic_maker[3];

	Mat		m_panorama;
	GpuMat	mg_panorama;

	//for 3 cameras
	Mat 	m_mid_split[2];
	GpuMat 	mg_mid_split[2];

	//for 4 cameras
	GpuMat	m_images;
	GpuMat  mg_base_img, mg_reg_img;
	GpuMat  mg_warp_reg_img;

	bool    m_initialized;
	Rect	m_roi_rct;
};

#endif 