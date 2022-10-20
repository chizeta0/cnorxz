
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


    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const ArrayBase<T,Ranges...>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(ma.data()),
        mOrigDataPtr(ma.data()),
	mIndex( ma.begin() )
    {
	mIndex(indices...);
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(std::shared_ptr<ArrayBase<T,Ranges...> > maptr,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(maptr->data()),
        mOrigDataPtr(maptr->data()),
	mIndex(maptr->begin()),
	mMaPtr(maptr)
    {
	mIndex(indices...);
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const T* data, const IndexType& ind) :
	mDataPtr(data),
        mOrigDataPtr(data),
	mIndex( ind )
    {
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline const T& ConstOperationRoot<T,Ranges...>::get(ET pos) const
    {
	return mDataPtr[pos.val()];
    }

    template <typename T, class... Ranges>
    template <typename V, class ET>
    inline const V& ConstOperationRoot<T,Ranges...>::vget(ET pos) const
    {
	return *(reinterpret_cast<const V*>(mDataPtr+pos.val()));
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline ConstOperationRoot<T,Ranges...>& ConstOperationRoot<T,Ranges...>::set(ET pos)
    {
        mDataPtr = mOrigDataPtr + pos.val();
	return *this;
    }

    template <typename T, class... Ranges>
    const T* ConstOperationRoot<T,Ranges...>::data() const
    {
        return mDataPtr;
    }
    
    template <typename T, class... Ranges>
    MExt<None> ConstOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<None>(RangeHelper::getStepSize( mIndex, iPtrNum ));
    }


    template <typename T, class... Ranges>
    template <class Expr>
    Expr ConstOperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }

    /********************
     *   StaticCast     *
     ********************/

    template <typename T, class Op>
    StaticCast<T,Op>::StaticCast(const Op& op) : mOp(op) {}

    template <typename T, class Op>
    template <class ET>
    inline T StaticCast<T,Op>::get(ET pos) const
    {
	return static_cast<T>( mOp.get(pos) );
    }

    template <typename T, class Op>
    template <typename V, class ET>
    inline V StaticCast<T,Op>::vget(ET pos) const
    {
	assert(0); // !!!
	return V();
    }

    template <typename T, class Op>
    template <class ET>
    inline StaticCast<T,Op>& StaticCast<T,Op>::set(ET pos)
    {
	mOp.set(pos);
	return *this;
    }
    
    template <typename T, class Op>
    auto StaticCast<T,Op>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(iPtrNum))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op>
    template <class Expr>
    Expr StaticCast<T,Op>::loop(Expr exp) const
    {
	return mOp.loop(exp);
    }
    
    
    /****************************
     *   MetaOperationRoot     *
     ****************************/

    template <class Range>
    MetaOperationRoot<Range>::
    MetaOperationRoot(const std::shared_ptr<IndexType>& ind) :
	mWorkIndex(*ind), mIndex( ind ) {}

    
    template <class Range>
    template <class ET>
    inline typename MetaOperationRoot<Range>::value_type
    MetaOperationRoot<Range>::get(ET pos) const
    {
	return (mWorkIndex = pos.val()).meta();
    }

    template <class Range>
    template <typename V, class ET>
    inline V
    MetaOperationRoot<Range>::vget(ET pos) const
    {
	assert(0); // !!!
	return V();
    }

    template <class Range>
    template <class ET>
    inline MetaOperationRoot<Range>& MetaOperationRoot<Range>::set(ET pos)
    {
	return *this;
    }

    template <class Range>
    MExt<None> MetaOperationRoot<Range>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<None>(RangeHelper::getStepSize( *mIndex, iPtrNum ));
    }


    template <class Range>
    template <class Expr>
    Expr MetaOperationRoot<Range>::loop(Expr exp) const
    {
	return exp;
    }

    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(ma.data()),
        mOrigDataPtr(ma.data()),
        mDataAcc( ma.data(), ma.data() ),
	mIndex( ma.begin() )
    {
	mIndex(indices...);
        mDataAcc.set( mIndex.pos() );
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableArrayBase<T,Ranges...>& ma,
		  const std::tuple<std::shared_ptr<typename Ranges::IndexType>...>& indices) :
	mDataPtr(ma.data()),
        mOrigDataPtr(ma.data()),
        mDataAcc( ma.data(), ma.data() ),
	mIndex( ma.begin() )
    {
	mIndex(indices);
        mDataAcc.set( mIndex.pos() );
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(T* data, const IndexType& ind) :
	mDataPtr(data),
        mOrigDataPtr(data),
        mDataAcc( data, data ),
	mIndex( ind )
    {
        mDataAcc.set( mIndex.pos() );
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    template <class Func, class Access, class OpClass>
    auto OperationRoot<T,Ranges...>::asx(const OpClass& in) const
    {
	static_assert(Access::VSIZE > 0, "Access::VSIZE failed");
	return mIndex.ifor(1,this->template asxExpr<Func,Access>(in)).template vec<Access::VSIZE>();
    }

    template <typename T, class... Ranges>
    template <class Func, class Access, class OpClass>
    auto OperationRoot<T,Ranges...>::asxExpr(const OpClass& in) const
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return in.loop(AssignmentExpr<T,Func,Access,OperationRoot<T,Ranges...>,OpClass>
                       (Access(mDataAcc),*this,in));
    }
    
    template <typename T, class... Ranges>
    template <class Func, class Access, class OpClass, class Index>
    auto OperationRoot<T,Ranges...>::asx(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
	static_assert(Access::VSIZE > 0, "Access::VSIZE failed");
        return i->ifor(1,this->template asxExpr<Func,Access>(in)).template vec<Access::VSIZE>();
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto OperationRoot<T,Ranges...>::assign(const OpClass& in) const
    {
        return this->template asx<identity<T>,PointerAccess<T>>(in);
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto OperationRoot<T,Ranges...>::assignExpr(const OpClass& in) const
    {
        return this->template asxExpr<identity<T>,PointerAccess<T>>(in);
    }
    
    template <typename T, class... Ranges>
    template <class OpClass, class Index>
    auto OperationRoot<T,Ranges...>::assign(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
        return this->template asx<identity<T>,PointerAccess<T>>(in,i);
    }

    template <typename T>
    using xplus = plus<T>;

    template <typename T, class... Ranges>
    template <class OpClass>
    auto OperationRoot<T,Ranges...>::plus(const OpClass& in) const
    {
        return this->template asx<xplus<T>,PointerAccess<T>>(in);
    }

    template <typename T, class... Ranges>
    template <class OpClass, class Index>
    auto OperationRoot<T,Ranges...>::plus(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
        return this->template asx<xplus<T>,PointerAccess<T>>(in,i);
    }
    
    template <bool VABLE, class Func, typename TarOp, class OpClass>
    inline void vexec(TarOp& th, const OpClass& in)
    {
	// VECTORIZATION!!!
	// PARALLEL!!!
        if constexpr(VABLE){
            CHECK;
            typedef typename TarOp::value_type T;
            auto x = th.template asx<VFunc<Func>,VPointerAccess<T>>(in);
            if(x.rootSteps(x.vI()) == 1){
		//if(0){
                CHECK;
                x();
            }
            else {
                th.template asx<Func,PointerAccess<T>>(in)();
            }
        }
        else {
            typedef typename TarOp::value_type T;
            //IAccess<T,F<T>> tmp;
            th.template asx<Func,PointerAccess<T>>(in)();
        }
    }
    
    template <typename T, class... Ranges>
    template <class OpClass>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
	vexec<OpClass::VABLE,identity<T>>(*this,in);
        return *this;
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator+=(const OpClass& in)
    {
	vexec<OpClass::VABLE,xplus<T>>(*this,in);
        return *this;
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator=(const OperationRoot<T,Ranges...>& in)
    {
	return operator=<OperationRoot<T,Ranges...> >(in);
    }
    /*
    template <typename T, class... Ranges>
    ParallelOperationRoot<T,Ranges...> OperationRoot<T,Ranges...>::par()
    {
        return ParallelOperationRoot<T,Ranges...>(mOrigDataPtr, mIndex);
    }
    */
    template <typename T, class... Ranges>
    template <class ET>
    inline T& OperationRoot<T,Ranges...>::get(ET pos) const
    {
        return *mDataAcc.get(pos.val());
    }

    template <typename T, class... Ranges>
    template <typename V, class ET>
    inline V& OperationRoot<T,Ranges...>::vget(ET pos) const
    {
        return *(reinterpret_cast<V*>(mDataAcc.get(pos.val())));
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::set(ET pos)
    {
        mDataAcc.set(pos.val());
        mDataPtr = mOrigDataPtr + pos.val();
	return *this;
    }

    template <typename T, class... Ranges>
    MExt<None> OperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<None>(RangeHelper::getStepSize( mIndex, iPtrNum ));
    }

    template <typename T, class... Ranges>
    template <class Expr>
    Expr OperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }

    template <typename T, class... Ranges>
    T* OperationRoot<T,Ranges...>::data() const
    {
        return mDataAcc.get(0);
    }

    template <typename T, class... Ranges>
    template <class... Indices>
    auto OperationRoot<T,Ranges...>::sl(const std::shared_ptr<Indices>&... inds)
	-> Slice<T,typename Indices::RangeType...>
    {
	Slice<T,typename Indices::RangeType...> out(inds->range()...);
	out.define(inds...) = *this;
	return out;
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
