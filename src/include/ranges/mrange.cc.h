
#ifndef __cxz_mrange_cc_h__
#define __cxz_mrange_cc_h__

#include "mrange.h"
#include "range_helper.h"
#include "statics/static_for.h"

namespace CNORXZ
{
    /**************
     *   MIndex   *
     **************/
    
    template <class... Indices>
    template <class MRange>
    MIndex<Indices...>::MIndex(const Sptr<MRange>& range) :
	IndexInterface<MIndex<Indices...>,Tuple<typename Indices::MetaType...> >(0)
    {
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	sfor_mn<sizeof...(Indices),0>
	    ( [&](auto i) {
		  auto r = range->template getPtr<i>();
		  std::get<i>(mIPack) = r->beginPtr();
		  *std::get<i>(mIPack) = 0;
		  
		  std::get<i>(mBlockSizes) = sfor_p<i,sizeof...(Indices)>
		      ( [&](auto j) { return std::get<j>(mIPack)->max(); } ,
			[&](auto a, auto b) { return a * b; });
		  return 0;
	      });
	
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator()(Sptr<Indices>&... indices)
    {
	return (*this)(std::make_tuple(indices...));
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator()(const Tuple<Sptr<Indices>...>& indices)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack) = std::get<i>(indices); return 0; } );
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, IB::mPos);
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator=(SizeT pos)
    {
	IB::mPos = pos;
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator++()
    {
	sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.last() and i != 0) { si = 0; return true; }
		else { ++si; return false; }
		return false;
	    } );
	++IB::mPos;
	return *this;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::operator--()
    {
	sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.first() and i != 0) { si = si.max()-1; return true; }
		else { --si; return false; }
		return false;
	    } );
	--IB::mPos;
	return *this;
    }


    template <class... Indices>
    int MIndex<Indices...>::pp(PtrId idxPtrNum)
    {
	const int tmp = RangeHelper::ppx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <class... Indices>
    int MIndex<Indices...>::mm(PtrId idxPtrNum)
    {
	const int tmp = RangeHelper::mmx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class... Indices>
    String MIndex<Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class... Indices>
    typename MIndex<Indices...>::MetaType MIndex<Indices...>::meta() const
    {
	MetaType metaTuple;
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(metaTuple) = std::get<i>(mIPack)->meta(); return 0; } );
	return metaTuple;
    }

    template <class... Indices>
    MIndex<Indices...>& MIndex<Indices...>::at(const MetaType& metaPos)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); return 0; } );
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	return *this;
    }

    template <class... Indices>
    SizeT MIndex<Indices...>::dim()
    {
	return sizeof...(Indices);
    }

    template <class... Indices>
    Sptr<typename MIndex<Indices...>::RangeType>
    MIndex<Indices...>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( mRangePtr );
    }

    template <class... Indices>	
    SizeT MIndex<Indices...>::getStepSize(SizeT n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    /*
    template <class... Indices>
    template <class Exprs>
    auto MIndex<Indices...>::ifor(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkFor<0>(step, mIPack, mBlockSizes, exs);
    }

    template <class... Indices>
    template <class Exprs>
    auto MIndex<Indices...>::iforh(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkForh<0>(step, mIPack, mBlockSizes, exs);
    }

    template <class... Indices>
    template <class Exprs>
    auto MIndex<Indices...>::pifor(SizeT step, Exprs exs) const
    {
	return RangeHelper::mkPFor<0>(step, mIPack, mBlockSizes, exs);
    }
    */
    
    /*********************
     *   MRangeFactory   *
     *********************/

    template <class... Ranges>
    MRangeFactory<Ranges...>::MRangeFactory(const Sptr<Ranges>&... rs)
    {
	mProd = Sptr< MRange<Ranges...> >( new MRange<Ranges...>( rs... ) );
    }
    
    template <class... Ranges>
    MRangeFactory<Ranges...>::MRangeFactory(const typename MRange<Ranges...>::Space& st)
    {
	mProd = Sptr< MRange<Ranges...> >( new MRange<Ranges...>( st ) );
    }

    template <class... Ranges>
    Sptr<RangeBase> MRangeFactory<Ranges...>::create()
    {
	mProd = checkIfCreated( std::dynamic_pointer_cast<oType>( mProd )->mSpace );
	setSelf();
	return mProd;
    }
    
    /******************
     *   MRange   *
     ******************/

    template <class... Ranges>
    MRange<Ranges...>::MRange(const Sptr<Ranges>&... rs) : mSpace(std::make_tuple(rs...)) {}

    template <class... Ranges>
    MRange<Ranges...>::MRange(const Space& space) : mSpace( space ) {}

    template <class... Ranges>
    SizeT MRange<Ranges...>::getMeta(const MetaType& metaPos) const
    {
	return RangeHelper::getMeta<sizeof...(Ranges)-1>(mSpace,metaPos);
    }

    template <class... Ranges>
    SizeT MRange<Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class... Ranges>
    SizeT MRange<Ranges...>::size() const
    {
	return sfor_p<0,sizeof...(Ranges)>
	    ( [&](auto i) { return std::get<i>(mSpace)->size(); },
	      [&](auto a, auto b) { return a * b; } );
    }

    template <class... Ranges>
    String MRange<Ranges...>::stringMeta(SizeT pos) const
    {
	auto i = begin();
	i = pos;
	return "[" + RangeHelper::getStringMeta<0>(i) + "]";
    }

}

#endif
