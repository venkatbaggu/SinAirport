#include "Data.h"
#include "Stitch.h"
#include "CameraManager.h"

/**
* @brief CameraManager::CameraManager
*
* 构造函数
*/
CameraManager::CameraManager(void)
{
	// 摄像头SDK初始化
	NET_DVR_Init();
	// 设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
}

/**
* @brief CameraManager::add
*
* 添加摄像头并登陆
* @param[in] ip  摄像头IP地址
* @param[in] port  摄像头连接端口
* @param[in] userName  摄像头登陆用户名
* @param[in] password  摄像头登陆密码
* @return bool 登陆成功返回true,否则返回false
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
* 所有摄像头开始实时预览，获取回调数据
* @param[in] void
* @return bool 成功返回true,否则返回false
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
* 所有摄像头开始实时预览，获取回调数据
* @param[in] void
* @return bool 成功返回true,否则返回false
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
* 根据播放句柄，删除对应摄像头并释放资源
* @param[in] playHandle
* @return bool 成功返回true,否则返回false
*/
bool CameraManager::del(LONG playHandle)
{
	return true;
}

/**
* @brief  CameraManager::write
*
* 根据播放端口号，将回调数据写入相应缓冲区
* @param[in] cur 当前数据
* @param[in] playPort 播放端口号
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
* 关闭所有摄像头，释放相应资源
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
