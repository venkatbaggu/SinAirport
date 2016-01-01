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
