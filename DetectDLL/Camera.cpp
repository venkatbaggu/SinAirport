#include "Camera.h"
#include "DetectManager.h"

/**
* @brief Camera::Camera
*
* 构造函数
* 未使用的指针为空，未使用的变量为-1
* 并为指针分配空间
*/
Camera::Camera(const string& ip, WORD port, 
	const string& userName, const string& password) :
	m_ip(ip), m_port(port), m_userName(userName), m_password(password),
	m_userID(-1), m_playHandle(-1), playPort(-1)
{
	m_previewInfo = { 0 };
	//需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	m_previewInfo.hPlayWnd = NULL;
	//预览通道号
	m_previewInfo.lChannel = 1;
	//0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	m_previewInfo.dwStreamType = 0;
	//0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
	m_previewInfo.dwLinkMode = 0;
}

/**
* @brief Camera::Camera
*
* 构造函数
* 未使用的指针为空，未使用的变量为-1
*/
Camera::Camera(void):
	m_port(-1), m_userID(-1), m_playHandle(-1), playPort(-1)
{
	m_previewInfo = { 0 };
	//需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	m_previewInfo.hPlayWnd = NULL;
	//预览通道号
	m_previewInfo.lChannel = 1;
	//0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	m_previewInfo.dwStreamType = 0;
	//0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
	m_previewInfo.dwLinkMode = 0;
}

/**
* @brief Camera::~Camera
*
* 析构函数
* 释放摄像头相关资源
*/
Camera::~Camera()
{
	this->release();
}

/**
* @brief Camera::login
*
* 摄像头登陆，获取userID
* @param[in] void
* @return bool 成功返回true,否则返回false.
*/
bool Camera::login(void)
{
	m_userID = 
		NET_DVR_Login_V30(
		const_cast<char*>(m_ip.c_str()), 
		m_port, 
		const_cast<char*>(m_userName.c_str()),
		const_cast<char*>(m_password.c_str()),
		&m_deviceInfo);

	if (m_userID < 0) {
		return false;
	}

	return true;
}

/**
* @brief Camera::start
*
* 通过userID实时预览，获取playHandle
* @param[in] void
* @return bool 成功返回true，否则返回false
*/
bool Camera::start(void)
{
	m_playHandle = 
		NET_DVR_RealPlay_V40(m_userID, 
			&m_previewInfo, decodeCallBack, this);

	if (m_playHandle < 0) {
		NET_DVR_Logout(m_userID);
		return false;
	}

	return true;
}

/**
* @brief Camera::stop
*
* 停止实时预览
* @param[in] void
* @return bool 成功返回true，否则返回false
*/
bool Camera::stop(void)
{
	return NET_DVR_StopRealPlay(m_playHandle);
}

/**
* @brief Camera::release
*
* 停止实时预览
* @param[in] void
* @return bool 成功返回true，否则返回false
*/
void Camera::release(void)
{
	//关闭预览
	if (-1 != m_playHandle) {
		NET_DVR_StopRealPlay(m_playHandle);
	}

	//释放播放库资源
	if (-1 != playPort) {
		PlayM4_Stop(playPort);
		PlayM4_CloseStream(playPort);
		PlayM4_FreePort(playPort);
	}

	//注销用户
	if (-1 != m_userID) {
		NET_DVR_Logout(m_userID);
	}

	return;
}


void CALLBACK dataCallBack(
	long nPort, char* pBuf, long nSize,
	FRAME_INFO *pFrame, long nReserved1, long nReserved2)
{
	DetectManager::instance()->data.lock();

	std::async(boost::bind(&DetectManager::update, DetectManager::instance(),
		cv::Mat(pFrame->nHeight, pFrame->nWidth, CV_8UC1, pBuf)));

	//std::cout << "call update" << std::endl;

	DetectManager::instance()->data.unlock();

	return;
}

void CALLBACK decodeCallBack(
	LONG lRealHandle, DWORD dwDataType,
	BYTE *pBuffer, DWORD dwBufSize, void* dwUser)
{
	Camera* cur = static_cast<Camera*>(dwUser);
	
	switch (dwDataType) {

		//系统头
		case NET_DVR_SYSHEAD: {

			if (false == PlayM4_GetPort(&cur->playPort)) {
				std::cerr << "GetPort Error." << std::endl;
				break;
			}

			if (dwBufSize > 0) {

				if (false == PlayM4_SetStreamOpenMode(
					cur->playPort, STREAME_REALTIME)) {
					std::cerr << "SetStream Error." << std::endl;
					break;
				}

				if (false == PlayM4_OpenStream(
					cur->playPort, pBuffer, dwBufSize, 3 * 1920 * 1080)) {
					std::cerr << "OpenStream Error." << std::endl;
					break;
				}

				if (false == PlayM4_SetDecCBStream(cur->playPort, 1)) {
					std::cerr << "SetDecStream Error." << std::endl;
					break;
				}
			
				if (false == PlayM4_SetDecCallBack(
					cur->playPort, dataCallBack)) {
					std::cerr << "SetDecCallBack Error." << std::endl;
					break;
				}

				if (false == PlayM4_Play(cur->playPort, NULL)) {
					std::cerr << "Play Error." << std::endl;
					break;
				}
			}

			break;
		}
		
		//码流数据
		case NET_DVR_STREAMDATA: {
			if (dwBufSize > 0 && cur->playPort != -1) {
				if (false == PlayM4_InputData(
					cur->playPort, pBuffer, dwBufSize)) {
					std::cerr << "InputData Error." << std::endl;
					break;
				}
			}

			break;
		}
	
		//其他数据
		default: {
			if (dwBufSize > 0 && cur->playPort != -1) {
				if (false == PlayM4_InputData(
					cur->playPort, pBuffer, dwBufSize)) {
					std::cerr << "InputData Error." << std::endl;
					break;
				}
			}

			break;
		}
	}
}

void CALLBACK exceptionCallBack(
	DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	switch (dwType) { 
		//预览时重连
		case EXCEPTION_RECONNECT: {
			std::cerr << "exception reconnect." << std::endl;
			break;
		}

		default: {
			break;
		}
	}
}

