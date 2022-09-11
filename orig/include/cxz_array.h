// -*- C++ -*-

#ifndef __cxz_array_h__
#define __cxz_array_h__

#include <algorithm>

#include "cxz_array_base.h"
#include "ranges/anonymous_range.h"

namespace CNORXZ
{
    template <typename T>
    struct ArrayCatter;

    
    template <typename T>
    struct ArrayCatter
    {
	template <class... Ranges>
	static auto cat(const Array<T,Ranges...>& ma)
	    -> Array<T,Ranges...>
	{
	    return ma;
	}
    };
    
    template <typename T, class... SRanges>
    class Array : public MutableArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef ArrayBase<T,SRanges...> MAB;
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IndexType;
	
        using ArrayBase<T,SRanges...>::operator[];
	using MutableArrayBase<T,SRanges...>::operator[];

	DEFAULT_MEMBERS(Array);
	Array(const std::shared_ptr<SRanges>&... ranges);
	Array(const std::shared_ptr<SRanges>&... ranges, const T& val);
	Array(const std::shared_ptr<SRanges>&... ranges, const vector<T>& vec);
	Array(const std::shared_ptr<SRanges>&... ranges, vector<T>&& vec);

        template <class... Ranges>
	Array(const std::shared_ptr<SRanges>&... ranges, Array<T,Ranges...>&& in); // same effect as format

        Array(const typename CRange::Space& space);
	Array(const typename CRange::Space& space, const vector<T>& vec);
	Array(Array<T,AnonymousRange>&& ama, SIZET<SRanges>... sizes);
	
	// Only if ALL ranges have default extensions:
	//Array(const vector<T>& vec);
	//Array(vector<T>&& vec);
	
	// template <class Range2, class Range3>
	// Array(const Array<Array<T,Range2>,Range3> in);

	// implement contstructor using FunctionalArray as Input !!!
	
	//template <class Range2, class Range3>
	//Array& operator=(const Array<Array<T,Range2>,Range3> in);
	
	virtual T& operator[](const IndexType& i) final;
	virtual const T& operator[](const IndexType& i) const final;
	virtual T& at(const typename IndexType::MetaType& meta) override;
	virtual const T& at(const typename IndexType::MetaType& meta) const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	template <class... SRanges2>
	Array<T,SRanges2...> format(const std::shared_ptr<SRanges2>&... nrs); // reformat array using 'nr' which in
	//                                                                 total must have the same size as mRange

	template <class... SRanges2>
	Array<T,SRanges2...> format(const std::tuple<std::shared_ptr<SRanges2>...>& nrs);

        template <class... SRanges2>
        Slice<T,SRanges2...> slformat(const std::shared_ptr<SRanges2>&... nrs);

        template <class... SRanges2>
        ConstSlice<T,SRanges2...> slformat(const std::shared_ptr<SRanges2>&... nrs) const;

	virtual const T* data() const override;
	virtual T* data() override;
	virtual vector<T>& vdata() { return mCont; }
	virtual const vector<T>& vdata() const { return mCont; }
        vector<T>&& vmove() { MAB::mInit = false; return std::move(mCont); }
        
	virtual std::shared_ptr<ArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;
	//virtual std::shared_ptr<ArrayBase<T,AnonymousRange> > anonymousMove() override;
	
	auto cat() const
	    -> decltype(ArrayCatter<T>::cat(*this));
	
	operator T() const;	

	Array& operator=(const T& in);
	
	Array& operator+=(const Array& in);
	Array& operator-=(const Array& in);
	Array& operator*=(const T& in);
	Array& operator/=(const T& in);
	
	template <typename U, class... SRanges2>
	friend class Array;
	
    private:
	
	vector<T> mCont;
    };

    template <typename T>
    using Scalar = Array<T,NullRange>;

    template <typename T>
    Scalar<T> scalar(const T& in);

    template <typename T, class... ERanges>
    struct ArrayCatter<Array<T,ERanges...> >
    {
	template <class... Ranges>
	static auto cat(const Array<Array<T,ERanges...>,Ranges...>& ma)
	    -> Array<T,Ranges...,ERanges...>
	{
	    auto sma = *ma.begin();
	    const size_t smas = sma.size();
	    const size_t mas = ma.size();
	    auto cr = ma.range()->cat(sma.range());
	    vector<T> ov;
	    ov.reserve(mas * smas);

	    for(auto& x: ma){
		assert(x.size() == smas);
		ov.insert(ov.end(), x.vdata().begin(), x.vdata().end());
	    }
	    return Array<T,Ranges...,ERanges...>(cr->space(), std::move(ov));
	}
    };

    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */


#endif
