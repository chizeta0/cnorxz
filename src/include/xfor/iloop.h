
#ifndef __ma_iloop_h__
#define __ma_iloop_h__

#include <cstdlib>
#include <memory>
#include <tuple>
#include <array>
#include <omp.h>

namespace MultiArrayHelper
{
    template <size_t N>
    struct NN
    {
        template <class LTp>
        static inline constexpr size_t LSIZE()
        {
            typedef typename std::tuple_element<N,LTp>::type LType;
            return LType::SIZE + NN<N-1>::template LSIZE<LTp>();
        }
        
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
        static inline constexpr size_t LSIZE()
        {
            typedef typename std::tuple_element<0,LTp>::type LType;
            return LType::SIZE;
        }

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
        //static constexpr size_t SIZE = NN<LTpSize-1>::lsize(std::declval<LTp>());
        static constexpr size_t SIZE = NN<LTpSize-1>::template LSIZE<LTp>();
        static constexpr bool CONT = false;
        typedef decltype(NN<LTpSize-1>::rootSteps(mLTp)) ExtType;

        ILoop() { assert(omp_get_thread_num() == 0); }
        
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

        VarTp& var() const
        {
            return mVarTp;
        }
    };

    template <size_t N>
    struct CreateLoops
    {
        template <class CLTp, class VarTp>
        static inline auto apply(CLTp& cltp, VarTp& varTp)
        {
            return std::tuple_cat( CreateLoops<N-1>::apply(cltp,varTp) , std::make_tuple( std::get<N>(cltp)(varTp) ) );
        }
    };

    template <>
    struct CreateLoops <0>
    {
        template <class CLTp, class VarTp>
        static inline auto apply(CLTp& cltp, VarTp& varTp)
        {
            return std::make_tuple( std::get<0>(cltp)(varTp) );
        }
    };

    template <size_t N>
    struct ThrCpy
    {
        template <class VarTp>
        static inline auto apply(size_t thread1, size_t thread2, const VarTp& varTp)
        {
            auto& var = std::get<N>(varTp);
            typedef typename std::remove_reference<decltype(*var)>::type Type;
            return std::tuple_cat( ThrCpy<N-1>::apply(thread1, thread2, varTp),
                                   std::make_tuple( (thread1 == thread2) ? var : std::make_shared<Type>(*var) ) );
        }
    };

    template <>
    struct ThrCpy <0>
    {
        template <class VarTp>
        static inline auto apply(size_t thread1, size_t thread2, const VarTp& varTp)
        {
            auto& var = std::get<0>(varTp);
            typedef typename std::remove_reference<decltype(*var)>::type Type;
            return std::make_tuple( (thread1 == thread2) ? var : std::make_shared<Type>(*var) );
        }
    };

    template <class... Vars>
    auto thrCpy(size_t thread1, size_t thread2, const std::tuple<std::shared_ptr<Vars>...>& vars)
    {
        return ThrCpy<sizeof...(Vars)>::apply(thread1, thread2, vars);
    }

    template <class... CLs>
    auto createLoops(std::tuple<CLs...>& cls)
    {
        return CreateLoops<sizeof...(CLs)>::apply(cls);
    }

    // CF = creation function
    // if instance copied to different thread, the "copy" will be newly created from this function
    // -> ensures that there is NO SHARED WORKSPACE
    template <class CF>
    class PILoop
    {
    private:
        size_t mThreadId = 0;
        CF mCF;
        
        typedef decltype(mCF()) LType;

        LType mL;
        
        static constexpr size_t LTpSize = LType::LTpSize;
        static constexpr size_t VarTpSize = LType::VarTpSize;
            
    public:
        static constexpr size_t SIZE = LType::SIZE;
        static constexpr bool CONT = LType::CONT;
        typedef typename LType::ExtType ExtType;

        PILoop() : mThreadId(omp_get_thread_num()) {}

        PILoop(const PILoop& in) : mThreadId(omp_get_thread_num()), mCF(in.mCF), mL((mThreadId == in.mThreadId) ? in.mL : mCF()) {}
        PILoop& operator=(const PILoop& in)
        {
            mThreadId = omp_get_thread_num();
            mCF = in.mCF;
            mL = (mThreadId == in.mThreadId) ? in.mL : mCF();
            return *this;
        }

        PILoop(PILoop&& in) : mThreadId(omp_get_thread_num()), mCF(std::move(in.mCF)), mL((mThreadId == in.mThreadId) ? std::move(in.mL) : std::move(mCF())) {}
        PILoop& operator=(PILoop&& in)
        {
            mThreadId = omp_get_thread_num();
            mCF = std::move(in.mCF);
            mL = (mThreadId == in.mThreadId) ? std::move(in.mL) : std::move(mCF());
            return *this;
        }

        PILoop(const CF& cf) : mThreadId(omp_get_thread_num()), mCF(cf), mL(mCF()) {}

        inline size_t operator()(size_t mpos, ExtType pos)
        {
            return mL(mpos, pos);
        }

        auto rootSteps(std::intptr_t i = 0) const
            -> ExtType
        {
            return mL.rootSteps(i);
        }

        auto var() const
        {
            return mL.var();
        }
    };

}

#endif
