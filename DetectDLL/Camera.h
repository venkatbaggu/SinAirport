#ifndef CAMERA_H
#define CAMERA_H

#include <future>
#include <string>
using std::string;

#include <boost/bind.hpp>

#include <opencv.hpp>

#include "HCNetSDK.h"
#include "plaympeg4.h"

void CALLBACK dataCallBack(
	long nPort, char* pBuf, long nSize, FRAME_INFO *pFrame,
	long nReserved1, long nReserved2);

void CALLBACK decodeCallBack(
	LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,
	DWORD dwBufSize, void* dwUser);

void CALLBACK exceptionCallBack(
	DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);


/**
* @class Camera
* @brief
*
* 负责连接和管理单个摄像头
*/
class Camera
{
public:
	Camera(const string& ip, WORD port, 
				const string& userName, const string& password);
	Camera(void);
	~Camera();

	bool login(void);
	bool start(void);
	bool stop(void);

	void release(void);

public:
	LONG playPort;

private:
	string m_ip;
	WORD m_port;
	string m_userName;
	string m_password;

	NET_DVR_DEVICEINFO_V30 m_deviceInfo;
	NET_DVR_PREVIEWINFO m_previewInfo;

	LONG m_userID;
	LONG m_playHandle;
};

#endif // CAMERA_H

