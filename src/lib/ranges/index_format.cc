
#include "ranges/index_format.h"

namespace CNORXZ
{
    YFormat::YFormat(const Vector<UPos>& b) :
	mB(b)
    {}

    const Vector<UPos>& YFormat::all() const
    {
	return mB;
    }

    SizeT YFormat::size() const
    {
	return mB.size();
    }

    const UPos& YFormat::get(SizeT i) const
    {
	return mB[i];
    }

    const UPos& YFormat::operator[](SizeT i) const
    {
	return mB[i];
    }

}
