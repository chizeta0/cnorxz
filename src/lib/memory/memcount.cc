
#include "memory/memcount.h"

namespace CNORXZ
{
    SizeT MemCount::sMemUsage = 0;

    void MemCount::add(SizeT x)
    {
	sMemUsage += x;
    }
    
    void MemCount::sub(SizeT x)
    {
	sMemUsage -= x;
    }

    SizeT MemCount::usage()
    {
	return sMemUsage;
    }

}
