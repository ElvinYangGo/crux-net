#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

namespace Crux
{
/**
 * our own Queue
 * a rotate queue
 */
template<typename T>
class Queue
{
public:
	/**
	 * rotate queue constructor
	 * @param size the max count of the elements in the queue
	 */
	Queue( size_t size );
	~Queue();

public:
	size_t Size();
	bool PushBack( const T& element );
	void PopFront();
	bool Front( T& element );

private:
	T* m_List;
	size_t m_Size;
	size_t m_Capacity;
private:
	int m_Front;
	int m_Back;
};

template<typename T>
inline
Queue<T>::Queue( size_t size )
: m_List(new T[size]), m_Size(0), m_Capacity(size),
m_Front(0), m_Back(0)
{}

template<typename T>
inline
Queue<T>::~Queue()
{
	delete [] m_List;
	m_List = NULL;
}

template<typename T>
inline
size_t Queue<T>::Size()
{
	return m_Size;
}

template<typename T>
inline
bool Queue<T>::PushBack( const T& element )
{
	if( m_Size == m_Capacity )
	{
		return false;
	}
	m_List[m_Back] = element;
//	m_Back = (m_Back+1) % m_Capacity;
	m_Back = (m_Back+1) % static_cast<int>( m_Capacity );

	++m_Size;
	return true;
}

template<typename T>
inline
bool Queue<T>::Front( T& element )
{
	if( 0 == Size() )
	{
		return false;
	}

	element = m_List[m_Front];

	return true;
}

template<typename T>
inline
void Queue<T>::PopFront()
{
	if( 0 == Size() )
	{
		return;
	}

//	m_Front = (m_Front+1) % m_Capacity;
	m_Front = (m_Front+1) % static_cast<int>( m_Capacity );
	if( m_Front == m_Back )
	{
		m_Front = m_Back = 0;
	}

	--m_Size;
}


} //namespace Crux

#endif //QUEUE_H
