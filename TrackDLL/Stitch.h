#ifndef STITCH_H
#define STITCH_H

#include "Mosaic.h"
#include "Singleton.h"

/**
* @class Stitch
* @brief
*
* 拼接类，用于拼接摄像头
* 调用顺序：init->iniLogging->run
* 使用类中静态函数方式实现
*/
class Stitch : public Singleton<Stitch> {
	friend class Singleton<Stitch>;
	Stitch(void);

public:
	/*
	* @brief init
	*
	* 初始化拼接参数
	* @param[in] parameters 拼接参数文件
	* @return int 成功返回值0，不成功则返回出错代码（<0）
	*/
	int init(const string& parameters);

	/*
	* @brief run
	*
	* 运行目标检测算法
	* @param[in] left 当前帧（左摄像头）
	* @param[in] mid 当前帧（中摄像头）
	* @param[in] right 当前帧（右摄像头）
	* @return mat 成功返回拼接结果，不成功返回值为空
	*/
	//cv::Mat run(const cv::Mat& left, 
	//				const cv::Mat& mid, const cv::Mat& right);

	void run(void);

private:
	CuStitcher stitcher;
	cv::Mat m_cur;
};

#endif // STITCH_H