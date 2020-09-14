
#include "high_level_operation.h"

namespace MultiArrayTools
{

    template <typename T, class Op>
    DynamicO<T> mkDynOp1(const Op& op)
    {
        return DynamicO<T>(op);
    }

    template <class ROP>
    template <class... Indices>
    template <class Op, class... Ops>
    void HighLevelOpBase<ROP>::RetT<Indices...>::appendOuterM(const Op& op, const Ops&... ops)
    {
        // does not check anything regarding input !!!
        if(outer.init()){
            outer = mkDynOp1<size_t>(mkMOp<size_t>(outer,op,ops...));
        }
        else {
            outer = mkDynOp1<size_t>(mkMOp<size_t>(op,ops...));
        }
    }

    template <class ROP>
    template <class... Indices>
    void HighLevelOpBase<ROP>::RetT<Indices...>::appendOuterM()
    {}

    template <class ROP>
    template <class... Indices>
    void HighLevelOpBase<ROP>::RetT<Indices...>::appendOuter(const DynamicO<size_t>& in)
    {
        if(in.init()){
            if(outer.init()){
                outer = mkDynOp1<size_t>(mkMOp<size_t>(outer,in));
            }
            else {
                outer = in;
            }
        }
    }

    template <class ROP>
    template <class... Indices>
    void HighLevelOpBase<ROP>::RetT<Indices...>::appendOuter(const RetT& in)
    {
        appendOuter(in.outer);
    }

    template <class ROP>
    HighLevelOpRoot<ROP>::HighLevelOpRoot(const ROP& op) : mOp(op) {}

    template <class ROP>
    bool HighLevelOpRoot<ROP>::root() const
    {
        return true;
    }
    
    template <class ROP>
    template <class... Inds>
    auto HighLevelOpRoot<ROP>::xcreate(const std::shared_ptr<Inds>&... inds)
        -> typename B::template RetT<Inds...>
    {
        assert(0);
        return typename B::template RetT<Inds...>();
    }

    template <class ROP>
    const ROP* HighLevelOpRoot<ROP>::get() const
    {
        return &mOp;
    }

    namespace
    {
        template <size_t N>
        struct Create
        {
            template <class... Indices>
            struct cx
            {
                template <class ROP, class OpF, class... OPs>
                struct ccx
                {
                    template <size_t M, class... DOPs>
                    static inline void
                    cccx(typename HighLevelOpBase<ROP>::template RetT<Indices...>& res,
                         const std::array<std::shared_ptr<HighLevelOpBase<ROP>>,M>& in,
                         const std::shared_ptr<Indices>&... inds,
                         const OPs&... ops,
                         const DOPs&... dops)
                    {
                        static_assert(N > 0, "N > 0 failed");
                        auto& inn = std::get<N>(in);
                        if(not inn->root()){
                            auto dop = inn->create(inds...);
                            auto op = *dop.op.data()->mOp;
                            typedef decltype(op) OP;
                            res.appendOuter(dop);
                            Create<N-1>::template cx<Indices...>::template ccx<ROP,OpF,OP,OPs...>::template cccx<M>
                                (res, in, inds..., op, ops..., dop, dops...);
                        }
                        else {
                            auto& op = *inn->get();
                            typedef typename std::remove_reference<decltype(op)>::type OP;
                            Create<N-1>::template cx<Indices...>::template ccx<ROP,OpF,OP,OPs...>::template cccx<M>
                                (res, in, inds..., op, ops..., dops...);
                        }
                    }
                };
            };
        };

        template <>
        struct Create<0>
        {
            template <class... Indices>
            struct cx
            {
                template <class ROP, class OpF, class... OPs>
                struct ccx
                {
                    template <size_t M, class... DOPs>
                    static inline void
                    cccx(typename HighLevelOpBase<ROP>::template RetT<Indices...>& res,
                         const std::array<std::shared_ptr<HighLevelOpBase<ROP>>,M>& in,
                         const std::shared_ptr<Indices>&... inds,
                         const OPs&... ops,
                         const DOPs&... dops)
                    {
                        auto& inn = std::get<0>(in);
                        if(not inn->root()){
                            auto dop = inn->create(inds...);
                            auto op = *dop.op.data()->mOp;
                            res.appendOuter(dop);
                            res.op = mkDynOutOp(mkFOp<OpF>(op,ops...), inds...);
                            res.appendOuterM(dop.op,dops.op...);
                        }
                        else {
                            auto& op = *inn->get();
                            res.op = mkDynOutOp(mkFOp<OpF>(op,ops...), inds...);
                            res.appendOuterM(dops.op...);
                        }
                    }
                };
            };
        };
    }

    template <class ROP, class OpF, size_t N>
    HighLevelOp<ROP,OpF,N>::HighLevelOp(std::array<std::shared_ptr<HighLevelOpBase<ROP>>,N> in) : mIn(in) {}
    
    template <class ROP, class OpF, size_t N>
    bool HighLevelOp<ROP,OpF,N>::root() const
    {
        return false;
    }
    
    template <class ROP, class OpF, size_t N>
    const ROP* HighLevelOp<ROP,OpF,N>::get() const
    {
	assert(0);
	return nullptr;
    }

    template <class ROP, class OpF, size_t N>
    template <class... Inds>
    auto HighLevelOp<ROP,OpF,N>::xcreate(const std::shared_ptr<Inds>&... inds)
        -> typename B::template RetT<Inds...>
    {
        typename B::template RetT<Inds...> res;
        Create<N-1>::template cx<Inds...>::template ccx<ROP,OpF>::template cccx<N>
	    (res,mIn,inds...);
        return res;
    }

    template <class ROP>
    HighLevelOpHolder<ROP>::HighLevelOpHolder(const std::shared_ptr<HighLevelOpBase<ROP>>& op) : mOp(op) {}

    template <class ROP>
    bool HighLevelOpHolder<ROP>::root() const
    {
        return mOp->root();
    }
    
    template <class ROP>
    template <class... Inds>
    auto HighLevelOpHolder<ROP>::create(const std::shared_ptr<Inds>&... inds) const
        -> decltype(mOp->create(inds...))
    {
        return mOp->create(inds...);
    }

    template <class ROP>
    auto HighLevelOpHolder<ROP>::get() const
        -> decltype(mOp->get())
    {
        return mOp->get();
    }

    template <class ROP>
    std::shared_ptr<HighLevelOpBase<ROP>> HighLevelOpHolder<ROP>::op() const
    {
        return mOp;
    }
    
    template <class ROP>
    HighLevelOpHolder<ROP> HighLevelOpHolder<ROP>::operator*(const HighLevelOpHolder& in) const
    {
        return HighLevelOpHolder<ROP>
            ( std::make_shared<HighLevelOp<ROP,multipliesx<double,double>,2>>
              ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }

    template <class ROP>
    HighLevelOpHolder<ROP> HighLevelOpHolder<ROP>::operator+(const HighLevelOpHolder& in) const
    {
        return HighLevelOpHolder<ROP>
            ( std::make_shared<HighLevelOp<ROP,plusx<double,double>,2>>
              ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }

    template <class ROP>
    HighLevelOpHolder<ROP> HighLevelOpHolder<ROP>::operator-(const HighLevelOpHolder& in) const
    {
        return HighLevelOpHolder<ROP>
            ( std::make_shared<HighLevelOp<ROP,minusx<double,double>,2>>
              ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }
    
    template <class ROP>
    HighLevelOpHolder<ROP> HighLevelOpHolder<ROP>::operator/(const HighLevelOpHolder& in) const
    {
        return HighLevelOpHolder<ROP>
            ( std::make_shared<HighLevelOp<ROP,dividesx<double,double>,2>>
              ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,2>({mOp, in.mOp}) ) );
    }

    template <class F, class ROP, class... ROPs>
    HighLevelOpHolder<ROP> mkSFunc(const HighLevelOpHolder<ROP>& a, const HighLevelOpHolder<ROPs>&... as)
    {
        constexpr size_t N = sizeof...(ROPs)+1;
        return HighLevelOpHolder<ROP>
            ( std::make_shared<HighLevelOp<ROP,F,N>>
              ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,N>({a.op(), as.op()...}) ) );
    }

    template <class ROP>
    template <class... Indices>
    HighLevelOpHolder<ROP>& HighLevelOpHolder<ROP>::xassign(const HighLevelOpHolder& in,
                                                            const std::shared_ptr<DynamicIndex>& di,
                                                            const std::shared_ptr<Indices>&... is)
    {
        const size_t dim = di->dim();
        if(dim >= 2){
	    auto ci1 = di->getP(dim-2)->reduced();
	    auto ci2 = di->getP(dim-1)->reduced();
            //auto ci1 = std::dynamic_pointer_cast<IndexWrapper<CI>>(di->getP(dim-2));
            //auto ci2 = std::dynamic_pointer_cast<IndexWrapper<CI>>(di->getP(dim-1));
            assert(ci1 != nullptr);
            assert(ci2 != nullptr);
            auto odi = mkSubSpaceX(di, dim-2);
            auto mi = mkMIndex(is..., odi);
            //this->assign(in, mi, ci1->getIndex(), ci2->getIndex());
	    this->assign(in, mi, ci1, ci2);
        }
        else {
            assert(dim == 1);
            //auto ci1 = std::dynamic_pointer_cast<IndexWrapper<CI>>(di->getP(dim-2));
	    auto ci1 = di->getP(dim-2)->reduced();
            assert(ci1 != nullptr);
            auto odi = mkSubSpaceX(di, dim-1);
            auto mi = mkMIndex(is..., odi);
            this->assign(in, mi, ci1);
        }
        //INDS<ROP,Indices...>::template CallHLOp<> call;
        //call.assign(*this, in, is..., di);
        return *this;
    }

    template <class Ind1, class Ind2, class... Inds>
    std::string printInd(const std::shared_ptr<Ind1>& ind1, const std::shared_ptr<Ind2>& ind2,
                         const std::shared_ptr<Inds>&... inds)
    {
        return std::to_string(reinterpret_cast<std::intptr_t>(ind1.get())) + "(" +
            std::to_string(ind1->max()) + "), " + printInd(ind2, inds...);
    }

    template <class Ind1>
    std::string printInd(const std::shared_ptr<Ind1>& ind1)
    {
        return std::to_string(reinterpret_cast<std::intptr_t>(ind1.get())) + "(" + std::to_string(ind1->max()) + ")";
    }
    
    template <class ROP>
    template <class MIndex, class... Indices>
    HighLevelOpHolder<ROP>& HighLevelOpHolder<ROP>::assign(const HighLevelOpHolder& in,
                                                           const std::shared_ptr<MIndex>& mi,
                                                           const std::shared_ptr<Indices>&... inds)
    {
        //VCHECK(printInd(inds...));
        auto xx = mkArrayPtr<double>(nullr());
        auto& opr = *mOp->get();
        auto loop = mkPILoop
            ( [&opr,&in,&xx,&inds...,this](){
                auto inx = in;
                auto dop = inx.create(inds...);
                auto gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.outer,dop.op));
                auto xloop = mkILoop(std::make_tuple(*dop.op.data()->mOp),
                                     std::make_tuple(inds...),
                                     std::make_tuple(xx),
                                     std::make_tuple(opr.assign( *dop.op.data()->mOp,
                                                                 mkMIndex(inds...) )),
                                     std::array<size_t,1>({1}), std::array<size_t,1>({0}));
                return mkGetExpr(gexp, xloop); });
        mi->pifor(1,loop)();
        return *this;
    }

    template <class ROP>
    template <class MIndex, class... Indices>
    HighLevelOpHolder<ROP>& HighLevelOpHolder<ROP>::plus(const HighLevelOpHolder& in,
                                                         const std::shared_ptr<MIndex>& mi,
                                                         const std::shared_ptr<Indices>&... inds)
    {
        auto xx = mkArrayPtr<double>(nullr());
        auto& opr = *mOp->get();
        auto loop = mkPILoop
            ( [&opr,&in,&xx,&inds...,this](){
                auto inx = in;
                auto dop = inx.create(inds...);
                auto gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.outer,dop.op));
                auto xloop = mkILoop(std::make_tuple(*dop.op.data()->mOp),
                                     std::make_tuple(inds...),
                                     std::make_tuple(xx),
                                     std::make_tuple(opr.plus( *dop.op.data()->mOp,
                                                               mkMIndex(inds...) )),
                                     std::array<size_t,1>({1}), std::array<size_t,1>({0}));
                return mkGetExpr(gexp, xloop); });
        mi->pifor(1,loop)();
        return *this;
    }

    template <class ROP>
    HighLevelOpHolder<ROP> mkHLO(const ROP& op)
    {
        return HighLevelOpHolder<ROP>(std::make_shared<HighLevelOpRoot<ROP>>( op ) );
    }

#define SP " "
#define regFunc1(fff) template <class ROP> \
    HighLevelOpHolder<ROP> hl_##fff (const HighLevelOpHolder<ROP>& in)  \
    { return HighLevelOpHolder<ROP>( std::make_shared<HighLevelOp<ROP,x_##fff <double>,1>> \
                                     ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,1>( {in.op()} ) ) ); } \
       
#include "extensions/math.h"
#undef regFunc1
#undef SP

    /*
    template <size_t N>
    template <class ITuple>
    inline void SetLInds<N>::mkLIT(const ITuple& itp, const std::shared_ptr<DynamicIndex>& di)
    {
	constexpr size_t NN = std::tuple_size<ITuple>::value-N-1;
	const size_t nn = di->dim()-N-1;
	typedef typename std::remove_reference<decltype(*std::get<NN>(itp))>::type T;
	std::get<NN>(itp) =
	    std::dynamic_pointer_cast<T>(di->get(nn))->getIndex();
	SetLInds<N-1>::mkLIT(itp, di);
    }
	    
    template <size_t N>
    template <class Tar, class ITp, typename... Args>
    template <class... Is>
    inline void SetLInds<N>::xx<Tar,ITp,Args...>::
    assign(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
    {
	SetLInds<N-1>::template xx<ITp,Args...>::assign(tar, args..., itp, std::get<N>(itp), is...);
    }

    template <size_t N>
    template <class Tar, class ITp, typename... Args>
    template <class... Is>
    inline void SetLInds<N>::xx<Tar,ITp,Args...>::
    plus(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
    {
	SetLInds<N-1>::template xx<ITp,Args...>::plus(tar, args..., itp, std::get<N>(itp), is...);
    }

    //template <>
    template <class ITuple>
    inline void SetLInds<0>::mkLIT(const ITuple& itp, const std::shared_ptr<DynamicIndex>& di)
    {
	constexpr size_t NN = std::tuple_size<ITuple>::value-1;
	const size_t nn = di->dim()-1;
	typedef typename std::remove_reference<decltype(*std::get<NN>(itp))>::type T;
	std::get<NN>(itp) =
	    std::dynamic_pointer_cast<T>(di->get(nn))->getIndex();
    }

    //template <>
    template <class Tar, class ITp, typename... Args>
    template <class... Is>
    inline void SetLInds<0>::xx<Tar,ITp,Args...>::
    assign(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
    {
	tar.assign(args..., std::get<0>(itp), is...);
    }

    //template <>
    template <class Tar, class ITp, typename... Args>
    template <class... Is>
    inline void SetLInds<0>::xx<Tar,ITp,Args...>::
    plus(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
    {
	tar.plus(args..., std::get<0>(itp), is...);
    }

    template <class ROP, class... Indices>
    size_t INDS<ROP,Indices...>::CallHLOpBase::depth() const
    {
	return mDepth;
    }

    
    template <class ROP, class... Indices>
    template <class... LIndices>
    void INDS<ROP,Indices...>::CallHLOp<LIndices...>::
    assign(HighLevelOpHolder<ROP>& target, const HighLevelOpHolder<ROP>& source,
	   const std::shared_ptr<Indices>&... is,
	   const std::shared_ptr<DynamicIndex>& di) const
    {
	auto ip = di->get(di->dim() - this->depth());
	auto iregn = ip->regN();
	if(iregn.type >= 0 and iregn.depth > sizeof...(LIndices)){
	    sNext[iregn.type]->assign(target, source, is..., di);
	}
	else {
	    ITuple itp;
	    SetLInds<sizeof...(LIndices)-1>::mkLIT(itp,di);
	    auto mi = mkIndex(is...,mkSubSpaceX(di, di->dim() - this->depth()));
	    SetLInds<sizeof...(LIndices)-1>::
		template xx<HighLevelOpHolder<ROP>,ITuple,HighLevelOpHolder<ROP>,decltype(mi)>::
		assign(target, source, mi, itp);
	}
    }

    template <class ROP, class... Indices>
    template <class... LIndices>
    void INDS<ROP,Indices...>::CallHLOp<LIndices...>::
    plus(HighLevelOpHolder<ROP>& target, const HighLevelOpHolder<ROP>& source,
	 const std::shared_ptr<Indices>&... is,
	 const std::shared_ptr<DynamicIndex>& di) const
    {
	auto ip = di->get(di->dim() - this->depth());
	auto iregn = ip->regN();
	if(iregn.type >= 0 and iregn.depth > sizeof...(LIndices)){
	    sNext[iregn.type]->plus(target, source, is..., di);
	}
	else {
	    ITuple itp;
	    SetLInds<sizeof...(LIndices)-1>::mkLIT(itp,di);
	    auto mi = mkIndex(is...,mkSubSpaceX(di, di->dim() - this->depth()));
	    SetLInds<sizeof...(LIndices)-1>::
		template xx<HighLevelOpHolder<ROP>,ITuple,HighLevelOpHolder<ROP>,decltype(mi)>::
		plus(target, source, mi, itp);
	}
    }
    */
}

