#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "DataStructure.h"
#include <opencv.hpp>

/**
* @class KalmanFilterTracker
* @brief
*
* ʵ���˿������˲�������
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
	int dynameCount;	/**< ״̬����ά�� */
	int measureCount;	/**< �۲�����ά�� */

	cv::Mat measurement;	/**< �۲����� */
	cv::Mat prediction;		/**< Ԥ��״̬ */

	cv::Rect predictedRect;
	cv::Rect correctedRect;
};


#endif // KALMAN_FILTER_H