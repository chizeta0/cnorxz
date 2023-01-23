
#ifndef __cxz_h5_table_cc_h__
#define __cxz_h5_table_cc_h__

#include "h5_table.h"
#include "h5_types.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename... Ts>
	template <class F>
	decltype(auto) STable<Ts...>::iterRecords(F&& f) const
	{
	    CXZ_ERROR("not implemented");
	    return f(0);
	}

	template <typename... Ts>
	STable<Ts...>::STable(const String& name, const ContentBase* _parent, const RangePtr& fields) :
	    Table(name, _parent)
	{
	    constexpr SizeT N = sizeof...(Ts);
	    if(mFields == nullptr){
		CXZ_ASSERT(fields != nullptr, "field names have to be initialized");
		mFields = fields;
	    }
	    CXZ_ASSERT(mFields->size() == sizeof...(Ts), "expected tuple of size = " << mFields->size()
		       << ", got: " << sizeof...(Ts));

	    Tuple<Ts...> x;
	    if(mRecords == nullptr) {
		auto mOffsets = MArray<SizeT>( mFields, iter<0,N>
		    ( [&](auto i) { return &std::get<i>(x) - &x; },
		      [](const auto&... e) { return Vector<SizeT>({e...}); }) );
		auto mSizes = MArray<SizeT>( mFields, iter<0,N>
		    ( [&](auto i) { return sizeof(std::get<i>(x)); },
		      [](const auto&... e) { return Vector<SizeT>({e...}); }) );
		auto mTypes = MArray<hid_t>( mFields, iter<0,N>
		    ( [&](auto i) { return getTypeId(std::get<i>(x)); },
		      [](const auto&... e) { return Vector<hid_t>({e...}); }) );
	    }
	    else {
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( &std::get<i>(x) - &x == mOffsets.data()[i],
			      "wrong offset for field " << i << ": " << &std::get<i>(x) - &x
			      << " vs " << mOffsets.data()[i] ); }, NoF{} );
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( sizeof(std::get<i>(x)) == mSizes.data()[i],
			      "wrong size for field " << i << ": " << sizeof(std::get<i>(x))
			      << " vs " << mSizes.data()[i] ); }, NoF{} );
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( getTypeId(std::get<i>(x)) == mTypes.data()[i],
			      "wrong type for field " << i << ": " << getTypeId(std::get<i>(x))
			      << " vs " << mTypes.data()[i] ); }, NoF{} );
	    }
	}

	template <typename... Ts>
	STable<Ts...>& STable<Ts...>::appendRecord(const Tuple<Ts...>& t)
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
