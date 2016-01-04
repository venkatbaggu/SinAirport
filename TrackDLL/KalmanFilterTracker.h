#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "DataStructure.h"

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
	void init(const Rect& rect);
	const Rect predict(void);
	const Rect correct(const cv::Rect& rect);

private:
	cv::KalmanFilter kf;
	int dynameCount;	/**< 状态向量维数 */
	int measureCount;	/**< 观测向量维数 */

	Mat measurement;	/**< 观测向量 */
	Mat prediction;		/**< 预测状态 */

	Rect predictedRect;
	Rect correctedRect;
};


#endif // KALMAN_FILTER_H