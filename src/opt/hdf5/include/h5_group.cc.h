
#ifndef __cxz_h5_group_cc_h__
#define __cxz_h5_group_cc_h__

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename T>
	Group& Group::addData(const String& name, const ArrayBase<T>& data)
	{
	    CXZ_ASSERT(this->isOpen(), "tried to extend closed group");
	    CXZ_ERROR("not implemented!!!");
	    return *this;
	}

	template <typename... Ts>
	Group& Group::addTable(const String& name, const ArrayBase<Tuple<Ts...>>& data,
			       const RangePtr& fields)
	{
	    CXZ_ASSERT(this->isOpen(), "tried to extend closed group");
	    
	    return *this;
	}

    }
}

#endif
