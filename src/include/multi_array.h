// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <algorithm>

#include "multi_array_base.h"
#include "ranges/anonymous_range.h"

namespace MultiArrayTools
{
    template <typename T>
    struct ArrayCatter;

    
    template <typename T>
    struct ArrayCatter
    {
	template <class... Ranges>
	static auto cat(const MultiArray<T,Ranges...>& ma)
	    -> MultiArray<T,Ranges...>
	{
	    return ma;
	}
    };

    
    template <typename T, class... SRanges>
    class MultiArray : public MutableMultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;
	
	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const std::shared_ptr<SRanges>&... ranges);
	MultiArray(const std::shared_ptr<SRanges>&... ranges, const T& val);
	MultiArray(const std::shared_ptr<SRanges>&... ranges, const std::vector<T>& vec);
	MultiArray(const std::shared_ptr<SRanges>&... ranges, std::vector<T>&& vec);
	MultiArray(const typename CRange::Space& space);
	MultiArray(const typename CRange::Space& space, std::vector<T>&& vec);
	MultiArray(MultiArray<T,AnonymousRange>&& ama, SIZET<SRanges>... sizes);
	
	// Only if ALL ranges have default extensions:
	//MultiArray(const std::vector<T>& vec);
	//MultiArray(std::vector<T>&& vec);
	
	// template <class Range2, class Range3>
	// MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in);

	// implement contstructor using FunctionalMultiArray as Input !!!
	
	//template <class Range2, class Range3>
	//MultiArray& operator=(const MultiArray<MultiArray<T,Range2>,Range3> in);
	
	virtual T& operator[](const IndexType& i) final;
	virtual const T& operator[](const IndexType& i) const final;
	virtual T& at(const typename IndexType::MetaType& meta) override;
	virtual const T& at(const typename IndexType::MetaType& meta) const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	template <class... SRanges2>
	MultiArray<T,SRanges2...> format(const std::shared_ptr<SRanges2>&... nrs); // reformat array using 'nr' which in
	//                                                                 total must have the same size as mRange

	template <class... SRanges2>
	MultiArray<T,SRanges2...> format(const std::tuple<std::shared_ptr<SRanges2>...>& nrs);
	
	virtual const T* data() const override;
	virtual T* data() override;
	virtual std::vector<T>& vdata() { return mCont; }
	virtual const std::vector<T>& vdata() const { return mCont; }

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;
	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymousMove() override;
	
	auto cat() const
	    -> decltype(ArrayCatter<T>::cat(*this));
	
	operator T() const;	

	MultiArray& operator=(const T& in);
	
	MultiArray& operator+=(const MultiArray& in);
	MultiArray& operator-=(const MultiArray& in);
	MultiArray& operator*=(const T& in);
	MultiArray& operator/=(const T& in);
	
	template <typename U, class... SRanges2>
	friend class MultiArray;
	
    private:
	
	std::vector<T> mCont;
    };

    template <typename T>
    using Scalar = MultiArray<T,NullRange>;

    template <typename T>
    Scalar<T> scalar(const T& in);

    template <typename T, class... ERanges>
    struct ArrayCatter<MultiArray<T,ERanges...> >
    {
	template <class... Ranges>
	static auto cat(const MultiArray<MultiArray<T,ERanges...>,Ranges...>& ma)
	    -> MultiArray<T,Ranges...,ERanges...>
	{
	    auto sma = *ma.begin();
	    const size_t smas = sma.size();
	    const size_t mas = ma.size();
	    auto cr = ma.range()->cat(sma.range());
	    std::vector<T> ov;
	    ov.reserve(mas * smas);

	    for(auto& x: ma){
		assert(x.size() == smas);
		ov.insert(ov.end(), x.vdata().begin(), x.vdata().end());
	    }
	    return MultiArray<T,Ranges...,ERanges...>(cr->space(), std::move(ov));
	}
    };

    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */


#endif
