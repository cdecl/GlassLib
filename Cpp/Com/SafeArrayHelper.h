// SafeArrayHelper.h: interface for the CSafeArrayHelper class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SafeArrayHelper_H__CEB5FFAE_BDB4_4CA2_A27F_997C67073242__INCLUDED_)
#define AFX_SafeArrayHelper_H__CEB5FFAE_BDB4_4CA2_A27F_997C67073242__INCLUDED_

#include <comdef.h>
#include <vector>


namespace GLASS 
{
	class SafeArrayHelper
	{
	public:
		enum { MAX_DIM_SIZE = 2 };
		typedef std::vector<std::pair<size_t, size_t> > Dimension;

	private:
		class SafeArrayAccessProxy 
		{
		public: 
			SafeArrayAccessProxy() : pParent_(NULL), pvar_(NULL), ix_(0) {}
			~SafeArrayAccessProxy() {}

			void Create(SafeArrayHelper *pParent) 
			{
				pParent_ = pParent;
			}

		public:
			_variant_t& operator=(const _variant_t &var)
			{
				return (pvar_[ix_] = var);
			}

			_variant_t*& GetRefPtr()
			{
				return pvar_;
			}
		
			SafeArrayAccessProxy& operator[](size_t i)
			{
				size_t row = pParent_->GetUBound() - pParent_->GetLBound() + 1;
				i -= pParent_->GetLBound(2);
				
				ix_ += (row * i);
				return *this;
			}

			operator _variant_t()
			{
				return pvar_[ix_];
			}

			void SetIndex(size_t i)
			{
				ix_ = i;
			}

		private:
			SafeArrayHelper *pParent_;
			_variant_t *pvar_;
			size_t ix_;
		};

	public:
		SafeArrayHelper() : psa_(NULL), bAddRef_(false)
		{
			ptr_.Create(this);
		}

		virtual ~SafeArrayHelper() 
		{
			Release();
		}

		void Create(size_t lSize, size_t nDim = 1, size_t lLBound = 0)
		{
			bAddRef_ = false;
			SAFEARRAYBOUND rgsabound[MAX_DIM_SIZE];

			for (size_t i = 0; i < nDim; ++i) {
				rgsabound[i].lLbound = lLBound;
				rgsabound[i].cElements = lSize;
			}

			psa_ = SafeArrayCreate(VT_VARIANT, nDim, rgsabound);

			MakeDimensionCache();
			SafeArrayAccessData(psa_, reinterpret_cast<void**>(&ptr_.GetRefPtr()));
		}
		 
		void Create2(size_t lSize1, size_t lSize2, size_t lLBound1 = 0, size_t lLBound2 = 0)
		{
			bAddRef_ = false;
			SAFEARRAYBOUND rgsabound[MAX_DIM_SIZE];
			size_t nDim = MAX_DIM_SIZE; 

			rgsabound[0].lLbound = lLBound1;
			rgsabound[0].cElements = lSize1;

			rgsabound[1].lLbound = lLBound2;
			rgsabound[1].cElements = lSize2;

			psa_ = SafeArrayCreate(VT_VARIANT, nDim, rgsabound);

			MakeDimensionCache();
			SafeArrayAccessData(psa_, reinterpret_cast<void**>(&ptr_.GetRefPtr()));
		}

		void Create(SAFEARRAY *psa, bool bAddRef = true)
		{
			bAddRef_ = bAddRef;
			
			if (bAddRef_) {
				psa_ = psa;
			}
			else {
				SafeArrayCopy(psa, &psa_);
			}

			MakeDimensionCache();
			SafeArrayAccessData(psa_, reinterpret_cast<void**>(&ptr_.GetRefPtr()));
		}

		void Release()
		{
			if (psa_) {
				SafeArrayUnaccessData(psa_);
			}
			
			if (psa_ && !bAddRef_) {
				SafeArrayDestroy(psa_);
			}

			dm_.clear();
		}

		SAFEARRAY* Detach()
		{
			SafeArrayUnaccessData(psa_);

			SAFEARRAY *pReturn = psa_;
			psa_ = NULL;

			return pReturn;
		}

		void VariantDetach(VARIANT &var)
		{
			var.vt = (VT_ARRAY | VT_VARIANT);
			var.parray = Detach();
		}

		//////////////////////////////////////////////////////////////////////////
		

		SafeArrayAccessProxy& operator[](size_t i)
		{
			ptr_.SetIndex(i - GetLBound());
			return ptr_;
		}
	
		size_t GetDim() const 
		{
			return dm_.size();
		}

		size_t GetUBound(size_t nDim = 1) 
		{
			return dm_[nDim - 1].second;
		}

		size_t GetLBound(size_t nDim = 1) 
		{
			return dm_[nDim - 1].first;
		}

		operator bool()
		{
			return psa_ ? true : false;
		}


		//////////////////////////////////////////////////////////////////////////
		// Static members
		
		static SAFEARRAY* GetSafeArrayPtr(VARIANT &var)
		{
			SAFEARRAY *psa = NULL;
			VARIANT *pRefVar = &var;

			if (var.vt == (VT_BYREF | VT_VARIANT)) { 
				pRefVar = var.pvarVal;
			}

			if (V_VT(pRefVar) == (VT_BYREF | VT_ARRAY | VT_VARIANT)) {
				psa = *(pRefVar->pparray);
			}
			else if (V_VT(pRefVar) == (VT_ARRAY | VT_VARIANT)) {
				psa = pRefVar->parray;
			}

			return psa;
		}


	private:
		void MakeDimensionCache()
		{
			size_t c = GetDim_();
			for (size_t i = 1; i <= c; ++i) {
				dm_.push_back(Dimension::value_type(GetLBound_(i), GetUBound_(i)));
			}
		}

		size_t GetDim_() const 
		{
			return SafeArrayGetDim(psa_);
		}

		size_t GetUBound_(size_t nDim = 1) 
		{
			long lUBound = 0;
			SafeArrayGetUBound(psa_, nDim, &lUBound);
			return lUBound;
		}

		size_t GetLBound_(size_t nDim = 1) 
		{
			long lLBound = 0;
			SafeArrayGetLBound(psa_, nDim, &lLBound);
			return lLBound;
		}

	private:
		SAFEARRAY *psa_;
		SafeArrayAccessProxy ptr_;
		bool bAddRef_;
		Dimension dm_;
	};

}


#endif

