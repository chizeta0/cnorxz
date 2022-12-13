
#ifndef __cxz_index_utils_cc_h__
#define __cxz_index_utils_cc_h__

#include "index_utils.h"

namespace CNORZX
{
    inline decltype(auto) getPack(const XIndexPtr& i)
    {
	typedef Vector<XIndexPtr> OutT;
	OutT p = i.pack();
	while(p.size() == 1u){
	    p = p[0]->pack();
	}
	return p;
    }

    // -> base
    template <typename... T>
    Vector<T> mkVector(const T&... t) { return Vector<T>({t...}); }
    
    template <class F>
    inline decltype(auto) indexZip(const XIndexPtr& a, const XIndexPtr& b, F&& f)
    {
	const auto ap = getPack(a);
	const auto bp = getPack(b);
	if(ap.size() == 1u or bp.size() == 1u){
	    return mkVector( f(a, b) );
	}
	else {
	    return indexPackZip(ap, bp, std::forward<F>(f));
	}
    }
    
    template <class F>
    inline decltype(auto) indexPackZip(const Vector<XIndexPtr>& a, const Vector<XIndexPtr>& b, F&& f)
    {
	if(a.size() > b.size()) {
	    Vector<XIndexPtr> bn;
	    for(const auto& x: b){
		auto p = getPack(x);
		if(p.size() == 0){
		    bn.push_back(x);
		}
		else {
		    bn.insert(bn.end(), p.begin(), p.end());
		}
	    }
	    return indexPackZip(a, bn, std::forward<F>(f));
	}
	else if(a.size() < b.size()) {
	    Vector<XIndexPtr> an;
	    for(const auto& x: a){
		auto p = getPack(x);
		if(p.size() == 0){
		    an.push_back(x);
		}
		else {
		    an.insert(bn.end(), p.begin(), p.end());
		}
	    }
	    return indexPackZip(an, b, std::forward<F>(f));
	}
	else {
	    typedef decltype(indexZip(a[0], b[0], std::forward<T>(f))) OutT;
	    OutT o(a.size());
	    std::transform(o.begin(), o.end(), a.begin(), b.begin(),
			   [](const auto& ax, const auto& bx) { F fc = f; return indexZip(ax, bx, std::move(fc)); });
	    return o;
	}
    }

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
