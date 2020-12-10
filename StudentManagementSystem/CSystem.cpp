#include "CSystem.h"

CSystem::CSystem()
{
    // 打开文件(不创建新的文件)
    FILE* fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL)
    {
        // 文件不存在，则创建文件
        fp = fopen("data.bin", "wb+");
        if (fp == NULL)
        {
            cout << "创建文件失败" << endl;
            exit(-1);
        }

        tagBlock block[3];
        // 空闲链表头部
        block[0] = { -1, sizeof(tagBlock) * 2, 0 };
        // 已使用链表头部
        block[1] = { -1, -1, 0 };
        // 第一个空闲块
        block[2] = { 0, -1, 1024 - sizeof(tagBlock) * 3 };

        // 写入文件
        WriteFile(fp, block, 0, sizeof(tagBlock) * 3);
        CloseFile(fp);

        // 申请内存空间(初始大小为1024)
        m_nMemorySize = 1024;
        m_pMemory = new char[m_nMemorySize];
        memset(m_pMemory, 0, m_nMemorySize);

        // 写入内存
        memmove(m_pMemory, block, sizeof(tagBlock) * 3);
    }
    else
    {
        // 计算文件大小
        fseek(fp, 0, SEEK_END);
        int nFileSize = ftell(fp);

        // 申请内存空间(按1024字节取上整)
        m_nMemorySize = (nFileSize / 1024 + 1) * 1024;
        m_pMemory = new char[m_nMemorySize];
        memset(m_pMemory, 0, m_nMemorySize);

        // 将文件读到内存
        ReadFile(fp, m_pMemory, 0, nFileSize);
        CloseFile(fp);

        // 构建AVL树
        // 获取已使用链表头部哨兵节点的下一个
        int nNextOffset = GetBlock(USED_HEAD_OFFSET)->m_nNext;
        while (nNextOffset != -1)
        {
            CStudent student = CStudent::ToStudent(GetData(nNextOffset));
            m_IdTree.Insert(tagIdNode(student.GetId(), nNextOffset));
            m_NameTree.Insert(tagNameNode(student.GetName(), nNextOffset));

            nNextOffset = GetBlock(nNextOffset)->m_nNext;
        }
    }
}

CSystem::~CSystem()
{
    delete[] m_pMemory;
    m_IdTree.Clear();
    m_NameTree.Clear();
}

// 获取一个非空行(30字节以内，不包含换行符)
void GetLine(char* szLine)
{
    while (true)
    {
        fgets(szLine, LINE_SIZE, stdin);

        // 空行(只含有\n)
        if (strlen(szLine) == 1)
        {
            continue;
        }

        // 包含\n，则证明输入在30字节以内
        if (szLine[strlen(szLine) - 1] == '\n')
        {
            // 去掉\n
            szLine[strlen(szLine) - 1] = 0;
            return;
        }

        // 清空缓冲区
        rewind(stdin);
        cout << "请输入30字节以内的字符串" << endl;
    }
}

// 去除字符串尾部空格和Tab
void Trim(char* szLine)
{
    int i = strlen(szLine) - 1;
    while (i >= 0 && (szLine[i] == ' ' || szLine[i] == '\t'))
    {
        szLine[i--] = 0;
    }
}

// 获取一个int，且一行仅有一个int
int GetInt()
{
    char szLine[LINE_SIZE + 2] = { 0 };
    while (true)
    {
        GetLine(szLine);
        Trim(szLine);

        int a = 0;
        if (sscanf(szLine, "%d%c", &a) == 1)
        {
            return a;
        }
        cout << "请输入整数" << endl;
    }
}

// 获取一个char，且一行仅有一个char
int GetChar() {
    char szLine[LINE_SIZE + 2] = { 0 };
    while (true) 
    {
        GetLine(szLine);
        Trim(szLine);

        if (strlen(szLine) == 1)
        {
            return szLine[0];
        }
        cout << "请输入一个char" << endl;
    }
}

void CSystem::PrintHelp()
{
    cout << "学生管理系统，请输入序号：" << endl;
    cout << "1. 添加学生" << endl;
    cout << "2. 删除学生" << endl;
    cout << "3. 修改学生" << endl;
    cout << "4. 根据ID查询学生" << endl;
    cout << "5. 根据姓名查询学生" << endl;
    cout << "6. 显示所有学生" << endl;
}

void CSystem::Start()
{
    PrintHelp();

    char szLine[LINE_SIZE] = { 0 };
    int nId = 0;
    CString str;
    CStudent student;
    CList<CStudent> studentList;

    // 插入100万条数据
    /*for (int i = 0; i < 1000000; i++)
    {
        CString str = "xcb";
        str.Append(i);
        AddStudent(CStudent(i, 1900 + i, 1, 1, str));
    }*/

    while (true)
    {
        // debug
        /*char buf[1024] = { 0 };
        FILE* fp = OpenFile();
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        ReadFile(fp, buf, 0, size);
        CloseFile(fp);
        for (int i = 0; i < size; i++)
        {
            if (m_pMemory[i] != buf[i])
            {
                cout << "不相同" << endl;
                break;
            }
        }*/

        switch (GetInt())
        {
        case 1:
            cout << "请输入学生ID" << endl;
            nId = GetInt();

            student = QueryStudent(nId);
            if (student.GetId() != -1)
            {
                cout << "ID重复" << endl;
                break;
            }

            student.SetId(nId);
            cout << "请输入学生姓名" << endl;
            GetLine(szLine);
            student.SetName(szLine);
            cout << "请输入学生出生年份" << endl;
            student.SetBirthYear(GetInt());
            cout << "请输入学生出生月份" << endl;
            student.SetBirthMonth(GetInt());
            cout << "请输入学生性别(1.男，2.女，其他数字为未知)" << endl;
            student.SetSex(GetInt());

            AddStudent(student);
            break;
        case 2:
            cout << "请输入要删除的学生ID" << endl;
            DeleteStudent(GetInt());
            break;
        case 3:
            cout << "请输入要修改的学生ID" << endl;

            student = QueryStudent(GetInt());
            nId = student.GetId();
            if (nId == -1)
            {
                cout << "查无此人" << endl;
                break;
            }

            cout << "请输入要修改的内容: " << endl;
            cout << "1. 姓名" << endl;
            cout << "2. 出生年份" << endl;
            cout << "3. 出生月份" << endl;
            cout << "4. 性别" << endl;

            student.Clear();

            switch (GetInt())
            {
            case 1:
                cout << "请输入新的姓名" << endl;
                GetLine(szLine);
                student.SetName(szLine);
                ModifyStudent(nId, student);
                break;
            case 2:
                cout << "请输入新的出生年份" << endl;
                student.SetBirthYear(GetInt());
                ModifyStudent(nId, student);
                break;
            case 3:
                cout << "请输入新的出生月份" << endl;
                student.SetBirthMonth(GetInt());
                ModifyStudent(nId, student);
                break;
            case 4:
                cout << "请输入新的性别(1.男，2.女，其他数字为未知)" << endl;
                student.SetSex(GetInt());
                ModifyStudent(nId, student);
                break;
            default:
                break;
            }
            break;
        case 4:
            cout << "请输入要查询的学生ID" << endl;
            student = QueryStudent(GetInt());

            if (student.GetId() == -1)
            {
                cout << "查无此人" << endl;
            }
            else
            {
                cout << student << endl;
            }
            break;
        case 5:
            cout << "请输入要查询的学生姓名" << endl;
            cin >> str;
            studentList = QueryStudent(str);

            if (studentList.Size() == 0)
            {
                cout << "查无此人" << endl;
            }
            else
            {
                for (CStudent student : studentList)
                {
                    cout << student << endl;
                }
            }
            break;
        case 6:
            ShowAllStudents();
            break;
        default:
            PrintHelp();
            break;
        }

    }
}

FILE* CSystem::OpenFile()
{
    FILE* fp = NULL;
    fp = fopen(FILE_NAME, "rb+");

    if (!fp)
    {
        cout << "打开文件失败" << endl;
    }

    return fp;
}

bool CSystem::ReadFile(FILE* fp, void* pBuf, int nOffset, int nSize)
{
    if (fseek(fp, nOffset, SEEK_SET) != 0)
    {
        cout << "移动文件指针失败" << endl;
        return false;
    }

    if (fread(pBuf, nSize, 1, fp) != 1)
    {
        cout << "读取文件失败" << endl;
        return false;
    }

    return true;
}

bool CSystem::WriteFile(FILE* fp, void* pBuf, int nOffset, int nSize)
{
    if (fseek(fp, nOffset, SEEK_SET) != 0)
    {
        cout << "移动文件指针失败" << endl;
        return false;
    }

    if (fwrite(pBuf, nSize, 1, fp) != 1)
    {
        cout << "写入文件失败" << endl;
        return false;
    }

    return true;
}

bool CSystem::CloseFile(FILE* fp)
{
    if (fp == NULL)
    {
        return true;
    }

    if (fclose(fp) != 0)
    {
        cout << "关闭文件失败" << endl;
        return false;
    }

    return true;
}

bool CSystem::AddStudent(const CStudent& student)
{
    char aryBytes[100] = { 0 };
    int nLength = student.ToByteArray(aryBytes);
    tagBlock* pBlock = GetBlock(FREE_HEAD_OFFSET);

    while (true)
    {
        if (pBlock->m_nSize >= nLength)
        {
            // 整块分配
            if (pBlock->m_nSize - nLength <= sizeof(tagBlock))
            {
                // 从空闲链表中删除
                DeleteBlock(pBlock);
                // 插入已使用链表
                InsertUsedBlockList(pBlock);
            }
            else
            {
                // 删除空闲块
                DeleteBlock(pBlock);
                // 插入剩余的空闲块
                tagBlock* pRemainBlock = GetBlock(GetOffset(pBlock) + sizeof(tagBlock) + nLength);
                pRemainBlock->m_nSize = pBlock->m_nSize - sizeof(tagBlock) - nLength;
                InsertFreeBlockList(pRemainBlock);
                // 插入已使用链表
                pBlock->m_nSize = nLength;
                InsertUsedBlockList(pBlock);
            }

            // 复制内容
            memmove(GetData(GetOffset(pBlock)), aryBytes, nLength);
            // 写入文件
            FILE* fp = OpenFile();
            WriteFile(fp, aryBytes, GetOffset(pBlock) + sizeof(tagBlock), nLength);
            CloseFile(fp);
            // 插入AVL树
            m_IdTree.Insert(tagIdNode(student.GetId(), GetOffset(pBlock)));
            m_NameTree.Insert(tagNameNode(student.GetName(), GetOffset(pBlock)));

            return true;
        }

        if (pBlock->m_nNext == -1)
        {
            // 暂时保存pBlock偏移
            int nOffset = GetOffset(pBlock);

            // 扩大内存
            int nNewSize = m_nMemorySize + 1024;
            char* pNewMemory = new char[nNewSize];
            memmove(pNewMemory, m_pMemory, m_nMemorySize);
            delete[] m_pMemory;
            m_pMemory = pNewMemory;
            m_nMemorySize = nNewSize;

            // 指向新的内存
            pBlock = GetBlock(nOffset);

            // 增加空闲块
            tagBlock* pNewBlock = GetBlock(nNewSize - 1024);
            pNewBlock->m_nSize = 1024 - sizeof(tagBlock);
            pNewBlock->m_nNext = -1;
            pNewBlock->m_nPre = nOffset;
            pBlock->m_nNext = GetOffset(pNewBlock);

            // 写入文件
            FILE* fp = OpenFile();
            WriteFile(fp, pBlock, GetOffset(pBlock), sizeof(tagBlock));
            WriteFile(fp, pNewBlock, GetOffset(pNewBlock), sizeof(tagBlock));
            CloseFile(fp);

            continue;
        }

        pBlock = GetBlock(pBlock->m_nNext);
    }

    cout << "未知原因" << endl;
    return false;
}

bool CSystem::DeleteStudent(int nId)
{
    // 查询
    tagIdNode idNode = m_IdTree.Find(tagIdNode(nId, 0));
    if (idNode.m_nOffset == -1)
    {
        return false;
    }

    DeleteBlock(GetBlock(idNode.m_nOffset));
    InsertFreeBlockList(GetBlock(idNode.m_nOffset));

    // 删除AVL树的节点
    m_IdTree.Remove(idNode);
    CStudent student = CStudent::ToStudent(GetData(idNode.m_nOffset));
    m_NameTree.Remove(tagNameNode(student.GetName(), idNode.m_nOffset));

    return true;
}

bool CSystem::ModifyStudent(int nId, const CStudent& newStudent)
{
    // 查询
    tagIdNode idNode = m_IdTree.Find(tagIdNode(nId, 0));
    if (idNode.m_nOffset == -1)
    {
        return false;
    }

    CStudent student = CStudent::ToStudent(GetData(idNode.m_nOffset));
    CString strOldName = student.GetName();

    // 更新
    student.UpdateStudent(newStudent);

    char pBuf[100] = { 0 };
    int nSize = student.ToByteArray(pBuf);
    // 原来的空间够大了
    if (nSize <= GetBlock(idNode.m_nOffset)->m_nSize)
    {
        // 写入内存
        memmove(GetData(idNode.m_nOffset), pBuf, nSize);
        // 写入文件
        FILE* fp = OpenFile();
        WriteFile(fp, pBuf, idNode.m_nOffset + sizeof(tagBlock), nSize);
        CloseFile(fp);
        // 修改AVL树
        m_NameTree.Remove(tagNameNode(strOldName, idNode.m_nOffset));
        m_NameTree.Insert(tagNameNode(student.GetName(), idNode.m_nOffset));
    }
    // 原来的空间不够
    else
    {
        DeleteStudent(nId);
        AddStudent(student);
    }

    return true;
}

CStudent CSystem::QueryStudent(int nId)
{
    tagIdNode idNode = m_IdTree.Find(tagIdNode(nId, 0));

    if (idNode.m_nOffset == -1)
    {
        return CStudent();
    }
    return CStudent::ToStudent(GetData(idNode.m_nOffset));
}

CList<CStudent> CSystem::QueryStudent(CString strName)
{
    CList<tagNameNode2> nameNodeList = (*(CAVLTree<tagNameNode2>*)&m_NameTree).FindAll(tagNameNode2(strName, 0));

    CList<CStudent> studentList;
    for (auto nameNode : nameNodeList)
    {
        studentList.AddTail(CStudent::ToStudent(GetData(nameNode.m_nOffset)));
    }

    return studentList;
}

void CSystem::ShowAllStudents()
{
    CList<CStudent> lt;
    int nNextOffset = GetBlock(sizeof(tagBlock))->m_nNext;
    while (nNextOffset != -1)
    {
        lt.AddTail(CStudent::ToStudent(GetData(nNextOffset)));

        nNextOffset = GetBlock(nNextOffset)->m_nNext;
    }

    for (CStudent student : lt)
    {
        cout << student << endl;
    }
}

void CSystem::DeleteBlock(tagBlock* pBlock)
{
    FILE* fp = OpenFile();

    GetBlock(pBlock->m_nPre)->m_nNext = pBlock->m_nNext;
    WriteFile(fp, GetBlock(pBlock->m_nPre), pBlock->m_nPre, sizeof(tagBlock));
    if (pBlock->m_nNext != -1)
    {
        GetBlock(pBlock->m_nNext)->m_nPre = pBlock->m_nPre;
        WriteFile(fp, GetBlock(pBlock->m_nNext), pBlock->m_nNext, sizeof(tagBlock));
    }

    CloseFile(fp);
}

void CSystem::InsertUsedBlockList(tagBlock* pBlock)
{
    FILE* fp = OpenFile();

    tagBlock* pBlockUsedHead = GetBlock(USED_HEAD_OFFSET);
    pBlock->m_nNext = pBlockUsedHead->m_nNext;
    if (pBlock->m_nNext != -1)
    {
        GetBlock(pBlock->m_nNext)->m_nPre = GetOffset(pBlock);
        WriteFile(fp, GetBlock(pBlock->m_nNext), pBlock->m_nNext, sizeof(tagBlock));
    }
    pBlockUsedHead->m_nNext = GetOffset(pBlock);
    pBlock->m_nPre = GetOffset(pBlockUsedHead);

    WriteFile(fp, pBlockUsedHead, GetOffset(pBlockUsedHead), sizeof(tagBlock));
    WriteFile(fp, pBlock, GetOffset(pBlock), sizeof(tagBlock));

    CloseFile(fp);
}

void CSystem::InsertFreeBlockList(tagBlock* pBlock)
{
    FILE* fp = OpenFile();

    // 按地址顺序插入空闲块
    int nNextOffset = FREE_HEAD_OFFSET;
    int nPreOffset = -1;
    while (nNextOffset != -1 && nNextOffset < GetOffset(pBlock))
    {
        nPreOffset = nNextOffset;
        nNextOffset = GetBlock(nNextOffset)->m_nNext;
    }

    // 合并空闲块
    /*int nPreTailOffset = nPreOffset + sizeof(tagBlock) + GetBlock(nPreOffset)->m_nSize;
    int nCurTailOffset = GetOffset(pBlock) + sizeof(tagBlock) + pBlock->m_nSize;

    if (nPreTailOffset == GetOffset(pBlock) && nCurTailOffset == nNextOffset)
    {
        GetBlock(nPreOffset)->m_nSize += sizeof(tagBlock) + pBlock->m_nSize;
    }
    else if (nPreTailOffset == GetOffset(pBlock) && nCurTailOffset != nNextOffset)
    {

    }
    else if (nPreTailOffset != GetOffset(pBlock) && nCurTailOffset == nNextOffset)
    {

    }
    else
    {

    }*/

    GetBlock(nPreOffset)->m_nNext = GetOffset(pBlock);
    pBlock->m_nPre = nPreOffset;
    pBlock->m_nNext = nNextOffset;
    WriteFile(fp, GetBlock(nPreOffset), nPreOffset, sizeof(tagBlock));
    WriteFile(fp, pBlock, GetOffset(pBlock), sizeof(tagBlock));
    if (nNextOffset != -1)
    {
        GetBlock(nNextOffset)->m_nPre = GetOffset(pBlock);
        WriteFile(fp, GetBlock(nNextOffset), nNextOffset, sizeof(tagBlock));
    }

    CloseFile(fp);
}

CSystem::tagBlock* CSystem::GetBlock(int nOffset)
{
    return (tagBlock*)(m_pMemory + nOffset);
}

int CSystem::GetOffset(tagBlock* pBlock)
{
    return (int)pBlock - (int)m_pMemory;
}

char* CSystem::GetData(int nOffset)
{
    return m_pMemory + nOffset + sizeof(tagBlock);
}
