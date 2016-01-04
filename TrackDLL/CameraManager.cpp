#include "Data.h"
#include "Stitch.h"
#include "CameraManager.h"

/**
* @brief CameraManager::CameraManager
*
* ���캯��
*/
CameraManager::CameraManager(void)
{
	// ����ͷSDK��ʼ��
	NET_DVR_Init();
	// ��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
}

/**
* @brief CameraManager::add
*
* �������ͷ����½
* @param[in] ip  ����ͷIP��ַ
* @param[in] port  ����ͷ���Ӷ˿�
* @param[in] userName  ����ͷ��½�û���
* @param[in] password  ����ͷ��½����
* @return bool ��½�ɹ�����true,���򷵻�false
*/
bool CameraManager::add(const char* ip, int port,
	const char* userName, const char* password)
{
	if (nullptr == ip || 
		port <= 0 || 
		nullptr == userName || 
		nullptr == password) {
		return false;
	}

	m_cameras.push_back(Camera(ip, port, userName, password));
	return true;
}

/**
* @brief CameraManager::loginAll
*
* ��������ͷ��ʼʵʱԤ������ȡ�ص�����
* @param[in] void
* @return bool �ɹ�����true,���򷵻�false
*/
bool CameraManager::login(void)
{
	const auto end = m_cameras.end();
	for (auto iter = m_cameras.begin();
		end != iter;
		++iter) {
		if (!iter->login())
			return false;
	}

	return true;
}

/**
* @brief CameraManager::startAll
*
* ��������ͷ��ʼʵʱԤ������ȡ�ص�����
* @param[in] void
* @return bool �ɹ�����true,���򷵻�false
*/
bool CameraManager::start(void)
{
	const auto end = m_cameras.end();
	for (auto iter = m_cameras.begin();
		end != iter;
		++iter) {
		if (!iter->start())
			return false;
	}
	
	return true;
}

/**
* @brief  CameraManager::del
*
* ���ݲ��ž����ɾ����Ӧ����ͷ���ͷ���Դ
* @param[in] playHandle
* @return bool �ɹ�����true,���򷵻�false
*/
bool CameraManager::del(LONG playHandle)
{
	return true;
}

/**
* @brief  CameraManager::write
*
* ���ݲ��Ŷ˿ںţ����ص�����д����Ӧ������
* @param[in] cur ��ǰ����
* @param[in] playPort ���Ŷ˿ں�
* @return void
*/
void CameraManager::write(const cv::Mat& cur, long playPort)
{
	if (cur.empty() || playPort < 0)
		return;

	if (!m_buff.try_lock())
		return;

	cv::Mat rgb;
	cv::cvtColor(cur, rgb, CV_YUV2BGR_YV12);
	cv::resize(rgb, rgb, cv::Size(rgb.cols / 2, rgb.rows / 2));
	if (playPort == m_cameras[0].playPort) {
		Data::instance()->writeLeft(rgb);
	} else if (playPort == m_cameras[1].playPort) {
		Data::instance()->writeMid(rgb);
	} else {
		Data::instance()->writeRight(rgb);
	}

	//
	//std::async(&Stitch::run, Stitch::instance());
	Stitch::instance()->run();

	m_buff.unlock();

	return;
}

/**
* @brief  CameraManager::del
*
* �ر���������ͷ���ͷ���Ӧ��Դ
* @param[in] void
* @return void
*/
void CameraManager::release(void)
{
	const auto end = m_cameras.end();
	for (auto iter = m_cameras.begin();
		end != iter;
		++iter) {
		iter->release();
	}

	NET_DVR_Cleanup();
	return ;
}
