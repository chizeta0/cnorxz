
#include "expressions.h"

namespace MultiArrayTools
{

    template <class Expr>
    inline ExpressionHolder<Expr> Expressions1::ifor(size_t step,
						     ExpressionHolder<Expr> ex) const
    {
	return iforx(step, ex);
    }
    
    template <class Expr>
    inline ExpressionHolder<Expr> Expressions1::iforh(size_t step,
						      ExpressionHolder<Expr> ex) const
    {
	return iforhx(step, ex);
    }

    template <class Expr>
    inline ExpressionHolder<Expr> Expressions1::ifori(size_t step, Expr ex) const
    {
	return iforxi(step, ex);
    }
    
    template <class Expr>
    inline ExpressionHolder<Expr> Expressions1::iforhi(size_t step, Expr ex) const
    {
	return iforhxi(step, ex);
    }

    template <class EC, class Index>
    std::shared_ptr<EC> makeec(const std::shared_ptr<Index>& i)
    {
        return std::make_shared<EC>(i);
    }

} // namespace MultiArrayTools
