
#include "rop_types.h"

namespace CNORXZ
{
    namespace mpi
    {

	/*================+
	 |    CROpRoot    |
	 +================*/
    
	template <typename T, class IndexT>
	template <class PosT>
	constexpr decltype(auto) CROpRoot<T,IndexT>::operator()(const PosT& pos) const
	{
	    return (mData+mPos[pos.val()])[pos.next().val()];
	}

	template <typename T, class IndexT>
	constexpr decltype(auto) CROpRoot<T,IndexT>::operator()() const
	{
	    return (mData+mPos[0])[0];
	}

	template <typename T, class IndexT>
	template <SizeT I>
	constexpr decltype(auto) CROpRoot<T,IndexT>::rootSteps(const IndexId<I>& id) const
	{
	    
	}
    
	/*===============+
	 |    ROpRoot    |
	 +===============*/

	template <typename T, class IndexT>
	template <class PosT>
	constexpr decltype(auto) ROpRoot<T,IndexT>::operator()(const PosT& pos) const
	{

	}

	template <typename T, class IndexT>
	constexpr decltype(auto) ROpRoot<T,IndexT>::operator()() const
	{

	}

	template <typename T, class IndexT>
	template <SizeT I>
	constexpr decltype(auto) ROpRoot<T,IndexT>::rootSteps(const IndexId<I>& id) const
	{

	}
    
	/*====================+
	 |    RContraction    |
	 +====================*/

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

    } // namespace mpi    
} // namespace CNORXZ
