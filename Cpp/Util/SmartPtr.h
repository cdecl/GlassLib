// SmartPtr.h
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
//////////////////////////////////////////////////////////////////////

#ifndef __SMARTPTR_H__BY_CDECL__
#define __SMARTPTR_H__BY_CDECL__


#include <algorithm>

namespace GLASS {



template <class T>
class SmartPtr
{
public:
	typedef size_t	RefT;

public:
	explicit SmartPtr(T *pType = NULL) : ptr_(NULL)
	{
		if (pType) {
			ptr_ = new ItemPtr<T>(pType);
		}
	}
	
	~SmartPtr()
	{
		Release();
	}
		
	SmartPtr(const SmartPtr<T> &sp) 
	{
		ptr_ = sp.ptr_;
		ptr_->AddRef();
	}
	
	SmartPtr<T>& operator=(const SmartPtr<T> &sp)
	{
		if (this != &sp) {
			Release();
			
			ptr_ = sp.ptr_;
			ptr_->AddRef();
		}
		return *this;
	}
	
public:
	void Reset(T *pType)
	{
		Release();
		if (pType) {
			ptr_ = new ItemPtr<T>(pType);
		}
	}
	
	void Release()
	{
		if (ptr_ && ptr_->SubRef() <= 0) {
			delete ptr_;
			ptr_ = NULL;
		}
	}

	void Swap(SmartPtr<T> &sp)
	{
		std::swap(ptr_, sp.ptr_);
	}

	

	T& operator*()
	{
		return *ptr_->Get();
	}

	const T& operator*() const
	{
		return *ptr_->Get();
	}



	T* operator->()
	{
		return ptr_->Get();
	}

	const T* operator->() const
	{
		return ptr_->Get();
	}



	T* Get() const
	{
		return ptr_->Get();
	}
	
	RefT RefCount() const
	{
		ptr_->RefCount();
	}

	operator bool() const 
	{
		return ptr_ != NULL;
	}

	bool operator!() const
	{
		return ptr_ == NULL;
	}
	
private:
	template <class T_>
	class ItemPtr
	{
	

	public:
		ItemPtr(T_ *pType) : pType_(pType), nRef_(0)
		{
			if (pType_) AddRef();
		}
		
		~ItemPtr() 
		{
			delete pType_;
			pType_ = NULL;
		}

	private:
		ItemPtr(const ItemPtr<T_> &);
		ItemPtr& operator=(const ItemPtr<T_> &);
		
	public:
		void AddRef() 
		{
			++nRef_;
		}
		
		RefT SubRef()
		{
			if (--nRef_ <= 0) {
				delete pType_;
				pType_ = NULL;
			}
			
			return RefCount();
		}	
		
		RefT RefCount() const
		{
			return nRef_;
		}
		
		T_* Get() const 
		{
			return pType_;
		}
		
	private:
		T_ *pType_;
		RefT nRef_;	
	};

	
private:
	ItemPtr<T> *ptr_;
};


} // namespace GLASS

#endif	// __SMARTPTR_H__BY_CDECL__


