#ifndef DATA_H
#define DATA_H

#include "Singleton.h"

#include <vector>

#include <boost/circular_buffer.hpp>

#include <opencv.hpp>

class Data : public Singleton<Data>
{	
	friend class Singleton<Data>;
	Data(void);

public:
	void writeLeft(const cv::Mat& cur);
	void writeMid(const cv::Mat& cur);
	void writeRight(const cv::Mat& cur);

	std::vector<cv::Mat> read(void);

private:
	boost::circular_buffer<cv::Mat> m_left;
	boost::circular_buffer<cv::Mat> m_mid;
	boost::circular_buffer<cv::Mat> m_right;
};


#endif // DATA_H