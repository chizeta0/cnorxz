
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
		mOffsets = MArray<SizeT>( mFields, iter<0,N>
		    ( [&](auto i) { return getTupleOffset(x, i); },
		      [](const auto&... e) { return Vector<SizeT>({e...}); }) );
		mSizes = MArray<SizeT>( mFields, iter<0,N>
		    ( [&](auto i) { return sizeof(std::get<i>(x)); },
		      [](const auto&... e) { return Vector<SizeT>({e...}); }) );
		mTypes = MArray<hid_t>( mFields, iter<0,N>
		    ( [&](auto i) { return getTypeId(std::get<i>(x)); },
		      [](const auto&... e) { return Vector<hid_t>({e...}); }) );
	    }
	    else {
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( getTupleOffset(x, i) == mOffsets.data()[i],
			      "wrong offset for field " << i << ": " << getTupleOffset(x, i)
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
	    constexpr hsize_t chunk_size = sizeof(t);
	    constexpr Int compress = 0;
	    RangePtr appr = CRangeFactory(1).create();
	    if(mRecords == nullptr){
		mRecords = appr;
		Vector<const char*> fields(mFields->size());
		auto fr = std::dynamic_pointer_cast<URange<String>>(mFields);
		for(auto fi = fr->begin(); fi != fr->end(); ++fi){
		    fields[fi.lex()] = (*fi).c_str();
		}
		H5TBmake_table(mName.c_str(), mParent->id(), mName.c_str(), mFields->size(),
			       mRecords->size(), sizeof(t), fields.data(), mOffsets.data(),
			       mTypes.data(), chunk_size, NULL, compress, &t);
	    }
	    else {
		mRecords = mRecords->extend(appr);
		H5TBappend_records(mParent->id(), mName.c_str(), 1, sizeof(t),
				   mOffsets.data(), mSizes.data(), &t);
	    }
	    return *this;
	}
    }
}

#endif
