#include "CStudent.h"
#include <iostream>
using namespace std;

CStudent::CStudent()
    : m_nId(-1), m_nBirthYear(-1), m_chBirthMonth(-1), m_chSex(-1), m_strName()
{
}

CStudent::CStudent(int nId, int nBirthYear, char chBirthMonth, char chSex, CString strName)
    : m_nId(nId), m_nBirthYear(nBirthYear), m_chBirthMonth(chBirthMonth), m_chSex(chSex), m_strName(strName)
{
}

CStudent::CStudent(const CStudent& student)
{
    m_nId = student.m_nId;
    m_nBirthYear = student.m_nBirthYear;
    m_chBirthMonth = student.m_chBirthMonth;
    m_chSex = student.m_chSex;
    m_strName = student.m_strName;
}

void CStudent::SetId(int nId)
{
    m_nId = nId;
}

int CStudent::GetId() const
{
    return m_nId;
}

void CStudent::SetBirthYear(int nBirthYear)
{
    m_nBirthYear = nBirthYear;
}

int CStudent::GetBirthYear() const
{
    return m_nBirthYear;
}

void CStudent::SetBirthMonth(char chBirthMonth)
{
    m_chBirthMonth = chBirthMonth;
}

char CStudent::GetBirthMonth() const
{
    return m_chBirthMonth;
}

void CStudent::SetSex(char chSex)
{
    m_chSex = chSex;
}

char CStudent::GetSex() const
{
    return m_chSex;
}

void CStudent::SetName(CString strName)
{
    m_strName = strName;
}

CString CStudent::GetName() const
{
    return m_strName;
}

void CStudent::Clear()
{
    m_nId = -1;
    m_nBirthYear = -1;
    m_chBirthMonth = -1;
    m_chSex = -1;
    m_strName = CString();
}

void CStudent::UpdateStudent(const CStudent& student)
{
    if (student.m_strName.Length() != 0)
    {
        m_strName = student.m_strName;
    }
    if (student.m_nBirthYear != -1)
    {
        m_nBirthYear = student.m_nBirthYear;
    }
    if (student.m_chBirthMonth != -1)
    {
        m_chBirthMonth = student.m_chBirthMonth;
    }
    if (student.m_chSex != -1)
    {
        m_chSex = student.m_chSex;
    }
}

int CStudent::ToByteArray(char* pBuf) const
{
    char* p = pBuf;
    memmove(p, &m_nId, sizeof(m_nId));
    p += sizeof(m_nId);
    memmove(p, &m_nBirthYear, sizeof(m_nBirthYear));
    p += sizeof(m_nBirthYear);
    memmove(p, &m_chBirthMonth, sizeof(m_chBirthMonth));
    p += sizeof(m_chBirthMonth);
    memmove(p, &m_chSex, sizeof(m_chSex));
    p += sizeof(m_chSex);
    memmove(p, m_strName.GetString(), m_strName.Length() + 1);
    p += m_strName.Length() + 1;

    return p - pBuf;
}

CStudent CStudent::ToStudent(char* pByteArray)
{
    CStudent student;
    student.SetId(*(int*)pByteArray);
    pByteArray += sizeof(m_nId);
    student.SetBirthYear(*(int*)pByteArray);
    pByteArray += sizeof(m_nBirthYear);
    student.SetBirthMonth(*pByteArray);
    pByteArray += sizeof(m_chBirthMonth);
    student.SetSex(*pByteArray);
    pByteArray += sizeof(m_chSex);
    student.SetName(pByteArray);

    return student;
}

bool CStudent::operator<(const CStudent& student)
{
    return m_nId < student.m_nId;
}

bool CStudent::operator>(const CStudent& student)
{
    return m_nId > student.m_nId;
}

ostream& operator<<(ostream& os, CStudent& student)
{
    os << "Id: " << student.m_nId << " "
        << "姓名: " << student.m_strName << " "
        << "出生年月: " << student.m_nBirthYear << "年"
        << (unsigned int)student.m_chBirthMonth << "月 ";

    switch (student.m_chSex)
    {
    case 1:
        os << "性别: 男" << " ";
        break;
    case 2:
        os << "性别: 女" << " ";
        break;
    default:
        os << "性别: 未知" << " ";
        break;
    }

    return os;
}
