#pragma once
#include <iostream>
using namespace std;

class CString
{
public:
    CString();
    CString(const char* szStr);
    CString(const CString& str);
    ~CString();

    int Length() const;
    const char* GetString() const;

    //拼接 - 相当于strcat
    CString& Append(const char* szStr);
    CString& Append(const CString& str);
    CString& Append(int nVal);
    CString& Append(double dblVal);
    CString& Append(char chVal);
    CString& Append(float fVal);

    //查找 -找不到返回-1
    int Find(const char* szStr);
    int Find(const CString& str);
    int ReserveFind(const char* szStr);
    int ReserveFind(const CString& str);

    //替换 - 把src替换成dst
    CString& Replace(const char* szDst);
    CString& Replace(const CString& strDst);
    CString& Replace(int nIdxBegin, const char* szDst);
    CString& Replace(int nIdxBegin, const CString& strDst);

    //拷贝 - strcpy
    void Copy(const char* szStr);
    void Copy(const CString& str);

    //提取子串
    CString Mid(int nIdx, int nLen);//从索引nIdx开始提取nLen个字节为新的字符串
    CString Left(int nLen);//从字符串左侧开始提取nLen个字节为新的字符串
    CString Right(int nLen);//从字符串右侧开始提取nLen个字节为新的字符串

    //去除空白字符
    void Strip();//空格, tab, 回车

    //格式化
    //void Format(const char* szFmt, ...); //vsprintf

    static CString ValueOf(bool b);  //true ==> "true"
    static CString ValueOf(char c);     //a ==> "a"
    static CString ValueOf(double d);  //3.5 ==> "3.5"
    static CString ValueOf(float f);    //4.5f ==> "4.5"
    static CString ValueOf(int i);      //123 ==> "123"
    static CString ValueOf(long i);     //123 ==> "123"
    static CString ValueOf(short i);    //123 ==> "123"
    static CString ValueOf(const char* data); //char[] = "123" ==> string "123"
    static CString CopyValueOf(const char* data, int offset, int count); //char[] = "123" ==> string "123"
    static CString Format(const char* format, ...);  //("%d", 123) ==> string "123"

    CString& operator=(const char* szStr);
    CString& operator=(const CString& str);
    CString operator+(const char* szStr);
    CString operator+(const CString& str);
    char operator[](int nIndex);
    bool operator==(const char* szStr);
    bool operator==(const CString& str);
    bool operator!=(const char* szStr);
    bool operator!=(const CString& str);
    bool operator>(const char* szStr);
    bool operator>(const CString& str);
    bool operator<(const CString& str);

    friend ostream& operator<<(ostream& os, CString& str);
    friend istream& operator>>(istream& is, CString& str);

private:
    void InitStr(const char* szStr);
    void ClearStr();
    void CopyOnWrite();

private:
    char* m_pStrBuff; //指向字符串的缓冲区
    int* m_pRefCount; //每个字符串分配一个引用计数
    int   m_nBuffLen; //缓冲区的大小Capacity
    int   m_nStrLen;  //缓冲区中字符串的大小
};

