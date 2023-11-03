/**
   
   @file include/base/obj_handle.h
   @brief ObjHandle template declaration
   
   Pointer wrapper for directly handling objects via abstract base class;
   Provides appropriate copy implementation
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_obj_handle_h__
#define __cxz_obj_handle_h__

#include "types.h"

namespace CNORXZ
{

    template <typename T>
    class ObjHandle
    {
    protected:
	Uptr<T> mC;

    public:

	ObjHandle();
	ObjHandle(Uptr<T>&& a);
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
