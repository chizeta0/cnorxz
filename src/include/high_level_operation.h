
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

    template <typename T, class Op>
    DynamicO<T> mkDynOp1(const Op& op);

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
    
        virtual RetT<CI,CI> create(const std::shared_ptr<CI>& ind1,
                                   const std::shared_ptr<CI>& ind2) = 0;

        virtual const ROP* get() const = 0;

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
    
        virtual typename B::template RetT<CI,CI>
        create(const std::shared_ptr<CI>& ind1,
               const std::shared_ptr<CI>& ind2) override final { return xcreate(ind1,ind2); }

        virtual const ROP* get() const override final;

   
    };

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
    
        virtual const ROP* get() const override final;
    
        virtual typename B::template RetT<CI,CI>
        create(const std::shared_ptr<CI>& ind1,
               const std::shared_ptr<CI>& ind2) override final { return xcreate(ind1,ind2); }
	
    };


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

        auto get() const -> decltype(mOp->get());

        std::shared_ptr<HighLevelOpBase<ROP>> op() const;
        HighLevelOpHolder operator*(const HighLevelOpHolder& in) const;
        HighLevelOpHolder operator+(const HighLevelOpHolder& in) const;
        HighLevelOpHolder operator-(const HighLevelOpHolder& in) const;
        HighLevelOpHolder operator/(const HighLevelOpHolder& in) const;

        template <class MIndex, class... Indices>
        HighLevelOpHolder& assign(const HighLevelOpHolder& in,
                                  const std::shared_ptr<MIndex>& mi,
                                  const std::shared_ptr<Indices>&... inds);

        template <class MIndex, class... Indices>
        HighLevelOpHolder& plus(const HighLevelOpHolder& in,
                                const std::shared_ptr<MIndex>& mi,
                                const std::shared_ptr<Indices>&... inds);
    };


    template <class ROP>
    HighLevelOpHolder<ROP> mkHLO(const ROP& op);

#define SP " "
#define regFunc1(fff) template <class ROP> \
    HighLevelOpHolder<ROP> hl_##fff (const HighLevelOpHolder<ROP>& in);
#include "extensions/math.h"
#undef regFunc1
#undef SP
    /*
    template <class ROP>
    HighLevelOpHolder<ROP> exp(const HighLevelOpHolder<ROP>& in);
    */
    
}

#endif
