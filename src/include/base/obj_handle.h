// -*- C++ -*-
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

    /** ****
	unique pointer wrapper
	
	Allows to handle objects accessed through abstract base class pointers
	as if they were complete (non-virtual) types.
	Each type to be handled is required to have a copy() member function
	that returns a copy of itself

	@tparam T object type
     */
    template <typename T>
    class ObjHandle
    {
    protected:
	Uptr<T> mC; /**< pointer to the object data */

    public:

	/** default constructor */
	ObjHandle();

	/** construct from unique pointer
	    @param a unique pointer
	 */
	ObjHandle(Uptr<T>&& a);

	/** copy construct
	    @param a input
	 */
	ObjHandle(const ObjHandle& a);

	/** move construct
	    @param a input
	 */
	ObjHandle(ObjHandle&& a);

	/** copy assign
	    @param a input
	*/
	ObjHandle& operator=(const ObjHandle& a);

	/** move assign
	    @param a input
	*/
	ObjHandle& operator=(ObjHandle&& a);

	/** access data */
	T& operator*();

	/** get pointer to data */
	T* operator->();

	/** access data (const) */
	const T& operator*() const;

	/** get pointer to data (const) */
	const T* operator->() const;

	/** get object. */
	T* get();

	/** get object. */
	const T* get() const;
    };
    
}

#endif
