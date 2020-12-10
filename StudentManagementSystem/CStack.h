#pragma once
#include "CList.h"

template<typename T>
class CStack
{
public:
    void Push(const T& val);
    void Pop();
    T Top();
    void Clear();
    bool Empty();
    int Size();

private:
    CList<T> m_list;
};

template<typename T>
void CStack<T>::Push(const T& val)
{
    m_list.AddTail(val);
}

template<typename T>
void CStack<T>::Pop()
{
    m_list.RemoveTail();
}

template<typename T>
T CStack<T>::Top()
{
    return *(--m_list.end());
}

template<typename T>
void CStack<T>::Clear()
{
    m_list.Clear();
}

template<typename T>
bool CStack<T>::Empty()
{
    return m_list.Empty();
}

template<typename T>
inline int CStack<T>::Size()
{
    return m_list.Size();
}

