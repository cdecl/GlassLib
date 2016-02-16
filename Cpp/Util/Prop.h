//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


#ifndef __PROP_H__BY_CDECL__
#define __PROP_H__BY_CDECL__

namespace GLASS {

	class IYES {};
	class YES : public IYES {};
	class NO {};

	template <class T, class Parent, class ReadWrite = YES>
	class Prop
	{
	public:
		typedef Prop<T, Parent>	this_type;
		typedef T (Parent::*get_fun)();
		typedef void (Parent::*set_fun)(const T&);

		Prop() : getf_(NULL), setf_(NULL), parent_(NULL)
		{}

		void SetParent(Parent *p) {
			parent_ = p;
		}

		void Set(set_fun f) {
			ReadWrite yn;
			IYES *p = &yn;	// 읽기전용일때 이함수를 사용하면 컴파일 에러 !!

			setf_ = f;
		}

		void Get(get_fun f) {
			getf_ = f;
		}

		T operator=(const T &value) {
			ReadWrite yn;
			IYES *p = &yn;	// 읽기전용일때 이함수를 사용하면 컴파일 에러 !!

			if (!setf_) {
				throw exception();
			}

			(parent_->*setf_)(value);
			return value;
		}

		T operator=(this_type &type) {
			return operator=(static_cast<T>(type));
		}

		operator T() {
			if (!getf_) {
				throw exception();
			}

			return (parent_->*getf_)();
		}

	private:
		Parent *parent_;
		get_fun getf_;
		set_fun setf_;

	};


} // namespace GLASS
	
		
#endif	// __PROP_H__BY_CDECL__

//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////



