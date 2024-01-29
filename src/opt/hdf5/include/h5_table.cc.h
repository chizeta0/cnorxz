
#ifndef __cxz_h5_table_cc_h__
#define __cxz_h5_table_cc_h__

#include "h5_table.h"
#include "h5_type_id.h"

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
	STable<Ts...>::STable(const String& name, const ContentBase* _parent) :
	    Table(name, _parent)
	{
	    constexpr SizeT N = sizeof...(Ts);
	    if(mFields != nullptr){

		CXZ_ASSERT(mFields->size() == N, "expected tuple of size = " << mFields->size()
			   << ", got: " << N);
		Tuple<Ts...> x;
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( getTupleOffset(x, i) == mOffsets.data()[i],
			      "wrong offset for field " << i << ": " << getTupleOffset(x, i)
			      << " vs " << mOffsets.data()[i] ); }, NoF{} );
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( sizeof(tget<i>(x)) == mSizes.data()[i],
			      "wrong size for field " << i << ": " << sizeof(tget<i>(x))
			      << " vs " << mSizes.data()[i] ); }, NoF{} );
		iter<0,N>( [&](auto i) { CXZ_ASSERT
			    ( H5Tget_class(getTypeId(tget<i>(x))) == mTypes.data()[i],
			      "wrong type for field " << i
			      << ": " << H5Tget_class(getTypeId(tget<i>(x)))
			      << " vs " << mTypes.data()[i] ); }, NoF{} );
	    }
	}

	template <typename... Ts>
	STable<Ts...>::STable(const String& name, const ContentBase* _parent,
			      const Arr<String,sizeof...(Ts)>& fnames) :
	    Table(name, _parent)
	{
	    initFields(fnames);
	}

	template <typename... Ts>
	STable<Ts...>& STable<Ts...>::initFields(const Arr<String,sizeof...(Ts)>& fnames)
	{
	    constexpr SizeT N = sizeof...(Ts);
	    CXZ_ASSERT(mFields == nullptr and mRecords == nullptr,
		       "tried to initialize an existing table");

	    Vector<FieldID> fields(fnames.size());
	    for(SizeT i = 0; i != fields.size(); ++i){
		fields[i].first = i;
		fields[i].second = fnames[i];
	    }
	    mFields = URangeFactory<FieldID>(fields).create();

	    Tuple<Ts...> x;
	    mOffsets = MArray<SizeT>
		( mFields, iter<0,N>
		  ( [&](auto i) { return getTupleOffset(x, i); },
		    [](const auto&... e) { return Vector<SizeT>({e...}); }) );
	    mSizes = MArray<SizeT>
		( mFields, iter<0,N>
		  ( [&](auto i) { return sizeof(tget<i>(x)); },
		    [](const auto&... e) { return Vector<SizeT>({e...}); }) );
	    mTypes = MArray<hid_t>
		( mFields, iter<0,N>
		  ( [&](auto i) { return getTypeId(tget<i>(x)); },
		    [](const auto&... e) { return Vector<hid_t>({e...}); }) );
	    return *this;
	}

	template <typename... Ts>
	STable<Ts...>& STable<Ts...>::appendRecord(const Tuple<Ts...>& t)
	{
	    RangePtr appr = CRangeFactory(1).create();
	    if(mRecords == nullptr){
		initTable(1, &t, sizeof(t), sizeof(t));
	    }
	    else {
		Table::appendRecords(1, &t);
	    }
	    return *this;
	}

	template <typename... Ts>
	MArray<Tuple<Ts...>> STable<Ts...>::read() const
	{
	    CXZ_ASSERT(isOpen(), "attempt to read table that has not been opened");
	    MArray<Tuple<Ts...>> out(mRecords);
	    H5TBread_table(mParent->id(), mName.c_str(), mTypesize, mOffsets.data(),
			   mSizes.data(), out.data());
	    return out;
	}
    }
}

#endif
