
#include "cxz_operation.h"
#include "xpr/pos_type.h"
#include "ranges/range_helper.h"
#include "ranges/xpr/op_xpr.h"


namespace CNORXZ
{
    
    /**********************
     *   COpInterface     *
     **********************/

    template <class OpT>
    template <class IndexT>
    auto COpInterface<OpT>::c(const std::shared_ptr<IndexT>& ind) const
    {
	return Contraction<T,OpT,IndexT>(THIS(), ind);
    }

    template <class OpT>
    template <class F, class... Args>
    constexpr auto a(F&& f, Args&&... args) const;
    auto COpInterface<OpT>::o(F&& f, Args&&... args) const
    {
        return Operation<R,F,OpT,Args...>(f, THIS(), args...);
    }

    /*********************
     *   OpInterface     *
     *********************/

    template <class OpT>
    template <class IndexT, class F, class... Args>
    constexpr decltype(auto) OpInterface<OpT>::ax(const Sptr<IndexT>& ind, F&& f, Args&&... args)
    {
	return ind->ifor( SPos<1>(), OpXpr<F,OpT,Args...>(f, THIS(), args...) );
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
    constexpr decltype(auto) COpRoot<T,IndexT>::get(const PosT& pos) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return vreg(mData,pos); // distinguish between consecutive/non-consecutive
	}
	else {
	    return mData[pos.val()];
	}
    }

    template <typename T, class IndexT>
    template <SizeT I>
    constexpr decltype(auto) COpRoot<T,IndexT>::rootSteps(const IndexId<I>& id) const
    {
	return mIndex->stepSize(id);
    }

    template <typename T, class IndexT>
    template <class Expr>
    constexpr decltype(auto) COpRoot<T,IndexT>::loop(Xpr&& xpr) const
    {
	return xpr;
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
    constexpr decltype(auto) OpRoot<T,IndexT>::get(const PosT& pos) const
    {
	if constexpr(is_epos_type<PosT>::value){
	    return vreg(mData,pos); // distinguish between consecutive/non-consecutive
	}
	else {
	    return mData[pos.val()];
	}
    }

    template <typename T, class IndexT>
    template <SizeT I>
    constexpr decltype(auto) OpRoot<T,IndexT>::rootSteps(const IndexId<I>& id) const
    {
	return mIndex->stepSize(id);
    }

    template <typename T, class IndexT>
    template <class Expr>
    constexpr decltype(auto) OpRoot<T,IndexT>::loop(Xpr&& xpr) const;
    {
	return xpr;
    }

    template <typename T, class IndexT>
    T* OpRoot<T,IndexT>::data() const
    {
        return mData;
    }


    /*******************
     *   Operation     *
     *******************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	mOps(ops...)
    {
	static_assert( FISSTATIC, "need function instance for non-static function" );
    }

    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(std::shared_ptr<OpFunction> ff,
					      const Ops&... ops) :
	mOps(ops...),
	mF(ff)
    {
	static_assert( not FISSTATIC, "using instance of static function" );
    }

    template <size_t I, class OpFunction, class ETuple, class OpTuple, typename... Args>
    inline auto
    mkOpExpr(std::shared_ptr<OpFunction> f, const ETuple& pos, const OpTuple& ops, Args... args)
    {
	if constexpr(I == std::tuple_size<OpTuple>{}){
	    if constexpr(OpFunction::FISSTATIC){
		return OpFunction::apply(args...);
	    }
	    else {
		(*f)(args...);
	    }
	}
	else {
	    typedef typename std::remove_reference<decltype(std::get<I>(ops))>::type NextOpType;
	    return mkOpExpr<I+1>
		( f, getX<NextOpType::SIZE>(pos), ops, args..., std::get<I>(ops).get(pos));
	}
    }

    template <size_t I, typename V, class OpFunction, class ETuple, class OpTuple, typename... Args>
    inline auto
    mkVOpExpr(std::shared_ptr<OpFunction> f, const ETuple& pos, const OpTuple& ops, Args... args)
    {
	if constexpr(I == std::tuple_size<OpTuple>{}){
	    if constexpr(OpFunction::FISSTATIC){
		return VFunc<OpFunction>::apply(args...);
	    }
	    else {
		auto vf = mkVFuncPtr(f);
		(*vf)(args...);
	    }
	}
	else {
	    typedef typename std::remove_reference<decltype(std::get<I>(ops))>::type NextOpType;
	    return mkVOpExpr<I+1,V>( f, getX<NextOpType::SIZE>(pos), ops, args...,
				     std::get<I>(ops).template vget<V>(pos));
	}
    }

    template <typename T, class OpFunction, class... Ops>
    template <class ET>
    inline auto Operation<T,OpFunction,Ops...>::get(ET pos) const
    {
	return mkOpExpr<0>(mF, pos, mOps.mOps);
    }

    template <typename T, class OpFunction, class... Ops>
    template <typename V, class ET>
    inline auto Operation<T,OpFunction,Ops...>::vget(ET pos) const
    {
	return mkVOpExpr<0,V>(mF, pos, mOps.mOps);
    }

    template <size_t I, class OpTuple, class ETuple>
    static inline void setOpPos(OpTuple& ot, const ETuple& et)
    {
	if constexpr(I != std::tuple_size<OpTuple>{}){
	    typedef typename std::remove_reference<decltype(std::get<I>(ot))>::type NextOpType;
	    std::get<I>( ot ).set( et );
	    setOpPos<I+1>(ot, getX<NextOpType::SIZE>(et));
	}
    }
    
    template <typename T, class OpFunction, class... Ops>
    template <class ET>
    inline Operation<T,OpFunction,Ops...>& Operation<T,OpFunction,Ops...>::set(ET pos)
    {
	setOpPos<0>(mOps.mOps,pos);
	return *this;
    }

    template <typename T, class OpFunction, class... Ops>
    auto Operation<T,OpFunction,Ops...>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mOps.rootSteps(iPtrNum);
    }

    template <typename T, class OpFunction, class... Ops>
    template <class Expr>
    auto Operation<T,OpFunction,Ops...>::loop(Expr exp) const
    {
	return sfor_m<sizeof...(Ops),0>
	    ( [&](auto i){ return std::get<i>(mOps.mOps); },
	      [&](auto f, auto next) { return f.loop(next); },
	      exp );
    }

    
    /*********************
     *   Contraction     *
     *********************/

    template <typename T, class Op, class IndexType>
    Contraction<T,Op,IndexType>::Contraction(const Op& op, std::shared_ptr<IndexType> ind) :
	mOp(op),
	mInd(ind) {}

    // forward loop !!!!
    template <typename T, class Op, class IndexType>
    template <class ET>
    inline auto Contraction<T,Op,IndexType>::get(ET pos) const
        -> decltype(mOp.template get<ET>(pos))
    {
	return mOp.template get<ET>(pos);
    }

    template <typename T, class Op, class IndexType>
    template <typename V, class ET>
    inline auto Contraction<T,Op,IndexType>::vget(ET pos) const
        -> decltype(mOp.template vget<V,ET>(pos))
    {
	return mOp.template vget<V,ET>(pos);
    }

    template <typename T, class Op, class IndexType>
    template <class ET>
    inline Contraction<T,Op,IndexType>& Contraction<T,Op,IndexType>::set(ET pos)
    {
	mOp.set(pos);
	return *this;
    }

    template <typename T, class Op, class IndexType>
    auto Contraction<T,Op,IndexType>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(iPtrNum))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op, class IndexType>
    template <class Expr>
    auto Contraction<T,Op,IndexType>::loop(Expr exp) const
	-> decltype(mInd->iforh(1,mOp.loop(exp)))
    {
	return mInd->iforh(0,mOp.loop(exp));
    }

}
