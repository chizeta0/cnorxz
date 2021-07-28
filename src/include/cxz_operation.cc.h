
#include "cxz_operation.h"
#include "xfor/exttype.h"
#include "ranges/range_helper.h"


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
    template <typename U, class Second>
    auto OperationBase<T,OperationClass>::operator+(const OperationBase<U,Second>& in) const
    {
    	return Operation<plusv<T,U>,plusx<T,U>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <typename U, class Second>
    auto OperationBase<T,OperationClass>::operator-(const OperationBase<U,Second>& in) const
    {
    	return Operation<minusv<T,U>,minusx<T,U>,OperationClass,Second>(THIS(), in.THIS());
    }
    
    template <typename T, class OperationClass>
    template <typename U, class Second>
    auto OperationBase<T,OperationClass>::operator*(const OperationBase<U,Second>& in) const
    {
    	return Operation<multipliesv<T,U>,multipliesx<T,U>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <typename U, class Second>
    auto OperationBase<T,OperationClass>::operator/(const OperationBase<U,Second>& in) const
    {
    	return Operation<dividesv<T,U>,dividesx<T,U>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class IndexType>
    auto OperationBase<T,OperationClass>::c(const std::shared_ptr<IndexType>& ind) const
    {
	return Contraction<T,OperationClass,IndexType>(THIS(), ind);
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::sl(const std::shared_ptr<Indices>&... inds) const
    {
	ConstSlice<T,typename Indices::RangeType...> out(inds->range()...);
	out.define(inds...) = THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::p(const std::shared_ptr<Indices>&... inds) const
    {
	auto ma = std::make_shared<MultiArray<T,typename Indices::RangeType...>>
	    (inds->range()... , static_cast<T>(0));
	(*ma)(inds...) = THIS();
	return ConstOperationRoot<T,typename Indices::RangeType...>(ma, inds...);
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::to(const std::shared_ptr<Indices>&... inds) const
    {
	MultiArray<T,typename Indices::RangeType...> out(inds->range()...);
	out(inds...) = THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::addto(const std::shared_ptr<Indices>&... inds) const
    {
	MultiArray<T,typename Indices::RangeType...> out(inds->range()...,
                                                         static_cast<T>(0));
        out(inds...) += THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::pto(const std::shared_ptr<Indices>&... inds) const
    {
	MultiArray<T,typename Indices::RangeType...> out(inds->range()...);
	out(inds...).par() = THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::paddto(const std::shared_ptr<Indices>&... inds) const
    {
	MultiArray<T,typename Indices::RangeType...> out(inds->range()...,
                                                         static_cast<T>(0));
        out(inds...).par() += THIS();
	return out;
    }

    template <typename T, class OperationClass>
    template <typename R, class... Args>
    auto OperationBase<T,OperationClass>::a(const std::shared_ptr<function<R,T,typename Args::value_type...>>& ll,
                                            const Args&... args) const
    {
        return Operation<R,function<R,T,typename Args::value_type...>,OperationClass, Args...>(ll, THIS(), args...);
    }

    template <typename T, class OperationClass>
    auto OperationBase<T,OperationClass>::ptr() const
    {
	return OperationPointer<T,OperationClass>(THIS());
    }
    
    /*
    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::ho(const std::shared_ptr<Indices>& inds...) const
    {
        typedef XX SubOp;
	return HyperOperation<T,SubOp,Indices...>()
    }
    */

    /************************
     *   AccessTemplate     *
     ************************/

    template <class AccessClass>
    auto AccessTemplate<AccessClass>::get(size_t pos)
    {
        return THIS().get(pos);
    }

    template <class AccessClass>
    auto AccessTemplate<AccessClass>::get(size_t pos) const
    {
        return THIS().get(pos);
    }

    template <class AccessClass>
    auto AccessTemplate<AccessClass>::oget(size_t pos) const
    {
	return THIS().oget(pos);
    }

    template <class AccessClass>
    template <class F, typename Op, class ExtType>
    void AccessTemplate<AccessClass>::exec(size_t pos, const Op& op, ExtType e) const
    {
        return THIS().template exec<F>(pos,op,e);
    }

    /************************
     *   AssignmentExpr     *
     ************************/

    template <OpIndexAff OIA, class ExtType>
    inline size_t opIndexResolve(size_t start, ExtType last)
    {
        if constexpr(OIA == OpIndexAff::EXTERN){
            return last.val();
        }
        if constexpr(OIA == OpIndexAff::TARGET){
            return start;
        }
        return 0;
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::AssignmentExpr(const AccessTemplate<AT>& dataAcc, const Target& tar, const OpClass& sec) :
        mTar(tar), mSec(sec), mDataAcc(static_cast<const AT&>(dataAcc)) {}

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline void AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::operator()(size_t start)
    {
        ExtType last = rootSteps();
	last.zero();
	// TODO: ask MA container for data (ptr)!!!
	mDataAcc.template exec<Func>(opIndexResolve<OIA>(start,last),mSec,last.next());
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline void AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::operator()(size_t start, ExtType last)
    {
	mDataAcc.template exec<Func>(opIndexResolve<OIA>(start,last),mSec,last.next());
    }
    
    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    typename AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::ExtType
    AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mTar.rootSteps(iPtrNum).extend( mSec.rootSteps(iPtrNum) );
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline void AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::operator()(size_t mlast, DExt last)
    {
        (*this)(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline DExt AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::dRootSteps(std::intptr_t iPtrNum) const
    {
        return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
    }

    template <typename T, class Func, class AT, class Target, class OpClass, OpIndexAff OIA>
    inline DExt AssignmentExpr<T,Func,AT,Target,OpClass,OIA>::dExtension() const
    {
        return nullptr; //???!!!
    }

    template <typename T, class... Ops>
    MOp<T,Ops...>::MOp(const Ops&... exprs) : mOps(exprs...)
    {
	static_assert(SIZE == sizeof...(Ops), "size missmatch");
    }

    template <size_t N, class ExtType, class... Exprs>
    inline size_t MOpGetX( ExtType last, const std::tuple<Exprs...>& etp)
    {
	if constexpr(N > 0){
	    std::get<sizeof...(Exprs)-N-1>(etp).get(last);
	    return MOpGetX<N-1>(last.next(),etp);
	}
	else {
	    std::get<sizeof...(Exprs)-1>(etp).get(last);
	    return 0;
	}
    }

    template <size_t N, class ExtType, class... Exprs>
    inline void MOpSetX( ExtType last, std::tuple<Exprs...>& etp)
    {
	if constexpr(N > 0){
	    std::get<sizeof...(Exprs)-N-1>(etp).set(last);
	    MOpSetX<N-1>(last.next(),etp);
	}
	else {
	    std::get<sizeof...(Exprs)-1>(etp).set(last);
	}
    }

    template <typename T, class... Ops>
    inline size_t MOp<T,Ops...>::get(ExtType last) const
    {
	return MOpGetX<sizeof...(Ops)-1>(last,mOps.mOps);
    }

    template <typename T, class... Ops>
    inline MOp<T,Ops...>& MOp<T,Ops...>::set(ExtType last)
    {
	MOpSetX<sizeof...(Ops)-1>(last,mOps.mOps);
	return *this;
    }
    
    template <typename T, class... Ops>
    template <class Expr>
    auto MOp<T,Ops...>::loop(Expr exp) const
    {
	return sfor_m<sizeof...(Ops),0>
	    ( [&](auto i){ return std::get<i>(mOps.mOps); },
	      [&](auto f, auto next) { return f.loop(next); },
	      exp );
    }

    template <typename T, class... Ops>
    auto MOp<T,Ops...>::rootSteps(std::intptr_t iPtrNum) const -> ExtType
    {
	return mOps.rootSteps(iPtrNum);
    }

    template <class OpClass, class NextExpr>
    GetExpr<OpClass,NextExpr>::GetExpr(const OpClass& sec, const NextExpr& nexpr) :
        mSec(sec), mNExpr(nexpr) {}
    
    template <class OpClass, class NextExpr>
    inline void GetExpr<OpClass,NextExpr>::operator()(size_t start)
    {
        ExtType last = rootSteps();
	last.zero();
        mSec.get(last);
        mNExpr(start,last.next());
    }

    template <class OpClass, class NextExpr>
    inline void GetExpr<OpClass,NextExpr>::operator()(size_t start, ExtType last)
    {
        mSec.get(last);
        mNExpr(start,last.next());
    }

    template <class OpClass, class NextExpr>
    inline void GetExpr<OpClass,NextExpr>::get(ExtType last)
    {
	(*this)(0,last);
    }

    template <class OpClass, class NextExpr>
    typename GetExpr<OpClass,NextExpr>::ExtType
    GetExpr<OpClass,NextExpr>::rootSteps(std::intptr_t iPtrNum) const
    {
	return mSec.rootSteps(iPtrNum).extend( mNExpr.rootSteps(iPtrNum) );
    }

    template <class OpClass, class NextExpr>
    inline void GetExpr<OpClass,NextExpr>::operator()(size_t mlast, DExt last)
    {
        (*this)(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
    }

    template <class OpClass, class NextExpr>
    inline DExt GetExpr<OpClass,NextExpr>::dRootSteps(std::intptr_t iPtrNum) const
    {
        return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
    }

    template <class OpClass, class NextExpr>
    inline DExt GetExpr<OpClass,NextExpr>::dExtension() const
    {
        CHECK;
        return nullptr; //???!!!
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
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
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
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
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
        return in.loop(AssignmentExpr<T,Func,PointerAccess<T>,OperationRoot<T,Ranges...>,OpClass>
                       (mDataAcc,*this,in));
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

    template <typename T, class... Ranges>
    template <class OpClass>
    auto OperationRoot<T,Ranges...>::plus(const OpClass& in) const
    {
        return this->template asx<xxxplus<T>,PointerAccess<T>>(in);
    }

    template <typename T, class... Ranges>
    template <class OpClass, class Index>
    auto OperationRoot<T,Ranges...>::plus(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
        return this->template asx<xxxplus<T>,PointerAccess<T>>(in,i);
    }
    
    template <bool VABLE, class Func, typename TarOp, class OpClass>
    inline void vexec(TarOp& th, const OpClass& in)
    {
	// VECTORIZATION!!!
	// PARALLEL!!!
        if constexpr(VABLE){
            CHECK;
            typedef typename TarOp::value_type T;
            auto x = th.template asx<Func,VPointerAccess<T>>(in);
            //if(x.rootSteps(x.vI()) == 1){
	    if(0){
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
	//asx<identity<T>>(in)();
	vexec<OpClass::VABLE,identity<T>>(*this,in);
        return *this;
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::operator+=(const OpClass& in)
    {
	//asx<xxxplus<T>>(in)();
	vexec<OpClass::VABLE,xxxplus<T>>(*this,in);
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

    /*******************************
     *   ParallelOperationRoot     *
     ******************************
    
    template <typename T, class... Ranges>
    ParallelOperationRoot<T,Ranges...>::
    ParallelOperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(ma.data()),
        mOrigDataPtr(ma.data()),
        mDataAcc( ma.data(), ma.data() ),
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
        mDataAcc( data, data ),
	mIndex( ind )
    {
        mDataPtr = mOrigDataPtr + mIndex.pos();
    }

    template <typename T, class... Ranges>
    template <class Func, class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::asx(const OpClass& in) const
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return mIndex.pifor(1,in.loop(AssignmentExpr<T,Func,PointerAccess<T>,ParallelOperationRoot<T,Ranges...>,OpClass,OpIndexAff::TARGET>
				      (mDataAcc,*this,in))).template vec<Access::VSIZE>();
    }

    template <typename T, class... Ranges>
    template <class Func, class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::asxExpr(const OpClass& in) const
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return in.loop(AssignmentExpr<T,Func,PointerAccess<T>,ParallelOperationRoot<T,Ranges...>,OpClass>
                       (mDataAcc,*this,in));
    }
    
    template <typename T, class... Ranges>
    template <class Func, class OpClass, class Index>
    auto ParallelOperationRoot<T,Ranges...>::asx(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
        static_assert( OpClass::SIZE == decltype(in.rootSteps())::SIZE, "Ext Size mismatch" );
        return i->pifor(1,in.loop(AssignmentExpr<T,Func,PointerAccess<T>,ParallelOperationRoot<T,Ranges...>,OpClass>
                                 (mDataAcc,*this,in))).template vec<Access::VSIZE>();
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::assign(const OpClass& in) const
    {
        return this->template asx<IAssign<T>>(in);
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::assignExpr(const OpClass& in) const
    {
        return this->template asxExpr<IAssign<T>>(in);
    }
    
    template <typename T, class... Ranges>
    template <class OpClass, class Index>
    auto ParallelOperationRoot<T,Ranges...>::assign(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
        return this->template asx<IAssign<T>>(in,i);
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    auto ParallelOperationRoot<T,Ranges...>::plus(const OpClass& in) const
    {
        return this->template asx<IPlus<T>>(in);
    }

    template <typename T, class... Ranges>
    template <class OpClass, class Index>
    auto ParallelOperationRoot<T,Ranges...>::plus(const OpClass& in, const std::shared_ptr<Index>& i) const
    {
        return this->template asx<IPlus<T>>(in,i);
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    ParallelOperationRoot<T,Ranges...>& ParallelOperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
	vexec<OpClass::VABLE,identity>(*this,in);
        return *this;
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    ParallelOperationRoot<T,Ranges...>& ParallelOperationRoot<T,Ranges...>::operator+=(const OpClass& in)
    {
	vexec<OpClass::VABLE,xxxplus>(*this,in);
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
	return mDataPtr[pos.val()+mOff];
    }

    template <typename T, class... Ranges>
    template <typename V, class ET>
    inline V& ParallelOperationRoot<T,Ranges...>::vget(ET pos) const
    {
	return *(reinterpret_cast<V*>(mDataPtr+pos.val()));
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
    MExt<None> ParallelOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<None>(RangeHelper::getStepSize( mIndex, iPtrNum ));
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
    */

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
    template <typename V, class ET>
    inline V OperationValue<T>::vget(ET pos) const
    {
	return static_cast<V>(mVal); // implement???!!!
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

    
    /**************************
     *   OperationPointer     *
     **************************/

    template <typename T, class Op>
    template <class ET>
    inline const T* OperationPointer<T,Op>::get(ET pos) const
    {
	return &mOp.get(pos);
    }

    template <typename T, class Op>
    template <class ET>
    inline OperationPointer<T,Op>& OperationPointer<T,Op>::set(ET pos)
    {
	mOp.set(pos);
	return *this;
    }

    template <typename T, class Op>
    auto OperationPointer<T,Op>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(0))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op>
    template <class Expr>
    auto OperationPointer<T,Op>::loop(Expr exp) const
	-> decltype(mOp.loop(exp))
    {
	return mOp.loop(exp);
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
