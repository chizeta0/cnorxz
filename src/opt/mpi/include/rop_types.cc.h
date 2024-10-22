
#ifndef __cxz_mpi_rop_types_cc_h__
#define __cxz_mpi_rop_types_cc_h__

#include "rop_types.h"

namespace CNORXZ
{
    namespace mpi
    {

	/*================+
	 |    CROpRoot    |
	 +================*/
    
	template <typename T, class RIndexT, class IndexT>
	constexpr CROpRoot<T,RIndexT,IndexT>::CROpRoot(const RCArray<T>& a, const Sptr<RIndexT>& ri,
						       const Sptr<IndexT>& li) :
	    mData(a.buffermap().data()),
	    mRIndex(ri),
	    mIndex(li),
	    mLOff(mIndex->pos())
	{
	    CXZ_ASSERT(a.buffermap().size() == ri->lmax().val(),
		       "data map not properly initialized: map size = " << a.buffermap().size()
		       << ", rank index range size = " << ri->lmax().val());
	}

	template <typename T, class RIndexT, class IndexT>
	template <class PosT>
	constexpr decltype(auto) CROpRoot<T,RIndexT,IndexT>::operator()(const PosT& pos) const
	{
	    //CXZ_ASSERT(pos.val() < mRIndex->pmax().val(), pos.val() << ">=" << mRIndex->pmax().val());
	    //CXZ_ASSERT(mData[pos.val()] != nullptr, "data[" << pos.val() << "] == null");
	    //CXZ_ASSERT(pos.next().val() < mIndex->pmax().val(), pos.val() << ">=" << mIndex->pmax().val());
	    if constexpr(is_epos_type<PosT>::value){
		// PRELIMINARY!!!
		// discards potential vector entries in pos (only pos.next() is considered)!!!
		return vreg(mData[pos.val()[0]]+mLOff,pos.next());
	    }
	    else {
		return (mData[pos.val()])[mLOff+pos.next().val()];
	    }
	}

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) CROpRoot<T,RIndexT,IndexT>::operator()() const
	{
	    //CXZ_ASSERT(mData[0] != nullptr, "data[" << 0 << "] == null");
	    return (mData[0])[mLOff];
	}

	template <typename T, class RIndexT, class IndexT>
	template <SizeT I>
	constexpr decltype(auto) CROpRoot<T,RIndexT,IndexT>::rootSteps(const IndexId<I>& id) const
	{
	    return mRIndex->stepSize(id) << mIndex->stepSize(id);
	}

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) croproot(const RCArray<T>& a, const Sptr<RIndexT>& ri,
					  const Sptr<IndexT>& li)
	{
	    return CROpRoot<T,RIndexT,IndexT>(a,ri,li);
	}
    
	/*===============+
	 |    ROpRoot    |
	 +===============*/

	template <typename T, class RIndexT, class IndexT>
	constexpr ROpRoot<T,RIndexT,IndexT>::ROpRoot(RArray<T>& a, const Sptr<RIndexT>& ri,
						     const Sptr<IndexT>& li) :
	    mLocal(&a.local()),
	    mData(a.buffermap().data()),
	    mRIndex(ri),
	    mIndex(li),
	    mLOff(mIndex->pos())
	{
	    CXZ_ASSERT(a.buffermap().size() == ri->lmax().val(),
		       "data map not properly initialized: map size = " << a.buffermap().size()
		       << ", rank index range size = " << ri->lmax().val());
	}
	
	template <typename T, class RIndexT, class IndexT>
	template <class Op>
	constexpr ROpRoot<T,RIndexT,IndexT>& ROpRoot<T,RIndexT,IndexT>::operator=(const Op& in)
	{
	    (*mLocal)(mindexPtr(mRIndex->local()*mIndex)).a
		(mindexPtr(mRIndex*mIndex),[](auto& a, const auto& b) { a = b; }, in);
	    return *this;
	}
	
	template <typename T, class RIndexT, class IndexT>
	template <class Op>
	constexpr ROpRoot<T,RIndexT,IndexT>& ROpRoot<T,RIndexT,IndexT>::operator+=(const Op& in)
	{
	    (*mLocal)(mindexPtr(mRIndex->local()*mIndex)).a
		(mindexPtr(mRIndex*mIndex),[](auto& a, const auto& b) { a += b; }, in);
	    return *this;
	}
	
	template <typename T, class RIndexT, class IndexT>
	constexpr ROpRoot<T,RIndexT,IndexT>& ROpRoot<T,RIndexT,IndexT>::operator=(const ROpRoot& in)
	{
	    (*mLocal)(mindexPtr(mRIndex->local()*mIndex)).a
		(mindexPtr(mRIndex*mIndex),[](auto& a, const auto& b) { a = b; }, in);
	    return *this;
	}

	template <typename T, class RIndexT, class IndexT>
	template <class PosT>
	constexpr decltype(auto) ROpRoot<T,RIndexT,IndexT>::operator()(const PosT& pos) const
	{
	    //CXZ_ASSERT(pos.val() < mRIndex->pmax().val(), pos.val() << ">=" << mRIndex->pmax().val());
	    //CXZ_ASSERT(mData[pos.val()] != nullptr, "data[" << pos.val() << "] == null");
	    //CXZ_ASSERT(pos.next().val() < mIndex->pmax().val(), pos.val() << ">=" << mIndex->pmax().val());
	    if constexpr(is_epos_type<PosT>::value){
		// PRELIMINARY!!!
		// discards potential vector entries in pos (only pos.next() is considered)!!!
		if constexpr(pos_type_is_consecutive<PosT>::value){
		    return vreg(mData[pos.val()[0]]+mLOff,pos.next());
		}
		else {
		    // non-consecutive data cannot be directly accessed
		    // so there is no non-const (write) access!
		    return vreg(const_cast<const T*>(mData[pos.val()[0]]+mLOff),pos.next());
		}
	    }
	    else {
		return (mData[pos.val()])[mLOff+pos.next().val()];
	    }
	}

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) ROpRoot<T,RIndexT,IndexT>::operator()() const
	{
	    //CXZ_ASSERT(mData[0] != nullptr, "data[" << 0 << "] == null");
	    return (mData[0])[0];
	}

	template <typename T, class RIndexT, class IndexT>
	template <SizeT I>
	constexpr decltype(auto) ROpRoot<T,RIndexT,IndexT>::rootSteps(const IndexId<I>& id) const
	{
	    return mRIndex->stepSize(id) << mIndex->stepSize(id);
	}
    
	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) roproot(RArray<T>& a, const Sptr<RIndexT>& ri,
					 const Sptr<IndexT>& li)
	{
	    return ROpRoot<T,RIndexT,IndexT>(a,ri,li);
	}

	/*====================+
	 |    RContraction    |
	 +====================*/
	/*
	template <class CXpr>
	constexpr RContraction<CXpr>::RContraction(CXpr&& cxpr) :
	    mCXpr(std::forward<CXpr>(cxpr))
	{}

	template <class CXpr>
	template <class PosT>
	constexpr decltype(auto) RContraction<CXpr>::operator()(const PosT& pos) const
	{

	}

	template <class CXpr>
	constexpr decltype(auto) RContraction<CXpr>::operator()() const
	{

	}

	template <class CXpr>
	template <SizeT I>
	constexpr decltype(auto) RContraction<CXpr>::rootSteps(const IndexId<I>& id) const
	{

	}
	*/
    } // namespace mpi    
} // namespace CNORXZ

#endif
