
#ifndef __cxz_op_types_cc_h__
#define __cxz_op_types_cc_h__

#include "op_types.h"
#include "xpr/pos_type.h"
#include "xpr/op_xpr.h"
#include "op_utility.h"

namespace CNORXZ
{
    
    /**********************
     *   COpInterface     *
     **********************/

    template <class OpT>
    template <class F, class IndexT>
    decltype(auto) COpInterface<OpT>::c(F&& f, const Sptr<IndexT>& ind) const
    {
	return mkContraction(std::forward<F>(f), THIS(), ind);
    }

    template <class OpT>
    template <class IndexT>
    decltype(auto) COpInterface<OpT>::c(const Sptr<IndexT>& ind) const
    {
	return mkContraction([](auto& a, const auto& b) { a += b; },
			     THIS(), ind);
    }

    template <class OpT>
    template <class F, class... Args>
    constexpr auto a(F&& f, Args&&... args) const;
    decltype(auto) COpInterface<OpT>::o(F&& f, Args&&... args) const
    {
        return Operation<F,OpT,Args...>(std::forward<F>(f), THIS(), args...);
    }

    
    /*********************
     *   OpInterface     *
     *********************/

    template <class OpT>
    template <class IndexT, class F, class... Args>
    constexpr decltype(auto) OpInterface<OpT>::ax(const Sptr<IndexT>& ind, F&& f, Args&&... args)
    {
	return ind->ifor( Operation<F,OpT,Args...>(f, THIS(), args...) );
    }

    template <class OpT>
    template <class IndexT, class F, class... Args>
    inline SizeT OpInterface<OpT>::a(const Sptr<IndexT>& ind, F&& f, Arg&&... args)
    {
	return ax(ind, f, args...)();
    }

    
    /***************
     *   COpRoot   *
     ***************/

    template <typename T, class IndexT>
    constexpr COpRoot<T,IndexT>::COpRoot(const DArrayBase<T>& a, const Sptr<IndexT>& ind) :
	mData(a.data()),
	mIndex(ind)
    {}

    template <typename T, class IndexT>
    constexpr COpRoot<T,IndexT>::COpRoot(const T* data, const Sptr<IndexT>& ind) :
	mData(data),
	mIndex(ind)
    {}

    template <typename T, class IndexT>
    template <class PosT>
    constexpr decltype(auto) COpRoot<T,IndexT>::operator()(const PosT& pos) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return vreg(mData,pos); // distinguish between consecutive/non-consecutive
	}
	else {
	    return mData[pos.val()];
	}
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) COpRoot<T,IndexT>::operator()() const
    {
	return mData[0];
    }
    
    template <typename T, class IndexT>
    template <SizeT I>
    constexpr decltype(auto) COpRoot<T,IndexT>::rootSteps(const IndexId<I>& id) const
    {
	return mIndex->stepSize(id);
    }

    template <typename T, class IndexT>
    const T* COpRoot<T,IndexT>::data() const
    {
        return mData;
    }
    

    /****************
     *   OpRoot     *
     ****************/
    
    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>::OpRoot(MDArrayBase<T>& a, const Sptr<IndexT>& ind);
	mData(a.data()),
	mIndex(ind)
    {}

    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>::OpRoot(T* data, const Sptr<IndexT>& ind) :
	mData(data),
	mIndex(ind)
    {}
    
    template <typename T, class IndexT>
    template <class Op>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::operator=(const Op& o)
    {
	a(mInd, [](auto& a, const auto& b) { a = b; }, o)
        return *this;
    }

    template <typename T, class IndexT>
    template <class Op>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::operator+=(const Op& o)
    {
	a(mInd, [](auto& a, const auto& b) { a += b; }, o)
        return *this;
    }

    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::operator=(const OpRoot<T,IndexT>& in)
    {
	a(mInd, [](auto& a, const auto& b) { a = b; }, o)
	return *this;
    }

    template <typename T, class IndexT>
    template <class PosT>
    constexpr decltype(auto) OpRoot<T,IndexT>::operator()(const PosT& pos) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    if constexpr(pos_type_is_consecutive<PosT>::value){
		return vreg(mData,pos);
	    }
	    else {
		// non-consecutive data cannot be directly accessed
		// so there is no non-const (write) access!
		return vreg(const_cast<const T*>(mData),pos);
	    }
	}
	else {
	    return mData[pos.val()];
	}
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) OpRoot<T,IndexT>::operator()() const
    {
	return mData[0];
    }
    
    template <typename T, class IndexT>
    template <SizeT I>
    constexpr decltype(auto) OpRoot<T,IndexT>::rootSteps(const IndexId<I>& id) const
    {
	return mIndex->stepSize(id);
    }


    /*******************
     *   Operation     *
     *******************/
    
    template <class F, class... Ops>
    Operation<F,Ops...>::Operation(F&& f, Ops&&... ops) :
	mOps(ops...),
	mF(ff)
    {}

    template <class F, class... Ops>
    template <class PosT>
    inline decltype(auto) Operation<F,Ops...>::operator()(const PosT& pos) const
    {
	return pos_unpack_args(mF, pos, mOps);
    }

    template <class F, class... Ops>
    template <class PosT>
    inline decltype(auto) Operation<F,Ops...>::operator()() const
    {
	return exec(std::make_index_sequence<sizeof...(Ops)>{});
    }

    template <class F, class... Ops>
    constexpr decltype(auto) Operation<F,Ops...>::rootSteps(const IndexId<I>& id) const
    {
	return mOps.rootSteps(id);
    }

    template <class F, class... Ops>
    constexpr decltype(auto) Operation<F,Ops...>::exec(std::index_sequence<Is...> is) const
    {
	return mF( std::get<Is>(mOps)() ... );
    }

    
    /*********************
     *   Contraction     *
     *********************/

    template <class CXpr>
    Contraction<CXpr>::Contraction(CXpr&& cxpr) :
	mCXpr(cxpr)
    {}

    template <class CXpr>
    template <class PosT>
    constexpr decltype(auto) Contraction<CXpr>::operator()(const PosT& pos) const
    {
	return mCXpr(pos);
    }

    template <class CXpr>
    constexpr decltype(auto) Contraction<CXpr>::operator()() const
    {
	return mCXpr();
    }

    template <class CXpr>
    template <SizeT I>
    constexpr decltype(auto) Contraction<CXpr>::rootSteps(const IndexId<I>& id) const;
    {
	return mCXpr.stepSize(id);
    }

    template <class F, class Op, class IndexT>
    constexpr decltype(auto) mkContracion(F&& f, Op&& op, const Sptr<IndexT>& i)
    {
	typedef decltype(i->ifor( op, f )) CXprT; // TODO: implement ifor with func arg!!!
	return Contraction<CXprT>( i->ifor( op, f ) );
    }
}

#endif
