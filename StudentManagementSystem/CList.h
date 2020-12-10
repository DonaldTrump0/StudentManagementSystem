#pragma once
#include <cassert>

template<typename T>
class CList
{
private:
    class CNode
    {
    public:
        CNode()
        {
            m_pPre = nullptr;
            m_pNext = nullptr;
        }
        CNode(T val)
        {
            m_val = val;
            m_pPre = nullptr;
            m_pNext = nullptr;
        }

    public:
        T m_val;
        CNode* m_pPre;
        CNode* m_pNext;
    };

public:
    class iterator
    {
        friend class CList;
    public:
        iterator& operator++()
        {
            assert(m_pNode != m_pTailGuard);
            m_pNode = m_pNode->m_pNext;
            return *this;
        }
        iterator operator++(int)
        {
            assert(m_pNode != m_pTailGuard);
            m_pNode = m_pNode->m_pNext;
            return iterator(m_pNode->m_pPre, m_pHeadGuard, m_pTailGuard);
        }
        iterator& operator--()
        {
            assert(m_pNode->m_pPre != m_pHeadGuard);
            m_pNode = m_pNode->m_pPre;
            return *this;
        }
        iterator operator--(int)
        {
            assert(m_pNode->m_pPre != m_pHeadGuard);
            m_pNode = m_pNode->m_pPre;
            return iterator(m_pNode->m_pNext, m_pHeadGuard, m_pTailGuard);
        }
        T& operator*()
        {
            return m_pNode->m_val;
        }
        bool operator==(const iterator& itr)
        {
            return m_pNode == itr.m_pNode;
        }
        bool operator!=(const iterator& itr)
        {
            return m_pNode != itr.m_pNode;
        }

    private:
        iterator(CNode* pNode, CNode* pHeadGuard, CNode* pTailGuard) :
            m_pNode(pNode),
            m_pHeadGuard(pHeadGuard),
            m_pTailGuard(pTailGuard)
        {}

    private:
        CNode* m_pNode;
        CNode* m_pHeadGuard;
        CNode* m_pTailGuard;
    };

public:
    class reverse_iterator
    {
    public:
        reverse_iterator& operator++()
        {
            assert(m_pNode != m_pHeadGuard);

            m_pNode = m_pNode->m_pPre;

            return *this;
        }
        reverse_iterator operator++(int)
        {
            assert(m_pNode != m_pHeadGuard);

            m_pNode = m_pNode->m_pPre;

            return reverse_iterator(m_pNode->m_pNext, m_pHeadGuard, m_pTailGuard);
        }
        reverse_iterator& operator--()
        {
            assert(m_pNode->m_pNext != m_pTailGuard);

            m_pNode = m_pNode->m_pNext;

            return *this;
        }
        reverse_iterator operator--(int)
        {
            assert(m_pNode->m_pNext != m_pTailGuard);

            m_pNode = m_pNode->m_pNext;

            return reverse_iterator(m_pNode->m_pPre, m_pHeadGuard, m_pTailGuard);
        }
        T& operator*()
        {
            return m_pNode->m_val;
        }
        bool operator== (const reverse_iterator& itr)
        {
            return m_pNode == itr.m_pNode;
        }
        bool operator!= (const reverse_iterator& itr)
        {
            return m_pNode != itr.m_pNode;
        }
    private:
        friend class CList;
        reverse_iterator(CNode* pNode, CNode* pHeadGuard, CNode* pTailGuard) :
            m_pNode(pNode),
            m_pHeadGuard(pHeadGuard),
            m_pTailGuard(pTailGuard)
        {}

    private:
        CNode* m_pNode;
        CNode* m_pHeadGuard;
        CNode* m_pTailGuard;
    };

public:
    CList();
    CList(const CList<T>& lt);
    ~CList();

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

    // 查询
    CNode* Find(T val);

    // 获取数组元素的个数
    int Size();
    bool Empty();

    // 清空
    void Clear();

    CList<T>& operator=(const CList<T>& lt);

private:
    int m_nSize;
    CNode* m_pHeadGuard;
    CNode* m_pTailGuard;
};

template<typename T>
CList<T>::CList()
{
    m_nSize = 0;
    m_pHeadGuard = new CNode();
    m_pTailGuard = new CNode();
    m_pHeadGuard->m_pNext = m_pTailGuard;
    m_pTailGuard->m_pPre = m_pHeadGuard;
}

template<typename T>
CList<T>::CList(const CList<T>& lt)
{
    *this = lt;
}

template<typename T>
CList<T>::~CList()
{
    CNode* pNode = m_pHeadGuard;
    while (pNode != nullptr)
    {
        CNode* pNext = pNode->m_pNext;
        delete pNode;
        pNode = pNext;
    }
}

template<typename T>
typename CList<T>::iterator CList<T>::begin()
{
    return iterator(m_pHeadGuard->m_pNext, m_pHeadGuard, m_pTailGuard);
}

template<typename T>
typename CList<T>::iterator CList<T>::end()
{
    return iterator(m_pTailGuard, m_pHeadGuard, m_pTailGuard);
}

template<typename T>
typename CList<T>::reverse_iterator CList<T>::rbegin()
{
    return reverse_iterator(m_pTailGuard->m_pPre, m_pHeadGuard, m_pTailGuard);
}

template<typename T>
typename CList<T>::reverse_iterator CList<T>::rend()
{
    return reverse_iterator(m_pHeadGuard, m_pHeadGuard, m_pTailGuard);
}

template<typename T>
inline bool CList<T>::AddHead(T val)
{
    return Insert(begin(), val);
}

template<typename T>
inline bool CList<T>::AddTail(T val)
{
    return Insert(end(), val);
}

template<typename T>
bool CList<T>::Insert(iterator it, T val)
{
    CNode* pNode = it.m_pNode;

    CNode* pNewNode = new CNode(val);
    CNode* pPreNode = pNode->m_pPre;

    pPreNode->m_pNext = pNewNode;
    pNewNode->m_pNext = pNode;
    pNode->m_pPre = pNewNode;
    pNewNode->m_pPre = pPreNode;

    m_nSize++;

    return true;
}

template<typename T>
inline bool CList<T>::RemoveHead()
{
    return Remove(begin());
}

template<typename T>
inline bool CList<T>::RemoveTail()
{
    return Remove(--end());
}

template<typename T>
bool CList<T>::Remove(iterator it)
{
    CNode* pNode = it.m_pNode;

    CNode* pPreNode = pNode->m_pPre;
    CNode* pNextNode = pNode->m_pNext;
    pPreNode->m_pNext = pNextNode;
    pNextNode->m_pPre = pPreNode;

    delete pNode;
    m_nSize--;

    return true;
}

template<typename T>
bool CList<T>::SetVal(iterator it, T val)
{
    it.m_pNode->m_val = val;
    return true;
}

template<typename T>
typename CList<T>::CNode* CList<T>::Find(T val)
{
    CNode* pNode = m_pHeadGuard->m_pNext;
    while (pNode != m_pTailGuard)
    {
        if (pNode->m_val == val)
        {
            return pNode;
        }
    }

    return nullptr;
}

template<typename T>
int CList<T>::Size()
{
    return m_nSize;
}

template<typename T>
bool CList<T>::Empty()
{
    return m_nSize == 0;
}

template<typename T>
void CList<T>::Clear()
{
    while (m_nSize != 0)
    {
        RemoveHead();
    }
}

template<typename T>
CList<T>& CList<T>::operator=(const CList<T>& lt)
{
    m_nSize = 0;
    m_pHeadGuard = new CNode();
    m_pTailGuard = new CNode();
    m_pHeadGuard->m_pNext = m_pTailGuard;
    m_pTailGuard->m_pPre = m_pHeadGuard;

    CNode* pNode = lt.m_pHeadGuard->m_pNext;
    while (pNode != lt.m_pTailGuard)
    {
        AddTail(pNode->m_val);
        pNode = pNode->m_pNext;
    }

    return *this;
}
