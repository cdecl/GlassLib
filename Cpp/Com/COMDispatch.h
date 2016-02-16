// COMDispatch.h: interface for the IDispatch class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#ifndef _DISPATCH_H_20011012_BY_CDECL_
#define _DISPATCH_H_20011012_BY_CDECL_

#include <comdef.h>
#include <vector>


#define BYREF(x)	GLASS::COMDispatch::ToRefParam((x))

#define COMDISP_PARAM_10	\
	PT, PT, PT, PT, PT, PT, PT, PT, PT, PT
#define COMDISP_PARAM_20	\
	COMDISP_PARAM_10, COMDISP_PARAM_10
#define COMDISP_PARAM_30	\
	COMDISP_PARAM_10, COMDISP_PARAM_10, COMDISP_PARAM_10


namespace GLASS {


class COMDispatch 
{
public:
	typedef std::vector<_variant_t> vec_param;
	typedef _variant_t PT;	

	// Proxy Class
	class Proxy
	{	
	public:
		void SetParent(COMDispatch *p) {
			pDisp_ = p;
		}

		void SetPropName(const _bstr_t &bstrName) {
			bstrName_ = bstrName;
		}

		_variant_t operator=(const _variant_t &var) const {
			pDisp_->PutProperty(bstrName_, var);
			return var;
		}

		operator _variant_t() const {
			return pDisp_->GetProperty(bstrName_);
		}

		_variant_t operator() ();
		_variant_t operator() (PT);
		_variant_t operator() (PT, PT);
		_variant_t operator() (PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (PT, PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_10, PT, PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_20, PT, PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT, PT, PT, PT, PT, PT);
		_variant_t operator() (COMDISP_PARAM_30, PT, PT, PT, PT, PT, PT, PT, PT, PT, PT);

	private:
		_variant_t InvokeParam_(PT* pArg, int nParamCount);

	private:
		COMDispatch *pDisp_;
		_bstr_t bstrName_;
	};

public:
	COMDispatch();
	COMDispatch(const COMDispatch &disp);
	explicit COMDispatch(const _variant_t &var);
	virtual ~COMDispatch();

	COMDispatch& operator=(const COMDispatch &disp);
	COMDispatch& operator=(const _variant_t &var);

public:
	_variant_t Call(const _bstr_t &bstrMethodName, vec_param &vecParams);

	void PutProperty(const _bstr_t &strPropName, const _variant_t &var, bool bRef = false);
	_variant_t GetProperty(const _bstr_t &strPropName);

	virtual Proxy& operator[](const _bstr_t &bstrPropName);

	virtual void CreateObject(const _bstr_t &bstrProgID);
	virtual void Release();

public:
	virtual void GetIDsOfNames(const _bstr_t &bstrMethodName, DISPID &lDispID);

protected:
	virtual void Invoke(const DISPID lDispID, vec_param &vecParams, _variant_t &varRetVal, const WORD wFlags = DISPATCH_METHOD);

public:
	static _variant_t ToRefParam(_variant_t &var);

protected:
	Proxy proxy_;
	IDispatchPtr spDispatch_;	
};



class RDSDataspace : public COMDispatch
{
public:
	RDSDataspace();
	virtual ~RDSDataspace();

public:
	virtual void CreateObject(const _bstr_t &bstrProgID, const _bstr_t &bstrUrl);
	virtual void Release();
};



} // end namespace 


#endif // _DISPATCH_H_20011012_BY_CDECL_

//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


