
#ifndef __xfor_h__
#define __xfor_h__

#include <cstdlib>
#include <memory>
#include <tuple>

namespace MultiArrayHelper
{
    namespace {
	template <class Op>
	using to_size_t = size_t;
    }
    
    template <class IndexClass, class Expr, class... Ops>
    class For
    {
    public:

	For(For&& in) = default;
	For& operator=(For&& in) = default;
	
	template <typename... Args>
	For(const std::shared_ptr<IndexClass>& indPtr,
	    std::tuple<to_size_t<Ops>...>&& ext, const Args&... args);

	For(const std::shared_ptr<IndexClass>& indPtr,
	    Expr&& expr, std::tuple<to_size_t<Ops>...>&& ext);
	
	inline void operator()(size_t mlast, const std::tuple<to_size_t<Ops>...>& last) const;
	
    private:
	For() = default;
	
	mutable std::shared_ptr<IndexClass> mIndPtr;
	const Expr mExpr;
	const std::tuple<to_size_t<Ops>...> mExt;
    };

    
    
} // namespace MultiArrayHelper

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayHelper
{

    template <class IndexClass, class Expr, class... Ops>
    template <typename... Args>
    For<IndexClass,Expr,Ops...>::For(const std::shared_ptr<IndexClass>& indPtr,
				     std::tuple<to_size_t<Ops>...>&& ext,
				     const Args&... args) :
	mIndPtr(indPtr), mExpr(args...), mExt(ext) {}

    template <class IndexClass, class Expr, class... Ops>
    For<IndexClass,Expr,Ops...>::For(const std::shared_ptr<IndexClass>& indPtr,
				     Expr&& expr, std::tuple<to_size_t<Ops>...>&& ext) :
	mIndPtr(indPtr), mExpr(expr), mExt(ext) {}
    
    template <class IndexClass, class Expr, class... Ops>
    inline void For<IndexClass,Expr,Ops...>::operator()(size_t mlast,
							const std::tuple<to_size_t<Ops>...>& last) const
    {
	static const size_t opNum = sizeof...(Ops);
	auto& ind = *mIndPtr;
	const size_t max = ind.max();
	for(ind = 0; ind.pos() != max; ++ind){
	    const size_t mnpos = mlast * max + ind.pos();
	    const std::tuple<to_size_t<Ops>...> npos( /* !!! fancy code !!! */ ); 
	    mExpr(mnpos, npos);
	}
    }
    
} // namespace MultiArrayHelper

#endif
