#pragma once
#include <iostream>
#include "CList.h"
#include "CStack.h"
#include "CQueue.h"
using namespace std;

template<typename T>
class CAVLTree
{
private:
    typedef struct tagNode
    {
        tagNode(T val, int nHeight, tagNode* pParent, tagNode* pLeft, tagNode* pRight) :
            m_val(val),
            m_nHeight(nHeight),
            m_pParent(pParent),
            m_pLeft(pLeft),
            m_pRight(pRight)
        {}

        T m_val;
        int m_nHeight;

        tagNode* m_pParent;
        tagNode* m_pLeft;
        tagNode* m_pRight;
    } NODE, * PNODE;

public:
    CAVLTree();
    CAVLTree(const CAVLTree& bst);
    CAVLTree& operator=(const CAVLTree& bst);
    virtual ~CAVLTree();

    bool Insert(T val);
    bool Remove(T val);
    bool Update(T valSrc, T valDst);
    T Find(T val);
    CList<T> FindAll(T val);
    void Clear();

    // 递归
    void LMR(); // 中序遍历
    void MLR(); // 前序遍历
    void LRM(); // 后序遍历

    // 非递归
    void MLRUseLoop();  // 前序遍历
    void LMRUseLoop();  // 中序遍历
    void LRMUseLoop();  // 后序遍历

    // 层序遍历
    void Level();

    void PrintTree();

private:
    PNODE FindNode(T val);
    void DelNode(PNODE pNode);
    void DelLeaf(PNODE pNode);
    void DelSingleChildNode(PNODE pNode);
    void DelDoubleChildNode(PNODE pNode);
    void LMR(PNODE pNode);
    void MLR(PNODE pNode);
    void LRM(PNODE pNode);

private:
    int GetHeight(PNODE pNode);
    void UpdateHeight(PNODE pNode);
    void AdjustHeight(PNODE pNode);
    void RotateLeft(PNODE pNode);
    void RotateRight(PNODE pNode);

private:
    void _FindAll(CList<T>& lt, PNODE pNode, T val);

private:
    PNODE m_pRoot;
};

template<typename T>
CAVLTree<T>::CAVLTree()
{
    m_pRoot = nullptr;
}

template<typename T>
CAVLTree<T>::CAVLTree(const CAVLTree& bst)
{
    *this = bst;
}

template<typename T>
CAVLTree<T>& CAVLTree<T>::operator=(const CAVLTree& bst)
{
    if (this == &bst)
    {
        return *this;
    }

    Clear();

    if (bst.m_pRoot == nullptr)
    {
        return *this;
    }

    CQueue<T> que;
    que.Push(bst.m_pRoot);
    while (!que.Empty());
    {
        PNODE pNode = que.Front();
        que.Pop();
        if (pNode->m_pLeft != nullptr)
        {
            que.Push(pNode->m_pLeft);
        }
        if (pNode->m_pRight != nullptr)
        {
            que.Push(pNode->m_pRight);
        }
        Insert(pNode->m_val);
    }

    return *this;
}

template<typename T>
CAVLTree<T>::~CAVLTree()
{
    Clear();
}

template<typename T>
bool CAVLTree<T>::Insert(T val)
{
    if (m_pRoot == nullptr)
    {
        m_pRoot = new NODE(val, 0, nullptr, nullptr, nullptr);
        return true;
    }

    PNODE pNode = m_pRoot;
    while (true)
    {
        if (pNode->m_val > val)
        {
            if (pNode->m_pLeft == nullptr)
            {
                pNode->m_pLeft = new NODE(val, 0, pNode, nullptr, nullptr);
                AdjustHeight(pNode);
                return true;
            }
            else
            {
                pNode = pNode->m_pLeft;
            }
        }
        else if (pNode->m_val < val)
        {
            if (pNode->m_pRight == nullptr)
            {
                pNode->m_pRight = new NODE(val, 0, pNode, nullptr, nullptr);
                AdjustHeight(pNode);
                return true;
            }
            else
            {
                pNode = pNode->m_pRight;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

template<typename T>
bool CAVLTree<T>::Remove(T val)
{
    PNODE pNode = FindNode(val);

    if (pNode == nullptr)
    {
        return false;
    }

    DelNode(pNode);

    return true;
}

template<typename T>
bool CAVLTree<T>::Update(T valSrc, T valDst)
{
    PNODE pNode = FindNode(valSrc);
    if (pNode == nullptr)
    {
        return false;
    }

    DelNode(pNode);
    Insert(valDst);

    return true;
}

template<typename T>
T CAVLTree<T>::Find(T val)
{
    PNODE pNode = FindNode(val);
    if (pNode == nullptr)
    {
        return T();
    }
    else 
    {
        return pNode->m_val;
    }
}

template<typename T>
void CAVLTree<T>::_FindAll(CList<T>& lt, PNODE pNode, T val)
{
    while (pNode != nullptr)
    {
        if (pNode->m_val > val)
        {
            pNode = pNode->m_pLeft;
        }
        else if (pNode->m_val < val)
        {
            pNode = pNode->m_pRight;
        }
        else
        {
            lt.AddTail(pNode->m_val);
            _FindAll(lt, pNode->m_pLeft, val);
            _FindAll(lt, pNode->m_pRight, val);
            return;
        }
    }
}

template<typename T>
CList<T> CAVLTree<T>::FindAll(T val)
{
    CList<T> lt;
    _FindAll(lt, m_pRoot, val);
    return lt;
}

template<typename T>
void CAVLTree<T>::Clear()
{
    if (m_pRoot == nullptr)
    {
        return;
    }

    CQueue<PNODE> que;
    que.Push(m_pRoot);

    while (!que.Empty())
    {
        PNODE pNode = que.Front();
        que.Pop();
        if (pNode->m_pLeft != nullptr)
        {
            que.Push(pNode->m_pLeft);
        }
        if (pNode->m_pRight != nullptr)
        {
            que.Push(pNode->m_pRight);
        }
        delete pNode;
        pNode = nullptr;
    }

    m_pRoot = nullptr;
}

template<typename T>
void CAVLTree<T>::LMR()
{
    LMR(m_pRoot);
}

template<typename T>
void CAVLTree<T>::MLR()
{
    MLR(m_pRoot);
}

template<typename T>
void CAVLTree<T>::LRM()
{
    LRM(m_pRoot);
}

template<typename T>
void CAVLTree<T>::MLRUseLoop()
{
    CStack<PNODE> stk;
    PNODE pNode = m_pRoot;

    while (true)
    {
        while (pNode != nullptr)
        {
            cout << pNode->m_val << " ";
            if (pNode->m_pRight != nullptr)
            {
                stk.Push(pNode->m_pRight);
            }
            pNode = pNode->m_pLeft;
        }

        if (stk.Empty())
        {
            break;
        }

        pNode = stk.Top();
        stk.Pop();
    }
}

template<typename T>
void CAVLTree<T>::LMRUseLoop()
{
    CStack<PNODE> stk;
    PNODE pNode = m_pRoot;

    while (true)
    {
        while (pNode != nullptr)
        {
            stk.Push(pNode);
            pNode = pNode->m_pLeft;
        }

        if (stk.Empty())
        {
            break;
        }

        pNode = stk.Top();
        stk.Pop();
        cout << pNode->m_val << " ";

        pNode = pNode->m_pRight;
    }
}

template<typename T>
void CAVLTree<T>::LRMUseLoop()
{
    CStack<PNODE> stk;
    PNODE pNode = m_pRoot;
    PNODE pPreNode = nullptr;

    while (true)
    {
        while (pNode != nullptr)
        {
            stk.Push(pNode);
            pNode = pNode->m_pLeft;
        }

        if (stk.Empty())
        {
            break;
        }

        pNode = stk.Top();
        if (pNode->m_pRight == nullptr || pNode->m_pRight == pPreNode)
        {
            stk.Pop();
            cout << pNode->m_val << " ";
            pPreNode = pNode;
            pNode = nullptr;
        }
        else
        {
            pNode = pNode->m_pRight;
        }
    }
}

template<typename T>
void CAVLTree<T>::Level()
{
    if (m_pRoot == nullptr)
    {
        return;
    }

    CQueue<PNODE> que;
    que.Push(m_pRoot);
    while (!que.Empty())
    {
        PNODE pNode = que.Front();
        que.Pop();
        cout << pNode->m_val << " ";
        if (pNode->m_pLeft != nullptr)
        {
            que.Push(pNode->m_pLeft);
        }
        if (pNode->m_pRight != nullptr)
        {
            que.Push(pNode->m_pRight);
        }
    }
}

template<typename T>
void CAVLTree<T>::PrintTree()
{

}

template<typename T>
typename CAVLTree<T>::PNODE CAVLTree<T>::FindNode(T val)
{
    PNODE pNode = m_pRoot;
    while (pNode != nullptr)
    {
        if (pNode->m_val > val)
        {
            pNode = pNode->m_pLeft;
        }
        else if (pNode->m_val < val)
        {
            pNode = pNode->m_pRight;
        }
        else
        {
            return pNode;
        }
    }
    return nullptr;
}

template<typename T>
void CAVLTree<T>::DelNode(PNODE pNode)
{
    if (pNode->m_pLeft == nullptr && pNode->m_pRight == nullptr)
    {
        DelLeaf(pNode);
    }
    else if (pNode->m_pLeft != nullptr && pNode->m_pRight != nullptr)
    {
        DelDoubleChildNode(pNode);
    }
    else
    {
        DelSingleChildNode(pNode);
    }
}

template<typename T>
void CAVLTree<T>::DelLeaf(PNODE pNode)
{
    PNODE pParentNode = pNode->m_pParent;

    if (pParentNode == nullptr)
    {
        m_pRoot = nullptr;
    }
    else
    {
        pParentNode->m_pLeft == pNode ? pParentNode->m_pLeft = nullptr : pParentNode->m_pRight = nullptr;
        AdjustHeight(pParentNode);
    }

    delete pNode;
    pNode = nullptr;
}

template<typename T>
void CAVLTree<T>::DelSingleChildNode(PNODE pNode)
{
    PNODE pChildNode = pNode->m_pLeft != nullptr ? pNode->m_pLeft : pNode->m_pRight;
    PNODE pParentNode = pNode->m_pParent;

    pChildNode->m_pParent = pParentNode;

    if (pParentNode == nullptr)
    {
        m_pRoot = pChildNode;
    }
    else
    {
        pParentNode->m_pLeft == pNode ? pParentNode->m_pLeft = pChildNode : pParentNode->m_pRight = pChildNode;
        AdjustHeight(pParentNode);
    }

    delete pNode;
    pNode = nullptr;
}

template<typename T>
void CAVLTree<T>::DelDoubleChildNode(PNODE pNode)
{
    PNODE pMaxNode = pNode->m_pLeft;
    while (pMaxNode->m_pRight != nullptr)
    {
        pMaxNode = pMaxNode->m_pRight;
    }

    pNode->m_val = pMaxNode->m_val;

    DelNode(pMaxNode);
}

template<typename T>
void CAVLTree<T>::LMR(PNODE pNode)
{
    if (pNode == nullptr)
    {
        return;
    }

    LMR(pNode->m_pLeft);
    cout << pNode->m_val << " ";
    LMR(pNode->m_pRight);
}

template<typename T>
void CAVLTree<T>::MLR(PNODE pNode)
{
    if (pNode == nullptr)
    {
        return;
    }

    cout << pNode->m_val << " ";
    MLR(pNode->m_pLeft);
    MLR(pNode->m_pRight);
}

template<typename T>
void CAVLTree<T>::LRM(PNODE pNode)
{
    if (pNode == nullptr)
    {
        return;
    }

    LRM(pNode->m_pLeft);
    LRM(pNode->m_pRight);
    cout << pNode->m_val << " ";
}

template<typename T>
int CAVLTree<T>::GetHeight(PNODE pNode)
{
    return pNode == nullptr ? -1 : pNode->m_nHeight;
}

template<typename T>
void CAVLTree<T>::UpdateHeight(PNODE pNode)
{
    int nLeftHeight = GetHeight(pNode->m_pLeft);
    int nRightHeight = GetHeight(pNode->m_pRight);
    pNode->m_nHeight = nLeftHeight > nRightHeight ? (nLeftHeight + 1) : (nRightHeight + 1);
}

template<typename T>
void CAVLTree<T>::AdjustHeight(PNODE pNode)
{
    while (pNode != nullptr)
    {
        UpdateHeight(pNode);

        int nLeftHeight = GetHeight(pNode->m_pLeft);
        int nRightHeight = GetHeight(pNode->m_pRight);
        if (nLeftHeight - nRightHeight == -2)
        {
            if (GetHeight(pNode->m_pRight->m_pLeft) - GetHeight(pNode->m_pRight->m_pRight) == 1)
            {
                RotateRight(pNode->m_pRight);
            }
            RotateLeft(pNode);
        }
        else if (nLeftHeight - nRightHeight == 2)
        {
            if (GetHeight(pNode->m_pLeft->m_pLeft) - GetHeight(pNode->m_pLeft->m_pRight) == -1)
            {
                RotateLeft(pNode->m_pLeft);
            }
            RotateRight(pNode);
        }

        // 沿着父节点
        pNode = pNode->m_pParent;
    }
}

template<typename T>
void CAVLTree<T>::RotateLeft(PNODE pNode)
{
    PNODE pParent = pNode->m_pParent;
    PNODE pRightSon = pNode->m_pRight;
    PNODE pRightSonLeftSon = pNode->m_pRight->m_pLeft;

    if (pParent != nullptr)
    {
        pParent->m_pLeft == pNode ? pParent->m_pLeft = pRightSon : pParent->m_pRight = pRightSon;
    }
    else
    {
        m_pRoot = pRightSon;
    }

    pNode->m_pParent = pRightSon;
    pNode->m_pRight = pRightSonLeftSon;

    pRightSon->m_pParent = pParent;
    pRightSon->m_pLeft = pNode;
    
    if (pRightSonLeftSon != nullptr)
    {
        pRightSonLeftSon->m_pParent = pNode;
    }

    UpdateHeight(pNode);
}

template<typename T>
void CAVLTree<T>::RotateRight(PNODE pNode)
{
    PNODE pParent = pNode->m_pParent;
    PNODE pLeftSon = pNode->m_pLeft;
    PNODE pLeftSonRightSon = pNode->m_pLeft->m_pRight;

    if (pParent != nullptr)
    {
        pParent->m_pLeft == pNode ? pParent->m_pLeft = pLeftSon : pParent->m_pRight = pLeftSon;
    }
    else
    {
        m_pRoot = pLeftSon;
    }

    pNode->m_pParent = pLeftSon;
    pNode->m_pLeft = pLeftSonRightSon;

    pLeftSon->m_pParent = pParent;
    pLeftSon->m_pRight = pNode;

    if (pLeftSonRightSon != nullptr)
    {
        pLeftSonRightSon->m_pParent = pNode;
    }

    UpdateHeight(pNode);
}
