
#include "single_range.h"

namespace MultiArrayTools
{
    /******************
     *  SingleIndex   *	     
     ******************/

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range) :
	IndexInterface<U>(range, 0) {}

    template <typename U, RangeType TYPE>
    IndexType SingleIndex<U,TYPE>::type() const
    {
	return IndexType::SINGLE;
    }
	
    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }
    
    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename U, RangeType TYPE>
    U SingleIndex<U,TYPE>::meta() const
    {
	return std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->get( IB::pos() );
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::at(const U& metaPos)
    {
	operator=( std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->getMeta( metaPos ) );
	return *this;
    }

    template <typename U, RangeType TYPE>
    size_t SingleIndex<U,TYPE>::dim() const
    {
	return 1;
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::last() const
    {
	return IB::mPos == IB::mRangePtr->size() - 1;
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::first() const
    {
	return IB::mPos == 0;
    }

    template <typename U, RangeType TYPE>
    std::shared_ptr<const IndexBase> SingleIndex<U,TYPE>::getPtr(size_t n) const
    {
	return std::shared_ptr<const IndexBase>();
    }
    
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, RangeType TYPE>
    SingleRangeFactory<U,TYPE>::SingleRangeFactory(const std::vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new SingleRange<U,TYPE>( space ) );
    }

    template <typename U, RangeType TYPE>
    std::shared_ptr<RangeBase> SingleRangeFactory<U,TYPE>::create()
    {
	setSelf();
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    template <typename U, RangeType TYPE>
    SingleRange<U,TYPE>::SingleRange(const std::vector<U>& space) : RangeInterface<SingleIndex<U,TYPE> >(),
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
    size_t SingleRange<U,TYPE>::dim() const
    {
	return 1;
    }

    template <typename U, RangeType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::begin() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename U, RangeType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::end() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    template <typename U, RangeType TYPE>
    std::shared_ptr<IndexBase> SingleRange<U,TYPE>::index() const
    {
	return std::make_shared<SingleIndex<U,TYPE> >
	    ( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
	      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
    }

    /*
    // specializations (not updated!!!)

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
    */    
}
