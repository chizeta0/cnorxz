
#ifndef __xfor_h__
#define __xfor_h__

#include <cstdlib>
#include <memory>

namespace MultiArrayHelper
{

    template <class IndexClass, class Expr>
    class For
    {
    public:

	For(For&& in) = default;
	For& operator=(For&& in) = default;
	
	template <typename... Args>
	For(const std::shared_ptr<IndexClass>& indPtr, const Args&... args);

	For(const std::shared_ptr<IndexClass>& indPtr, Expr&& expr);
	
	inline void operator()(size_t start = 0);
	
    private:
	For() = default;
	
	std::shared_ptr<IndexClass> mIndPtr;
	Expr mExpr;
    };

} // namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayHelper
{

    template <class IndexClass, class Expr>
    template <typename... Args>
    For<IndexClass,Expr>::For(const std::shared_ptr<IndexClass>& indPtr,
			      const Args&... args) : mIndPtr(indPtr), mExpr(args...) {}

    template <class IndexClass, class Expr>
    For<IndexClass,Expr>::For(const std::shared_ptr<IndexClass>& indPtr,
			      Expr&& expr) : mIndPtr(indPtr), mExpr(expr) {}
    
    template <class IndexClass, class Expr>
    inline void For<IndexClass,Expr>::operator()(size_t start)
    {
	auto& ind = *mIndPtr;
	const size_t max = ind.max();
	for(ind = 0; ind.pos() != max; ++ind){
	    mExpr(start * max + ind.pos()); // CHECK!!
	}
    }
    
} // namespace MultiArrayHelper

#endif
