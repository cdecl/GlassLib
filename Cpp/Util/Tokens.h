// Tokens.h: interface for the CTokens class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#ifndef __TOKENS_H__BY_CDECL__
#define __TOKENS_H__BY_CDECL__


#include <string>		// for string class 


namespace GLASS 
{

class Tokens 
{
public:
	typedef std::string::size_type  size_type;
	typedef std::string	 buff_type;

public:
	Tokens();
	Tokens(const std::string &strBuff, size_type nPos = 0);
	virtual ~Tokens();

public:
	bool IsEOF();
	bool SetPosition(const size_type nPos);
	size_type GetPosition() const;
	std::string GetNextWord(const size_type nLen = 0);
	std::string GetNextWord(const std::string &strFD);
	std::string GetNextWord(const char cFD);
	void Str(const std::string &strBuff, size_type nPos = 0);
	std::string Str();

private:
	size_type m_nPos;
	buff_type m_strBuff;
};


} // GLASS namespace 


#endif // !defined(AFX_TOKENS_H__8AA8097B_79A0_4B2A_960E_DA4B536B7EAA__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


