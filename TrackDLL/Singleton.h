#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
using std::shared_ptr;

/**
* @class Singleton
* @brief
*
* 单件基类
*/
template< typename Type>
class Singleton
{
public:
	/****************************************************************************/
	//*	@brief		获取单件指针
	//*	@return		共享的单件指针
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
