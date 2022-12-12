
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

}

#endif
