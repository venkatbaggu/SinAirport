#include "KalmanFilterTracker.h"
#include "Global.h"

/**
* @brief KalmanFilterTracker::KalmanFilterTracker.
*
* 构造函数
* @param[in] dynamParams  状态向量维数
* @param[in] measureParams  观测向量维数
*/
KalmanFilterTracker::KalmanFilterTracker(const int dynamParams, const int measureParams)
	: kf(dynamParams, measureParams)
	, dynameCount(dynamParams)
	, measureCount(measureParams)
{
}

/**
* @brief KalmanFilterTracker::init
*
* 初始化卡尔曼滤波器
* @param[in] rect  目标区域
* @return void
*/
void KalmanFilterTracker::init(const cv::Rect& rect)
{
	measurement = cv::Mat::zeros(measureCount, 1, CV_32F);

	//初始化状态向量
	kf.statePost.at<float>(0, 0) = float(rect.x);
	kf.statePost.at<float>(1, 0) = float(rect.y);
	kf.statePost.at<float>(2, 0) = float(rect.width);
	kf.statePost.at<float>(3, 0) = float(rect.height);
	for (int i = 4; i < 8; i++)
	{
		kf.statePost.at<float>(i, 0) = 0;
	}

	//初始化状态转移矩阵
	setIdentity(kf.transitionMatrix);//此处的参数随输入维度而更改，状态转移矩阵
	for (int i = 0; i < dynameCount / 2; i++)//速度所在列的delta设为1
	{
		int j = dynameCount / 2 + i;
		kf.transitionMatrix.at<float>(i, j) = 1;
	}

	setIdentity(kf.measurementMatrix);//初始化观测矩阵为单位阵
	setIdentity(kf.processNoiseCov, cv::Scalar::all(1e-5));
	setIdentity(kf.measurementNoiseCov, cv::Scalar::all(1e-1));
	setIdentity(kf.errorCovPost, cv::Scalar::all(1));
}

/**
* @brief KalmanFilterTracker::predict
*
* 预测目标区域
* @param[in] void
* @return const cv::Rect  预测的目标区域
*/
const cv::Rect KalmanFilterTracker::predict(void)
{
	prediction = kf.predict();//预测后返回状态（一个角度）
	predictedRect.x = int(prediction.at<float>(0, 0));
	predictedRect.y = int(prediction.at<float>(1, 0));
	predictedRect.width = int(prediction.at<float>(2, 0));
	predictedRect.height = int(prediction.at<float>(3, 0));
	predictedRect &= gWholeRect;
	return predictedRect;
}

/**
* @brief KalmanFilterTracker::correct
*
* 修正目标区域
* @param[in] rect  当前目标区域
* @return const cv::Rect  修正后的目标区域
*/
const cv::Rect KalmanFilterTracker::correct(const cv::Rect& rect)
{
	measurement.at<float>(0, 0) = float(rect.x);
	measurement.at<float>(1, 0) = float(rect.y);
	measurement.at<float>(2, 0) = float(rect.width);
	measurement.at<float>(3, 0) = float(rect.height);

	kf.correct(measurement);

	correctedRect.x = int(kf.statePost.at<float>(0, 0));
	correctedRect.y = int(kf.statePost.at<float>(1, 0));
	correctedRect.width = int(kf.statePost.at<float>(2, 0));
	correctedRect.height = int(kf.statePost.at<float>(3, 0));
	correctedRect &= gWholeRect;
	return correctedRect;
}
