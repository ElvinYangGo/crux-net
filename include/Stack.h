#ifndef STACK_H
#define STACK_H

namespace Crux
{

/**
 * our own stack
 */
template<typename T>
class Stack
{
public:
	/**
	 * stack constructor
	 * @param size the max count of the elements in stack
	 */
	Stack( size_t size );
	~Stack();

public:
	size_t Size() const;
	bool Push( const T& element );
	bool Pop();
	bool Top( T& element );

private:
	T* m_List;
	size_t m_Size;
	size_t m_Capacity;
private:
	int m_Top;
};

template<typename T>
inline
Stack<T>::Stack( size_t size )
: m_List( new T[size] ), m_Size(0), m_Capacity(size), m_Top(-1)
{
}

template<typename T>
inline
Stack<T>::~Stack()
{
	delete [] m_List;
	m_List = NULL;
}

template<typename T>
inline
size_t Stack<T>::Size() const
{
	return m_Size;
}

template<typename T>
inline
bool Stack<T>::Push( const T& element )
{
	if( m_Capacity <= Size() )
	{
		return false;
	}

	++m_Top;
	m_List[m_Top] = element;

	++m_Size;

	return true;
}

template<typename T>
inline
bool Stack<T>::Pop()
{
	if( Size() <= 0 )
	{
		return false;
	}

	--m_Top;
	
	--m_Size;

	return true;
}

template<typename T>
inline
bool Stack<T>::Top( T& element )
{
	if( Size() <= 0 )
	{
		return false;
	}

	element = m_List[m_Top];

	return true;
}


}

#endif //STACK_H
