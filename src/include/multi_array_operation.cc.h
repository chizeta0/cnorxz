
#include "multi_array_operation.h"

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }
    
    /***************************
     *   OperationTemplate     *
     ***************************/

    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator+(const OperationBase<T,Second>& in) const
	-> Operation<T,plus<T>,OperationClass,Second>
    {
    	return Operation<T,plus<T>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator-(const OperationBase<T,Second>& in) const
	-> Operation<T,minus<T>,OperationClass,Second>
    {
    	return Operation<T,minus<T>,OperationClass,Second>(THIS(), in.THIS());
    }
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator*(const OperationBase<T,Second>& in) const
	-> Operation<T,multiplies<T>,OperationClass,Second>
    {
    	return Operation<T,multiplies<T>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator/(const OperationBase<T,Second>& in) const
	-> Operation<T,divides<T>,OperationClass,Second>
    {
    	return Operation<T,divides<T>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class IndexType>
    auto OperationBase<T,OperationClass>::c(const std::shared_ptr<IndexType>& ind) const
	-> Contraction<T,OperationClass,IndexType>
    {
	return Contraction<T,OperationClass,IndexType>(THIS(), ind);
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::sl(const std::shared_ptr<Indices>&... inds) const
	-> ConstSlice<T,typename Indices::RangeType...>
    {
	ConstSlice<T,typename Indices::RangeType...> out(inds->range()...);
	out.define(inds...) = THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::slc(const std::shared_ptr<Indices>&... inds) const
	-> SliceContraction<T,OperationClass,Indices...>
    {
	return SliceContraction<T,OperationClass,Indices...>
	    (THIS(), inds...);
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::p(const std::shared_ptr<Indices>&... inds) const
	-> ConstOperationRoot<T,typename Indices::RangeType...>
    {
	auto ma = std::make_shared<MultiArray<T,typename Indices::RangeType...>>
	    (inds->range()... , static_cast<T>(0));
	(*ma)(inds...) = THIS();
	return ConstOperationRoot<T,typename Indices::RangeType...>(ma, inds...);
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::to(const std::shared_ptr<Indices>&... inds) const
	-> MultiArray<T,typename Indices::RangeType...>
    {
	MultiArray<T,typename Indices::RangeType...> out(inds->range()...);
	out(inds...) = THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::addto(const std::shared_ptr<Indices>&... inds) const
	-> MultiArray<T,typename Indices::RangeType...>
    {
	MultiArray<T,typename Indices::RangeType...> out(inds->range()...,
                                                         static_cast<T>(0));
        out(inds...) += THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <typename R, class... Args>
    auto OperationBase<T,OperationClass>::a(const std::shared_ptr<function<R,T,typename Args::value_type...>>& ll,
                                            const Args&... args) const
        -> Operation<R,function<R,T,typename Args::value_type...>,OperationClass, Args...>
    {
        return Operation<R,function<R,T,typename Args::value_type...>,OperationClass, Args...>(ll, THIS(), args...);
    }

    /************************
     *   AssignmentExpr     *
     ************************/

    template <typename T, class OpClass>
    AssignmentExpr<T,OpClass>::AssignmentExpr(T* dataPtr, const OpClass& sec) :
        mSec(sec), mDataPtr(dataPtr) {}
    
    template <typename T, class OpClass>
    inline void AssignmentExpr<T,OpClass>::operator()(size_t start, ExtType last)
    {
        mDataPtr[start] = mSec.template get<ExtType>(last);
    }
    
    template <typename T, class OpClass>
    typename AssignmentExpr<T,OpClass>::ExtType AssignmentExpr<T,OpClass>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mSec.rootSteps(iPtrNum);
    }

    template <typename T, class Target, class OpClass>
    AssignmentExpr2<T,Target,OpClass>::AssignmentExpr2(T* dataPtr, const Target& tar, const OpClass& sec) :
        mTar(tar), mSec(sec), mDataPtr(dataPtr) {}
    
    template <typename T, class Target, class OpClass>
    inline void AssignmentExpr2<T,Target,OpClass>::operator()(size_t start, ExtType last)
    {
        mDataPtr[last.val()] = mSec.get(last.next());
    }
    
    template <typename T, class Target, class OpClass>
    typename AssignmentExpr2<T,Target,OpClass>::ExtType
    AssignmentExpr2<T,Target,OpClass>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mTar.rootSteps(iPtrNum).extend( mSec.rootSteps(iPtrNum) );
    }

    template <typename T, class OpClass>
    AddExpr<T,OpClass>::AddExpr(T* dataPtr, const OpClass& sec) :
        mSec(sec), mDataPtr(dataPtr) {}
    
    template <typename T, class OpClass>
    inline void AddExpr<T,OpClass>::operator()(size_t start, ExtType last)
    {
        mDataPtr[start] += mSec.template get<ExtType>(last);
    }
    
    template <typename T, class OpClass>
    typename AddExpr<T,OpClass>::ExtType AddExpr<T,OpClass>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mSec.rootSteps(iPtrNum);
    }

   
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
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
    ConstOperationRoot(std::shared_ptr<MultiArrayBase<T,Ranges...> > maptr,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(maptr->data()),
        mOrigDataPtr(maptr->data()),
	mIndex(maptr->begin()),
	mMaPtr(maptr)
    {
	mIndex(indices...);
        mDataPtr = mOrigDataPtr + mIndex.pos();
	//mOff = mIndex.pos();
    }

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const T* data, const IndexType& ind) :
	mDataPtr(data),
        mOrigDataPtr(data),
	mIndex( ind )
    {
        mDataPtr = mOrigDataPtr + mIndex.pos();
	//mOff = mIndex.pos();
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline const T& ConstOperationRoot<T,Ranges...>::get(ET pos) const
    {
	return mDataPtr[pos.val()/*+mOff*/];
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline ConstOperationRoot<T,Ranges...>& ConstOperationRoot<T,Ranges...>::set(ET pos)
    {
	mIndex = pos.val();
	mDataPtr = mOrigDataPtr + mIndex.pos();
	return *this;
    }

    template <typename T, class... Ranges>
    const T* ConstOperationRoot<T,Ranges...>::data() const
    {
        auto i = mIndex;
	return mOrigDataPtr + i().pos();
    }
    
    template <typename T, class... Ranges>
    MExt<void> ConstOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex, iPtrNum ));
	//return MExt<void>(getStepSize( getRootIndices( mIndex->info() ), iPtrNum ));
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
	//VCHECK(pos.val());
	//VCHECK(mDataPtr);
	//VCHECK(mDataPtr[pos.val()])
	return (mWorkIndex = pos.val()).meta();
    }

    template <class Range>
    template <class ET>
    inline MetaOperationRoot<Range>& MetaOperationRoot<Range>::set(ET pos)
    {
        assert(0);
	//(*mIndex) = pos.val();
	return *this;
    }

    template <class Range>
    MExt<void> MetaOperationRoot<Range>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( *mIndex, iPtrNum ));
	//return MExt<void>(getStepSize( getRootIndices( mIndex->info() ), iPtrNum ));
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
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(ma.data()),
        mOrigDataPtr(ma.data()),
	mIndex( ma.begin() )
    {
	mIndex(indices...);
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(T* data, const IndexType& ind) :
	mDataPtr(data),
        mOrigDataPtr(data),
	mIndex( ind )
    {
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto OperationRoot<T,Ranges...>::assign(const OpClass& in) const
        -> decltype(mIndex.ifor(1,in.loop(AssignmentExpr<T,OpClass>(mOrigDataPtr,in))))
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return mIndex.ifor(1,in.loop(AssignmentExpr<T,OpClass>(mOrigDataPtr,in)));
    }

    template <typename T, class... Ranges>
    template <class OpClass, class Index>
    auto OperationRoot<T,Ranges...>::assign(const OpClass& in, const std::shared_ptr<Index>& i) const
        -> decltype(i->ifor(1,in.loop(AssignmentExpr2<T,OperationRoot<T,Ranges...>,OpClass>
                                      (mOrigDataPtr,*this,in))))
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return i->ifor(1,in.loop(AssignmentExpr2<T,OperationRoot<T,Ranges...>,OpClass>
                                 (mOrigDataPtr,*this,in)));
        //return mIndex.ifor(1,in.loop(AssignmentExpr<T,OpClass>(mOrigDataPtr,in)));
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto OperationRoot<T,Ranges...>::plus(const OpClass& in) const
        -> decltype(mIndex.ifor(1,in.loop(AddExpr<T,OpClass>(mOrigDataPtr,in))))
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return mIndex.ifor(1,in.loop(AddExpr<T,OpClass>(mOrigDataPtr,in)));
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
        assign(in)();
        return *this;
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator+=(const OpClass& in)
    {
        plus(in)();
        return *this;
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator=(const OperationRoot<T,Ranges...>& in)
    {
	return operator=<OperationRoot<T,Ranges...> >(in);
    }

    template <typename T, class... Ranges>
    ParallelOperationRoot<T,Ranges...> OperationRoot<T,Ranges...>::par()
    {
        return ParallelOperationRoot<T,Ranges...>(mOrigDataPtr, mIndex);
    }
    
    template <typename T, class... Ranges>
    template <class ET>
    inline T& OperationRoot<T,Ranges...>::get(ET pos) const
    {
	return mDataPtr[pos.val()];
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::set(ET pos)
    {
	mIndex = pos.val();
	mDataPtr = mOrigDataPtr + mIndex.pos();
	return *this;
    }

    template <typename T, class... Ranges>
    MExt<void> OperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex, iPtrNum ));
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
        auto i = mIndex;
	return mOrigDataPtr + i().pos();
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

    /*******************************
     *   ParallelOperationRoot     *
     *******************************/
    
    template <typename T, class... Ranges>
    ParallelOperationRoot<T,Ranges...>::
    ParallelOperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(ma.data()),
        mOrigDataPtr(ma.data()),
	mIndex( ma.begin() )
    {
	mIndex(indices...);
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    ParallelOperationRoot<T,Ranges...>::
    ParallelOperationRoot(T* data, const IndexType& ind) :
	mDataPtr(data),
        mOrigDataPtr(data),
	mIndex( ind )
    {
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::assign(const OpClass& in)
        -> decltype(mIndex.pifor(1,in.loop(AssignmentExpr<T,OpClass>(mOrigDataPtr,in))))
    {
        return mIndex.pifor(1,in.loop(AssignmentExpr<T,OpClass>(mOrigDataPtr,in)));
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::plus(const OpClass& in)
        -> decltype(mIndex.pifor(1,in.loop(AddExpr<T,OpClass>(mOrigDataPtr,in))))
    {
        return mIndex.pifor(1,in.loop(AddExpr<T,OpClass>(mOrigDataPtr,in)));
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    ParallelOperationRoot<T,Ranges...>& ParallelOperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
        assign(in)();
        return *this;
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    ParallelOperationRoot<T,Ranges...>& ParallelOperationRoot<T,Ranges...>::operator+=(const OpClass& in)
    {
        plus(in)();
        return *this;
    }

    template <typename T, class... Ranges>
    ParallelOperationRoot<T,Ranges...>&
    ParallelOperationRoot<T,Ranges...>::operator=(const ParallelOperationRoot<T,Ranges...>& in)
    {
	return operator=<ParallelOperationRoot<T,Ranges...> >(in);
    }
    
    template <typename T, class... Ranges>
    template <class ET>
    inline T& ParallelOperationRoot<T,Ranges...>::get(ET pos) const
    {
	return mDataPtr[pos.val()/*+mOff*/];
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline ParallelOperationRoot<T,Ranges...>& ParallelOperationRoot<T,Ranges...>::set(ET pos)
    {
	mIndex = pos.val();
	mDataPtr = mOrigDataPtr + mIndex.pos();
	return *this;
    }

    template <typename T, class... Ranges>
    MExt<void> ParallelOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex, iPtrNum ));
	//return MExt<void>(getStepSize( mIndex.info(), iPtrNum ));
    }

    template <typename T, class... Ranges>
    template <class Expr>
    Expr ParallelOperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }

    template <typename T, class... Ranges>
    T* ParallelOperationRoot<T,Ranges...>::data() const
    {
        auto i = mIndex;
	return mOrigDataPtr + i().pos();
    }


    /************************
     *   OperationValue     *
     ************************/

    template <typename T>
    OperationValue<T>::OperationValue(const T& val) : mVal(val) {}

    template <typename T>
    template <class ET>
    inline const T& OperationValue<T>::get(ET pos) const
    {
	return mVal;
    }

    template <typename T>
    template <class ET>
    inline OperationValue<T>& OperationValue<T>::set(ET pos)
    {
	return *this;
    }

    template <typename T>
    None OperationValue<T>::rootSteps(std::intptr_t iPtrNum) const
    {
	return None();
    }

    template <typename T>
    template <class Expr>
    Expr OperationValue<T>::loop(Expr exp) const
    {
	return exp;
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
	static_assert( not FISSTATIC, "using instance of function supposed to be static" );
    }

    template <typename T, class OpFunction, class... Ops>
    template <class ET>
    inline T Operation<T,OpFunction,Ops...>::get(ET pos) const
    {
	typedef std::tuple<Ops...> OpTuple;
	return PackNum<sizeof...(Ops)-1>::
	    template mkOpExpr<SIZE,T,ET,OpTuple,OpFunction>(mF, pos, mOps);
    }

    template <typename T, class OpFunction, class... Ops>
    template <class ET>
    inline Operation<T,OpFunction,Ops...>& Operation<T,OpFunction,Ops...>::set(ET pos)
    {
	PackNum<sizeof...(Ops)-1>::setOpPos(mOps,pos);
	return *this;
    }

    template <typename T, class OpFunction, class... Ops>
    auto Operation<T,OpFunction,Ops...>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(PackNum<sizeof...(Ops)-1>::mkSteps(iPtrNum, mOps))
    {
	return PackNum<sizeof...(Ops)-1>::mkSteps(iPtrNum, mOps);
    }

    template <typename T, class OpFunction, class... Ops>
    template <class Expr>
    auto Operation<T,OpFunction,Ops...>::loop(Expr exp) const
	-> decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, exp ))
    {
	return PackNum<sizeof...(Ops)-1>::mkLoop( mOps, exp );
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

    /**************************
     *   SliceContraction     *
     **************************/

    template <typename T, class Op, class... Indices>
    SliceContraction<T,Op,Indices...>::SliceContraction(const Op& op,
							std::shared_ptr<Indices>... ind) :
	mOp(op),
	mCont(std::make_shared<MultiArray<T,typename Indices::RangeType...> >(ind->range()...)),
	mTarOp(*mCont,ind...)
    { }

    // forward loop !!!!
    template <typename T, class Op, class... Indices>
    template <class ET>
    inline const MultiArray<T,typename Indices::RangeType...>&
    SliceContraction<T,Op,Indices...>::get(ET pos) const
    {
	*mCont = 0;
	mOp.set(pos);
	mTarOp = mOp;
	return *mCont;
    }

    template <typename T, class Op, class... Indices>
    template <class ET>
    inline SliceContraction<T,Op,Indices...>& SliceContraction<T,Op,Indices...>::set(ET pos)
    {
	mOp.set(pos);
	return *this;
    }

    template <typename T, class Op, class... Indices>
    auto SliceContraction<T,Op,Indices...>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(iPtrNum))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op, class... Indices>
    template <class Expr>
    auto SliceContraction<T,Op,Indices...>::loop(Expr exp) const -> decltype(mOp.loop(exp))
    {
	return mOp.loop(exp);
    }

}
