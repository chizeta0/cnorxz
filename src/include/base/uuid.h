// -*- C++ -*-
/**
   
   @file include/base/uuid.h
   @brief uuid implementation

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_uuid_h__
#define __cxz_uuid_h__

#include <stdint.h>
#include <functional>

namespace CNORXZ
{
    /** ***
	uuid
     */
    struct Uuid
    {
	uint64_t i1; /**< first 8 bytes */
	uint64_t i2; /**< second 8 bytes */
    };

    /** create new uuid */
    Uuid mkUuid();

    /** operator equal to
	@param a left hand side
	@param b right hand side
     */
    inline bool operator==(const Uuid& a, const Uuid& b)
    {
	return a.i1 == b.i1 and a.i2 == b.i2;
    }

    /** operator not equal to
	@param a left hand side
	@param b right hand side
     */
    inline bool operator!=(const Uuid& a, const Uuid& b)
    {
	return a.i1 != b.i1 or a.i2 != b.i2;
    }

    /** operator less than
	@param a left hand side
	@param b right hand side
     */
    inline bool operator<(const Uuid& a, const Uuid& b)
    {
	return (a.i1 == b.i1) ? a.i2 < b.i2 : a.i1 < b.i1;
    }

    /** operator greater than
	@param a left hand side
	@param b right hand side
     */
    inline bool operator>(const Uuid& a, const Uuid& b)
    {
	return (a.i1 == b.i1) ? a.i2 > b.i2 : a.i1 > b.i1;
    }

    /** operator less or equal
	@param a left hand side
	@param b right hand side
     */
    inline bool operator<=(const Uuid& a, const Uuid& b)
    {
	return not( (a.i1 == b.i1) ? a.i2 > b.i2 : a.i1 > b.i1 );
    }

    /** operator greater or equal
	@param a left hand side
	@param b right hand side
     */
    inline bool operator>=(const Uuid& a, const Uuid& b)
    {
	return not( (a.i1 == b.i1) ? a.i2 < b.i2 : a.i1 < b.i1 );
    }
}

#endif
