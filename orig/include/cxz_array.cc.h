
#include "cxz_array.h"
#include "statics/static_for.h"

namespace CNORXZ
{
    template <typename T>
    Scalar<T> scalar(const T& in)
    {
	NullRF nrf;
	return Scalar<T>( std::dynamic_pointer_cast<NullRange>( nrf.create() ), vector<T>( { in } ) );
    }
    
    /*******************
     *  Array     *	     
     *******************/

    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(const typename CRange::Space& space) :
	MutableArrayBase<T,SRanges...>(space),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(const typename CRange::Space& space,
					 const vector<T>& vec) :
	MutableArrayBase<T,SRanges...>(space),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    
    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(const std::shared_ptr<SRanges>&... ranges) :
	MutableArrayBase<T,SRanges...>(ranges...),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }
    
    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(const std::shared_ptr<SRanges>&... ranges, const T& val) : 
	MutableArrayBase<T,SRanges...>(ranges...),
	mCont(MAB::mRange->size(), val)
    {
	MAB::mInit = true;
    }
    
    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(const std::shared_ptr<SRanges>&... ranges, const vector<T>& vec) :
	MutableArrayBase<T,SRanges...>(ranges...),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(const std::shared_ptr<SRanges>&... ranges, vector<T>&& vec) :
	MutableArrayBase<T,SRanges...>(ranges...),
	mCont(std::forward<vector<T>>(vec))
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class... SRanges>
    template <class... Ranges>
    Array<T,SRanges...>::Array(const std::shared_ptr<SRanges>&... ranges, Array<T,Ranges...>&& in) :
	MutableArrayBase<T,SRanges...>(ranges...),
        mCont( std::move( in.mCont ) )
    {
        // maybe some checks here in the future...
        assert(mCont.size() == MAB::mRange->size());
	MAB::mInit = true;
        in.mInit = false;
    }

    template <typename T, class... SRanges>
    Array<T,SRanges...>::Array(Array<T,AnonymousRange>&& ama, SIZET<SRanges>... sizes) :
	MutableArrayBase<T,SRanges...>
	( ama.range()->template get<0>().template scast<SRanges...>(sizes...)->space() ),
	mCont( std::move( ama.mCont ) )
    {
	MAB::mInit = true;
        ama.mInit = false;
    }

    template <typename T, class... SRanges>
    T& Array<T,SRanges...>::operator[](const IndexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class... SRanges>
    const T& Array<T,SRanges...>::operator[](const IndexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class... SRanges>
    T& Array<T,SRanges...>::at(const typename IndexType::MetaType& meta)
    {
	return mCont[ MAB::cbegin().at(meta).pos() ];
    }

    template <typename T, class... SRanges>
    const T& Array<T,SRanges...>::at(const typename IndexType::MetaType& meta) const
    {
	return mCont[ MAB::cbegin().at(meta).pos() ];
    }
	
    template <typename T, class... SRanges>
    bool Array<T,SRanges...>::isConst() const
    {
	return false;
    }
    
    template <typename T, class... SRanges>
    bool Array<T,SRanges...>::isSlice() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    Array<T,SRanges2...> Array<T,SRanges...>::format(const std::shared_ptr<SRanges2>&... nrs)
    {
	//MAB::mInit = false;
	return Array<T,SRanges2...>( nrs... , mCont );
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    Array<T,SRanges2...> Array<T,SRanges...>::format(const std::tuple<std::shared_ptr<SRanges2>...>& nrs)
    {
	//MAB::mInit = false;
	return Array<T,SRanges2...>( nrs , mCont );
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    Slice<T,SRanges2...> Array<T,SRanges...>::slformat(const std::shared_ptr<SRanges2>&... nrs)
    {
        return Slice<T,SRanges2...>( nrs..., mCont.data() );
    }

    template <typename T, class... SRanges>
    template <class... SRanges2>
    ConstSlice<T,SRanges2...> Array<T,SRanges...>::slformat(const std::shared_ptr<SRanges2>&... nrs) const
    {
        return ConstSlice<T,SRanges2...>( nrs..., mCont.data() );
    }
    
    template <typename T, class... SRanges>
    const T* Array<T,SRanges...>::data() const
    {
	return mCont.data();
    }

    template <typename T, class... SRanges>
    T* Array<T,SRanges...>::data()
    {
	return mCont.data();
    }

    template <typename T, class... SRanges>
    std::shared_ptr<ArrayBase<T,AnonymousRange> > Array<T,SRanges...>::anonymous(bool slice) const
    {
	AnonymousRangeFactory arf(MAB::mRange->space());
	if(slice){
	    return std::make_shared<ConstSlice<T,AnonymousRange> >
		( std::dynamic_pointer_cast<AnonymousRange>( arf.create() ),
		  data() );
	}
	else {
	    return std::make_shared<Array<T,AnonymousRange> >
		( std::dynamic_pointer_cast<AnonymousRange>( arf.create() ),
		  mCont );
	}
    }

    template <typename T, class... SRanges>
    Array<T,SRanges...>& Array<T,SRanges...>::operator=(const T& in)
    {
	for(auto& x: mCont){
	    x = in;
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    Array<T,SRanges...>& Array<T,SRanges...>::operator+=(const Array& in)
    {
	if(not MAB::mInit){ // not initialized by default constructor !!
	    (*this) = in;
	}
	else {
	    sfor_p<0,sizeof...(SRanges),0>
		( [&](auto i) { return std::get<i>(MAB::mRange->space()).get() == std::get<i>(in.mRange->space()).get(); },
		  [&](auto a, auto b) { return a and b; });
            for(size_t i = 0; i != mCont.size(); ++i){
                mCont[i] += in.mCont[i];
            }
	}
	return *this;
    }

    template <typename T, class... SRanges>
    Array<T,SRanges...>& Array<T,SRanges...>::operator-=(const Array& in)
    {
	if(not MAB::mInit){ // not initialized by default constructor !!
	    (*this) = in;
	}
	else {
	    sfor_p<0,sizeof...(SRanges),0>
		( [&](auto i) { return std::get<i>(MAB::mRange->space()).get() == std::get<i>(in.mRange->space()).get(); },
		  [&](auto a, auto b) { return a and b; });
            for(size_t i = 0; i != mCont.size(); ++i){
                mCont[i] -= in.mCont[i];
            }
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    Array<T,SRanges...>& Array<T,SRanges...>::operator*=(const T& in)
    {
	for(auto& x: mCont){
	    x *= in;
	}
	return *this;
    }

    template <typename T, class... SRanges>
    Array<T,SRanges...>& Array<T,SRanges...>::operator/=(const T& in)
    {
	for(auto& x: mCont){
	    x /= in;
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    Array<T,SRanges...>::operator T() const
    {
	//static_assert( sizeof...(SRanges) == 1, "try to cast non-scalar type into scalar" );
	// TODO: check that SIZE is statically = 1 !!!
	return mCont[0];
    }

    template <typename T, class... SRanges>
    auto Array<T,SRanges...>::cat() const
	-> decltype(ArrayCatter<T>::cat(*this))
    {
	return ArrayCatter<T>::cat(*this);
    }
}
