
#ifndef __cxz_obj_handle_cc_h__
#define __cxz_obj_handle_cc_h__

#include "obj_handle.h"

namespace CNORXZ
{
    
    template <typename T>
    ObjHandle<T>::ObjHandle() : mC(std::make_unique<T>()) {}

    template <typename T>
    ObjHandle<T>::ObjHandle(const T& a) : mC(std::make_unique<T>(a)) {}

    template <typename T>
    ObjHandle<T>::ObjHandle(const ObjHandle& a) : mC(std::make_unique<T>(*a.mC)) {}

    template <typename T>
    ObjHandle<T>::ObjHandle(ObjHandle&& a) : mC(a.mC) {}

    template <typename T>
    ObjHandle& ObjHandle<T>::operator=(const ObjHandle& a)
    {
	mC = std::make_unique<T>(*a.mC);
	return *this;
    }

    template <typename T>
    ObjHandle& ObjHandle<T>::operator=(ObjHandle&& a)
    {
	mC = a.mC;
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
