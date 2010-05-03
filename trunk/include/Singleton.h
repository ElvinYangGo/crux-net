#ifndef SINGLETON_H
#define SINGLETON_H

namespace Crux
{
/**
 * Singleton, used to create single class
 *
 * example:
 * class Foo
 * {
 * 		friend class Singleton<Foo>;
 * 	private:
 * 		Foo() {}
 * 		~Foo() {}
 * 	}
 * 	#define s_Foo Singleton<Foo>::Instance()
 */

template<typename T>
class Singleton
{
public:
	static T& Instance();
};

template<typename T>
inline T& Singleton<T>::Instance()
{
	static T m_instance;
	return m_instance;
}


} //namespace Crux

#endif //SINGLETON_H
