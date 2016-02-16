// Tokens.cpp: implementation of the Tokens class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Tokens.h"
using namespace GLASS;
using std::string;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Tokens::Tokens()
{
	Str(string(""));
}


Tokens::Tokens(const string &strBuff, size_type nPos)
{
	Str(strBuff, nPos);
}


Tokens::~Tokens()
{

}

//////////////////////////////////////////////////////////////////////
// 초기화 
void Tokens::Str(const string &strBuff, size_type nPos)
{
	m_strBuff = strBuff;
	m_nPos = nPos;
}


//////////////////////////////////////////////////////////////////////
// Get Buffer String
string Tokens::Str()
{
	return m_strBuff;
}

//////////////////////////////////////////////////////////////////////
// 고정길이 파싱 
string Tokens::GetNextWord(const size_type nLen)
{
	if (IsEOF()) {
		return string("");
	}

	string str;
	// 0을 넘기면 나머지 모든 문자열을 리턴한다 
	if (nLen == 0) {
		str = m_strBuff.substr(m_nPos);
		m_nPos = m_strBuff.length();
	}
	else {
		str = m_strBuff.substr(m_nPos, nLen);
		m_nPos += nLen;
	}
	
	return str;
}

//////////////////////////////////////////////////////////////////////
// 구분자 파싱 
string Tokens::GetNextWord(const string &strFD)
{
	if (IsEOF()) {
		return string("");
	}
	size_type nPos = m_nPos;

	size_type nFind = m_strBuff.find_first_of(strFD, m_nPos); 
	if (nFind == string::npos) {	
		m_nPos = m_strBuff.length();
		return m_strBuff.substr(nPos);
	}
	
	m_nPos = nFind + 1;
	return m_strBuff.substr(nPos, nFind - nPos);
}

string Tokens::GetNextWord(const char cFD)
{
	return GetNextWord(string(&cFD, 1));
}

//////////////////////////////////////////////////////////////////////
// Get Position Index
Tokens::size_type Tokens::GetPosition() const
{
	return m_nPos;
}

//////////////////////////////////////////////////////////////////////
// Set Position Index
bool Tokens::SetPosition(const size_type nPos)
{
	if (m_strBuff.length() <= nPos) {
		return false;
	}

	m_nPos = nPos;
	return true;
}

bool Tokens::IsEOF()
{
	return (m_nPos >= m_strBuff.length());
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


