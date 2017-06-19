#ifndef REF_H
#define REF_H

#if defined(_WIN32)
#  ifdef _GD_CPP_CORE_API_IMPL
#    define GD_CPP_CORE_API __declspec(dllexport)
#  else
#    define GD_CPP_CORE_API __declspec(dllimport)
#  endif
#else
#  define GD_CPP_CORE_API
#endif

#include "Variant.hpp"

namespace godot {

template<class T>
class Ref {
	T *reference;

	void ref(const Ref &from)
	{
		if (from.reference == reference)  return;
		
		unref();
		
		reference = from.reference;
		if (reference)  reference->reference();
	}
	
	void ref_pointer(T *r)
	{
		if (!r)  return;
		
		if (r->init_ref())  reference = r;
	}
	
	
public:
	inline bool operator==(const Ref<T> &r) const
	{
		return reference == r.reference;
	}
	
	inline bool operator!=(const Ref<T> &r) const
	{
		return reference != r.reference;
	}
	
	inline T *operator->()
	{
		return reference;
	}
	
	inline T *operator*()
	{
		return reference;
	}
	
	inline T *ptr()
	{
		return reference;
	}
	
	inline const T *operator->() const
	{
		return reference;
	}
	
	inline const T *operator*() const
	{
		return reference;
	}
	
	inline const T *ptr() const
	{
		return reference;
	}
	
	
	
	void operator=(const Ref &from)
	{
		ref(from);
	}
	
	void operator=(const Variant &variant)
	{
		T *r = variant;
		if (!r) {
			unref();
			return;
		}
		
		Ref re;
		re.reference = r;
		ref(re);
		re.reference = nullptr;
	}
	
	operator Variant() const
	{
		ref();
		return Variant((Object *) this);
	}
	
	
	Ref(const Ref &from)
	{
		reference = nullptr;
		ref(from);
	}
	
	Ref(T *r)
	{
		if (r)
			ref_pointer(r);
		else
			reference = nullptr;
	}
	
	Ref(const Variant &variant)
	{
		reference = nullptr;
		T *r = variant;
		if (!r) {
			unref();
			return;
		}
		
		Ref re;
		re.reference = r;
		ref(re);
		re.reference = nullptr;
	}
	
	
	inline bool is_valid() const { return reference != nullptr; }
	inline bool is_null() const { return reference == nullptr; }
	
	void unref()
	{
		if (reference && reference->unreference()) {
			godot_object_destroy((godot_object *) reference);
		}
		reference = nullptr;
	}
	
	Ref()
	{
		reference = nullptr;
	}
	
	~Ref()
	{
		unref();
	}
};

}

#endif