
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
	STabel<Ts...>::STabel(const String& name, const ContentBase* _parent, const RangePtr& fields) :
	    Table(name, _parent)
	{
	    constexpr SizeT N = sizeof...(Ts);
	    if(mFields == nullptr){
		CXZ_ASSERT(fields != nullptr, "field names have to be initialized");
		mFields = fields;
	    }
	    CXZ_ASSERT(mFields->size() == sizeof...(Ts), "expected tuple of size = " << mFields->size()
		       << ", got: " << sizeof...(Ts));

	    if(mRecords == nullptr) {
		auto mOffsets = MArray<SizeT>( mFields, iter<0,N>
		    ( [&](auto i) { return &std::get<i>(t) - &t; },
		      [](const auto&... e) { return Vector<SizeT>({e...}); }) );
		auto mSizes = MArray<SizeT>( mFields, iter<0,N>
		    ( [&](auto i) { return sizeof(std::get<i>(t)); },
		      [](const auto&... e) { return Vector<SizeT>({e...}); }) );
		auto mTypes = MArray<hid_t>( mFields, iter<0,N>
		    ( [&](auto i) { return getTypeId(std::get<i>(t)); },
		      [](const auto&... e) { return Vector<hid_t>({e...}); }) );
	    }
	    else {
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( &std::get<i>(t) - &t == mOffsets.data()[i],
			      "wrong offset for field " << i << ": " << &std::get<i>(t) - &t
			      << " vs " << mOffsets.data()[i] ); }, NoF{} );
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( sizeof(std::get<i>(t)) == mSizes.data()[i],
			      "wrong size for field " << i << ": " << sizeof(std::get<i>(t))
			      << " vs " << mSizes.data()[i] ); }, NoF{} );
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( getTypeId(std::get<i>(t)) == mTypes.data()[i],
			      "wrong type for field " << i << ": " << getTypeId(std::get<i>(t))
			      << " vs " << mTypes.data()[i] ); }, NoF{} );
	    }
	}

	template <typename... Ts>
	STabel& STabel<Ts...>::appendRecord(const Tuple<Ts...>& t)
	{
	    RangePtr appr = CRangeFactory(1).create();
	    if(mRecords == nullptr){
		mRecords = appr;
	    }
	    else {
		mRecords = mRecords->extend(appr);
	    }
	    H5TBappend_records(mParent->id(), mName, 1, sizeof(t),
			       mOffsets.data(), mSizes.data(), &t);
	    return *this;
	}
    }
}

#endif
