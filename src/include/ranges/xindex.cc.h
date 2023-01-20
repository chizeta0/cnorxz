
#ifndef __cxz_xindex_cc_h__
#define __cxz_xindex_cc_h__

#include "xindex.h"

namespace CNORXZ
{

    /**************
     *   XIndex   *
     **************/
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>::XIndex(const IndexPtr<Index,Meta>& i) :
	mI(i) {}

    template <class Index, typename Meta>
    XIndex<Index,Meta>::XIndex(const IndexInterface<Index,Meta>& i) :
	mI(std::make_shared<Index>(i.THIS())) {}

    template <class Index, typename Meta>
    XIndexPtr XIndex<Index,Meta>::copy() const
    {
	return std::make_shared<XIndex<Index,Meta>>(mI->THIS());
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::pos() const
    {
	return mI->pos();
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator=(SizeT lexpos)
    {
	mI->THIS() = lexpos;
	assert(mI->lex() == lexpos);
	return *this;
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator++()
    {
	++(*mI);
	return *this;
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator--()
    {
	--(*mI);
	return *this;
    }

    template <class Index, typename Meta>
    Sptr<XIndexBase> XIndex<Index,Meta>::operator+(Int n) const
    {
	return std::make_shared<XIndex<Index,Meta>>(*mI + n);
    }
    
    template <class Index, typename Meta>
    Sptr<XIndexBase> XIndex<Index,Meta>::operator-(Int n) const
    {
	return std::make_shared<XIndex<Index,Meta>>(*mI - n);
    }
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator+=(Int n)
    {
	(*mI) += n;
	return *this;
    }
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator-=(Int n)
    {
	(*mI) -= n;
	return *this;
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::lex() const
    {
	return mI->lex();
    }

    template <class Index, typename Meta>
    UPos XIndex<Index,Meta>::pmax() const
    {
	return UPos(mI->pmax());
    }

    template <class Index, typename Meta>
    UPos XIndex<Index,Meta>::lmax() const
    {
	return UPos(mI->lmax());
    }

    template <class Index, typename Meta>
    IndexId<0> XIndex<Index,Meta>::id() const
    {
	return IndexId<0>(mI->id().id());
    }
    
    template <class Index, typename Meta>
    DType XIndex<Index,Meta>::operator*() const
    {
	return DType(*(*mI));
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::dim() const
    {
	return mI->dim();
    }

    template <class Index, typename Meta>
    RangePtr XIndex<Index,Meta>::range() const
    {
	return mI->range();
    }

    template <class Index, typename Meta>
    UPos XIndex<Index,Meta>::stepSize(const IndexId<0>& id) const
    {
	return mI->stepSize(id);
    }
    /*
    template <class Index, typename Meta>
    Vector<XIndexPtr> XIndex<Index,Meta>::pack() const
    {
	if constexpr(has_static_sub<Index>::value){
	    constexpr SizeT D = index_dim<Index>::value;
	    return iter<0,D>
		( [&](auto i) { return xindexPtr(std::get<i>(mI->THIS().pack())); },
		  [](const auto&... e) { return Vector<XIndexPtr>({ e ... }); } );
	}
	else if constexpr(has_sub<Index>::value){
	    return mI->THIS().pack();
	}
	else {
	    return Vector<XIndexPtr>();
	}
    }

    template <class Index, typename Meta>
    Vector<SizeT> XIndex<Index,Meta>::blockSizes() const
    {
	if constexpr(has_static_sub<Index>::value){
	    constexpr SizeT D = index_dim<Index>::value;
	    const auto& bs = mI->THIS().blockSizes();
	    return iter<0,D>
		( [&](auto i) { return std::get<i>(bs); },
		  [](const auto&... e) { return Vector<SizeT>( { static_cast<SizeT>(e)... } ); } );
	}
	else if constexpr(has_sub<Index>::value) {
	    return mI->THIS().blockSizes();
	}
	else {
	    return Vector<SizeT>();
	}
    }

    template <class Index, typename Meta>
    XIndexPtr XIndex<Index,Meta>::setBlockSizes(const Vector<SizeT>& bs)
    {
	if constexpr(has_static_sub<Index>::value){
	    constexpr SizeT D = index_dim<Index>::value;
	    CXZ_ASSERT(bs.size() == D,
		       "got block sizes of wrong dimension: " << bs.size() << " vs " << D);
	    typedef decltype(mI->THIS().blockSizes()) BT;
	    Arr<UPos,D> arr;
	    std::copy_n(bs.begin(), D, arr.begin());
	    if constexpr(std::is_same<BT,Arr<UPos,D>>::value){
		mI->THIS().setBlockSizes(arr);
		return nullptr;
	    }
	    else {
		return xindexPtr(replaceBlockSizes(arr, std::static_pointer_cast<Index>(mI)));
	    }
	}
	else if constexpr(has_sub<Index>::value) {
	    mI->THIS().setBlockSizes(bs);
	    return nullptr;
	}
	else {
	    return nullptr;
	}
    }
    */
    template <class Index, typename Meta>
    String XIndex<Index,Meta>::stringMeta() const
    {
	return mI->stringMeta();
    }

    template <class Index, typename Meta>
    DType XIndex<Index,Meta>::meta() const
    {
	return DType(mI->meta());
    }

    template <class Index, typename Meta>
    XIndexBase& XIndex<Index,Meta>::at(const DType& meta)
    {
	// check!!!
	mI->at(std::any_cast<const Meta&>(meta.get()));
	return *this;
    }

    template <class Index, typename Meta>
    DXpr<SizeT> XIndex<Index,Meta>::xpr(const XIndexPtr& _this) const
    {
	auto xthis = std::dynamic_pointer_cast<Index>(_this);
	return DXpr<SizeT>( mI->xpr(xthis) );
    }

    template <class Index, typename Meta>
    DXpr<SizeT> XIndex<Index,Meta>::ifor(const DXpr<SizeT>& xpr,
					 std::function<SizeT(SizeT,SizeT)>&& f) const
    {
	return DXpr<SizeT>(mI->ifor(xpr, std::forward<std::function<SizeT(SizeT,SizeT)>>(f)));
    }

    template <class Index, typename Meta>
    Index& XIndex<Index,Meta>::get()
    {
	return mI->THIS();
    }
    
    template <class Index, typename Meta>
    const Index& XIndex<Index,Meta>::get() const
    {
	return mI->THIS();
    }

    template <class Index>
    inline XIndexPtr xindexPtr(const Sptr<Index>& i)
    {
	typedef typename Index::MetaType Meta;
	return std::make_shared<XIndex<Index,Meta>>
	    (std::dynamic_pointer_cast<IndexInterface<Index,Meta>>(i));
    }

    template <>
    inline XIndexPtr xindexPtr<XIndexBase>(const Sptr<XIndexBase>& i)
    {
	return i;
    }
}

#endif
