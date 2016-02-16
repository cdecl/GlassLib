// Base64.cpp: implementation of the Base64 class.
//
//////////////////////////////////////////////////////////////////////

#include "Base64.h"
#include <cassert>
using namespace std;
using namespace GLASS;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static const Base64::byte BASE64[] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


string Base64::Encoding(const char *pstr, int nLen)
{
	if (!nLen) {
		return "";
	}

	std::string strResult;

	for (int i = 0; i < nLen; i += 3) {
		strResult += (byte)BASE64[(pstr[i] & 0xFC) >> 2];

		if (nLen <= (i + 1)) {
			strResult += BASE64[((pstr[i] & 0x03) << 4)];
			strResult += "==";
			break;
		}
		else {
			strResult += BASE64[((pstr[i] & 0x03) << 4) | ((pstr[i + 1] & 0xF0) >> 4)];
		}
		
		if (nLen <= (i + 2)) {
			strResult += BASE64[((pstr[i + 1] & 0x0F) << 2)];
			strResult += "=";
			break;
		}
		else {
			strResult += BASE64[((pstr[i + 1] & 0x0F) << 2) | ((pstr[i + 2] & 0xC0) >> 6)];
		}

		strResult += BASE64[(pstr[i + 2] & 0x3F)];
	}

	return strResult;
}

string Base64::Encoding(const std::string &str)
{
	return Encoding(str.c_str(), str.length());
}

string Base64::Decoding(const char *pstr, int nLen)
{
	if (!nLen) {
		return "";
	}

	const byte END_CHAR = '@';
	std::string strResult;

	for (int i = 0; i < nLen; i += 4) {
		byte Pack[4];
		Pack[0] = DecodeByte(pstr[i]);
		Pack[1] = DecodeByte(pstr[i + 1]);
		Pack[2] = DecodeByte(pstr[i + 2]);
		Pack[3] = DecodeByte(pstr[i + 3]);

		if (Pack[0] == -1 || Pack[1] == -1 || Pack[2] == -1 || Pack[3] == -1) {
			break;
		}

		strResult += (Pack[0] << 2) | (Pack[1] >> 4);

		if (Pack[2] == END_CHAR) {
			break;
		}
		else {
			strResult += (Pack[1] << 4) | (Pack[2] >> 2);
		}

		if (Pack[3] == END_CHAR) {
			break;
		}
		else {
			strResult += (Pack[2] << 6) | Pack[3];
		}	
	}

	return strResult;
}


string Base64::Decoding(const std::string &str)
{
	return Decoding(str.c_str(), str.length());
}

int Base64::DecodeByte(const Base64::byte b)
{
	if (b >= 'A' && b <= 'Z') {
		return b - 'A';
	}
	else if (b >= 'a' && b <= 'z') {
		return b - 'a' + 26;
	}
	else if (b >= '0' && b <= '9') {
		return b - '0' + 52;
	}
	else if (b == '+') {
		return 62;
	}
	else if (b == '/') {
		return 63;
	}
	else if (b == '=') {
		return '@';
	}
	
	return -1;
}