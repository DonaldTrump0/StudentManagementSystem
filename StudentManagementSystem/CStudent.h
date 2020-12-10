#pragma once

#include "CString.h"
#include <iostream>
using namespace std;

class CStudent
{
public:
    CStudent();
    CStudent(int nId, int nBirthYear, char chBirthMonth, char chSex, CString strName);
    CStudent(const CStudent& student);

    void SetId(int nId);
    int GetId() const;
    void SetBirthYear(int nBirthYear);
    int GetBirthYear() const;
    void SetBirthMonth(char chBirthMonth);
    char GetBirthMonth() const;
    void SetSex(char chSex);
    char GetSex() const;
    void SetName(CString strName);
    CString GetName() const;

    // ���ѧ����Ϣ
    void Clear();
    // ����ѧ��
    void UpdateStudent(const CStudent& student);

    // ��ѧ����ת���ֽ����飬���ش�С
    int ToByteArray(char* pBuf) const;
    // ���ֽ�����ת��ѧ����
    static CStudent ToStudent(char* pByteArray);

    bool operator<(const CStudent& student);
    bool operator>(const CStudent& student);
    friend ostream& operator<<(ostream& os, CStudent& student);

private:
    int m_nId;
    int m_nBirthYear;
    char m_chBirthMonth;
    char m_chSex;
    CString m_strName;
};

