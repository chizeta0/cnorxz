
#ifndef __expressions_h__
#define __expressions_h__

#include "ranges/dynamic_range.h"
#include "multi_array_base.h"
#include "multi_array_operation.h"
#include "xfor/xfor.h"

#define Y() ,

namespace MultiArrayTools
{

    //class Expressions1;

    template <class EC>
    using DDMA = MultiArrayBase<double,DynamicRange<EC>>;

    template <class EC>
    using DDMMA = MutableMultiArrayBase<double,DynamicRange<EC>>;

    template <class EC, class MA>
    using oo = decltype(std::declval<MA>()(std::declval<std::shared_ptr<DynamicIndex<EC>>>()));

    template <template <class> class OpF, class... oos>
    using OO = Operation<double,OpF<double>,oos...>;

    template <class EC, template <class> class OpF, class... MAs>
    using OX = Operation<double,OpF<double>,oo<EC,MAs>...>;
    /*
    template <class EC>
    class ECInterface
    {
    public:

        EC& THIS() { return static_cast<EC&>(*this); }
        const EC& THIS() const { return static_cast<EC const&>(*this); }
        
        template <class Expr>
        auto ifor(size_t step, Expr ex) const
            -> decltype(THIS().ifor(step, ex))
        {
            return THIS().ifor(step, ex);
        }

        template <class Expr>
        auto iforh(size_t step, Expr ex) const
            -> decltype(THIS().iforh(step, ex))
        {
            return THIS().iforh(step, ex);
        }
    };
    */
    template <class EC, class Index>
    EC makeec(const std::shared_ptr<Index>& i);

#define V_IFOR_X(Expr) \
    virtual ExpressionHolder<Expr> iforx(size_t step, Expr ex) const = 0; \
    virtual ExpressionHolder<Expr> iforhx(size_t step, Expr ex) const = 0

    template <class Index>
    class E1;

    class Expressions1 //: public ECInterface<Expressions1<X>>
    {
    public:
        typedef Expressions1 EX;
        
        template <class Index>
        static auto make(const std::shared_ptr<Index>& i)
            -> decltype(makeec<E1<Index>>(i))
        {
            return makeec<E1<Index>>(i);
        }

    private:
        V_IFOR_X(OX<EX Y() plus Y() DDMA<EX> Y() DDMA<EX>>);
        V_IFOR_X(OX<EX Y() minus Y() DDMA<EX> Y() DDMA<EX>>);
        V_IFOR_X(OX<EX Y() multiplies Y() DDMA<EX> Y() DDMA<EX>>);
        V_IFOR_X(OX<EX Y() divides Y() DDMA<EX> Y() DDMA<EX>>);

    public:
        template <class Expr>
        auto ifor(size_t step, Expr ex) const
            -> decltype(iforx(step, ex))
        {
            return iforx(step, ex);
        }

        template <class Expr>
        auto iforh(size_t step, Expr ex) const
            -> decltype(iforhx(step, ex))
        {
            return iforhx(step, ex);
        }

    };

    
#define O_IFOR_X(Expr,Ind) \
    virtual ExpressionHolder<Expr> iforx(size_t step, Expr ex) const override final \
    { return ExpressionHolder<Expr>(Ind->ifor(step, ex)); } \
    virtual ExpressionHolder<Expr> iforhx(size_t step, Expr ex) const override final \
    { return ExpressionHolder<Expr>(Ind->iforh(step, ex)); }
    
    template <class Index>
    class E1 : public Expressions1
    {
    private:

        typedef Expressions1 EX;
        
        E1() = default;
        
        std::shared_ptr<Index> mI;

        O_IFOR_X(OX<EX Y() plus Y() DDMA<EX> Y() DDMA<EX>>, mI);
        O_IFOR_X(OX<EX Y() minus Y() DDMA<EX> Y() DDMA<EX>>, mI);
        O_IFOR_X(OX<EX Y() multiplies Y() DDMA<EX> Y() DDMA<EX>>, mI);
        O_IFOR_X(OX<EX Y() divides Y() DDMA<EX> Y() DDMA<EX>>, mI);

    public:
        E1(const E1& in) = default;
        E1(E1&& in) = default;
        E1& operator=(const E1& in) = default;
        E1& operator=(E1&& in) = default;

        E1(const std::shared_ptr<Index>& i) : mI(i) {} 
    };

    template <class EC, class Index>
    EC makeec(const std::shared_ptr<Index>& i)
    {
        return EC(i);
    }


} // namespace MultiArrayTools

#endif
