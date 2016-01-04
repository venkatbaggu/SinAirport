#include "IDManager.h"

/**
* @brief IDManager::IDManager.
*
* ���캯��
*/
IDManager::IDManager()
	: id(0)
{
}

/**
* @brief IDManager::nextID
*
* ��ȡ��һ��ID
* @param[in] void
* @return ID  ��һ��ID
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
* ���ñ��
* @param[in] void
* @return void
*/
void IDManager::reset(void)
{
	id = 0;
}