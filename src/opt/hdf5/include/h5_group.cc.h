
#ifndef __cxz_h5_group_cc_h__
#define __cxz_h5_group_cc_h__

#include "h5_group.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename... Ts>
	Sptr<STable<Ts...>> Group::getTable(const String& name, Tuple<Ts...> proto)
	{
	    auto i = this->getIndexTo(name);
	    auto tab = std::dynamic_pointer_cast<Table>( *i );
	    if(tab == nullptr){
		auto stab = std::dynamic_pointer_cast<STable<Ts...>>(*i);
		CXZ_ASSERT(stab != nullptr, "wrong format for table '" << name << "'");
		return stab;
	    }
	    else {
		const RangePtr fields = tab->fields();
		(*i)->close();
		*i = std::make_shared<STable<Ts...>>(name, this, fields);
		return *i;
	    }
	}

	
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
	    Vector<String> nvec({name});
	    Vector<DType> dvec({DType(name)});
	    auto extr = URangeFactory<String>( nvec ).create();
	    mCont.extend(extr);
	    auto ii = mCont.begin();
	    ii.at(dvec); // 'at' returns YIndex&, so cannot use it inline...
	    auto tab = std::make_shared<STable<Ts...>>(name, this, fields);
	    for(auto& d: data){
		tab->appendRecord(d);
	    }
	    *ii = tab;
	    return *this;
	}

    }
}

#endif
