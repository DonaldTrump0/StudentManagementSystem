#pragma once
#include <iostream>
#include <stdexcept>
#include <cassert>
using namespace std;

// 数组初始大小
#define INITIAL_SIZE 16

template<typename T>
class CVector
{
public:
    class iterator
    {
        friend class CVector;

    private:
        iterator(T* pBuff, int nIndex, int nElementSize)
        {
            m_pBuff = pBuff;
            m_nIndex = nIndex;
            m_nElementSize = nElementSize;
        }

    public:
        iterator& operator++()
        {
            assert(m_nIndex != m_nElementSize);
            m_nIndex++;
            return *this;
        }
        iterator operator++(int)
        {
            assert(m_nIndex != m_nElementSize);
            m_nIndex++;
            return iterator(m_pBuff, m_nIndex - 1, m_nElementSize);
        }
        iterator& operator--()
        {
            assert(m_nIndex != 0);
            m_nIndex--;
            return *this;
        }
        iterator operator--(int)
        {
            assert(m_nIndex != 0);
            m_nIndex--;
            return iterator(m_pBuff, m_nIndex + 1, m_nElementSize);
        }
        T& operator*()
        {
            assert(m_nIndex != m_nElementSize);
            return m_pBuff[m_nIndex];
        }
        bool operator==(const iterator& it)
        {
            return m_nIndex == it.m_nIndex;
        }
        bool operator!=(const iterator& it)
        {
            return m_nIndex != it.m_nIndex;
        }

    private:
        T* m_pBuff;
        int m_nIndex;
        int m_nElementSize;
    };

public:
    class reverse_iterator
    {
        friend class CVector;

    private:
        reverse_iterator(T* pBuff, int nIndex, int nElementSize)
        {
            m_pBuff = pBuff;
            m_nIndex = nIndex;
            m_nElementSize = nElementSize;
        }

    public:
        reverse_iterator& operator++()
        {
            assert(m_nIndex != -1);
            m_nIndex--;
            return *this;
        }
        reverse_iterator operator++(int)
        {
            assert(m_nIndex != -1);
            m_nIndex--;
            return reverse_iterator(m_pBuff, m_nIndex + 1, m_nElementSize);
        }
        reverse_iterator& operator--()
        {
            assert(m_nIndex != m_nElementSize - 1);
            m_nIndex++;
            return *this;
        }
        reverse_iterator operator--(int)
        {
            assert(m_nIndex != m_nElementSize - 1);
            m_nIndex++;
            return reverse_iterator(m_pBuff, m_nIndex - 1, m_nElementSize);
        }
        T& operator*()
        {
            assert(m_nIndex != m_nElementSize);
            return m_pBuff[m_nIndex];
        }
        bool operator==(const reverse_iterator& it)
        {
            return m_nIndex == it.m_nIndex;
        }
        bool operator!=(const reverse_iterator& it)
        {
            return m_nIndex != it.m_nIndex;
        }

    private:
        T* m_pBuff;
        int m_nIndex;
        int m_nElementSize;
    };

public:
    CVector();
    CVector(const CVector<T>& vt);
    ~CVector();

    iterator begin();
    iterator end();
    reverse_iterator rbegin();
    reverse_iterator rend();

    // 增加
    bool AddHead(T val);
    bool AddTail(T val);
    bool Insert(iterator it, T val);

    // 删除
    bool RemoveHead();
    bool RemoveTail();
    bool Remove(iterator it);

    // 修改
    bool SetVal(iterator it, T val);
    T& operator[](int nIndex);

    // 查询
    int Find(T val);

    // 获取数组元素的个数
    int Size();

    // 清空
    void Clear();

private:
    T* m_pBuf;
    int m_nBufSize;
    int m_nElementSize;
};


template<typename T>
CVector<T>::CVector()
{
    m_nElementSize = 0;
    m_nBufSize = INITIAL_SIZE;
    m_pBuf = new T[INITIAL_SIZE];
}

template<typename T>
CVector<T>::CVector(const CVector<T>& vt)
{
    m_nElementSize = vt.m_nElementSize;
    m_nBufSize = m_nElementSize;
    memmove(m_pBuf, vt.m_pBuf, m_nElementSize * sizeof(T));
}

template<typename T>
CVector<T>::~CVector()
{
    delete[] m_pBuf;
}

template<typename T>
typename CVector<T>::iterator CVector<T>::begin()
{
    return iterator(m_pBuf, 0, m_nElementSize);
}

template<typename T>
typename CVector<T>::iterator CVector<T>::end()
{
    return iterator(m_pBuf, m_nElementSize, m_nElementSize);
}

template<typename T>
typename CVector<T>::reverse_iterator CVector<T>::rbegin()
{
    return reverse_iterator(m_pBuf, m_nElementSize - 1, m_nElementSize);
}

template<typename T>
typename CVector<T>::reverse_iterator CVector<T>::rend()
{
    return reverse_iterator(m_pBuf, -1, m_nElementSize);
}

template<typename T>
bool CVector<T>::AddHead(T val)
{
    return Insert(begin(), val);
}

template<typename T>
bool CVector<T>::AddTail(T val)
{
    return Insert(end(), val);
}

template<typename T>
bool CVector<T>::Insert(iterator it, T val)
{
    int nIndex = it.m_nIndex;
    if (nIndex < 0 || nIndex > m_nElementSize)
    {
        return false;
    }

    // 2倍扩容
    if (m_nElementSize == m_nBufSize)
    {
        m_nBufSize *= 2;
        T* pBuf = new T[m_nBufSize];
        memmove(pBuf, m_pBuf, m_nElementSize * sizeof(T));
        delete m_pBuf;
        m_pBuf = pBuf;
    }

    memmove(m_pBuf + nIndex + 1, m_pBuf + nIndex, (m_nElementSize - nIndex) * sizeof(T));
    m_pBuf[nIndex] = val;
    m_nElementSize++;

    return true;
}

template<typename T>
bool CVector<T>::RemoveHead()
{
    return Remove(begin());
}

template<typename T>
bool CVector<T>::RemoveTail()
{
    return Remove(--end());
}

template<typename T>
bool CVector<T>::Remove(iterator it)
{
    int nIndex = it.m_nIndex;
    if (nIndex < 0 || nIndex >= m_nElementSize)
    {
        return false;
    }

    memmove(m_pBuf + nIndex, m_pBuf + nIndex + 1, (m_nElementSize - nIndex - 1) * sizeof(T));
    m_nElementSize--;

    return true;
}

template<typename T>
bool CVector<T>::SetVal(iterator it, T val)
{
    int nIndex = it.m_nIndex;
    if (nIndex < 0 || nIndex >= m_nElementSize)
    {
        return false;
    }

    m_pBuf[nIndex] = val;

    return true;
}

template<typename T>
T& CVector<T>::operator[](int nIndex)
{
    if (nIndex < 0 || nIndex >= m_nElementSize)
    {
        throw out_of_range("访问越界");
    }

    return m_pBuf[nIndex];
}

template<typename T>
int CVector<T>::Find(T val)
{
    for (int i = 0; i < m_nElementSize; i++)
    {
        if (m_pBuf[i] == val)
        {
            return i;
        }
    }

    return -1;
}

template<typename T>
int CVector<T>::Size()
{
    return m_nElementSize;
}

template<typename T>
void CVector<T>::Clear()
{
    m_nElementSize = 0;
}
