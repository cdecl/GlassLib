// cust_exception.h: interface for the cust_exception class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUST_EXCEPTION_H__84070A8C_8833_470B_8D33_0B433C90B15F__INCLUDED_)
#define AFX_CUST_EXCEPTION_H__84070A8C_8833_470B_8D33_0B433C90B15F__INCLUDED_

#include <exception>
#include <string>

class cust_exception : public std::exception
{
public:
	cust_exception() : _strError("unknown exception"), _nError(-1) {}
	explicit cust_exception(const std::string& strError) : _strError(strError) {}
	cust_exception(const std::string& strError, int nError)  : _strError(strError), _nError(nError) {}

	virtual ~cust_exception() {};

public:
	virtual const char *what() const {
		return _strError.c_str();
	}

	virtual int error_no() const {
		return _nError;
	}

private:
	std::string _strError;
	int _nError;
};


//////////////////////////////////////////////////////////////////////
// 참이 아니면 throw 
inline void assert_throw(bool bSuccess, cust_exception &_e) 
{
	if (!bSuccess) throw _e;
}


#endif // !defined(AFX_CUST_EXCEPTION_H__84070A8C_8833_470B_8D33_0B433C90B15F__INCLUDED_)
