
#include "multi_array.h"

namespace MultiArrayTools
{
    template <typename T>
    Scalar<T> scalar(const T& in)
    {
	NullRF nrf;
	return Scalar<T>( std::dynamic_pointer_cast<NullRange>( nrf.create() ), vector<T>( { in } ) );
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const typename CRange::Space& space) :
	MutableMultiArrayBase<T,SRanges...>(space),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const typename CRange::Space& space,
					 vector<T>&& vec) :
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
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, const T& val) : 
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(MAB::mRange->size(), val)
    {
	MAB::mInit = true;
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, const vector<T>& vec) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(const std::shared_ptr<SRanges>&... ranges, vector<T>&& vec) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::MultiArray(MultiArray<T,AnonymousRange>&& ama, SIZET<SRanges>... sizes) :
	MutableMultiArrayBase<T,SRanges...>
	( ama.range()->template get<0>().template scast<SRanges...>(sizes...)->space() ),
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
	MAB::mInit = false;
	return MultiArray<T,SRanges2...>( nrs... , std::move(mCont) );
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    MultiArray<T,SRanges2...> MultiArray<T,SRanges...>::format(const std::tuple<std::shared_ptr<SRanges2>...>& nrs)
    {
	MAB::mInit = false;
	return MultiArray<T,SRanges2...>( nrs , std::move(mCont) );
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
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > MultiArray<T,SRanges...>::anonymous(bool slice) const
    {
	AnonymousRangeFactory arf(MAB::mRange->space());
	if(slice){
	    return std::make_shared<ConstSlice<T,AnonymousRange> >
		( std::dynamic_pointer_cast<AnonymousRange>( arf.create() ),
		  data() );
	}
	else {
	    return std::make_shared<MultiArray<T,AnonymousRange> >
		( std::dynamic_pointer_cast<AnonymousRange>( arf.create() ),
		  mCont );
	}
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
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator=(const T& in)
    {
	for(auto& x: mCont){
	    x = in;
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator+=(const MultiArray& in)
    {
	if(not MAB::mInit){ // not initialized by default constructor !!
	    (*this) = in;
	}
	else {
	    assert( PackNum<sizeof...(SRanges)-1>::checkIfSameInstance( MAB::mRange->space(), in.mRange->space() ) );
            for(size_t i = 0; i != mCont.size(); ++i){
                mCont[i] += in.mCont[i];
            }
	    //std::transform(mCont.begin(), mCont.end(), in.mCont.begin(), mCont.begin(), std::plus<T>());
	}
	return *this;
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator-=(const MultiArray& in)
    {
	if(not MAB::mInit){ // not initialized by default constructor !!
	    (*this) = in;
	}
	else {
	    assert( PackNum<sizeof...(SRanges)-1>::checkIfSameInstance( MAB::mRange->space(), in.mRange->space() ) );
            for(size_t i = 0; i != mCont.size(); ++i){
                mCont[i] -= in.mCont[i];
            }
//std::transform(mCont.begin(), mCont.end(), in.mCont.begin(), mCont.begin(), std::minus<T>());
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator*=(const T& in)
    {
	for(auto& x: mCont){
	    x *= in;
	}
	return *this;
    }

    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>& MultiArray<T,SRanges...>::operator/=(const T& in)
    {
	for(auto& x: mCont){
	    x /= in;
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    MultiArray<T,SRanges...>::operator T() const
    {
	//static_assert( sizeof...(SRanges) == 1, "try to cast non-scalar type into scalar" );
	// TODO: check that SIZE is statically = 1 !!!
	return mCont[0];
    }

    template <typename T, class... SRanges>
    auto MultiArray<T,SRanges...>::cat() const
	-> decltype(ArrayCatter<T>::cat(*this))
    {
	return ArrayCatter<T>::cat(*this);
    }
}

