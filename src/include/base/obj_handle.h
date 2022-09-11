
#ifndef __cxz_obj_handle_h__
#define __cxz_obj_handle_h__

#include "types.h"

namespace CNORXZ
{

    // Pointer wrapper for directly handling objects via abstract base class;
    // Provides appropriate copy implementation
    template <typename T>
    class ObjHandle
    {
    private:
	std::unique_ptr<T> mC;

    public:

	ObjHandle();
	ObjHandle(const T& a);
	ObjHandle(const ObjHandle& a);
	ObjHandle(ObjHandle&& a);
	ObjHandle& operator=(const ObjHandle& a);
	ObjHandle& operator=(ObjHandle&& a);
	
	T& operator*();
	T* operator->();
	const T& operator*() const;
	const T* operator->() const;
    };
    
}

#endif
