
#include "single_range.h"

namespace MultiArrayTools
{
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
    
    /******************
     *  SingleIndex   *	     
     ******************/

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(RangeBase<SingleIndex<U,TYPE> > const* range,
				     const U& upos, size_t disambig) : IndexBase<SingleIndex<U,TYPE> >(range)
    {
	IIB::setPos( dynamic_cast<SingleRange<U,TYPE> const*>( IB::mRange )->get(upos) );
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(RangeBase<SingleIndex<U,TYPE> > const* range,
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
	return IB::mRange == i.mRange and IIB::pos() == i.mPos;
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::operator!=(const SingleIndex<U,TYPE>& i)
    {
	return IB::mRange != i.mRange or IIB::pos() != i.mPos;
    }

    template <typename U, RangeType TYPE>
    MultiRangeType SingleIndex<U,TYPE>::rangeType() const
    {
	return MultiRangeType(TYPE);
    }
    
    template <typename U, RangeType TYPE>
    const U& SingleIndex<U,TYPE>::getMetaPos() const
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
    void SingleIndex<U,TYPE>::linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
    }    

    template <typename U, RangeType TYPE>
    void SingleIndex<U,TYPE>::copyPos(const SingleIndex<U,TYPE>& in)
    {
	IIB::setPos(in.pos());
    }

    /*
    template <typename U, RangeType TYPE>
    void SingleIndex<U,TYPE>::eval()
    {
	IIB::setPos( evaluate( *this ) );
    }
    */
}
