// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include "multi_array_base.h"

namespace MultiArrayTools
{

    
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

	operator T() const;	
	
	template <typename U, class... SRanges2>
	friend class MultiArray;
	
    private:
	std::vector<T> mCont;
    };

    template <typename T>
    using Scalar = MultiArray<T>;
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    
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
    MultiArray<T,SRanges...>::operator T() const
    {
	static_assert( sizeof...(SRanges) == 0, "try to cast non-scalar type into scalar" );
	return mCont[0];
    }
}

#endif
