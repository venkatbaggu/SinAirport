#include "KalmanFilterTracker.h"
#include "Global.h"

/**
* @brief KalmanFilterTracker::KalmanFilterTracker.
*
* ���캯��
* @param[in] dynamParams  ״̬����ά��
* @param[in] measureParams  �۲�����ά��
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
* ��ʼ���������˲���
* @param[in] rect  Ŀ������
* @return void
*/
void KalmanFilterTracker::init(const cv::Rect& rect)
{
	measurement = cv::Mat::zeros(measureCount, 1, CV_32F);

	//��ʼ��״̬����
	kf.statePost.at<float>(0, 0) = float(rect.x);
	kf.statePost.at<float>(1, 0) = float(rect.y);
	kf.statePost.at<float>(2, 0) = float(rect.width);
	kf.statePost.at<float>(3, 0) = float(rect.height);
	for (int i = 4; i < 8; i++)
	{
		kf.statePost.at<float>(i, 0) = 0;
	}

	//��ʼ��״̬ת�ƾ���
	setIdentity(kf.transitionMatrix);//�˴��Ĳ���������ά�ȶ����ģ�״̬ת�ƾ���
	for (int i = 0; i < dynameCount / 2; i++)//�ٶ������е�delta��Ϊ1
	{
		int j = dynameCount / 2 + i;
		kf.transitionMatrix.at<float>(i, j) = 1;
	}

	setIdentity(kf.measurementMatrix);//��ʼ���۲����Ϊ��λ��
	setIdentity(kf.processNoiseCov, cv::Scalar::all(1e-5));
	setIdentity(kf.measurementNoiseCov, cv::Scalar::all(1e-1));
	setIdentity(kf.errorCovPost, cv::Scalar::all(1));
}

/**
* @brief KalmanFilterTracker::predict
*
* Ԥ��Ŀ������
* @param[in] void
* @return const cv::Rect  Ԥ���Ŀ������
*/
const cv::Rect KalmanFilterTracker::predict(void)
{
	prediction = kf.predict();//Ԥ��󷵻�״̬��һ���Ƕȣ�
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
* ����Ŀ������
* @param[in] rect  ��ǰĿ������
* @return const cv::Rect  �������Ŀ������
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
