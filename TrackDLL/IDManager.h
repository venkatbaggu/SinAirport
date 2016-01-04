#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include "Global.h"
#include "Singleton.h"

/**
* @class IDManager
* @brief
*
* �������Ŀ��ID
* @note ֻ�ܱ�TargetManager��������
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
	ID id;	/**< ��ǰID */
};

#endif // ID_MANAGER_H

