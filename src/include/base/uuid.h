
#ifndef __cxz_uuid_h__
#define __cxz_uuid_h__

//#include <cstdlib>
#include <stdint.h>
#include <functional>

namespace CNORXZ
{
    struct Uuid
    {
	uint64_t i1;
	uint64_t i2;
    };

    Uuid mkUuid();

    inline bool operator==(const Uuid& a, const Uuid& b)
    {
	return a.i1 == b.i1 and a.i2 == b.i2;
    }

    inline bool operator!=(const Uuid& a, const Uuid& b)
    {
	return a.i1 != b.i1 or a.i2 != b.i2;
    }

    inline bool operator<(const Uuid& a, const Uuid& b)
    {
	return (a.i1 == b.i1) ? a.i2 < b.i2 : a.i1 < b.i1;
    }

    inline bool operator>(const Uuid& a, const Uuid& b)
    {
	return (a.i1 == b.i1) ? a.i2 > b.i2 : a.i1 > b.i1;
    }

    inline bool operator<=(const Uuid& a, const Uuid& b)
    {
	return not( (a.i1 == b.i1) ? a.i2 > b.i2 : a.i1 > b.i1 );
    }

    inline bool operator>=(const Uuid& a, const Uuid& b)
    {
	return not( (a.i1 == b.i1) ? a.i2 < b.i2 : a.i1 < b.i1 );
    }
}

#endif
