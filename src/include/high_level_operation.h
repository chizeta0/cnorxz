
#ifndef __high_level_operation_h__
#define __high_level_operation_h__

#include "base_def.h"
#include "ranges/rheader.h"
#include "dynamic_operation.h"

namespace MultiArrayTools
{

    typedef ClassicRange CR;
    typedef CR::IndexType CI;
    typedef std::shared_ptr<CI> CIP;

    typedef OperationRoot<double,CR,DynamicRange> OpCD;
    typedef OperationRoot<double,DynamicRange> OpD;
    extern template class OperationRoot<double,CR,DynamicRange>;
    extern template class OperationRoot<double,DynamicRange>;
    
    template <typename T, class Op>
    DynamicO<T> mkDynOp1(const Op& op);

    std::shared_ptr<DynamicIndex> mkSubSpaceX(const std::shared_ptr<DynamicIndex>& di, size_t max);

    template <class ROP>
    class HighLevelOpBase
    {
    public:
    
        template <class... Indices>
        struct RetT
        {
            DynamicO<OpH<OperationRoot<double,typename Indices::RangeType...>>> op;
            DynamicO<size_t> outer;

            template <class Op, class... Ops>
            void appendOuterM(const Op& op, const Ops&... ops);

            void appendOuterM();
            void appendOuter(const DynamicO<size_t>& in);
            void appendOuter(const RetT& in);

        };
    
        virtual bool root() const = 0;

#define reg_ind1(I1) virtual RetT<I1> create \
        (const std::shared_ptr<I1>& ind1) = 0
#define reg_ind2(I1,I2) virtual RetT<I1,I2> create \
        (const std::shared_ptr<I1>& ind1,const std::shared_ptr<I2>& ind2) = 0
#define reg_ind3(I1,I2,I3) virtual RetT<I1,I2,I3> create   \
        (const std::shared_ptr<I1>& ind1,const std::shared_ptr<I2>& ind2,const std::shared_ptr<I3>& ind3) = 0

#include "hl_reg_ind.h"

#undef reg_ind1
#undef reg_ind2
#undef reg_ind3

        virtual ROP* get() = 0;

    };

    template <class ROP>
    class HighLevelOpRoot : public HighLevelOpBase<ROP>
    {
    private:
        typedef HighLevelOpBase<ROP> B;

	template <class... Inds>
	typename B::template RetT<Inds...> xcreate(const std::shared_ptr<Inds>&... inds);
    
        ROP mOp;
    public:

        HighLevelOpRoot(const ROP& op);
        
        virtual bool root() const override final;
    
#define reg_ind1(I1) virtual typename B::template RetT<I1> create \
        (const std::shared_ptr<I1>& ind1) \
            override final { return xcreate(ind1); }
#define reg_ind2(I1,I2) virtual typename B::template RetT<I1,I2> create \
        (const std::shared_ptr<I1>& ind1, const std::shared_ptr<I2>& ind2) \
            override final { return xcreate(ind1,ind2); }
#define reg_ind3(I1,I2,I3) virtual typename B::template RetT<I1,I2,I3> create \
        (const std::shared_ptr<I1>& ind1, const std::shared_ptr<I2>& ind2, const std::shared_ptr<I3>& ind3) \
            override final { return xcreate(ind1,ind2,ind3); }

#include "hl_reg_ind.h"

        virtual ROP* get() override final;

   
    };

    extern template class HighLevelOpBase<OpCD>;
    extern template class HighLevelOpBase<OpD>;
    extern template class HighLevelOpRoot<OpCD>;
    extern template class HighLevelOpRoot<OpD>;
    
    template <class OpF, class... Ops>
    auto mkFOp(const Ops&... ops)
    {
        return Operation<double,OpF,Ops...>(ops...);
    }



    template <class ROP, class OpF, size_t N>
    class HighLevelOp : public HighLevelOpBase<ROP>
    {
    public:
        typedef HighLevelOpBase<ROP> B;

    private:
        std::array<std::shared_ptr<HighLevelOpBase<ROP>>,N> mIn;

	template <class... Inds>
	auto xcreate(const std::shared_ptr<Inds>&... inds)
	    -> typename B::template RetT<Inds...>;
	
    public:
        HighLevelOp(std::array<std::shared_ptr<HighLevelOpBase<ROP>>,N> in);
        
        virtual bool root() const override final;
    
        virtual ROP* get() override final;
    
#include "hl_reg_ind.h"

#undef reg_ind1
#undef reg_ind2
#undef reg_ind3
    };

    extern template class HighLevelOp<OpCD,plusx<double,double>,2>;
    extern template class HighLevelOp<OpCD,minusx<double,double>,2>;
    extern template class HighLevelOp<OpCD,multipliesx<double,double>,2>;
    extern template class HighLevelOp<OpCD,dividesx<double,double>,2>;
    extern template class HighLevelOp<OpD,plusx<double,double>,2>;
    extern template class HighLevelOp<OpD,minusx<double,double>,2>;
    extern template class HighLevelOp<OpD,multipliesx<double,double>,2>;
    extern template class HighLevelOp<OpD,dividesx<double,double>,2>;
    
#define regFunc1(fff) \
    extern template class HighLevelOp<OpCD,x_##fff<double>,1>; \
    extern template class HighLevelOp<OpD,x_##fff<double>,1>;
#include "extensions/math.h"
#undef regFunc1
    
    template <class ROP>
    class HighLevelOpHolder
    {
    private:
        std::shared_ptr<HighLevelOpBase<ROP>> mOp;

    public:
        HighLevelOpHolder() = default;
        HighLevelOpHolder(const HighLevelOpHolder& in) = default; 
        HighLevelOpHolder(HighLevelOpHolder&& in) = default;
        HighLevelOpHolder& operator=(const HighLevelOpHolder& in) = default;
        HighLevelOpHolder& operator=(HighLevelOpHolder&& in) = default;
    
        HighLevelOpHolder(const std::shared_ptr<HighLevelOpBase<ROP>>& op);

        bool root() const;

	template <class... Inds>
        auto create(const std::shared_ptr<Inds>&... inds) const
            -> decltype(mOp->create(inds...));

        auto get() -> decltype(mOp->get());

        std::shared_ptr<HighLevelOpBase<ROP>> op() const;
        HighLevelOpHolder operator*(const HighLevelOpHolder& in) const;
        HighLevelOpHolder operator+(const HighLevelOpHolder& in) const;
        HighLevelOpHolder operator-(const HighLevelOpHolder& in) const;
        HighLevelOpHolder operator/(const HighLevelOpHolder& in) const;

        
        template <class... Indices>
        HighLevelOpHolder& xassign(const HighLevelOpHolder& in,
                                   const std::shared_ptr<DynamicIndex>& di,
                                   const std::shared_ptr<Indices>&... is);
        
        template <class... Indices>
        HighLevelOpHolder& xplus(const HighLevelOpHolder& in,
				 const std::shared_ptr<DynamicIndex>& di,
				 const std::shared_ptr<Indices>&... is);

        template <class MIndex, class... Indices>
        HighLevelOpHolder& assign(const HighLevelOpHolder& in,
                                  const std::shared_ptr<MIndex>& mi,
                                  const std::shared_ptr<Indices>&... inds);

        template <class MIndex, class... Indices>
        HighLevelOpHolder& plus(const HighLevelOpHolder& in,
                                const std::shared_ptr<MIndex>& mi,
                                const std::shared_ptr<Indices>&... inds);
    };

    extern template class HighLevelOpHolder<OpCD>;
    extern template class HighLevelOpHolder<OpD>;
    
    template <class F, class ROP, class... ROPs>
    HighLevelOpHolder<ROP> mkSFunc(const HighLevelOpHolder<ROP>& a, const HighLevelOpHolder<ROPs>&... as);


    template <class ROP>
    HighLevelOpHolder<ROP> mkHLO(const ROP& op);

#define regFunc1(fff) template <class ROP> \
    HighLevelOpHolder<ROP> hl_##fff (const HighLevelOpHolder<ROP>& in);
#include "extensions/math.h"
#undef regFunc1
    
#define regFunc1(fff) template <class ROP>				\
    HighLevelOpHolder<ROP> hl_##fff (const HighLevelOpHolder<ROP>& in); \
    extern template HighLevelOpHolder<OpCD> hl_##fff (const HighLevelOpHolder<OpCD>& in); \
    extern template HighLevelOpHolder<OpD> hl_##fff (const HighLevelOpHolder<OpD>& in);
#include "extensions/math.h"
#undef regFunc1
   

}

#endif
