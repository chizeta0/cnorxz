
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
