#pragma once
#include "CList.h"

template<typename T>
class CQueue
{
public:
    void Push(const T& val);
    void Pop();
    T Front();
    void Clear();
    int Size();
    bool Empty();

private:
    CList<T> m_list;
};

template<typename T>
void CQueue<T>::Push(const T& val)
{
    m_list.AddTail(val);
}

template<typename T>
void CQueue<T>::Pop()
{
    m_list.RemoveHead();
}

template<typename T>
T CQueue<T>::Front()
{
    return *(m_list.begin());
}

template<typename T>
void CQueue<T>::Clear()
{
    m_list.Clear();
}

template<typename T>
int CQueue<T>::Size()
{
    return m_list.Size();
}

template<typename T>
inline bool CQueue<T>::Empty()
{
    return m_list.Empty();
}
