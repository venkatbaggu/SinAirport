#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include "Singleton.h"

#include <vector>

#include <boost/thread.hpp>

/**
* @class CameraManager
* @brief
*
* 负责连接和管理所有摄像头
*/
class CameraManager : public Singleton<CameraManager>
{
	friend class Singleton<CameraManager>;
	CameraManager(void);

public:
	bool add(const char* ip, int port, 
			const char* userName, const char* password);

	bool del(long playHandle);

	bool start(void);

	bool login(void);

	void release(void);

	void write(const cv::Mat& cur, long playPort);

public:
	boost::mutex m_data;

private:
	std::vector<Camera> m_cameras;
	boost::mutex m_buff;
};

#endif // CAMERA_MANAGER_H

