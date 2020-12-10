#include "CSystem.h"

CSystem::CSystem()
{
    // ���ļ�(�������µ��ļ�)
    FILE* fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL)
    {
        // �ļ������ڣ��򴴽��ļ�
        fp = fopen("data.bin", "wb+");
        if (fp == NULL)
        {
            cout << "�����ļ�ʧ��" << endl;
            exit(-1);
        }

        tagBlock block[3];
        // ��������ͷ��
        block[0] = { -1, sizeof(tagBlock) * 2, 0 };
        // ��ʹ������ͷ��
        block[1] = { -1, -1, 0 };
        // ��һ�����п�
        block[2] = { 0, -1, 1024 - sizeof(tagBlock) * 3 };

        // д���ļ�
        WriteFile(fp, block, 0, sizeof(tagBlock) * 3);
        CloseFile(fp);

        // �����ڴ�ռ�(��ʼ��СΪ1024)
        m_nMemorySize = 1024;
        m_pMemory = new char[m_nMemorySize];
        memset(m_pMemory, 0, m_nMemorySize);

        // д���ڴ�
        memmove(m_pMemory, block, sizeof(tagBlock) * 3);
    }
    else
    {
        // �����ļ���С
        fseek(fp, 0, SEEK_END);
        int nFileSize = ftell(fp);

        // �����ڴ�ռ�(��1024�ֽ�ȡ����)
        m_nMemorySize = (nFileSize / 1024 + 1) * 1024;
        m_pMemory = new char[m_nMemorySize];
        memset(m_pMemory, 0, m_nMemorySize);

        // ���ļ������ڴ�
        ReadFile(fp, m_pMemory, 0, nFileSize);
        CloseFile(fp);

        // ����AVL��
        // ��ȡ��ʹ������ͷ���ڱ��ڵ����һ��
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

// ��ȡһ���ǿ���(30�ֽ����ڣ����������з�)
void GetLine(char* szLine)
{
    while (true)
    {
        fgets(szLine, LINE_SIZE, stdin);

        // ����(ֻ����\n)
        if (strlen(szLine) == 1)
        {
            continue;
        }

        // ����\n����֤��������30�ֽ�����
        if (szLine[strlen(szLine) - 1] == '\n')
        {
            // ȥ��\n
            szLine[strlen(szLine) - 1] = 0;
            return;
        }

        // ��ջ�����
        rewind(stdin);
        cout << "������30�ֽ����ڵ��ַ���" << endl;
    }
}

// ȥ���ַ���β���ո��Tab
void Trim(char* szLine)
{
    int i = strlen(szLine) - 1;
    while (i >= 0 && (szLine[i] == ' ' || szLine[i] == '\t'))
    {
        szLine[i--] = 0;
    }
}

// ��ȡһ��int����һ�н���һ��int
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
        cout << "����������" << endl;
    }
}

// ��ȡһ��char����һ�н���һ��char
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
        cout << "������һ��char" << endl;
    }
}

void CSystem::PrintHelp()
{
    cout << "ѧ������ϵͳ����������ţ�" << endl;
    cout << "1. ���ѧ��" << endl;
    cout << "2. ɾ��ѧ��" << endl;
    cout << "3. �޸�ѧ��" << endl;
    cout << "4. ����ID��ѯѧ��" << endl;
    cout << "5. ����������ѯѧ��" << endl;
    cout << "6. ��ʾ����ѧ��" << endl;
}

void CSystem::Start()
{
    PrintHelp();

    char szLine[LINE_SIZE] = { 0 };
    int nId = 0;
    CString str;
    CStudent student;
    CList<CStudent> studentList;

    // ����100��������
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
                cout << "����ͬ" << endl;
                break;
            }
        }*/

        switch (GetInt())
        {
        case 1:
            cout << "������ѧ��ID" << endl;
            nId = GetInt();

            student = QueryStudent(nId);
            if (student.GetId() != -1)
            {
                cout << "ID�ظ�" << endl;
                break;
            }

            student.SetId(nId);
            cout << "������ѧ������" << endl;
            GetLine(szLine);
            student.SetName(szLine);
            cout << "������ѧ���������" << endl;
            student.SetBirthYear(GetInt());
            cout << "������ѧ�������·�" << endl;
            student.SetBirthMonth(GetInt());
            cout << "������ѧ���Ա�(1.�У�2.Ů����������Ϊδ֪)" << endl;
            student.SetSex(GetInt());

            AddStudent(student);
            break;
        case 2:
            cout << "������Ҫɾ����ѧ��ID" << endl;
            DeleteStudent(GetInt());
            break;
        case 3:
            cout << "������Ҫ�޸ĵ�ѧ��ID" << endl;

            student = QueryStudent(GetInt());
            nId = student.GetId();
            if (nId == -1)
            {
                cout << "���޴���" << endl;
                break;
            }

            cout << "������Ҫ�޸ĵ�����: " << endl;
            cout << "1. ����" << endl;
            cout << "2. �������" << endl;
            cout << "3. �����·�" << endl;
            cout << "4. �Ա�" << endl;

            student.Clear();

            switch (GetInt())
            {
            case 1:
                cout << "�������µ�����" << endl;
                GetLine(szLine);
                student.SetName(szLine);
                ModifyStudent(nId, student);
                break;
            case 2:
                cout << "�������µĳ������" << endl;
                student.SetBirthYear(GetInt());
                ModifyStudent(nId, student);
                break;
            case 3:
                cout << "�������µĳ����·�" << endl;
                student.SetBirthMonth(GetInt());
                ModifyStudent(nId, student);
                break;
            case 4:
                cout << "�������µ��Ա�(1.�У�2.Ů����������Ϊδ֪)" << endl;
                student.SetSex(GetInt());
                ModifyStudent(nId, student);
                break;
            default:
                break;
            }
            break;
        case 4:
            cout << "������Ҫ��ѯ��ѧ��ID" << endl;
            student = QueryStudent(GetInt());

            if (student.GetId() == -1)
            {
                cout << "���޴���" << endl;
            }
            else
            {
                cout << student << endl;
            }
            break;
        case 5:
            cout << "������Ҫ��ѯ��ѧ������" << endl;
            cin >> str;
            studentList = QueryStudent(str);

            if (studentList.Size() == 0)
            {
                cout << "���޴���" << endl;
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
        cout << "���ļ�ʧ��" << endl;
    }

    return fp;
}

bool CSystem::ReadFile(FILE* fp, void* pBuf, int nOffset, int nSize)
{
    if (fseek(fp, nOffset, SEEK_SET) != 0)
    {
        cout << "�ƶ��ļ�ָ��ʧ��" << endl;
        return false;
    }

    if (fread(pBuf, nSize, 1, fp) != 1)
    {
        cout << "��ȡ�ļ�ʧ��" << endl;
        return false;
    }

    return true;
}

bool CSystem::WriteFile(FILE* fp, void* pBuf, int nOffset, int nSize)
{
    if (fseek(fp, nOffset, SEEK_SET) != 0)
    {
        cout << "�ƶ��ļ�ָ��ʧ��" << endl;
        return false;
    }

    if (fwrite(pBuf, nSize, 1, fp) != 1)
    {
        cout << "д���ļ�ʧ��" << endl;
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
        cout << "�ر��ļ�ʧ��" << endl;
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
            // �������
            if (pBlock->m_nSize - nLength <= sizeof(tagBlock))
            {
                // �ӿ���������ɾ��
                DeleteBlock(pBlock);
                // ������ʹ������
                InsertUsedBlockList(pBlock);
            }
            else
            {
                // ɾ�����п�
                DeleteBlock(pBlock);
                // ����ʣ��Ŀ��п�
                tagBlock* pRemainBlock = GetBlock(GetOffset(pBlock) + sizeof(tagBlock) + nLength);
                pRemainBlock->m_nSize = pBlock->m_nSize - sizeof(tagBlock) - nLength;
                InsertFreeBlockList(pRemainBlock);
                // ������ʹ������
                pBlock->m_nSize = nLength;
                InsertUsedBlockList(pBlock);
            }

            // ��������
            memmove(GetData(GetOffset(pBlock)), aryBytes, nLength);
            // д���ļ�
            FILE* fp = OpenFile();
            WriteFile(fp, aryBytes, GetOffset(pBlock) + sizeof(tagBlock), nLength);
            CloseFile(fp);
            // ����AVL��
            m_IdTree.Insert(tagIdNode(student.GetId(), GetOffset(pBlock)));
            m_NameTree.Insert(tagNameNode(student.GetName(), GetOffset(pBlock)));

            return true;
        }

        if (pBlock->m_nNext == -1)
        {
            // ��ʱ����pBlockƫ��
            int nOffset = GetOffset(pBlock);

            // �����ڴ�
            int nNewSize = m_nMemorySize + 1024;
            char* pNewMemory = new char[nNewSize];
            memmove(pNewMemory, m_pMemory, m_nMemorySize);
            delete[] m_pMemory;
            m_pMemory = pNewMemory;
            m_nMemorySize = nNewSize;

            // ָ���µ��ڴ�
            pBlock = GetBlock(nOffset);

            // ���ӿ��п�
            tagBlock* pNewBlock = GetBlock(nNewSize - 1024);
            pNewBlock->m_nSize = 1024 - sizeof(tagBlock);
            pNewBlock->m_nNext = -1;
            pNewBlock->m_nPre = nOffset;
            pBlock->m_nNext = GetOffset(pNewBlock);

            // д���ļ�
            FILE* fp = OpenFile();
            WriteFile(fp, pBlock, GetOffset(pBlock), sizeof(tagBlock));
            WriteFile(fp, pNewBlock, GetOffset(pNewBlock), sizeof(tagBlock));
            CloseFile(fp);

            continue;
        }

        pBlock = GetBlock(pBlock->m_nNext);
    }

    cout << "δ֪ԭ��" << endl;
    return false;
}

bool CSystem::DeleteStudent(int nId)
{
    // ��ѯ
    tagIdNode idNode = m_IdTree.Find(tagIdNode(nId, 0));
    if (idNode.m_nOffset == -1)
    {
        return false;
    }

    DeleteBlock(GetBlock(idNode.m_nOffset));
    InsertFreeBlockList(GetBlock(idNode.m_nOffset));

    // ɾ��AVL���Ľڵ�
    m_IdTree.Remove(idNode);
    CStudent student = CStudent::ToStudent(GetData(idNode.m_nOffset));
    m_NameTree.Remove(tagNameNode(student.GetName(), idNode.m_nOffset));

    return true;
}

bool CSystem::ModifyStudent(int nId, const CStudent& newStudent)
{
    // ��ѯ
    tagIdNode idNode = m_IdTree.Find(tagIdNode(nId, 0));
    if (idNode.m_nOffset == -1)
    {
        return false;
    }

    CStudent student = CStudent::ToStudent(GetData(idNode.m_nOffset));
    CString strOldName = student.GetName();

    // ����
    student.UpdateStudent(newStudent);

    char pBuf[100] = { 0 };
    int nSize = student.ToByteArray(pBuf);
    // ԭ���Ŀռ乻����
    if (nSize <= GetBlock(idNode.m_nOffset)->m_nSize)
    {
        // д���ڴ�
        memmove(GetData(idNode.m_nOffset), pBuf, nSize);
        // д���ļ�
        FILE* fp = OpenFile();
        WriteFile(fp, pBuf, idNode.m_nOffset + sizeof(tagBlock), nSize);
        CloseFile(fp);
        // �޸�AVL��
        m_NameTree.Remove(tagNameNode(strOldName, idNode.m_nOffset));
        m_NameTree.Insert(tagNameNode(student.GetName(), idNode.m_nOffset));
    }
    // ԭ���Ŀռ䲻��
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

    // ����ַ˳�������п�
    int nNextOffset = FREE_HEAD_OFFSET;
    int nPreOffset = -1;
    while (nNextOffset != -1 && nNextOffset < GetOffset(pBlock))
    {
        nPreOffset = nNextOffset;
        nNextOffset = GetBlock(nNextOffset)->m_nNext;
    }

    // �ϲ����п�
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
