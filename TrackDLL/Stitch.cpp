#include "Stitch.h"
#include "Data.h"
#include "Global.h"
#include "windows.h"

Stitch::Stitch(void) {

}

int Stitch::init(const string& parameters) {
	stitcher.initialize(parameters);
	return 1;
}

int cnt = 0;

void Stitch::run(void) {

	vector<cv::Mat> frames = Data::instance()->read();

	if (frames[0].empty() || frames[1].empty() ||
		frames[2].empty())  {
		return ;
	} else {
		stitcher.process3cam(frames[0], frames[1], frames[2]);
		stitcher.getPanorama(m_cur);

		//paint.
		cvNamedWindow("stitch", 0);
		imshow("stitch", m_cur);
		cvWaitKey(1);
		//std::cout << "stitch" << std::endl;

		/*cv::String str = cv::format("e:/1/%06d.jpg", cnt++);
		cv::Mat tmp;
		cv::resize(m_cur, tmp, cv::Size(800, 300));
		cv::imwrite(str, tmp);*/

	    //TIMER_BEGIN(1)
		//Mat tmp = m_cur(Rect(0, 0, m_cur.cols / 4 * 4, m_cur.rows)).clone();
		//my_imshow("stitch", tmp);
		//cvWaitKey(1);
		//TIMER_END(1, "clone")
	 
		return ;
	}
}