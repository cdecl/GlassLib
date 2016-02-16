// CryptDSAHash.cpp: implementation of the CryptDSAHash class.
//
//////////////////////////////////////////////////////////////////////

#include "CryptDSAHash.h"
using namespace std;
using namespace GLASS;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CryptDSAHash::CryptDSAHash() : hProv_(0), hHash_(0), hKey_(0)
{
	AcquireContext();
}

CryptDSAHash::~CryptDSAHash()
{
	ReleaseContext();
}

void CryptDSAHash::AcquireContext()
{
	CryptAcquireContext(&hProv_, NULL, NULL, PROV_RSA_FULL, 0);
}

void CryptDSAHash::ReleaseContext()
{
	CryptReleaseContext(hProv_, 0);
	hProv_ = 0;
}

void CryptDSAHash::CreateHash()
{
	// CALG_SHA : US DSA Secure Hash Algorithm 
	assert(CryptCreateHash(hProv_, CALG_SHA, 0, 0, &hHash_));
}

void CryptDSAHash::HashData(const string &strKey)
{
	assert(CryptHashData(hHash_, (BYTE*)strKey.c_str(), strKey.length(), 0));
}

void CryptDSAHash::DeriveKey(ALG_ID alg_id)
{
	assert(CryptDeriveKey(hProv_, alg_id, hHash_, 0, &hKey_));
}

void CryptDSAHash::DestroyKey()
{
	CryptDestroyKey(hKey_);
	hKey_ = 0;
}

void CryptDSAHash::DestroyHash()
{
	CryptDestroyHash(hHash_); 
	hHash_ = 0;
}


void CryptDSAHash::CreateKey(const string &strKey, ALG_ID alg_id)
{
	CreateHash();
	HashData(strKey);
	DeriveKey(alg_id);
}


void CryptDSAHash::DeleteKey()
{
	DestroyKey();
	DestroyHash();
}


string CryptDSAHash::Encrypt(const string &strData, const string &strKey, ALG_ID alg_id)
{
	if (strData.length() == 0 || strKey.length() == 0) {
		return "";
	}
	CreateKey(strKey, alg_id);

	char szBuff[BLOCK_SIZE];
	
	int nPastLen = 0;
	int nLen = strData.length();

	string strResult;
	DWORD dwSize = 0;

	while (nLen > nPastLen) {
		if (nLen - nPastLen <= BLOCK_SIZE - 1) {
			dwSize = nLen - nPastLen;
		}
		else {
			dwSize = BLOCK_SIZE - 1;
		}

		copy(
			&strData[nPastLen],
			&strData[nPastLen + dwSize], 
			szBuff
		);
		nPastLen += dwSize;

		assert(CryptEncrypt(hKey_, NULL, TRUE, 0, (BYTE*)szBuff, &dwSize, BLOCK_SIZE));
		strResult.insert(strResult.end(), szBuff, szBuff + dwSize);
	}

	DeleteKey();
	return strResult;
}

string CryptDSAHash::Decrypt(const string &strData, const string &strKey, ALG_ID alg_id)
{
	if (strData.length() == 0 || strKey.length() == 0) {
		return "";
	}
	CreateKey(strKey, alg_id);

	char szBuff[BLOCK_SIZE];
	
	int nPastLen = 0;
	int nLen = strData.length();

	string strResult;
	DWORD dwSize = 0;

	while (nLen > nPastLen) {
		if (nLen - nPastLen <= BLOCK_SIZE) {
			dwSize = nLen - nPastLen;
		}
		else {
			dwSize = BLOCK_SIZE;
		}

		copy(
			&strData[nPastLen],
			&strData[nPastLen + dwSize], 
			szBuff
		);
		nPastLen += dwSize;

		CryptDecrypt(hKey_, NULL, TRUE, 0, (BYTE*)szBuff, &dwSize);
		strResult.insert(strResult.end(), szBuff, szBuff + dwSize);
	}

	DeleteKey();
	return strResult;
}
