
#ifndef __expressions_h__
#define __expressions_h__

#include "ranges/dynamic_range.h"
#include "multi_array_base.h"
#include "multi_array_operation.h"
#include "xfor/xfor.h"

#define XCOMMAX() ,

namespace MultiArrayTools
{

    //class Expressions1;

    template <class EC>
    using DDMA = MultiArrayBase<double,DynamicRange<EC>>;

    template <class EC>
    using DDMMA = MutableMultiArrayBase<double,DynamicRange<EC>>;

    template <class EC, class MA>
    using oo = decltype(std::declval<MA>()(std::declval<const std::shared_ptr<DynamicIndex<EC>>&>()));

    template <template <class> class OpF, class... oos>
    using OO = Operation<double,OpF<double>,oos...>;

    template <class EC, template <class> class OpF, class... MAs>
    using OX = Operation<double,OpF<double>,oo<EC,MAs>...>;

    template <class EC, class Second>
    using AEXT = AssignmentExpr<double,Second>;

    template <class EC, class Second>
    using AEXT_P = AssignmentExpr<double,Second>;

    template <class EC, template <class> class OpF, class... MAs>
    using AEX = AEXT<EC,OX<EC,OpF,MAs...>>;
    
    template <class EC>
    using AEX_M = AEXT<EC,oo<EC,DDMMA<EC>>>;

    template <class EC>
    using AEX_C = AEXT<EC,oo<EC,DDMA<EC>>>;

    template <class EC, template <class> class OpF>
    using AEX_B_MM = AEX<EC,OpF,DDMMA<EC>,DDMMA<EC>>;

    template <class EC, template <class> class OpF>
    using AEX_B_MC = AEX<EC,OpF,DDMMA<EC>,DDMA<EC>>;

    template <class EC, template <class> class OpF>
    using AEX_B_CM = AEX<EC,OpF,DDMA<EC>,DDMMA<EC>>;

    template <class EC, template <class> class OpF>
    using AEX_B_CC = AEX<EC,OpF,DDMA<EC>,DDMA<EC>>;

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
    std::shared_ptr<EC> makeec(const std::shared_ptr<Index>& i);

#define V_IFOR_X(Expr) \
    virtual MultiArrayTools::ExpressionHolder<Expr> iforx(size_t step, MultiArrayTools::ExpressionHolder<Expr> ex) const = 0; \
    virtual MultiArrayTools::ExpressionHolder<Expr> iforhx(size_t step, MultiArrayTools::ExpressionHolder<Expr> ex) const = 0; \
    virtual MultiArrayTools::ExpressionHolder<Expr> iforxi(size_t step, Expr ex) const = 0; \
    virtual MultiArrayTools::ExpressionHolder<Expr> iforhxi(size_t step, Expr ex) const = 0

#define V_IFOR_A(EC,OpF) \
    V_IFOR_X(AEX_B_MM<EC XCOMMAX() OpF>); \
    V_IFOR_X(AEX_B_MC<EC XCOMMAX() OpF>); \
    V_IFOR_X(AEX_B_CM<EC XCOMMAX() OpF>); \
    V_IFOR_X(AEX_B_CC<EC XCOMMAX() OpF>)

#define V_IFOR_A_1(EC) \
    V_IFOR_X(AEX_M<EC>); \
    V_IFOR_X(AEX_C<EC>)

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
        V_IFOR_A(EX,MultiArrayTools::plus);
        V_IFOR_A(EX,MultiArrayTools::minus);
        V_IFOR_A(EX,MultiArrayTools::multiplies);
        V_IFOR_A(EX,MultiArrayTools::divides);
	V_IFOR_A_1(EX);
	
    public:
        template <class Expr>
        inline MultiArrayTools::ExpressionHolder<Expr> ifor(size_t step, MultiArrayTools::ExpressionHolder<Expr> ex) const;

        template <class Expr>
        inline MultiArrayTools::ExpressionHolder<Expr> iforh(size_t step, MultiArrayTools::ExpressionHolder<Expr> ex) const;

	template <class Expr>
        inline MultiArrayTools::ExpressionHolder<Expr> ifori(size_t step, Expr ex) const;

        template <class Expr>
        inline MultiArrayTools::ExpressionHolder<Expr> iforhi(size_t step, Expr ex) const;

    };

    
#define D_IFOR_X(Expr,Ind) \
    MultiArrayTools::ExpressionHolder<Expr> iforx(size_t step, MultiArrayTools::ExpressionHolder<Expr> ex) const \
    { return MultiArrayTools::ExpressionHolder<Expr>(Ind->ifor(step, ex)); } \
    MultiArrayTools::ExpressionHolder<Expr> iforhx(size_t step, MultiArrayTools::ExpressionHolder<Expr> ex) const \
    { return MultiArrayTools::ExpressionHolder<Expr>(Ind->iforh(step, ex)); } \
    MultiArrayTools::ExpressionHolder<Expr> iforxi(size_t step, Expr ex) const \
    { return MultiArrayTools::ExpressionHolder<Expr>(Ind->ifor(step, ex)); } \
    MultiArrayTools::ExpressionHolder<Expr> iforhxi(size_t step, Expr ex) const \
    { return MultiArrayTools::ExpressionHolder<Expr>(Ind->iforh(step, ex)); }

#define D_IFOR_A(EC,OpF,Ind) \
    D_IFOR_X(AEX_B_MM<EC XCOMMAX() OpF>,Ind); \
    D_IFOR_X(AEX_B_MC<EC XCOMMAX() OpF>,Ind); \
    D_IFOR_X(AEX_B_CM<EC XCOMMAX() OpF>,Ind); \
    D_IFOR_X(AEX_B_CC<EC XCOMMAX() OpF>,Ind)

#define D_IFOR_A_1(EC,Ind)				\
    D_IFOR_X(AEX_M<EC>,Ind);					\
    D_IFOR_X(AEX_C<EC>,Ind)

    template <class Index>
    class E1 : public Expressions1
    {
    private:

        typedef Expressions1 EX;
        
        E1() = default;
        
        std::shared_ptr<Index> mI;

        D_IFOR_A(EX,MultiArrayTools::plus,mI);
        D_IFOR_A(EX,MultiArrayTools::minus,mI);
        D_IFOR_A(EX,MultiArrayTools::multiplies,mI);
        D_IFOR_A(EX,MultiArrayTools::divides,mI);
	D_IFOR_A_1(EX,mI);
	
    public:
        E1(const E1& in) = default;
        E1(E1&& in) = default;
        E1& operator=(const E1& in) = default;
        E1& operator=(E1&& in) = default;

        E1(const std::shared_ptr<Index>& i) : mI(i) {} 
    };


} // namespace MultiArrayTools

#endif
