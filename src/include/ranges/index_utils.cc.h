
#ifndef __cxz_index_utils_cc_h__
#define __cxz_index_utils_cc_h__

#include "index_utils.h"

namespace CNORZX
{
    template <class Index>
    constexpr decltype(auto) getIndexDepth(const Index& ind)
    {
	static_assert(is_index<Index>::value, "got non-index type");
	if constexpr(has_sub<Index>::value){
	    if constexpr(has_static_sub<Index>::value){
		constexpr SizeT D = index_dim<Index>::value;
		return iter<0,D>
		    ( [&](auto i) { return getIndexDepth(*std::get<i>(ind.pack())); },
		      [](auto... e) {
			  if constexpr( is_integral_constant<decltype(e)>::value and ... ){
			      return std::integral_constant<SizeT,std::max({e...})> {};
			  }
			  else {
			      return std::max({e...});
			  }
		      } );
	    }
	    else {
		SizeT o = 0;
		if(ind.dim() == 1){
		    auto p = ind.pack();
		    if(p.size() == 0){
			o = 1;
		    }
		    else {
			o = getIndexDepth(*p[0]);
		    }
		}
		else {
		    for(SizeT i = 0; i != ind.dim(); ++i){
			const SizeT e = getIndexDepth(*ind.pack()[i]);
			if(e > o){
			    o = e;
			}
		    }
		}
		return o;
	    }
	}
	else {
	    return std::integral_constant<SizeT,1> {};
	}
    }

    template <class Index, typename IntT>
    constexpr decltype(auto) getDimension(const Index& ind, IntT depth)
    {
	static_assert(is_index<Index>::value, "got non-index type");
	if constexpr(std::is_same<std::integral_constant<SizeT,0>,IntT>::value) {
	    return std::integral_constant<SizeT,1> {}
	}
	else {
	    auto iminus = [](auto i) {
		if constexpr( is_integral_constant<decltype(i)>::value ) {
		    return std::integral_constant<SizeT,i-1> {}
		}
		else {
		    return i-1;
		}
	    };
	    if constexpr(has_sub<Index>::value){
		if constexpr(has_static_sub<Index>::value and is_integral_constant<IntT>::value){
		    constexpr SizeT D = index_dim<Index>::value;
		    return iter<0,D>
			( [&](auto i) { return getDimension(*std::get<i>(ind.pack()),
							    iminus(depth)); },
			    [](auto... e) {
				if constexpr( is_integral_constant<decltype(e)>::value and ... ){
				    return std::integral_constant<SizeT,(e + ...)> {};
				}
				else {
				    return (e + ...);
				}
			    } );
		}
		else {
		    SizeT o = 0;
		    if(depth == 0u){
			o = 1;
		    }
		    else if(ind.dim() == 1){
			auto p = ind.pack();
			if(p.size() == 0){
			    o = 1;
			}
			else {
			    o = getDimension(*p[0], iminus(depth));
			}
		    }
		    else {
			for(SizeT i = 0; i != ind.dim(); ++i){
			    const SizeT e = getDimension(*ind.pack()[i], iminus(depth));
			    if(e > o){
				o = e;
			    }
			}
		    }
		    return o;
		}
	    }
	    else {
		return std::integral_constant<SizeT,1> {};
	    }
	}
    }
}

#endif
