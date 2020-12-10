#include "CString.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

CString::CString()
{
    InitStr(nullptr);
}

CString::CString(const char* szStr)
{
    InitStr(szStr);
}

CString::CString(const CString& str)
{
    //浅拷贝
    m_pStrBuff = str.m_pStrBuff;
    m_pRefCount = str.m_pRefCount;
    m_nBuffLen = str.m_nBuffLen;
    m_nStrLen = str.m_nStrLen;

    if (m_pRefCount != nullptr)
    {
        (*m_pRefCount)++;
    }
}

CString::~CString()
{
    ClearStr();
}

int CString::Length() const
{
    return m_nStrLen;
}

const char* CString::GetString() const
{
    return m_pStrBuff;
}

CString& CString::Append(const char* szStr)
{
    if (szStr == nullptr || strlen(szStr) == 0)
    {
        return *this;
    }

    // 判断缓冲区是否足够放下拼接后的字符串
    int nCatStrLen = strlen(szStr) + m_nStrLen + 1;
    if (nCatStrLen > m_nBuffLen)
    {
        // 缓冲区过小, 不足以存放新的字符串
        // 申请新的缓冲区, 拼接字符串
        char* pNewBuff = new char[nCatStrLen];
        memset(pNewBuff, 0, nCatStrLen);
        if (m_pStrBuff != nullptr)
        {
            strcpy(pNewBuff, m_pStrBuff);
        }
        strcat(pNewBuff, szStr);

        // 释放原来的缓冲区
        ClearStr();

        // 成员赋新值
        InitStr(pNewBuff);

        delete[] pNewBuff;
        pNewBuff = nullptr;
    }
    else
    {
        CopyOnWrite();
        // 缓冲区足够存放新的字符串
        strcat(m_pStrBuff, szStr);
        m_nStrLen = nCatStrLen - 1;
    }

    return *this;
}

CString& CString::Append(const CString& str)
{
    return Append(str.m_pStrBuff);
}

CString& CString::Append(int nVal)
{
    return Append(ValueOf(nVal));
}

CString& CString::Append(char chVal)
{
    return Append(ValueOf(chVal));
}

int CString::Find(const char* szStr)
{
    if (m_pStrBuff == nullptr || szStr == nullptr)
    {
        return -1;
    }

    char* p = strstr(m_pStrBuff, szStr);

    if (p == nullptr)
    {
        return -1;
    }
    else
    {
        return p - szStr;
    }
}

int CString::Find(const CString& str)
{
    return Find(str.m_pStrBuff);
}

int CString::ReserveFind(const char* szStr)
{
    if (m_pStrBuff == nullptr || szStr == nullptr)
    {
        return -1;
    }

    char* p = nullptr;
    for (int i = strlen(m_pStrBuff) - 1; i >= 0; i--)
    {
        p = strstr(m_pStrBuff + i, szStr);
        if (p != nullptr)
        {
            break;
        }
    }

    if (p == nullptr)
    {
        return -1;
    }
    else
    {
        return p - szStr;
    }
}

int CString::ReserveFind(const CString& str)
{
    return ReserveFind(str.m_pStrBuff);
}

CString& CString::Replace(const char* szDst)
{
    if (szDst == nullptr)
    {
        ClearStr();
        return *this;
    }

    int nDstLen = strlen(szDst) + 1;

    if (m_nBuffLen >= nDstLen)
    {
        CopyOnWrite();
        strcpy(m_pStrBuff, szDst);
        m_nStrLen = nDstLen - 1;
    }
    else
    {
        char* pNewBuff = new char[nDstLen];
        strcpy(pNewBuff, szDst);

        ClearStr();

        InitStr(pNewBuff);
        delete[] pNewBuff;
        pNewBuff = nullptr;
    }

    return *this;
}

CString& CString::Replace(const CString& strDst)
{
    return Replace(strDst.m_pStrBuff);
}

CString& CString::Replace(int nIdxBegin, const char* szDst)
{
    if (szDst == nullptr)
    {
        ClearStr();
        return *this;
    }

    int nDstLen = nIdxBegin + strlen(szDst) + 1;

    if (m_nBuffLen >= nDstLen)
    {
        CopyOnWrite();
        strcpy(m_pStrBuff + nIdxBegin, szDst);
        m_nStrLen = nDstLen - 1;
    }
    else
    {
        char* pNewBuff = new char[nDstLen];
        strcpy(pNewBuff, m_pStrBuff);
        strcpy(m_pStrBuff + nIdxBegin, szDst);

        ClearStr();

        InitStr(pNewBuff);

        delete[] pNewBuff;
        pNewBuff = nullptr;
    }

    return *this;
}

CString& CString::Replace(int nIdxBegin, const CString& strDst)
{
    return Replace(nIdxBegin, strDst.m_pStrBuff);
}

void CString::Copy(const char* szStr)
{
    Replace(szStr);
}

void CString::Copy(const CString& str)
{
    Replace(str.m_pStrBuff);
}

CString CString::Mid(int nIdx, int nLen)
{
    if (nIdx >= m_nStrLen)
    {
        return CString();
    }

    char* szSubStr = new char[nLen + 1];
    memset(szSubStr, 0, nLen + 1);

    if ((nIdx + nLen) >= m_nStrLen)
    {
        strcpy(szSubStr, m_pStrBuff + nIdx);
    }
    else
    {
        memmove(szSubStr, m_pStrBuff + nIdx, nLen);
    }

    CString newStr(szSubStr);

    delete[] szSubStr;

    return newStr;
}

CString CString::Left(int nLen)
{
    return Mid(0, nLen);
}

CString CString::Right(int nLen)
{
    return Mid(m_nStrLen - nLen, nLen);
}

void CString::Strip()
{
    CopyOnWrite();

    char* szNewStr = new char[m_nStrLen + 1];
    memset(szNewStr, 0, m_nStrLen + 1);

    for (int i = 0, j = 0; i < m_nStrLen; i++)
    {
        if ((m_pStrBuff[i] != ' ') && (m_pStrBuff[i] != '\t') && (m_pStrBuff[i] != '\n'))
        {
            szNewStr[j] = m_pStrBuff[i];
            j++;
        }
    }

    Replace(szNewStr);

    delete[] szNewStr;
}

CString CString::ValueOf(bool b)
{
    if (b)
    {
        return CString("true");
    }
    else
    {
        return CString("false");
    }
}

CString CString::ValueOf(char c)
{
    char szBuf[2] = { 0 };
    szBuf[0] = c;
    return CString(szBuf);
}

CString CString::ValueOf(double d)
{
    char szBuf[100] = { 0 };
    sprintf(szBuf, "%llf", d);
    return CString(szBuf);
}

CString CString::ValueOf(float f)
{
    char szBuf[100] = { 0 };
    sprintf(szBuf, "%f", f);
    return CString(szBuf);
}

CString CString::ValueOf(int i)
{
    char szBuf[100] = { 0 };
    sprintf(szBuf, "%d", i);
    return CString(szBuf);
}

CString CString::ValueOf(long i)
{
    char szBuf[100] = { 0 };
    sprintf(szBuf, "%d", i);
    return CString(szBuf);
}

CString CString::ValueOf(short i)
{
    char szBuf[100] = { 0 };
    sprintf(szBuf, "%d", i);
    return CString(szBuf);
}

CString CString::ValueOf(const char* data)
{
    return CString(data);
}

CString CString::CopyValueOf(const char* data, int offset, int count)
{
    CString str(data);
    return str.Mid(offset, count);
}

CString CString::Format(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char szBuf[1024] = { 0 };
    vsprintf(szBuf, format, args);

    va_end(args);

    return CString(szBuf);
}

CString& CString::operator=(const char* szStr)
{
    ClearStr();
    InitStr(szStr);
    return *this;
}

CString& CString::operator=(const CString& str)
{
    if (this == &str)
    {
        return *this;
    }

    ClearStr();

    //浅拷贝
    m_pStrBuff = str.m_pStrBuff;
    m_pRefCount = str.m_pRefCount;
    m_nBuffLen = str.m_nBuffLen;
    m_nStrLen = str.m_nStrLen;

    if (m_pRefCount != nullptr)
    {
        (*m_pRefCount)++;
    }

    return *this;
}

CString CString::operator+(const char* szStr)
{
    CString str(*this);
    str.Append(szStr);
    return str;
}

CString CString::operator+(const CString& str)
{
    CString newStr(*this);
    newStr.Append(str);
    return newStr;
}

char CString::operator[](int nIndex)
{
    if (nIndex < 0 || nIndex >= m_nStrLen)
    {
        throw - 1;
    }
    return m_pStrBuff[nIndex];
}

bool CString::operator==(const char* szStr)
{
    return strcmp(m_pStrBuff, szStr) == 0;
}

bool CString::operator==(const CString& str)
{
    return strcmp(m_pStrBuff, str.m_pStrBuff) == 0;
}

bool CString::operator!=(const char* szStr)
{
    return strcmp(m_pStrBuff, szStr) != 0;
}

bool CString::operator!=(const CString& str)
{
    return strcmp(m_pStrBuff, str.m_pStrBuff) != 0;
}

bool CString::operator>(const char* szStr)
{
    return strcmp(m_pStrBuff, szStr) > 0;
}

bool CString::operator>(const CString& str)
{
    return strcmp(m_pStrBuff, str.m_pStrBuff) > 0;
}

bool CString::operator<(const CString& str)
{
    return strcmp(m_pStrBuff, str.m_pStrBuff) < 0;
}

void CString::InitStr(const char* szStr)
{
    if (szStr == nullptr)
    {
        m_pStrBuff = nullptr;
        m_pRefCount = nullptr;
        m_nBuffLen = 0;
        m_nStrLen = 0;
    }
    else
    {
        m_nStrLen = strlen(szStr);
        m_nBuffLen = m_nStrLen + 1;
        
        m_pStrBuff = new char[m_nBuffLen];

        strcpy(m_pStrBuff, szStr);
        m_pRefCount = new int;
        *m_pRefCount = 1;
    }
}

void CString::ClearStr()
{
    if (m_pRefCount == nullptr)
    {
        return;
    }

    (*m_pRefCount)--;

    if (*m_pRefCount == 0)
    {
        delete[] m_pStrBuff;
        delete m_pRefCount;
    }

    m_pStrBuff = nullptr;
    m_pRefCount = nullptr;
    m_nBuffLen = 0;
    m_nStrLen = 0;
}

void CString::CopyOnWrite()
{
    if (*m_pRefCount > 1)
    {
        CString str(m_pStrBuff);
        ClearStr();
        InitStr(str.m_pStrBuff);
    }
}

ostream& operator<<(ostream& os, CString& str)
{
    if (str.m_pStrBuff != nullptr)
    {
        os << str.m_pStrBuff;
    }
    return os;
}

istream& operator>>(istream& is, CString& str)
{
    char buf[1024] = { 0 };
    is >> buf;
    str = buf;

    return is;
}
