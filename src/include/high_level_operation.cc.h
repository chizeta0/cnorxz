
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
    auto HighLevelOpRoot<ROP>::create(const std::shared_ptr<CI> ind1,
                                      const std::shared_ptr<CI> ind2)
        -> typename B::RetT<CI,CI>
    {
        assert(0);
        return typename B::RetT<CI,CI>();
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
                    cccx(typename HighLevelOpBase<ROP>::RetT<Indices...>& res,
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
                    cccx(typename HighLevelOpBase<ROP>::RetT<Indices...>& res,
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
    { assert(0); return nullptr; }
    
    template <class ROP, class OpF, size_t N>
    auto HighLevelOp<ROP,OpF,N>::create(const std::shared_ptr<CI> ind1,
                                                           const std::shared_ptr<CI> ind2)
        -> typename B::RetT<CI,CI>
    {
        typename B::RetT<CI,CI> res;
        Create<N-1>::template cx<CI,CI>::template ccx<ROP,OpF>::template cccx<N>(res,mIn,ind1,ind2);
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
    auto HighLevelOpHolder<ROP>::create(const std::shared_ptr<CI> ind1,
                                        const std::shared_ptr<CI> ind2) const
        -> decltype(mOp->create(ind1,ind2))
    {
        return mOp->create(ind1,ind2);
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

    template <class ROP>
    template <class MIndex, class... Indices>
    HighLevelOpHolder<ROP>& HighLevelOpHolder<ROP>::assign(const HighLevelOpHolder& in,
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
    template <class ROP>
    HighLevelOpHolder<ROP> exp(const HighLevelOpHolder<ROP>& in)
    {
        return HighLevelOpHolder<ROP>
            ( std::make_shared<HighLevelOp<ROP,x_exp<double>,1>>
              ( std::array<std::shared_ptr<HighLevelOpBase<ROP>>,1>( {in.op()} ) ) );
    }
    */
}
