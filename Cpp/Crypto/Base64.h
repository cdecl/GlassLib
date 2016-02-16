// Base64.h: interface for the Base64 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BASE64_H__BY_CDECL__
#define __BASE64_H__BY_CDECL__

#include <string>

namespace GLASS {


class Base64
{
public:
	typedef unsigned char byte;

private:
	Base64();
	Base64(Base64&);
	Base64& operator=(Base64&);

public:
	static std::string Encoding(const std::string &str);
	static std::string Encoding(const char *pstr, int nLen);

	static std::string Decoding(const std::string &str);
	static std::string Decoding(const char *pstr, int nLen);

public:
	static int DecodeByte(const byte b);

};


}	//	namespace GLASS 

#endif // __BASE64_H__BY_CDECL__
