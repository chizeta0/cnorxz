// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_group.cc.h
   @brief Implementation of template member functions of Group.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_group_cc_h__
#define __cxz_h5_group_cc_h__

#include "h5_group.h"
#include "xpr/for.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <SizeT I, typename... Ts>
	constexpr const auto& tget(const Tuple<Ts...>& t)
	{
	    return std::get<sizeof...(Ts)-I-1>(t);
	}

	template <SizeT I, typename... Ts>
	constexpr auto& tget(Tuple<Ts...>& t)
	{
	    return std::get<sizeof...(Ts)-I-1>(t);
	}

	template <SizeT N, typename... Ts>
	SizeT getTupleOffset(const Tuple<Ts...>& t, CSizeT<N> i)
	{
	    const PtrId beg = reinterpret_cast<PtrId>(&t);
	    const PtrId pos = reinterpret_cast<PtrId>(&tget<i>(t));
	    return pos - beg;
	}

	template <typename... Ts>
	Sptr<STable<Ts...>> Group::getTable(const String& name, Tuple<Ts...> proto)
	{
	    auto i = this->getIndexTo(name);
	    CXZ_ASSERT((*i)->type() == ContentType::TABLE,
		       "element '" << name << "' is not of type TABLE");
	    auto tab = std::dynamic_pointer_cast<Table>( *i );
	    if(tab == nullptr){
		auto stab = std::dynamic_pointer_cast<STable<Ts...>>(*i);
		CXZ_ASSERT(stab != nullptr, "wrong format for table '" << name << "'");
		return stab;
	    }
	    else {
		(*i)->close();
		auto stab = std::make_shared<STable<Ts...>>(name, this);
		*i = stab;
		return stab;
	    }
	}

	template <typename T>
	Sptr<SDataset<T>> Group::getDataset(const String& name, T proto)
	{
	    auto i = this->getIndexTo(name);
	    CXZ_ASSERT((*i)->type() == ContentType::DSET,
		       "element '" << name << "' is not of type DSET");
	    auto dset = std::dynamic_pointer_cast<Dataset>( *i );
	    if(dset == nullptr){
		auto sdset = std::dynamic_pointer_cast<SDataset<T>>(*i);
		CXZ_ASSERT(sdset != nullptr, "wrong format for dataset '" << name << "'");
		return sdset;
	    }
	    else {
		(*i)->close();
		auto sdset = std::make_shared<SDataset<T>>(name, this);
		*i = sdset;
		return sdset;
	    }
	}

	template <typename T>
	Group& Group::addDataset(const String& name, const ArrayBase<T>& data)
	{
	    CXZ_ASSERT(this->isOpen(), "tried to extend closed group");
	    Vector<String> nvec({name});
	    mCont.extend( URangeFactory<String>( nvec ).create() );
	    auto ii = getIndexTo(name);
	    auto dset = std::make_shared<SDataset<T>>(name, this);
	    dset->init(data);
	    *ii = dset;
	    return *this;
	}

	template <typename... Ts>
	Group& Group::addTable(const String& name, const ArrayBase<Tuple<Ts...>>& data,
			       const Arr<String,sizeof...(Ts)>& fnames)
	{
	    CXZ_ASSERT(this->isOpen(), "tried to extend closed group");
	    Vector<String> nvec({name});
	    mCont.extend( URangeFactory<String>( nvec ).create() );
	    auto ii = getIndexTo(name);
	    auto tab = std::make_shared<STable<Ts...>>(name, this, fnames);
	    for(auto& d: data){
		tab->appendRecord(d);
	    }
	    *ii = tab;
	    return *this;
	}

	template <class F>
	decltype(auto) Group::iter(F&& f) const
	{
	    CXZ_ASSERT(isOpen(), "try to iterate over closed object");
	    RangePtr gr = *mCont.range()->sub().begin();
	    auto gi = std::make_shared<UIndex<String>>(gr);
	    return gi->ifor( operation(std::forward<F>(f), mCont(gi)), NoF{} );
	}

	template <class F>
	decltype(auto) Group::iterRecursive(F&& f) const
	{
	    return iter( [&](const auto& c) {
		f(c);
		recursion(c, std::forward<F>(f));
	    });
	}

	template <class F>
	decltype(auto) Group::iter(F&& f)
	{
	    CXZ_ASSERT(isOpen(), "try to iterate over closed object");
	    RangePtr gr = *mCont.range()->sub().begin();
	    auto gi = std::make_shared<UIndex<String>>(gr);
	    return gi->ifor( operation(std::forward<F>(f), mCont(gi)), NoF{} );
	}

	template <class F>
	decltype(auto) Group::iterRecursive(F&& f)
	{
	    return iter( [&](const auto& c) {
		f(c);
		recursion(c, std::forward<F>(f));
	    });
	}
    }
}

#endif
