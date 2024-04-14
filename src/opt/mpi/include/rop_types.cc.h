
#include "rop_types.h"

namespace CNORXZ
{
    namespace mpi
    {

	/*================+
	 |    CROpRoot    |
	 +================*/
    
	template <typename T, class RIndexT, class IndexT>
	constexpr CROpRoot<T,RIndexT,IndexT>::CROpRoot(const RArray<T>& a, const Sptr<RIndexT>& ri,
						       const Sptr<IndexT>& li) :
	    mData(a.map()),
	    mRIndex(ri),
	    mIndex(li)
	{
	    CXZ_ASSERT(a.map().size() == ri->lmax().val(),
		       "data map not properly initialized: map size = " << a.map.size()
		       << ", rank index range size = " << ri->lmax().val());
	}

	template <typename T, class RIndexT, class IndexT>
	template <class PosT>
	constexpr decltype(auto) CROpRoot<T,RIndexT,IndexT>::operator()(const PosT& pos) const
	{
	    return (mData[pos.val()])[pos.next().val()];
	}

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) CROpRoot<T,RIndexT,IndexT>::operator()() const
	{
	    return (mData[0])[0];
	}

	template <typename T, class RIndexT, class IndexT>
	template <SizeT I>
	constexpr decltype(auto) CROpRoot<T,RIndexT,IndexT>::rootSteps(const IndexId<I>& id) const
	{
	    return mRIndex->stepSize(id) << mIndex->stepSize(id);
	}

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) croproot(const RArray<T>& a, const Sptr<RIndexT>& ri,
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
	    mData(a.map()),
	    mRIndex(ri),
	    mIndex(li)
	{
	    CXZ_ASSERT(a.map().size() == ri->lmax().val(),
		       "data map not properly initialized: map size = " << a.map.size()
		       << ", rank index range size = " << ri->lmax().val());
	}

	template <typename T, class RIndexT, class IndexT>
	template <class PosT>
	constexpr decltype(auto) ROpRoot<T,RIndexT,IndexT>::operator()(const PosT& pos) const
	{
	    return (mData[pos.val()])[pos.next().val()];
	}

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) ROpRoot<T,RIndexT,IndexT>::operator()() const
	{
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
