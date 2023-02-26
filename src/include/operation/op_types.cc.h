
#ifndef __cxz_op_types_cc_h__
#define __cxz_op_types_cc_h__

#include "op_types.h"
//#include "xpr/xpr.h"
#include "op_utility.h"
#include "extensions/extensions.h"

namespace CNORXZ
{
    
    /**********************
     *   COpInterface     *
     **********************/

    template <class OpT>
    template <class F, class IndexT>
    constexpr decltype(auto) COpInterface<OpT>::c(F&& f, const Sptr<IndexT>& ind) const
    {
	return mkContraction(std::forward<F>(f), THIS().r(), ind);
    }

    template <class OpT>
    template <class IndexT>
    constexpr decltype(auto) COpInterface<OpT>::c(const Sptr<IndexT>& ind) const
    {
	return mkContraction([](auto& a, const auto& b) { a += b; },
			     THIS(), ind);
    }

    template <class OpT>
    template <class F, class... Args>
    constexpr decltype(auto) COpInterface<OpT>::o(F&& f, Args&&... args) const
    {
	return operation(std::forward<F>(f), THIS().r(), args...);
    }

    
    /*********************
     *   OpInterface     *
     *********************/

    template <class OpT>
    template <class IndexT, class F, class... Args>
    constexpr decltype(auto) OpInterface<OpT>::ax(const Sptr<IndexT>& ind, F&& f, const Args&... args)
    {
	return ind->ifor( operation(f, OI::THIS().r(), args...), NoF {} );
    }

    template <class OpT>
    template <class IndexT, class F, class... Args>
    inline void OpInterface<OpT>::a(const Sptr<IndexT>& ind, F&& f, const Args&... args)
    {
	ax(ind, f, args...)();
    }

    
    /***************
     *   COpRoot   *
     ***************/

    template <typename T, class IndexT>
    constexpr COpRoot<T,IndexT>::COpRoot(const CArrayBase<T>& a, const Sptr<IndexT>& ind) :
	mData(a.data()),
	mIndex(ind)
    {}

    template <typename T, class IndexT>
    constexpr COpRoot<T,IndexT>::COpRoot(const T* data, const Sptr<IndexT>& ind) :
	mData(data),
	mIndex(ind)
    {}

    template <typename T, class IndexT>
    constexpr COpRoot<T,IndexT>& COpRoot<T,IndexT>::init(const T* data, const Sptr<IndexT>& ind)
    {
	mData = data;
	mIndex = ind;
	return *this;
    }

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
    
    template <typename T, class IndexT>
    constexpr decltype(auto) coproot(const CArrayBase<T>& a, const Sptr<IndexT>& ind)
    {
	return COpRoot<T,IndexT>(a, ind);
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) coproot(const T* a, const Sptr<IndexT>& ind)
    {
	return COpRoot<T,IndexT>(a, ind);
    }

    /****************
     *   OpCont     *
     ****************/

    template <typename T, class IndexT>
    constexpr OpCont<T,IndexT>::OpCont(const Sptr<IndexT>& ind) :
	mIndex(ind),
	mC(std::make_shared<Vector<T>>(mIndex->pmax().val()))
    {}

    template <typename T, class IndexT>
    constexpr OpCont<T,IndexT>& OpCont<T,IndexT>::init(const Sptr<IndexT>& ind)
    {
	mIndex = ind;
	if(mC.size() != mIndex->pmax().val()){
	    mC.resize(mIndex->pmax().val());
	}
	return *this;
    }

    template <typename T, class IndexT>
    constexpr OpCont<T,IndexT>& OpCont<T,IndexT>::init(const Sptr<IndexT>& ind,
						       const Vector<T>& c)
    {
	init(ind);
	CXZ_ASSERT(c.size() == mC.size(),
		   "size-mismatch: expected " << mC.size() << ", got " << c.size());
	std::transform(c.begin(), c.end(), mC.begin(), [](const auto& x) { return x; } );
	return *this;
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) OpCont<T,IndexT>::r()
    {
	return OpRoot<T,IndexT>(data(), mIndex);
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) OpCont<T,IndexT>::r() const
    {
	return COpRoot<T,IndexT>(data(), mIndex);
    }

    template <typename T, class IndexT>
    template <class Op>
    constexpr OpCont<T,IndexT>& OpCont<T,IndexT>::operator=(const Op& o)
    {
	OI::a(mIndex, [](auto& a1, const auto& a2) { a1 = a2; }, o);
        return *this;
    }

    template <typename T, class IndexT>
    template <class Op>
    constexpr OpCont<T,IndexT>& OpCont<T,IndexT>::operator+=(const Op& o)
    {
	OI::a(mIndex, [](auto& a1, const auto& a2) { a1 += a2; }, o);
        return *this;
    }

    template <typename T, class IndexT>
    constexpr OpCont<T,IndexT>& OpCont<T,IndexT>::operator=(const OpCont<T,IndexT>& o)
    {
	OI::a(mIndex, [](auto& a1, const auto& a2) { a1 = a2; }, o);
	return *this;
    }

    template <typename T, class IndexT>
    template <class PosT>
    constexpr decltype(auto) OpCont<T,IndexT>::operator()(const PosT& pos) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    if constexpr(pos_type_is_consecutive<PosT>::value){
		return vreg(mC.data(),pos);
	    }
	    else {
		// non-consecutive data cannot be directly accessed
		// so there is no non-const (write) access!
		return vreg(const_cast<const T*>(mC.data()),pos);
	    }
	}
	else {
	    return mC[pos.val()];
	}
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) OpCont<T,IndexT>::operator()() const
    {
	return mC[0];
    }

    template <typename T, class IndexT>
    template <SizeT I>
    constexpr decltype(auto) OpCont<T,IndexT>::rootSteps(const IndexId<I>& id) const
    {
	return mIndex->stepSize(id);
    }

    template <typename T, class IndexT>
    T* OpCont<T,IndexT>::data()
    {
	return mC.data();
    }

    template <typename T, class IndexT>
    const T* OpCont<T,IndexT>::data() const
    {
	return mC.data();
    }
    
    /****************
     *   OpRoot     *
     ****************/
    
    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>::OpRoot(ArrayBase<T>& a, const Sptr<IndexT>& ind) :
	mData(a.data()),
	mIndex(ind)
    {}

    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>::OpRoot(T* data, const Sptr<IndexT>& ind) :
	mData(data),
	mIndex(ind)
    {}
    
    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::init(T* data, const Sptr<IndexT>& ind)
    {
	mData = data;
	mIndex = ind;
	return *this;
    }

    template <typename T, class IndexT>
    template <class Op>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::operator=(const Op& o)
    {
	OI::a(mIndex, [](auto& a, const auto& b) { a = b; }, o);
        return *this;
    }

    template <typename T, class IndexT>
    template <class Op>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::operator+=(const Op& o)
    {
	OI::a(mIndex, [](auto& a, const auto& b) { a += b; }, o);
        return *this;
    }

    template <typename T, class IndexT>
    constexpr OpRoot<T,IndexT>& OpRoot<T,IndexT>::operator=(const OpRoot<T,IndexT>& o)
    {
	OI::a(mIndex, [](auto& a, const auto& b) { a = b; }, o);
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
    
    template <typename T, class IndexT>
    T* OpRoot<T,IndexT>::data() const
    {
	return mData;
    }

    template <typename T, class IndexT>
    constexpr decltype(auto) oproot(ArrayBase<T>& a, const Sptr<IndexT>& ind)
    {
	return OpRoot<T,IndexT>(a, ind);
    }

    /*******************
     *   Operation     *
     *******************/
    
    template <class F, class... Ops>
    constexpr Operation<F,Ops...>::Operation(F&& f, const Ops&... ops) :
	mOps(ops...),
	mF(std::forward<F>(f))
    {}

    template <class F, class... Ops>
    template <class PosT>
    constexpr decltype(auto) Operation<F,Ops...>::operator()(const PosT& pos) const
    {
	return pos_unpack_args(mF, pos, mOps);
    }

    template <class F, class... Ops>
    constexpr decltype(auto) Operation<F,Ops...>::operator()() const
    {
	return exec(std::make_index_sequence<sizeof...(Ops)>{});
    }

    template <class F, class... Ops>
    template <SizeT I>
    constexpr decltype(auto) Operation<F,Ops...>::rootSteps(const IndexId<I>& id) const
    {
	return rootStepsi(id, std::make_index_sequence<sizeof...(Ops)>{});
    }

    template <class F, class... Ops>
    template <SizeT... Is>
    constexpr decltype(auto) Operation<F,Ops...>::exec(std::index_sequence<Is...> is) const
    {
	return mF( std::get<Is>(mOps)() ... );
    }

    template <class F, class... Ops>
    template <SizeT I, SizeT... Is>
    constexpr decltype(auto) Operation<F,Ops...>::rootStepsi(const IndexId<I>& id,
							     std::index_sequence<Is...> is) const
    {
	return ( std::get<Is>(mOps).rootSteps(id) << ... );
    }

    template <class F, class... Ops>
    constexpr decltype(auto) operation(F&& f, const Ops&... ops)
    {
	return Operation<F,Ops...>(std::forward<F>(f), ops...);
    }

    
    /*********************
     *   Contraction     *
     *********************/

    template <class CXpr>
    constexpr Contraction<CXpr>::Contraction(CXpr&& cxpr) :
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
    constexpr decltype(auto) Contraction<CXpr>::rootSteps(const IndexId<I>& id) const
    {
	return mCXpr.stepSize(id);
    }

    template <class F, class Op, class IndexT>
    constexpr decltype(auto) contracion(F&& f, Op&& op, const Sptr<IndexT>& i)
    {
	typedef decltype(i->ifor( op, f )) CXprT; // TODO: implement ifor with func arg!!!
	return Contraction<CXprT>( i->ifor( op, f ) );
    }

    /************************
     *   various functions  *
     ************************/
    
    template <class IndexT>
    constexpr decltype(auto) indexOp(const Sptr<IndexT>& i)
    {
	return i->xpr(i);
    }

    
}

#endif
