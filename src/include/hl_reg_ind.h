
#ifndef __hl_reg_ind_h__
#define __hl_reg_ind_h__

namespace MultiArrayTools
{

    template <class Index>
    struct RegIndNum
    {
        static constexpr size_t VALUE = -1;
        static constexpr size_t DEPTH = 0;
    };

    template <>
    struct RegIndNum<ClassicRange::IndexType>
    {
        static constexpr size_t VALUE = 0;
        static constexpr size_t DEPTH = 3;
    };

    // to be returned by IndexWrapper
    struct RegIndInfo
    {
        size_t type;
        size_t depth;

        template <class Index>
        RegIndInfo& set(const std::shared_ptr<Index>& i)
        {
            type = RegIndNum<Index>::VALUE;
            depth = RegIndNum<Index>::DEPTH;
        }
    };

    template <size_t N>
    struct SetLInds
    {
        template <class Tar, class ITp, typename... Args>
        struct xx
        {
            template <class... Is>
            static inline void assign(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
            {
                SetLInds<N-1>::template xx<ITp,Args...>::assign(tar, args..., itp, std::get<N>(itp), is...);
            }

            template <class... Is>
            static inline void plus(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
            {
                SetLInds<N-1>::template xx<ITp,Args...>::plus(tar, args..., itp, std::get<N>(itp), is...);
            }
        };
    };

    template <>
    struct SetLInds<0>
    {
        template <class ITp, typename... Args>
        struct xx
        {
            template <class... Is>
            static inline void assign(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
            {
                tar.assign(args..., std::get<0>(itp), is...);
            }

            template <class... Is>
            static inline void plus(Tar& tar, const Args&... args, const ITp& itp, const std::shared_ptr<Is>&... is)
            {
                tar.plus(args..., std::get<0>(itp), is...);
            }
        };
    };

    template <class... Indices>
    struct INDS
    {
        class CallHLOpBase
        {
        private:
            size_t mDepth;
        public:
            void assign(const HighLevelOpHolder& target, const HighLevelOpHolder& source,
                        const std::shared_ptr<Indices>&... is,
                        const std::shared_ptr<DynamicIndex>& di) const = 0;

            void plus(const HighLevelOpHolder& target, const HighLevelOpHolder& source,
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
            void assign(HighLevelOpHolder& target, const HighLevelOpHolder& source,
                        const std::shared_ptr<Indices>&... is,
                        const std::shared_ptr<DynamicIndex>& di) const override final
            {
                auto ip = di->get(di->dim()-mDepth);
                auto iregn = ip->regN();
                if(iregn.type >= 0 and iregn.depth > sizeof...(LIndices)){
                    sCall[iregn.type]->assign(target, source, is..., di);
                }
                else {
                    auto mi = mkIndex(is...,di->subSpaceI(0, di->dim()-mDepth));
                    SetLInds<sizeof...(LIndices)-1>::
                        template xx<HighLevelOpHolder,ITuple,HighLevelOpHolder,decltype(mi)>::
                        assign(target, source, mi,
                               SetLInds<sizeof...(LIndices)-1>::template mkLIT<LIndices...>(di));
                }
            }

            void plus(HighLevelOpHolder& target, const HighLevelOpHolder& source,
                      const std::shared_ptr<Indices>&... is,
                      const std::shared_ptr<DynamicIndex>& di) const override final
            {
                auto mi = mkIndex(is...,di);
                SetLInds::template xx<HighLevelOpHolder,ITuple,HighLevelOpHolder,decltype(mi)>::plus
                    (target, source, mi, mLInds);
            }
        };
    };
}

#endif

#ifdef reg_ind1
#ifdef reg_ind2
#ifdef reg_ind3

reg_ind1(ClassicRange::IndexType);
reg_ind2(ClassicRange::IndexType,ClassicRange::IndexType);
reg_ind3(ClassicRange::IndexType,ClassicRange::IndexType,ClassicRange::IndexType);

#endif
#endif
#endif
