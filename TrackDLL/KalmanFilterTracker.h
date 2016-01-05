#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "DataStructure.h"
#include <opencv.hpp>

/**
* @class KalmanFilterTracker
* @brief
*
* 实现了卡尔曼滤波跟踪器
*/
class KalmanFilterTracker
{
public:
	KalmanFilterTracker(const int dynamParams = 8, const int measureParams = 4);
	void init(const cv::Rect& rect);
	const cv::Rect predict(void);
	const cv::Rect correct(const cv::Rect& rect);

private:
	cv::KalmanFilter kf;
	int dynameCount;	/**< 状态向量维数 */
	int measureCount;	/**< 观测向量维数 */

	cv::Mat measurement;	/**< 观测向量 */
	cv::Mat prediction;		/**< 预测状态 */

	cv::Rect predictedRect;
	cv::Rect correctedRect;
};


#endif // KALMAN_FILTER_H