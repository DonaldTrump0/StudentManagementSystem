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

    //ƴ�� - �൱��strcat
    CString& Append(const char* szStr);
    CString& Append(const CString& str);
    CString& Append(int nVal);
    CString& Append(double dblVal);
    CString& Append(char chVal);
    CString& Append(float fVal);

    //���� -�Ҳ�������-1
    int Find(const char* szStr);
    int Find(const CString& str);
    int ReserveFind(const char* szStr);
    int ReserveFind(const CString& str);

    //�滻 - ��src�滻��dst
    CString& Replace(const char* szDst);
    CString& Replace(const CString& strDst);
    CString& Replace(int nIdxBegin, const char* szDst);
    CString& Replace(int nIdxBegin, const CString& strDst);

    //���� - strcpy
    void Copy(const char* szStr);
    void Copy(const CString& str);

    //��ȡ�Ӵ�
    CString Mid(int nIdx, int nLen);//������nIdx��ʼ��ȡnLen���ֽ�Ϊ�µ��ַ���
    CString Left(int nLen);//���ַ�����࿪ʼ��ȡnLen���ֽ�Ϊ�µ��ַ���
    CString Right(int nLen);//���ַ����Ҳ࿪ʼ��ȡnLen���ֽ�Ϊ�µ��ַ���

    //ȥ���հ��ַ�
    void Strip();//�ո�, tab, �س�

    //��ʽ��
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
    char* m_pStrBuff; //ָ���ַ����Ļ�����
    int* m_pRefCount; //ÿ���ַ�������һ�����ü���
    int   m_nBuffLen; //�������Ĵ�СCapacity
    int   m_nStrLen;  //���������ַ����Ĵ�С
};

