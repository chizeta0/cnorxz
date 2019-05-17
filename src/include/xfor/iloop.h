
#ifndef __ma_iloop_h__
#define __ma_iloop_h__

#include <cstdlib>
#include <memory>
#include <tuple>
#include <array>

namespace MultiArrayHelper
{
    template <size_t N>
    struct NN
    {
        template <class LTp>
        static inline constexpr size_t lsize(const LTp& ltp)
        {
            typedef typename std::tuple_element<N,LTp>::type LType;
            return LType::SIZE + NN<N-1>::lsize(ltp);
        }

        template <class LTp>
        static inline auto rootSteps(const LTp& ltp, std::intptr_t i = 0)
        {
            constexpr size_t M = std::tuple_size<LTp>::value;
            return std::get<M-N-1>(ltp).rootSteps(i).extend( NN<N-1>::rootSteps(ltp,i) );
        }

        template <class VarTp>
        static inline void zero(VarTp& vtp, const std::array<size_t,std::tuple_size<VarTp>::value>& setzero)
        {
            if(std::get<N>(setzero)){
                (*std::get<N>(vtp)) = 0.;
            }
            NN<N-1>::zero(vtp, setzero);
        }

        template <class LTp, class ExtType>
        static inline size_t exec(LTp& ltp, const std::array<size_t,std::tuple_size<LTp>::value>& umpos,
                                  size_t mpos, ExtType pos)
        {
            NN<N-1>::exec(ltp,umpos,mpos,pos);
            std::get<N>(ltp)(std::get<N>(umpos)*mpos, pos.template nn<NN<N-1>::lsize(ltp)>());
            return 0;
        }

    };

    template <>
    struct NN<0>
    {
        template <class LTp>
        static inline constexpr size_t lsize(const LTp& ltp)
        {
            typedef typename std::tuple_element<0,LTp>::type LType;
            return LType::SIZE;
        }

        template <class LTp>
        static inline auto rootSteps(const LTp& ltp, std::intptr_t i = 0)
        {
            constexpr size_t M = std::tuple_size<LTp>::value;
            return std::get<M-1>(ltp).rootSteps(i);
        }

        template <class VarTp>
        static inline void zero(VarTp& vtp, const std::array<size_t,std::tuple_size<VarTp>::value>& setzero)
        {
            if(std::get<0>(setzero)){
                (*std::get<0>(vtp)) = 0.;
            }
        }

        template <class LTp, class ExtType>
        static inline size_t exec(LTp& ltp, const std::array<size_t,std::tuple_size<LTp>::value>& umpos,
                                  size_t mpos, ExtType pos)
        {                
            std::get<0>(ltp)(std::get<0>(umpos)*mpos, pos);
            return 0;
        }

    };

    template <class OpTp, class IndTp, class VarTp, class LTp>
    class ILoop
    {
    private:
        static constexpr size_t LTpSize = std::tuple_size<LTp>::value;
        static constexpr size_t VarTpSize = std::tuple_size<VarTp>::value;
            
        OpTp mOpTp;
        IndTp mIndTp;

        mutable VarTp mVarTp;
        mutable LTp mLTp;

        std::array<size_t,LTpSize> mUmpos; // ll(mpos, pos) or ll(0, pos)
        std::array<size_t,VarTpSize> mSetzero; // set variable to zero after each cycle
            
    public:
        static constexpr size_t SIZE = NN<LTpSize-1>::lsize(std::declval<LTp>());
        static constexpr bool CONT = false;
        typedef decltype(NN<LTpSize-1>::rootSteps(mLTp)) ExtType;

        ILoop(const OpTp& opTp, const IndTp& indTp, const VarTp& varTp, const LTp& lTp,
              const std::array<size_t,LTpSize>& umpos, const std::array<size_t,VarTpSize>& setzero) :
            mOpTp(opTp), mIndTp(indTp), mVarTp(varTp), mLTp(lTp), mUmpos(umpos), mSetzero(setzero) {}
            
        inline size_t operator()(size_t mpos, ExtType pos)
        {
            NN<VarTpSize-1>::zero(mVarTp, mSetzero);
            //VCHECK(mpos);
            NN<LTpSize-1>::exec(mLTp, mUmpos, mpos, pos);
            return 0;
        }

        auto rootSteps(std::intptr_t i = 0) const
            -> ExtType
        {
            return NN<LTpSize-1>::rootSteps(mLTp,i);
        }
            
    };

}

#endif
