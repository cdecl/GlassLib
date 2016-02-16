

#ifndef __FACTORY_H__BY_CDECL__
#define __FACTORY_H__BY_CDECL__

#include <map>
#include <exception>

namespace GLASS {

class FactoryException : std::exception
{
public: 
	const char* what() 
	{
		return "FactoryException";
	}
};


template <typename AbstractT, typename Key, typename CreateFunT = AbstractT* (*)()>
class Factory
{
public:
	typedef std::map<Key, CreateFunT> FactoryMap;

public:
	bool Register(const Key &id, CreateFunT fun) 
	{
		return fmap_.insert(FactoryMap::value_type(id, fun)).second;
	}

	bool Unregister(const Key &id) 
	{
		return fmap_.erase(id) > 0;
	}

	AbstractT* CreateObject(const Key &id) 
	{
		FactoryMap::iterator it = fmap_.find(id);
		if (it != fmap_.end()) {
			return (it->second)();
		}

		throw FactoryException();
	}
	
private:
	FactoryMap fmap_;
};


}	// namespace GLASS

#endif // __FACTORY_H__BY_CDECL__