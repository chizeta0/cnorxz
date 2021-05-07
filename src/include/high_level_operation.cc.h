
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
    ROP* HighLevelOpRoot<ROP>::get()
    {
        return &mOp;
    }

    template <class ROP>
    auto HighLevelOpRoot<ROP>::vget()
	-> VOP*
    {
        return nullptr;
    }

    template <class ROP>
    HighLevelOpValue<ROP>::HighLevelOpValue(const VOP& op) : mOp(op) {}

    template <class ROP>
    bool HighLevelOpValue<ROP>::root() const
    {
        return true;
    }
    
    template <class ROP>
    template <class... Inds>
    auto HighLevelOpValue<ROP>::xcreate(const std::shared_ptr<Inds>&... inds)
        -> typename B::template RetT<Inds...>
    {
        assert(0);
        return typename B::template RetT<Inds...>();
    }

    template <class ROP>
    ROP* HighLevelOpValue<ROP>::get()
    {
        return nullptr;
    }

    template <class ROP>
    auto HighLevelOpValue<ROP>::vget()
	-> VOP*
    {
        return &mOp;
    }

    namespace
    {
        template <size_t N, class... Indices>
        struct Create
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
		    //static_assert(N > 0, "N > 0 failed");
		    auto& inn = std::get<N>(in);
		    if(not inn->root()){
			auto dop = inn->create(inds...);
			auto op = *dop.op.data()->mOp;
			res.appendOuter(dop);
			assert(dop.op.init());
			if constexpr(N > 0){
			    typedef decltype(op) OP;
			    Create<N-1,Indices...>::template ccx<ROP,OpF,OP,OPs...>::template cccx<M>
				(res, in, inds..., op, ops..., dop, dops...);
			}
			else {
                            res.op = mkDynOutOp(mkFOp<OpF>(op,ops...), inds...);
                            res.appendOuterM(dop.op,dops.op...);
			}
		    }
		    else {
			auto op = inn->get();
			auto vop = inn->vget();
			if constexpr(N > 0){
			    typedef typename std::remove_reference<decltype(*op)>::type OP;
			    typedef typename std::remove_reference<decltype(*vop)>::type VOP;
			    if(op != nullptr){
				Create<N-1,Indices...>::template ccx<ROP,OpF,OP,OPs...>::template cccx<M>
				    (res, in, inds..., *op, ops..., dops...);
			    }
			    else {
				Create<N-1,Indices...>::template ccx<ROP,OpF,VOP,OPs...>::template cccx<M>
				    (res, in, inds..., *vop, ops..., dops...);
			    }
			}
			else {
			    if(op != nullptr){
				res.op = mkDynOutOp(mkFOp<OpF>(*op,ops...), inds...);
			    }
			    else {
				res.op = mkDynOutOp(mkFOp<OpF>(*vop,ops...), inds...);
			    }
                            res.appendOuterM(dops.op...);
			}
		    }
		}
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
    ROP* HighLevelOp<ROP,OpF,N>::get()
    {
	assert(0);
	return nullptr;
    }
    
    template <class ROP, class OpF, size_t N>
    auto HighLevelOp<ROP,OpF,N>::vget()
	-> VOP*
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
        Create<N-1,Inds...>::template ccx<ROP,OpF>::template cccx<N>
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
    auto HighLevelOpHolder<ROP>::get()
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
        if(dim > 2){
	    auto ci1 = di->getP(dim-2)->reduced();
	    auto ci2 = di->getP(dim-1)->reduced();
            assert(ci1 != nullptr);
            assert(ci2 != nullptr);
            auto odi = mkSubSpaceX(di, dim-2);
            auto mi = mkMIndex(is..., odi);
	    this->assign(in, mi, ci1, ci2);
        }
        else {
            assert(dim == 2 or dim == 1);
	    auto ci1 = di->getP(dim-1)->reduced();
            assert(ci1 != nullptr);
            auto odi = mkSubSpaceX(di, dim-1);
            auto mi = mkMIndex(is..., odi);
            this->assign(in, mi, ci1);
        }
        return *this;
    }

    template <class ROP>
    template <class... Indices>
    HighLevelOpHolder<ROP>& HighLevelOpHolder<ROP>::xplus(const HighLevelOpHolder& in,
							  const std::shared_ptr<DynamicIndex>& di,
							  const std::shared_ptr<Indices>&... is)
    {
        const size_t dim = di->dim();
        if(dim > 2){
	    auto ci1 = di->getP(dim-2)->reduced();
	    auto ci2 = di->getP(dim-1)->reduced();
            assert(ci1 != nullptr);
            assert(ci2 != nullptr);
            auto odi = mkSubSpaceX(di, dim-2);
            auto mi = mkMIndex(is..., odi);
	    this->plus(in, mi, ci1, ci2);
        }
        else {
            assert(dim == 2 or dim == 1);
	    auto ci1 = di->getP(dim-1)->reduced();
            assert(ci1 != nullptr);
            auto odi = mkSubSpaceX(di, dim-1);
            auto mi = mkMIndex(is..., odi);
            this->plus(in, mi, ci1);
        }
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
        auto xx = mkArrayPtr<double>(nullr());
        ROP& opr = *mOp->get();
	if(in.root()){
	    auto inx = in;
	    opr.par().assign( *inx.get(), mkMIndex(mi,inds...) )();
	    return *this;
	}
        auto loop = mkPILoop
            ( [&opr,&in,&xx,&inds...,this](){
                auto inx = in;
                auto dop = inx.create(inds...);
		DynamicO<size_t> gexp;
		if(dop.outer.init()){
		    gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.outer,dop.op));
		}
		else {
		    gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.op));
		}
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
        ROP& opr = *mOp->get();
	if(in.root()){
	    auto inx = in;
	    opr.par().plus( *inx.get(), mkMIndex(mi,inds...) )();
	    return *this;
	}
        auto loop = mkPILoop
            ( [&opr,&in,&xx,&inds...,this](){
                auto inx = in;
                auto dop = inx.create(inds...);
		DynamicO<size_t> gexp;
		if(dop.outer.init()){
		    gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.outer,dop.op));
		}
		else {
		    gexp = mkDynOp1<size_t>(mkMOp<size_t>(dop.op));
		}
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

    template <class ROP>
    HighLevelOpHolder<ROP> mkHLOV(double val)
    {
        return HighLevelOpHolder<ROP>(std::make_shared<HighLevelOpValue<ROP>>
				      ( OperationValue<double>(val) ) );
    }


#define SP " "
#define regFunc1(fff) template <class ROP> \
    HighLevelOpHolder<ROP> hl_##fff (const HighLevelOpHolder<ROP>& in)  \
    { return HighLevelOpHolder<ROP>( std::make_shared<HighLevelOp<ROP,x_##fff <double>,1>> \
                                     ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,1>( {in.op()} ) ) ); } \
       
#include "extensions/math.h"
#undef regFunc1
#undef SP

}

