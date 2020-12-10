#pragma once
#include <iostream>
#include "CStudent.h"
#include "CString.h"
#include "CList.h"
#include "CAVLTree.h"
using namespace std;

// ������ݵ��ļ�
#define FILE_NAME "data.bin"
// һ�ζ�ȡ�����ֵ
#define LINE_SIZE 32
// ��������ͷ����ƫ��
#define FREE_HEAD_OFFSET 0
// ��ʹ������ͷ����ƫ��
#define USED_HEAD_OFFSET sizeof(tagBlock)

class CSystem
{
private:
    // ��¼�洢��Ϣ�Ŀ�
    struct tagBlock 
    {
        int m_nPre;     // ��һ�����ƫ��
        int m_nNext;    // ��һ�����ƫ��
        int m_nSize;    // �����洢�ռ�Ĵ�С(��������Ĵ�С)
    };

private:
    // AVL���ڵ�(����Id����)
    struct tagIdNode
    {
        int m_nId;      // ѧ����Id
        int m_nOffset;  // ѧ����Ϣ�洢λ�õ�ƫ��

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
    // AVL���ڵ�(����ѧ����������)
    // ͬʱ����m_strName��m_nOffset�Ƚϴ�С
    struct tagNameNode
    {
        CString m_strName;  // ѧ������
        int m_nOffset;      // ѧ����Ϣ�洢λ�õ�ƫ��

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
    // ֻ����m_strName�Ƚϴ�С
    struct tagNameNode2
    {
        CString m_strName;  // ѧ������
        int m_nOffset;      // ѧ����Ϣ�洢λ�õ�ƫ��

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
    // ����ϵͳ
    void Start();

private:
    // ��ӡ������Ϣ
    void PrintHelp();
    
private:
    // �ļ�����
    FILE* OpenFile();
    bool ReadFile(FILE* fp, void* pBuf, int nOffset, int nSize);
    bool WriteFile(FILE* fp, void* pBuf, int nOffset, int nSize);
    bool CloseFile(FILE* fp);

private:
    // ����ѧ��
    bool AddStudent(const CStudent& student);
    // ɾ��ѧ��
    bool DeleteStudent(int nId);
    // �޸�ѧ��
    bool ModifyStudent(int nId, const CStudent& newStudent);
    // ����Id��ѯ
    CStudent QueryStudent(int nId);
    // ����������ѯ
    CList<CStudent> QueryStudent(CString strName);
    // ��ʾ����ѧ��
    void ShowAllStudents();

private:
    // ɾ����
    void DeleteBlock(tagBlock* pBlock);
    // ������뵽��ʹ������
    void InsertUsedBlockList(tagBlock* pBlock);
    // ������뵽��������
    void InsertFreeBlockList(tagBlock* pBlock);

    tagBlock* GetBlock(int nOffset);
    int GetOffset(tagBlock* pBlock);
    char* GetData(int nOffset);

private:
    // ����ļ��ж�ȡ������
    char* m_pMemory;
    // m_pMemory�Ĵ�С
    int m_nMemorySize;
    // ����Id������AVL��
    CAVLTree<tagIdNode> m_IdTree;
    // ��������������AVL��
    CAVLTree<tagNameNode> m_NameTree;
};