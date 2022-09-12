
#ifndef __cxz_memcount_h__
#define __cxz_memcount_h__

#include "base/types.h"

namespace CNORXZ
{
    class MemCount
    {
    private:
	static SizeT sMemUsage;
	static void add(SizeT x) { sMemUsage += x; }
	static void sub(SizeT x) { sMemUsage -= x; }
	
    public:
	MemCount() = delete; // static only
	static SizeT usage() { return sMemUsage; }

	template <typename T>
	friend class Allocator;
    };

} // namespace CNORXZ

#endif
