#include "IDManager.h"

/**
* @brief IDManager::IDManager.
*
* 构造函数
*/
IDManager::IDManager()
	: id(0)
{
}

/**
* @brief IDManager::nextID
*
* 获取下一个ID
* @param[in] void
* @return ID  下一个ID
*/
ID IDManager::nextID(void)
{
	if (0 > ++id)
	{
		id = 1;
	}
	return id;
}

/**
* @brief IDManager::reset
*
* 重置编号
* @param[in] void
* @return void
*/
void IDManager::reset(void)
{
	id = 0;
}