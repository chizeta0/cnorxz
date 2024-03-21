// -*- C++ -*-
/**
   
   @file include/base/obj_handle.cc.h
   @brief ObjHandle function definition
   
   Implementation of the member functions of template ObjHandle
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_obj_handle_cc_h__
#define __cxz_obj_handle_cc_h__

#include "obj_handle.h"

namespace CNORXZ
{
    
    template <typename T>
    ObjHandle<T>::ObjHandle() {}

    template <typename T>
    ObjHandle<T>::ObjHandle(Uptr<T>&& a) : mC(std::forward<Uptr<T>>(a)) {}

    template <typename T>
    ObjHandle<T>::ObjHandle(const ObjHandle& a) : mC(a.mC->copy()) {}

    template <typename T>
    ObjHandle<T>::ObjHandle(ObjHandle&& a) : mC(a.mC) {}

    template <typename T>
    ObjHandle<T>& ObjHandle<T>::operator=(const ObjHandle& a)
    {
	mC = std::make_unique<T>(*a.mC);
	return *this;
    }

    template <typename T>
    ObjHandle<T>& ObjHandle<T>::operator=(ObjHandle&& a)
    {
	mC = std::move(a.mC);
	return *this;
    }
    
    template <typename T>
    T& ObjHandle<T>::operator*()
    {
	return *mC;
    }

    template <typename T>
    T* ObjHandle<T>::operator->()
    {
	return &*mC;
    }

    template <typename T>
    const T& ObjHandle<T>::operator*() const
    {
	return *mC;
    }

    template <typename T>
    const T* ObjHandle<T>::operator->() const
    {
	return &*mC;
    }

}

#endif
