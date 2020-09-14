
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
    
#include "hl_reg_ind.h"

#undef reg_ind1
#undef reg_ind2
#undef reg_ind3
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

        
        template <class... Indices>
        HighLevelOpHolder& xassign(const HighLevelOpHolder& in,
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

    template <class F, class ROP, class... ROPs>
    HighLevelOpHolder<ROP> mkSFunc(const HighLevelOpHolder<ROP>& a, const HighLevelOpHolder<ROPs>&... as);


    template <class ROP>
    HighLevelOpHolder<ROP> mkHLO(const ROP& op);

#define SP " "
#define regFunc1(fff) template <class ROP> \
    HighLevelOpHolder<ROP> hl_##fff (const HighLevelOpHolder<ROP>& in);
#include "extensions/math.h"
#undef regFunc1
#undef SP
    /*
    template <size_t N>
    struct SetLInds
    {
	template <class ITuple>
	static inline void mkLIT(const ITuple& itp, const std::shared_ptr<DynamicIndex>& di);
    
	template <class Tar, class ITp, typename... Args>
        struct xx
        {
            template <class... Is>
            static inline void assign(Tar& tar, const Args&... args,
				      const ITp& itp, const std::shared_ptr<Is>&... is);

            template <class... Is>
            static inline void plus(Tar& tar, const Args&... args,
				    const ITp& itp, const std::shared_ptr<Is>&... is);
        };
    };

    template <>
    struct SetLInds<0>
    {
	template <class ITuple>
	static inline void mkLIT(const ITuple& itp, const std::shared_ptr<DynamicIndex>& di);

        template <class Tar, class ITp, typename... Args>
        struct xx
        {
            template <class... Is>
            static inline void assign(Tar& tar, const Args&... args,
				      const ITp& itp, const std::shared_ptr<Is>&... is);

            template <class... Is>
            static inline void plus(Tar& tar, const Args&... args,
				    const ITp& itp, const std::shared_ptr<Is>&... is);
        };
    };

    template <class ROP, class... Indices>
    struct INDS
    {
        class CallHLOpBase
        {
        private:
            size_t mDepth;
        public:
	    size_t depth() const;
	    
            void assign(const HighLevelOpHolder<ROP>& target, const HighLevelOpHolder<ROP>& source,
                        const std::shared_ptr<Indices>&... is,
                        const std::shared_ptr<DynamicIndex>& di) const = 0;

            void plus(const HighLevelOpHolder<ROP>& target, const HighLevelOpHolder<ROP>& source,
                      const std::shared_ptr<Indices>&... is,
                      const std::shared_ptr<DynamicIndex>& di) const = 0;
        };

        template <class... LIndices>
        class CallHLOp
        {
        private:
            typedef std::tuple<std::shared_ptr<LIndices>...> ITuple;
            static vector<std::shared_ptr<CallHLOpBase>> sNext;
        public:
            void assign(HighLevelOpHolder<ROP>& target, const HighLevelOpHolder<ROP>& source,
                        const std::shared_ptr<Indices>&... is,
                        const std::shared_ptr<DynamicIndex>& di) const override final;

            void plus(HighLevelOpHolder<ROP>& target, const HighLevelOpHolder<ROP>& source,
                      const std::shared_ptr<Indices>&... is,
                      const std::shared_ptr<DynamicIndex>& di) const override final;
        };
    };
    */
}

#endif
