#include "Camera.h"
#include "DetectManager.h"

/**
* @brief Camera::Camera
*
* ���캯��
* δʹ�õ�ָ��Ϊ�գ�δʹ�õı���Ϊ-1
* ��Ϊָ�����ռ�
*/
Camera::Camera(const string& ip, WORD port, 
	const string& userName, const string& password) :
	m_ip(ip), m_port(port), m_userName(userName), m_password(password),
	m_userID(-1), m_playHandle(-1), playPort(-1)
{
	m_previewInfo = { 0 };
	//��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	m_previewInfo.hPlayWnd = NULL;
	//Ԥ��ͨ����
	m_previewInfo.lChannel = 1;
	//0-��������1-��������2-����3��3-����4���Դ�����
	m_previewInfo.dwStreamType = 0;
	//0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP
	m_previewInfo.dwLinkMode = 0;
}

/**
* @brief Camera::Camera
*
* ���캯��
* δʹ�õ�ָ��Ϊ�գ�δʹ�õı���Ϊ-1
*/
Camera::Camera(void):
	m_port(-1), m_userID(-1), m_playHandle(-1), playPort(-1)
{
	m_previewInfo = { 0 };
	//��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	m_previewInfo.hPlayWnd = NULL;
	//Ԥ��ͨ����
	m_previewInfo.lChannel = 1;
	//0-��������1-��������2-����3��3-����4���Դ�����
	m_previewInfo.dwStreamType = 0;
	//0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP
	m_previewInfo.dwLinkMode = 0;
}

/**
* @brief Camera::~Camera
*
* ��������
* �ͷ�����ͷ�����Դ
*/
Camera::~Camera()
{
	this->release();
}

/**
* @brief Camera::login
*
* ����ͷ��½����ȡuserID
* @param[in] void
* @return bool �ɹ�����true,���򷵻�false.
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
* ͨ��userIDʵʱԤ������ȡplayHandle
* @param[in] void
* @return bool �ɹ�����true�����򷵻�false
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
* ֹͣʵʱԤ��
* @param[in] void
* @return bool �ɹ�����true�����򷵻�false
*/
bool Camera::stop(void)
{
	return NET_DVR_StopRealPlay(m_playHandle);
}

/**
* @brief Camera::release
*
* ֹͣʵʱԤ��
* @param[in] void
* @return bool �ɹ�����true�����򷵻�false
*/
void Camera::release(void)
{
	//�ر�Ԥ��
	if (-1 != m_playHandle) {
		NET_DVR_StopRealPlay(m_playHandle);
	}

	//�ͷŲ��ſ���Դ
	if (-1 != playPort) {
		PlayM4_Stop(playPort);
		PlayM4_CloseStream(playPort);
		PlayM4_FreePort(playPort);
	}

	//ע���û�
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

		//ϵͳͷ
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
		
		//��������
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
	
		//��������
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
		//Ԥ��ʱ����
		case EXCEPTION_RECONNECT: {
			std::cerr << "exception reconnect." << std::endl;
			break;
		}

		default: {
			break;
		}
	}
}

