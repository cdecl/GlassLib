// CryptDSAHash.h: interface for the CryptDSAHash class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CRYPTDSAHASH_H__BY_CDECL__
#define __CRYPTDSAHASH_H__BY_CDECL__

#ifndef _WIN32_WINNT
 #define _WIN32_WINNT	0x0500
#endif 

#include <windows.h>
#include <wincrypt.h>

#include <string>
#include <algorithm>
#include <cassert>

namespace GLASS {


class CryptDSAHash  
{
public:
	enum { BLOCK_SIZE = 64 };

public:
	CryptDSAHash();
	virtual ~CryptDSAHash();

public:
	std::string Decrypt(const std::string &strData, const std::string &strKey, ALG_ID alg_id = CALG_RC2);
	std::string Encrypt(const std::string &strData, const std::string &strKey, ALG_ID alg_id = CALG_RC2);

protected:
	void DeriveKey(ALG_ID alg_id);
	void HashData(const std::string &strKey);
	void CreateHash();
	void DestroyHash();
	void DestroyKey();
	void ReleaseContext();
	void AcquireContext();

	void CreateKey(const std::string &strKey, ALG_ID alg_id);
	void DeleteKey();

protected:
	
	HCRYPTPROV hProv_;
	HCRYPTHASH hHash_;
	HCRYPTKEY hKey_;
	
};


}	// namespace GLASS 

#endif // __CRYPTDSAHASH_H__BY_CDECL__
