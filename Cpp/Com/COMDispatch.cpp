// COMDispatch.cpp: implementation of the IDispatch class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "COMDispatch.h"
#include <algorithm>
#include <cstdarg>

using namespace GLASS;


//////////////////////////////////////////////////////////////////////
// Dispatch implementation
COMDispatch::COMDispatch() : spDispatch_(NULL)
{
	proxy_.SetParent(this); 
}


COMDispatch::COMDispatch(const COMDispatch &disp) : spDispatch_(disp.spDispatch_)
{
	proxy_.SetParent(this);
}


COMDispatch::COMDispatch(const _variant_t &var) : spDispatch_(var)
{
	proxy_.SetParent(this);
}


COMDispatch::~COMDispatch()
{
	Release();
}

COMDispatch& COMDispatch::operator=(const COMDispatch &disp)
{
	if (this != &disp) {
		spDispatch_ = disp.spDispatch_;
		proxy_.SetParent(this);
	}

	return *this;
}


COMDispatch& COMDispatch::operator=(const _variant_t &var) 
{
	spDispatch_ = var;
	proxy_.SetParent(this);

	return *this;
}

void COMDispatch::Release()
{	
	if (spDispatch_ != NULL) {
		spDispatch_.Release();
	}
}

void COMDispatch::CreateObject(const _bstr_t &bstrProgID) 
{
	HRESULT hr = spDispatch_.CreateInstance((LPCSTR)bstrProgID);

	if (FAILED(hr)) {
		_com_raise_error(hr); 
	}
}

_variant_t COMDispatch::Call(const _bstr_t &bstrMethodName, vec_param &vecParams)
{
	DISPID lDispID;
	GetIDsOfNames(bstrMethodName, lDispID);
	
	_variant_t varRetVal;
	Invoke(lDispID, vecParams, varRetVal);

	return varRetVal;
}


COMDispatch::Proxy& COMDispatch::operator[](const _bstr_t &bstrPropName)
{
	proxy_.SetPropName(bstrPropName);
	return proxy_;
}


void COMDispatch::PutProperty(const _bstr_t &strPropName, const _variant_t &var, bool bRef)
{
	DISPID lDispID;
	GetIDsOfNames(strPropName, lDispID);

	vec_param vecParams;
	vecParams.push_back(var);
	 
	_variant_t varRetVal;
	Invoke(lDispID, vecParams, varRetVal, bRef ? DISPATCH_PROPERTYPUTREF : DISPATCH_PROPERTYPUT);
}

_variant_t COMDispatch::GetProperty(const _bstr_t &strPropName)
{
	DISPID lDispID;
	GetIDsOfNames(strPropName, lDispID);

	vec_param vecParams;
	
	_variant_t varRetVal;
	Invoke(lDispID, vecParams, varRetVal, DISPATCH_PROPERTYGET);

	return varRetVal;
}


void COMDispatch::GetIDsOfNames(const _bstr_t &bstrMethodName, DISPID &lDispID)
{
	BSTR bstrMN = static_cast<BSTR>(bstrMethodName); 
	
	HRESULT hr = spDispatch_->GetIDsOfNames(IID_NULL, &bstrMN, 1, LOCALE_SYSTEM_DEFAULT, &lDispID);

	if (FAILED(hr)) {
		_com_raise_error(hr); 
	}
}


void COMDispatch::Invoke(const DISPID lDispID, vec_param &vecParams, _variant_t &varRetVal, const WORD wFlags)
{
	DISPPARAMS dispparams = {0};
	DISPID dsipid_put = DISPID_PROPERTYPUT;
	
	if (DISPATCH_PROPERTYGET != wFlags) {
		dispparams.cArgs = vecParams.size();
		dispparams.rgvarg = (vecParams.size() == 0) ? NULL : &vecParams.front();
		
		if ((DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF) & wFlags) {
			dispparams.rgdispidNamedArgs = &dsipid_put;
			dispparams.cNamedArgs = 1;
		}
	}

	HRESULT hr = spDispatch_->Invoke (
						lDispID, 
						IID_NULL, 
						LOCALE_USER_DEFAULT, 
						wFlags, 
						&dispparams, 
						DISPATCH_PROPERTYPUT == wFlags ? NULL : &varRetVal, 
						NULL, 
						NULL 
				);

	if (FAILED(hr)) {
		_com_raise_error(hr); 
	}	
}
	

_variant_t COMDispatch::ToRefParam(_variant_t &var)
{
	_variant_t varRet;
	varRet.vt = VT_BYREF | VT_VARIANT;
	varRet.pvarVal = &var;

	return varRet;
}


_variant_t COMDispatch::Proxy::InvokeParam_(PT *pArg, int nParamCount)
{
	vec_param vecParams;
	
	for (int i = 0; i < nParamCount; ++i) {
		PT &param = *pArg;
		vecParams.push_back(param);
		++pArg;
	}

	if (!vecParams.empty()) {
		std::reverse(vecParams.begin(), vecParams.end()); 
	}
	
	return pDisp_->Call(bstrName_, vecParams);
}


// 0
_variant_t COMDispatch::Proxy::operator() ()
{
	vec_param vecParams;
	return pDisp_->Call(bstrName_, vecParams);
}


// 1
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1
)
{
	return InvokeParam_(&param1, 1);
}


// 2
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT
)
{
	return InvokeParam_(&param1, 2);
}

// 3
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT
)
{
	return InvokeParam_(&param1, 3);
}

// 4
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT
)
{
	return InvokeParam_(&param1, 4);
}


// 5
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT
)
{
	return InvokeParam_(&param1, 5);
}


// 6
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT
)
{
	return InvokeParam_(&param1, 6);
}

// 7
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT
)
{
	return InvokeParam_(&param1, 7);
}


// 8
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT
)
{
	return InvokeParam_(&param1, 8);
}

// 9
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT
)
{
	return InvokeParam_(&param1, 9);
}

// 10
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, PT
)
{
	return InvokeParam_(&param1, 10);
}

//////////

// 11
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 11);
}


// 12
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 12);
}

// 13
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 13);
}

// 14
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 14);
}


// 15
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 15);
}


// 16
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 16);
}

// 17
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 17);
}


// 18
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 18);
}

// 19
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 19);
}

// 20
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, PT, COMDISP_PARAM_10
)
{
	return InvokeParam_(&param1, 20);
}



// 21
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 21);
}


// 22
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 22);
}

// 23
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 23);
}

// 24
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 24);
}


// 25
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 25);
}


// 26
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 26);
}

// 27
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 27);
}


// 28
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 28);
}

// 29
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 29);
}

// 30
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, PT, COMDISP_PARAM_20
)
{
	return InvokeParam_(&param1, 30);
}


// 31
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 31);
}


// 32
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 32);
}

// 33
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 33);
}

// 34
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 34);
}


// 35
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 35);
}


// 36
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 36);
}

// 37
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 37);
}


// 38
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 38);
}

// 39
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 39);
}

// 40
_variant_t COMDispatch::Proxy::operator() ( 
	PT param1, PT , PT, PT, PT, PT, PT , PT, PT, PT, COMDISP_PARAM_30
)
{
	return InvokeParam_(&param1, 40);
}




//////////////////////////////////////////////////////////////////////
// RDSDataspace implementation
RDSDataspace::RDSDataspace() : COMDispatch()
{
}

RDSDataspace::~RDSDataspace()
{
	Release();
}

void RDSDataspace::Release()
{	
}

void RDSDataspace::CreateObject(const _bstr_t &bstrProgID, const _bstr_t &bstrUrl) 
{
	COMDispatch disp;
	disp.CreateObject("RDS.Dataspace");
	spDispatch_ = disp["CreateObject"](bstrProgID, bstrUrl);
}



//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////