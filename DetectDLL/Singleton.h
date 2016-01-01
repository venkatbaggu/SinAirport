#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
using std::shared_ptr;

/**
* @class Singleton
* @brief
*
* ��������
*/
template< typename Type>
class Singleton
{
public:
	/****************************************************************************/
	//*	@brief		��ȡ����ָ��
	//*	@return		����ĵ���ָ��
	/****************************************************************************/
	static const shared_ptr<Type> instance()
	{
		static shared_ptr<Type> instance(new Type);
		return instance;
	}

protected:
	Singleton(){}

private:
	Singleton(const Singleton<Type>&);
};

#endif // SINGLETON_H
