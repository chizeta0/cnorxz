
#ifndef __cxz_h5_table_cc_h__
#define __cxz_h5_table_cc_h__

#include "h5_table.h"
#include "h5_types.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <class F>
	decltype(auto) iterRecords(F&& f) const
	{

	}

	template <typename... Ts>
	STabel<Ts...>::STabel(const String& name, const ContentBase* _parent) :
	    Table(name, _parent)
	{
	    // checks..!!!!
	}

	template <typename... Ts>
	STabel& STabel<Ts...>::appendRecord(const Tuple<Ts...>& t)
	{
	    if(not mCheckedFile){
		this->open();
	    }
	    this->close(); // H5TB does not require an open dataset
	    CXZ_ASSERT(mFields != nullptr,
		       "field names have to be initialized before creating table");
	    CXZ_ASSERT(mFields->size() == sizeof...(Ts),
		       "expected tuple of size = " << mFields->size()
		       << ", got: " << sizeof...(Ts));
	    
	    RangePtr appr = CRangeFactory(1).create();
	    auto offsets = iter<0,sizeof...(Ts)>
		( [&](auto i) { return &std::get<i>(t) - &t; },
		  [](const auto&... e) { return Vector<SizeT>({e...}); });
	    auto sizes = iter<0,sizeof...(Ts)>
		( [&](auto i) { return sizeof(std::get<i>(t)); },
		  [](const auto&... e) { return Vector<SizeT>({e...}); });
	    auto types = iter<0,sizeof...(Ts)>
		( [&](auto i) { return getTypeId(std::get<i>(t)); },
		  [](const auto&... e) { return Vector<hid_t>({e...}); });
	    if(mRecords == nullptr){
		mRecords = appr;
		mSizes = MArray<SizeT>(std::move(sizes));
		mOffsets = MArray<SizeT>(std::move(offsets));
		mTypes = MArray<hid_t>(std::move(types));
		
		    // init
	    }
	    else {
		// check consistency

		mRecords = mRecords->extend(appr);
	    }
	    // append
	    H5TBappend_records(mParent->id(), mName, 1, sizeof(t),
			       mOffsets.data(), mSizes.data(), &t);
	    return *this;
	}
    }
}

#endif
