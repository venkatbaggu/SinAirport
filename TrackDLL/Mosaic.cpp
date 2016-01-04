#include <iostream>
#include <fstream>
#include <windows.h>

#include "Mosaic.h"

using namespace std;
using namespace cv;
using namespace cv::cuda;

HANDLE evtTerminate; //事件信号
static long ThreadCompleted = 0;

#include <boost/thread/thread.hpp>

struct threadInfo
{
	const Mat *base_img;
	const Mat *reg_img;

	CuMosaic* stitcher;
};

//线程定义
DWORD WINAPI  _stitch(LPVOID lpParam)
{
	Mat base_img, reg_img;
	CuMosaic *stitcher = NULL;
	try
	{
		//cout << lpParam << endl;

		base_img = *(((threadInfo*)lpParam)->base_img);
		reg_img = *(((threadInfo*)lpParam)->reg_img);
		stitcher = ((threadInfo*)lpParam)->stitcher;

		stitcher->process(base_img, reg_img);
	}
	catch (...)
	{
		cout << base_img.rows << reg_img.rows << endl;
		cout << stitcher->getPanorama().rows << endl;
	}

	InterlockedIncrement(&ThreadCompleted);
	if (ThreadCompleted == 2)
		SetEvent(evtTerminate);

	return 0;
}

int CuStitcher::initialize(const string& param_file)
{
	FileStorage fr(param_file.c_str(), FileStorage::READ);
	FileNode tm = fr["Output_ROI"];
	m_roi_rct = Rect((int)tm["x"], (int)tm["y"], \
		(int)tm["width"], (int)tm["height"]);

	int param_cnt = (int)fr["Trans_Param_Count"];

	for (int i = 0; i<param_cnt; i++)
		m_mosaic_maker[i].loadParam(param_file, i);

	m_initialized = true;

	return 1;
}

int CuStitcher::process2cam(const Mat& left_img,
	const Mat& right_img)
{
	if (!m_initialized)
		return 0;

	return m_mosaic_maker[0].process(left_img, right_img);
}

int CuStitcher::process3cam(const Mat& left_img,
	const Mat& mid_img,
	const Mat& right_img)
{
	if (!m_initialized)
		return 0;

	Rect left_part(0, 0, mid_img.cols / 2, mid_img.rows);
	Rect right_part(mid_img.cols / 2, 0,
				mid_img.cols / 2, mid_img.rows);
	if (m_mid_split[0].empty() || m_mid_split[1].empty())
	{
		m_mid_split[0] = mid_img(left_part).clone();
		mg_mid_split[0].create(Size(left_part.width, left_part.height),
			mid_img.type());

		m_mid_split[1] = mid_img(right_part).clone();
		mg_mid_split[1].create(Size(right_part.width, right_part.height),
			mid_img.type());
	}

	mid_img(left_part).copyTo(m_mid_split[0]);
	mid_img(right_part).copyTo(m_mid_split[1]);

	/*
	threadInfo left_data, right_data;
	left_data.base_img = &(m_mid_split[0]);
	left_data.reg_img = &left_img;
	left_data.stitcher = &(m_mosaic_maker[0]);

	right_data.base_img = &(m_mid_split[1]);
	right_data.reg_img = &right_img;
	right_data.stitcher = &(m_mosaic_maker[1]);
	*/

	//evtTerminate = CreateEvent(NULL, FALSE, FALSE, "Terminate");
	//evtTerminate = CreateEventA(NULL, FALSE, FALSE, "Terminate");

	////分别建立各个子线程
	//unsigned long TID1=0, TID2=0;
	//HANDLE handle1 = CreateThread(NULL, 0, &_stitch, &left_data, NULL, &TID1);
	//HANDLE handle2 = CreateThread(NULL, 0, &_stitch, &right_data, NULL, &TID2);

	////cout << handle1 << " " << handle2 << endl;

	//WaitForSingleObject(evtTerminate, INFINITE);
	//ThreadCompleted = 0;

	//CloseHandle(handle1);
	//CloseHandle(handle2);

	/*
	boost::thread_group threads;
	threads.create_thread(boost::bind(_stitch, &left_data));
	threads.create_thread(boost::bind(_stitch, &right_data));
	threads.join_all();
	*/

	//以m_mid_split[0]为基准图像
	m_mosaic_maker[0].process(m_mid_split[0],left_img);
	//并行处理右边的拼接
	m_mosaic_maker[1].process(m_mid_split[1],right_img);

	//左右的拼接结果连接起来
	int ex_len = m_mosaic_maker[0].m_left_ex_cols;
	int _height = min(m_mosaic_maker[0].getFinalSize().height,
		m_mosaic_maker[1].getFinalSize().height);

	if (_height == 0)
		return -1;

	//if (1)
	//{
	//	cout << m_mid_split[0].cols << endl;
	//	cout << m_mid_split[1].cols << endl;
	//	cout << left_img.cols << endl;
	//	cout << right_img.cols << endl;
	//	cout << m_mosaic_maker[0].getFinalSize().height << endl;
	//	cout << m_mosaic_maker[1].getFinalSize().height << endl;
	//}
	//	
	Rect left_msc_rct(0, 0, ex_len + m_mid_split[0].cols, _height);
	Rect right_msc_rct(0, 0, m_mosaic_maker[1].getFinalSize().width, _height);

	if (m_panorama.empty())
		m_panorama.create(_height, ex_len + m_mid_split[0].cols +
		m_mosaic_maker[1].getFinalSize().width, mid_img.type());

	Mat left_pano = m_mosaic_maker[0].getPanorama();
	left_pano = left_pano(left_msc_rct);
	Mat right_pano = m_mosaic_maker[1].getPanorama();
	right_pano = right_pano(right_msc_rct);
	right_msc_rct.x += ex_len + m_mid_split[0].cols;
	left_pano.copyTo(m_panorama(left_msc_rct));
	right_pano.copyTo(m_panorama(right_msc_rct));

	return 1;
}

int CuStitcher::process4cam(const Mat& left_img,
	const Mat& mid0_img,
	const Mat& mid1_img,
	const Mat& right_img)
{
	if (!m_initialized)
		return 0;

	threadInfo left_data, right_data;
	left_data.base_img = &mid0_img;
	left_data.reg_img = &left_img;
	left_data.stitcher = &(m_mosaic_maker[0]);

	right_data.base_img = &mid1_img;
	right_data.reg_img = &right_img;
	right_data.stitcher = &(m_mosaic_maker[1]);

	//evtTerminate = CreateEvent(NULL, FALSE, FALSE, "Terminate");
	evtTerminate = CreateEventA(NULL, FALSE, FALSE, "Terminate");

	//分别建立各个子线程
	unsigned long TID1, TID2;
	HANDLE handle1 = CreateThread(NULL, 0, &_stitch, &left_data, NULL, &TID1);
	HANDLE handle2 = CreateThread(NULL, 0, &_stitch, &right_data, NULL, &TID2);

	WaitForSingleObject(evtTerminate, INFINITE);
	ThreadCompleted = 0;

	CloseHandle(handle1);
	CloseHandle(handle2);

	//左右的拼接结果拼接一起
	Mat left_part = m_mosaic_maker[0].getPanorama();
	Mat right_part = m_mosaic_maker[1].getPanorama();
	m_mosaic_maker[2].process(left_part, right_part);

	return 1;
}
int CuMosaic::initialize(const Mat& base_img,
	const Mat& reg_img)
{
	if (base_img.empty() || reg_img.empty())
		return -1;
	Mat ex_img;
	try{
		m_bndPts = vector<Point2f>(4);

		ex_img = base_img.clone();
		if (m_left_ex_cols > 0)
			cuCreateMatMargin(ex_img, ex_img, 0, 0, m_left_ex_cols, 0, Scalar(0));
	}
	catch (...)
	{
		cout << "77777777aa!!!!" << endl;
	}

	Mat tmp_reg;
	try{
		m_reg_dsize = cuCalcBoundPts(reg_img, m_param, m_bndPts);

		tmp_reg = Mat(m_reg_dsize, reg_img.type());
		mg_reg_img.upload(reg_img);
		mg_warp_reg_img.create(m_reg_dsize, reg_img.type());
		cuWarpPerspective(mg_reg_img, mg_warp_reg_img,
			m_param, m_bndPts, m_reg_dsize);
	}
	catch (...)
	{
		cout << "88888888888888!!!!" << endl;
	}

	try{
		try{
			mg_warp_reg_img.download(tmp_reg);
			m_overlap_rct = cuCreateWeightsMatrix(ex_img, tmp_reg, m_reg_dsize, m_bndPts,
				m_baseWeights, m_regWeights, m_left_ex_cols);
		}
		catch (...)
		{
			cout << tmp_reg.cols << endl;
			cout << "9999999999aaaaaaa!!!!" << endl;
		}
		try{

			m_base_ovlap = Mat::zeros(m_overlap_rct.height,
				m_overlap_rct.width, base_img.type());

			mg_baseWeights.upload(m_baseWeights);
			mg_regWeights.upload(m_regWeights);

			m_final_dsize = Size(m_baseWeights.cols,
				m_baseWeights.rows);

			m_mosaic = Mat(m_final_dsize, base_img.type());
			mg_mosaic.create(m_final_dsize, base_img.type());
		}
		catch (...)
		{
			cout << "9999999999bbbbbb!!!!" << endl;
		}

	}
	catch (...)
	{
		cout << "999999999999!!!!" << endl;
	}
	m_initialized = true;
	return 1;
}

int CuMosaic::imageMosaic(const Mat& base_img,
	const Mat& reg_img,
	bool getGpuData)
{
	try
	{
		//TIMER_BEGIN(2);
		mg_reg_img.upload(reg_img);
		cuWarpPerspective(mg_reg_img, mg_warp_reg_img,
			m_param, m_bndPts, m_final_dsize);
		//TIMER_END(2,"cuWarpPerspective Time");
	}
	catch (...)
	{
		cout << mg_warp_reg_img.cols << endl;
		cout << m_final_dsize.width << endl;
		cout << "33333!!!!" << endl;
	}

	try
	{
		if (m_left_ex_cols > 0)
		{
			Rect base_ovlp_rct(0, 0, m_base_ovlap.cols, base_img.rows);
			base_img(base_ovlp_rct).copyTo(m_base_ovlap(Rect(0, 0,
				m_base_ovlap.cols, base_img.rows)));
		}
		else
		{
			Rect base_ovlp_rct(m_overlap_rct.x, 0,
				base_img.cols - m_overlap_rct.x, base_img.rows);
			base_img(base_ovlp_rct).copyTo(m_base_ovlap(Rect(0, 0,
				base_img.cols - m_overlap_rct.x, base_img.rows)));
		}
	}
	catch (...)
	{
		cout << "444444!!!!" << endl;
	}

	try
	{
		GpuMat _base_ovlp(m_base_ovlap);
		GpuMat _reg_ovlp = mg_warp_reg_img(m_overlap_rct);
		GpuMat _base_wt_ovlp = mg_baseWeights(m_overlap_rct);
		GpuMat _reg_wt_ovlp = mg_regWeights(m_overlap_rct);
		//GpuMat _mosaic_ovlp;

	//TIMER_BEGIN(3);
#ifdef _SIMPLE_FUSION_
		cuImageFusionSimpleProcess(_base_ovlp, _reg_ovlp, _base_wt_ovlp, \
			_reg_wt_ovlp, _mosaic_ovlp);
#else
		cuImageFusionProcess(_base_ovlp, _reg_ovlp, _base_wt_ovlp, \
			_reg_wt_ovlp, mg_mosaic_ovlp);
#endif
	

	//TIMER_END(3,"cuImageFusionProcess Time");

	if (getGpuData)
	{
		//TIMER_BEGIN(4);
		mg_warp_reg_img.copyTo(mg_mosaic);

		if (m_left_ex_cols > 0)
			mg_mosaic(Rect(m_left_ex_cols, 0, base_img.cols,
			base_img.rows)).upload(base_img);
		else
			mg_mosaic(Rect(0, 0, base_img.cols, base_img.rows)).upload(base_img);

		mg_mosaic_ovlp.copyTo(mg_mosaic(m_overlap_rct));
		//TIMER_END(4, "make mosaic Time");
	}
	else
	{
		//TIMER_BEGIN(5);
		mg_warp_reg_img.download(m_mosaic);
		if (m_left_ex_cols > 0)
			base_img.copyTo(m_mosaic(Rect(m_left_ex_cols, 0,
			base_img.cols, base_img.rows)));
		else
			base_img.copyTo(m_mosaic(Rect(0, 0, base_img.cols, base_img.rows)));
		mg_mosaic_ovlp.download(m_mosaic(m_overlap_rct));
		//TIMER_END(5, "make mosaic Time");
	}
	}
	catch (...)
	{
		cout << "66666666!!!!" << endl;
		Mat reg;
		mg_warp_reg_img.download(reg);
		imwrite("err_base.jpg", m_base_ovlap);
		imwrite("err_reg.jpg", reg);
	}

	return 1;
}

int CuMosaic::process(const Mat& base_img, const Mat& reg_img)
{
	if (base_img.empty() || reg_img.empty())
		return -1;
	try
	{
	if (!m_initialized)
		initialize(base_img, reg_img);
	m_base_img = base_img;
	m_reg_img = reg_img;
	}
	catch (...)
	{
		cout << "11111!!!!!!" << endl;
	}

	/*if (m_left_ex_cols > 0)
	{
	m_base_img = base_img.clone();
	cuCreateMatMargin(m_base_img, m_base_img, 0, 0,
	m_left_ex_cols, 0, Scalar(0));
	}*/
	try
	{
		imageMosaic(m_base_img, m_reg_img);
	}
	catch(...)
	{
		cout << "22222!!!!!!" << endl;
	}

	return 1;
}

void CuMosaic::loadParam(const string& file, int param_index)
{
	m_paramfile = file;
	cuReadTransMat(m_paramfile, m_param, param_index);
	FileStorage fr(m_paramfile.c_str(), FileStorage::READ);
	cv::String node = format("Left_Extend_Cols_%01d", param_index);
	m_left_ex_cols = (int)fr[node];
}

CuMosaic::~CuMosaic(void)
{

}
