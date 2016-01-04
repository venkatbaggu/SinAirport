#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include "Global.h"
#include "Singleton.h"

/**
* @class IDManager
* @brief
*
* 负责管理目标ID
* @note 只能被TargetManager类对象调用
*/
class IDManager : public Singleton<IDManager>
{
	friend class Singleton<IDManager>;

protected:
	IDManager();
public:
	void reset(void);
	ID nextID(void);

private:
	ID id;	/**< 当前ID */
};

#endif // ID_MANAGER_H

