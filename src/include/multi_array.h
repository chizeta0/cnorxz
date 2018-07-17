// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

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
	MultiArray(const std::shared_ptr<SRanges>&... ranges, const std::vector<T>& vec);
	MultiArray(const std::shared_ptr<SRanges>&... ranges, std::vector<T>&& vec);
	MultiArray(const typename CRange::SpaceType& space);
	MultiArray(const typename CRange::SpaceType& space, std::vector<T>&& vec);
	MultiArray(MultiArray<T,AnonymousRange>& ama, SIZET<SRanges>... sizes);
	
	// Only if ALL ranges have default extensions:
	//MultiArray(const std::vector<T>& vec);
	//MultiArray(std::vector<T>&& vec);
	
	// template <class Range2, class Range3>
	// MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in);

	// implement contstructor using FunctionalMultiArray as Input !!!
	
	//template <class Range2, class Range3>
	//MultiArray& operator=(const MultiArray<MultiArray<T,Range2>,Range3> in);
	
	virtual T& operator[](const IndexType& i) override;
	virtual const T& operator[](const IndexType& i) const override;
	virtual T& at(const typename IndexType::MetaType& meta) override;
	virtual const T& at(const typename IndexType::MetaType& meta) const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	template <class... SRanges2>
	MultiArray<T,SRanges2...> format(const std::shared_ptr<SRanges2>&... nrs); // reformat array using 'nr' which in
	//                                                                 total must have the same size as mRange
	
	virtual const T* data() const override;
	virtual T* data() override;
	virtual std::vector<T>& vdata() { return mCont; }
	virtual const std::vector<T>& vdata() const { return mCont; }

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous() const override;
	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymousMove() override;
	
	auto cat() const
	    -> decltype(ArrayCatter<T>::cat(*this));
	
	operator T() const;	
	
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

namespace MultiArrayTools
{
    template <typename T>
    Scalar<T> scalar(const T& in)
    {
	NullRF nrf;
	return Scalar<T>( std::dynamic_pointer_cast<NullRange>( nrf.create() ), std::vector<T>( { in } ) );
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const typename CRange::SpaceType& space) :
	MutableMultiArrayBase<T,SRanges...>(space),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const typename CRange::SpaceType& space,
					 std::vector<T>&& vec) :
	MutableMultiArrayBase<T,SRanges...>(space),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, const std::vector<T>& vec) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, std::vector<T>&& vec) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(MultiArray<T,AnonymousRange>& ama, SIZET<SRanges>... sizes) :
	MutableMultiArrayBase<T,SRanges...>( ama.range()->template scast<SRanges...>(sizes...)->space() ),
	mCont( std::move( ama.mCont ) )
    {
	MAB::mInit = true;
    }
    
    /*
    template <typename T, class... SRanges>
    template <class Range2, class Range3>
    MultiArray<T,SRanges...>::MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in) :
	MutableMultiArrayBase<T,SRanges...>(merge(in.range(), in[ in.beginIndex() ].range()))
	// assert that Range2 has always same extension
    {
	MAB::mInit = true;
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
    }
    */
    /*
    template <typename T, class... SRanges>
    template <class Range2, class Range3>
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator=(const MultiArray<MultiArray<T,Range2>,Range3> in)
    {
	MAB::mRange.reset(new Range(merge(in.range(), in[ in.beginIndex() ].range())));
	// assert that Range2 has always same extension
	mCont.clear();
	for(auto i = in.beginIndex(); i != in.endIndex(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
	return *this;
	} */   

    template <typename T, class... SRanges>
    T& MultiArray<T,SRanges...>::operator[](const IndexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class... SRanges>
    const T& MultiArray<T,SRanges...>::operator[](const IndexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class... SRanges>
    T& MultiArray<T,SRanges...>::at(const typename IndexType::MetaType& meta)
    {
	return mCont[ MAB::beginIndex().at(meta).pos() ];
    }

    template <typename T, class... SRanges>
    const T& MultiArray<T,SRanges...>::at(const typename IndexType::MetaType& meta) const
    {
	return mCont[ MAB::beginIndex().at(meta).pos() ];
    }
	
    template <typename T, class... SRanges>
    bool MultiArray<T,SRanges...>::isConst() const
    {
	return false;
    }
    
    template <typename T, class... SRanges>
    bool MultiArray<T,SRanges...>::isSlice() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    MultiArray<T,SRanges2...> MultiArray<T,SRanges...>::format(const std::shared_ptr<SRanges2>&... nrs)
    {
	return MultiArray<T,SRanges2...>( nrs... , std::move(mCont) );
    }
    
    template <typename T, class... SRanges>
    const T* MultiArray<T,SRanges...>::data() const
    {
	return mCont.data();
    }

    template <typename T, class... SRanges>
    T* MultiArray<T,SRanges...>::data()
    {
	return mCont.data();
    }

    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > MultiArray<T,SRanges...>::anonymous() const
    {
	AnonymousRangeFactory arf(MAB::mRange->space());
	return std::make_shared<MultiArray<T,AnonymousRange> >
	    ( std::dynamic_pointer_cast<AnonymousRange>( arf.create() ),
	      mCont );
    }	

    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > MultiArray<T,SRanges...>::anonymousMove()
    {
	AnonymousRangeFactory arf(MAB::mRange->space());
	MAB::mInit = false;
	return std::make_shared<MultiArray<T,AnonymousRange> >
	    ( std::dynamic_pointer_cast<AnonymousRange>( arf.create() ),
	      std::move(mCont) );
    }	

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::operator T() const
    {
	static_assert( sizeof...(SRanges) == 0, "try to cast non-scalar type into scalar" );
	return mCont[0];
    }

    template <typename T, class... SRanges>
    auto MultiArray<T,SRanges...>::cat() const
	-> decltype(ArrayCatter<T>::cat(*this))
    {
	return ArrayCatter<T>::cat(*this);
    }
}

#endif
