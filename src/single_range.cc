
#include "single_range.h"

namespace MultiArrayTools
{
    /******************
     *  SingleIndex   *	     
     ******************/

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(const U& upos,
				     std::shared_ptr<const RangeBase<SingleIndex<U,TYPE> > >& rangePtr) :
	IndexBase<SingleIndex<U,TYPE> >(rangePtr)
    {
	IIB::setPos( dynamic_pointer_cast<const SingleRange<U,TYPE> >( IB::mRangePtr )->get(upos) );
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(std::shared_ptr<const RangeBase<SingleIndex<U,TYPE> > >& rangePtr,
				     size_t pos) : IndexBase<SingleIndex<U,TYPE> >(range)
    {
	IIB::setPos( pos );
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator=(size_t pos)
    {
	IIB::setPos( pos );
	return *this;
    }
    
    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator++()
    {
	IIB::setPos( IIB::pos() + 1 );
	return *this;
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator--()
    {
	IIB::setPos( IIB::pos() - 1 );
	return *this;
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator+=(int n)
    {
	IIB::setPos( IIB::pos() + n );
	return *this;
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator-=(int n)
    {
	IIB::setPos( IIB::pos() - n );
	return *this;
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::operator==(const SingleIndex<U,TYPE>& i)
    {
	return IB::mRange == i.mRange and IIB::pos() == static_cast<size_t>( i.mPos );
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::operator!=(const SingleIndex<U,TYPE>& i)
    {
	return IB::mRange != i.mRange or IIB::pos() != static_cast<size_t>( i.mPos );
    }

    template <typename U, RangeType TYPE>
    MultiRangeType SingleIndex<U,TYPE>::rangeType() const
    {
	return MultiRangeType(TYPE);
    }
    
    template <typename U, RangeType TYPE>
    U SingleIndex<U,TYPE>::getMetaPos() const
    {
	return dynamic_cast<SingleRange<U,TYPE> const*>( IB::mRange )->get(IIB::pos());
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::atMeta(const U& metaPos)
    {
	operator=( dynamic_cast<SingleRange<U,TYPE> const*>( IB::mRange )->getMeta(metaPos) );
	return *this;
    }

    template <typename U, RangeType TYPE>
    size_t SingleIndex<U,TYPE>::dim() const
    {
	return 1;
    }
    
    template <typename U, RangeType TYPE>
    size_t SingleIndex<U,TYPE>::evaluate(const SingleIndex<U,TYPE>& in) const
    {
	return in.pos();
    }

    template <typename U, RangeType TYPE>
    void SingleIndex<U,TYPE>::copyPos(const SingleIndex<U,TYPE>& in)
    {
	IIB::setPos(in.pos());
    }

    template <typename U, RangeType TYPE>
    size_t SingleIndex<U,TYPE>::giveSubStepSize(IndefinitIndexBase* subIndex)
    {
	if(subIndex == this){
	    return 1;
	}
	else {
	    return 0;
	}
    }
    
    std::ostream& operator<<(std::ostream& os, VET vet)
    {
	os << ( (vet == VET::VALUE) ? std::string("VALUE") : std::string("ERROR") );
	return os;
    }
    
    /*
    template <typename U, RangeType TYPE>
    void SingleIndex<U,TYPE>::eval()
    {
	IIB::setPos( evaluate( *this ) );
    }
    */

    /********************
     *   SingleRange    *
     ********************/
    
    template <typename U, RangeType TYPE>
    SingleRange<U,TYPE>::SingleRange(const std::vector<U>& space) : RangeBase<SingleIndex<U,TYPE> >(),
	mSpace(space) {}
    
    template <typename U, RangeType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::getMeta(const U& metaPos) const
    {
	size_t cnt = 0;
	for(auto& x: mSpace){
	    if(x == metaPos){
		return cnt;
	    }
	    ++cnt;
	}
	return cnt;
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::size() const
    {
	return mSpace.size();
    }

    template <typename U, RangeType TYPE>
    MultiRangeType SingleRange<U,TYPE>::type() const
    {
	return MultiRangeType(TYPE);
    }
    
    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE> SingleRange<U,TYPE>::begin() const
    {
	return SingleIndex<U,TYPE>(this, 0);
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE> SingleRange<U,TYPE>::end() const
    {
	return SingleIndex<U,TYPE>(this, mSpace.size());
    }

    // specializations

    SingleRange<int,RangeType::SPACE>::SingleRange(size_t ext) :
	RangeBase<SingleIndex<int,RangeType::SPACE> >(),
	mExt(ext) {}
    
    int SingleRange<int,RangeType::SPACE>::get(size_t pos) const
    {
	return (pos <= mExt / 2) ? static_cast<int>( pos ) : static_cast<int>( pos ) - static_cast<int>( mExt );
    }

    size_t SingleRange<int,RangeType::SPACE>::getMeta(int metaPos) const
    {
	return (metaPos < 0) ? metaPos + mExt : metaPos;
    }

    
    size_t SingleRange<int,RangeType::SPACE>::size() const
    {
	return mExt;
    }

    
    MultiRangeType SingleRange<int,RangeType::SPACE>::type() const
    {
	return MultiRangeType(RangeType::SPACE);
    }
    
    
    SingleIndex<int,RangeType::SPACE> SingleRange<int,RangeType::SPACE>::begin() const
    {
	return SingleIndex<int,RangeType::SPACE>(this, 0);
    }

    
    SingleIndex<int,RangeType::SPACE> SingleRange<int,RangeType::SPACE>::end() const
    {
	return SingleIndex<int,RangeType::SPACE>(this, size());
    }

    // 
    
    SingleRange<size_t,RangeType::DISTANCE>::SingleRange(size_t ext) :
	RangeBase<SingleIndex<size_t,RangeType::DISTANCE> >(),
	mExt(ext) {}
    
    size_t SingleRange<size_t,RangeType::DISTANCE>::get(size_t pos) const
    {
	return pos;
    }

    size_t SingleRange<size_t,RangeType::DISTANCE>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t SingleRange<size_t,RangeType::DISTANCE>::size() const
    {
	return mExt;
    }
    
    MultiRangeType SingleRange<size_t,RangeType::DISTANCE>::type() const
    {
	return MultiRangeType(RangeType::DISTANCE);
    }
   
    SingleIndex<size_t,RangeType::DISTANCE> SingleRange<size_t,RangeType::DISTANCE>::begin() const
    {
	return SingleIndex<size_t,RangeType::DISTANCE>(this, 0);
    }
    
    SingleIndex<size_t,RangeType::DISTANCE> SingleRange<size_t,RangeType::DISTANCE>::end() const
    {
	return SingleIndex<size_t,RangeType::DISTANCE>(this, size());
    }

    // 
    
    SingleRange<size_t,RangeType::ENSEMBLE>::SingleRange(size_t num) :
	RangeBase<SingleIndex<size_t,RangeType::ENSEMBLE> >(),
	mNum(num) {}
    
    
    size_t SingleRange<size_t,RangeType::ENSEMBLE>::get(size_t pos) const
    {
	return pos;
    }

    
    size_t SingleRange<size_t,RangeType::ENSEMBLE>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }

    
    size_t SingleRange<size_t,RangeType::ENSEMBLE>::size() const
    {
	return mNum;
    }

    
    MultiRangeType SingleRange<size_t,RangeType::ENSEMBLE>::type() const
    {
	return MultiRangeType(RangeType::ENSEMBLE);
    }
    
    
    SingleIndex<size_t,RangeType::ENSEMBLE> SingleRange<size_t,RangeType::ENSEMBLE>::begin() const
    {
	return SingleIndex<size_t,RangeType::ENSEMBLE>(this, 0);
    }

    
    SingleIndex<size_t,RangeType::ENSEMBLE> SingleRange<size_t,RangeType::ENSEMBLE>::end() const
    {
	return SingleIndex<size_t,RangeType::ENSEMBLE>(this, size());
    }

    //
    
    VET SingleRange<VET,RangeType::VALUE_ERROR>::get(size_t pos) const
    {
	return static_cast<VET>( pos );
    }

    
    size_t SingleRange<VET,RangeType::VALUE_ERROR>::getMeta(VET metaPos) const
    {
	return static_cast<size_t>( metaPos );
    }

    
    size_t SingleRange<VET,RangeType::VALUE_ERROR>::size() const
    {
	return 2;
    }

    
    MultiRangeType SingleRange<VET,RangeType::VALUE_ERROR>::type() const
    {
	return MultiRangeType(RangeType::VALUE_ERROR);
    }
    
    
    SingleIndex<VET,RangeType::VALUE_ERROR> SingleRange<VET,RangeType::VALUE_ERROR>::begin() const
    {
	return SingleIndex<VET,RangeType::VALUE_ERROR>(this, 0);
    }

    
    SingleIndex<VET,RangeType::VALUE_ERROR> SingleRange<VET,RangeType::VALUE_ERROR>::end() const
    {
	return SingleIndex<VET,RangeType::VALUE_ERROR>(this, size());
    }

    // 

    
    size_t SingleRange<size_t,RangeType::LORENTZ>::get(size_t pos) const
    {
	return pos;
    }

    
    size_t SingleRange<size_t,RangeType::LORENTZ>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }

    
    size_t SingleRange<size_t,RangeType::LORENTZ>::size() const
    {
	#ifdef LORENTZ_DIMENSION
	return LORENTZ_DIMENSION;
	#else
	return 4; // 4
	#endif
    }

    
    MultiRangeType SingleRange<size_t,RangeType::LORENTZ>::type() const
    {
	return MultiRangeType(RangeType::LORENTZ);
    }
    
    
    SingleIndex<size_t,RangeType::LORENTZ> SingleRange<size_t,RangeType::LORENTZ>::begin() const
    {
	return SingleIndex<size_t,RangeType::LORENTZ>(this, 0);
    }

    
    SingleIndex<size_t,RangeType::LORENTZ> SingleRange<size_t,RangeType::LORENTZ>::end() const
    {
	return SingleIndex<size_t,RangeType::LORENTZ>(this, size());
    }
    
}
