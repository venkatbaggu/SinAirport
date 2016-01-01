#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <vector>
using std::vector;

#include "Camera.h"
#include "Singleton.h"

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

private:
	vector<Camera> m_cameras;
};

#endif // CAMERA_MANAGER_H

