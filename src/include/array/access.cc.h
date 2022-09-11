
#include "access.h"

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }

    /************************
     *   AccessTemplate     *
     ************************/

    template <class AccessClass>
    auto AccessTemplate<AccessClass>::get(size_t pos)
    {
        return THIS().get(pos);
    }

    template <class AccessClass>
    auto AccessTemplate<AccessClass>::get(size_t pos) const
    {
        return THIS().get(pos);
    }

    template <class AccessClass>
    auto AccessTemplate<AccessClass>::oget(size_t pos) const
    {
	return THIS().oget(pos);
    }

    template <class AccessClass>
    template <class F, typename Op, class ExtType>
    void AccessTemplate<AccessClass>::exec(size_t pos, const Op& op, ExtType e) const
    {
        return THIS().template exec<F>(pos,op,e);
    }


} // namespace CNORXZ
