/**
 * MemPool.h
 *
 * Memory Pooling Class
 *
 * Copyright (c) 2004 by cdecl (byung-kyu kim)
 *
 */

#ifndef __MEM_POOL_H__BY__CDECL
#define __MEM_POOL_H__BY__CDECL


#ifdef _WIN32
 #include <windows.h>
#else 
 #include <pthread.h>
#endif



namespace GLASS {


#ifdef _WIN32
#else
	typedef pthread_mutex_t		CRITICAL_SECTION;
	typedef pthread_mutex_t*	LPCRITICAL_SECTION;
#endif 


namespace CS {

class CriticalSection
{
public:
	CriticalSection() 
	{
#ifdef _WIN32
		InitializeCriticalSection(&cs_);
#else 
		pthread_mutex_init(&cs_, NULL);
#endif 
		
	}

	~CriticalSection() 
	{

#ifdef _WIN32
		DeleteCriticalSection(&cs_);
#else 
		pthread_mutex_destroy(&cs_);
#endif 
		
	}

	LPCRITICAL_SECTION operator&()
	{
		return &cs_;
	}

private:
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);

private:
	CRITICAL_SECTION cs_;
};


class CriticalSectionLock
{
public:
	//////////////////////////////////////////////////////////////////
	// EnterCriticalSection
	CriticalSectionLock(LPCRITICAL_SECTION pcs) : pcs_(pcs) 
	{ 
#ifdef _WIN32
		EnterCriticalSection(pcs_); 
#else 
		pthread_mutex_lock(pcs_); 
#endif 
	}

	//////////////////////////////////////////////////////////////////
	// LeaveCriticalSection
	~CriticalSectionLock() 
	{ 
#ifdef _WIN32
		LeaveCriticalSection(pcs_); 
#else 
		pthread_mutex_unlock(pcs_); 
#endif 
		
	}

private:
	LPCRITICAL_SECTION pcs_;
};


} // namespace CS






// MemPool Class 
class MemPool
{
public:	
	typedef unsigned int size_type;
	enum { DEFAULT_CHUNK_SIZE = 32 };

	struct MemNode { MemNode *next_; };

public:
	MemPool(size_type type_size, size_type chunk_size = DEFAULT_CHUNK_SIZE) : 
		pHead_(0), pFree_(0), nUsed_(0), 
		TYPESIZE(type_size > sizeof(MemNode*) ? type_size : sizeof(MemNode*)),
		CHUNK_SIZE(chunk_size)
	{
		ExpanseChunk();
	};
	~MemPool() { Release(); }
	
public:
	void* malloc()
	{
		CS::CriticalSectionLock scopeLock(&csLock_);

		if (IsFull()) {
			if (pFree_) return (NodeNext(pFree_));

			ExpanseChunk();
		}

		int nOffSet = sizeof(MemNode*) + (TYPESIZE * nUsed_++);
		return (reinterpret_cast<char*>(pHead_) + nOffSet);
	}

	void free(void *p)
	{
		CS::CriticalSectionLock scopeLock(&csLock_);

		MemNode *pmn = reinterpret_cast<MemNode*>(p);
		pmn->next_ = pFree_;
		pFree_ = pmn;
	}

	void ExpanseChunk()
	{
		size_type nAllocSize = TYPESIZE * CHUNK_SIZE + sizeof(MemNode*);

		MemNode *pmn = 
			reinterpret_cast<MemNode *>(::operator new(nAllocSize));

		nUsed_ = 0;;

		pmn->next_ = pHead_;
		pHead_ = pmn;
	}

	bool IsFull() const 
	{
		return nUsed_ >= CHUNK_SIZE;
	}

	void Release()
	{
		while (pHead_ != 0) {
			::operator delete(NodeNext(pHead_));
		}
	}

private:
	inline MemNode* NodeNext(MemNode *&pmn)
	{
		MemNode *p = pmn;
		pmn = pmn->next_;
		return p;
	}

private:
	// No Copy
	MemPool(const MemPool&);
	MemPool& operator=(const MemPool&);

private:
	MemNode *pHead_;
	MemNode *pFree_;
	
	size_type nUsed_;

private:
	const size_type TYPESIZE;
	const size_type CHUNK_SIZE;

private:
	CS::CriticalSection csLock_;

};




} // namespace GLASS

#endif 


