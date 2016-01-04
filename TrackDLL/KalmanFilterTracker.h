#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "DataStructure.h"

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
	void init(const Rect& rect);
	const Rect predict(void);
	const Rect correct(const cv::Rect& rect);

private:
	cv::KalmanFilter kf;
	int dynameCount;	/**< ״̬����ά�� */
	int measureCount;	/**< �۲�����ά�� */

	Mat measurement;	/**< �۲����� */
	Mat prediction;		/**< Ԥ��״̬ */

	Rect predictedRect;
	Rect correctedRect;
};


#endif // KALMAN_FILTER_H