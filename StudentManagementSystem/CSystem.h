#pragma once
#include <iostream>
#include "CStudent.h"
#include "CString.h"
#include "CList.h"
#include "CAVLTree.h"
using namespace std;

// 存放数据的文件
#define FILE_NAME "data.bin"
// 一次读取的最大值
#define LINE_SIZE 32
// 空闲链表头部的偏移
#define FREE_HEAD_OFFSET 0
// 已使用链表头部的偏移
#define USED_HEAD_OFFSET sizeof(tagBlock)

class CSystem
{
private:
    // 记录存储信息的块
    struct tagBlock 
    {
        int m_nPre;     // 上一个块的偏移
        int m_nNext;    // 下一个块的偏移
        int m_nSize;    // 块后面存储空间的大小(不包括块的大小)
    };

private:
    // AVL树节点(根据Id构建)
    struct tagIdNode
    {
        int m_nId;      // 学生的Id
        int m_nOffset;  // 学生信息存储位置的偏移

        tagIdNode(int nId = -1, int nOffset = -1) : m_nId(nId), m_nOffset(nOffset) {}

        bool operator>(const tagIdNode& idNode)
        {
            return m_nId > idNode.m_nId;
        }
        bool operator<(const tagIdNode& idNode)
        {
            return m_nId < idNode.m_nId;
        }
        bool operator==(const tagIdNode& idNode)
        {
            return m_nId == idNode.m_nId;
        }
    };

private:
    // AVL树节点(根据学生姓名构建)
    // 同时根据m_strName和m_nOffset比较大小
    struct tagNameNode
    {
        CString m_strName;  // 学生姓名
        int m_nOffset;      // 学生信息存储位置的偏移

        tagNameNode() : m_strName(), m_nOffset(-1) {}
        tagNameNode(CString strName, int nOffset) : m_strName(strName), m_nOffset(nOffset) {}

        bool operator>(const tagNameNode& nameNode)
        {
            if (m_strName == nameNode.m_strName)
            {
                return m_nOffset > nameNode.m_nOffset;
            }
            return m_strName > nameNode.m_strName;
        }
        bool operator<(const tagNameNode& nameNode)
        {
            if (m_strName == nameNode.m_strName)
            {
                return m_nOffset < nameNode.m_nOffset;
            }
            return m_strName < nameNode.m_strName;
        }
        bool operator==(const tagNameNode& nameNode)
        {
            return m_strName == nameNode.m_strName && m_nOffset == nameNode.m_nOffset;
        }
    };
    // 只根据m_strName比较大小
    struct tagNameNode2
    {
        CString m_strName;  // 学生姓名
        int m_nOffset;      // 学生信息存储位置的偏移

        tagNameNode2() : m_strName(), m_nOffset(-1) {}
        tagNameNode2(CString strName, int nOffset) : m_strName(strName), m_nOffset(nOffset) {}

        bool operator>(const tagNameNode2& nameNode)
        {
            return m_strName > nameNode.m_strName;
        }
        bool operator<(const tagNameNode2& nameNode)
        {
            return m_strName < nameNode.m_strName;
        }
        bool operator==(const tagNameNode2& nameNode)
        {
            return m_strName == nameNode.m_strName;
        }
    };

public:
    CSystem();
    ~CSystem();

public:
    // 启动系统
    void Start();

private:
    // 打印帮助信息
    void PrintHelp();
    
private:
    // 文件操作
    FILE* OpenFile();
    bool ReadFile(FILE* fp, void* pBuf, int nOffset, int nSize);
    bool WriteFile(FILE* fp, void* pBuf, int nOffset, int nSize);
    bool CloseFile(FILE* fp);

private:
    // 增加学生
    bool AddStudent(const CStudent& student);
    // 删除学生
    bool DeleteStudent(int nId);
    // 修改学生
    bool ModifyStudent(int nId, const CStudent& newStudent);
    // 根据Id查询
    CStudent QueryStudent(int nId);
    // 根据姓名查询
    CList<CStudent> QueryStudent(CString strName);
    // 显示所有学生
    void ShowAllStudents();

private:
    // 删除块
    void DeleteBlock(tagBlock* pBlock);
    // 将块插入到已使用链表
    void InsertUsedBlockList(tagBlock* pBlock);
    // 将块插入到空闲链表
    void InsertFreeBlockList(tagBlock* pBlock);

    tagBlock* GetBlock(int nOffset);
    int GetOffset(tagBlock* pBlock);
    char* GetData(int nOffset);

private:
    // 存放文件中读取的数据
    char* m_pMemory;
    // m_pMemory的大小
    int m_nMemorySize;
    // 根据Id构建的AVL树
    CAVLTree<tagIdNode> m_IdTree;
    // 根据姓名构建的AVL树
    CAVLTree<tagNameNode> m_NameTree;
};