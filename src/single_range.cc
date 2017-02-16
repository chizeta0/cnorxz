
#include "single_range.h"

namespace MultiArrayTools
{
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, RangeType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::get(const U& metaPos) const
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

    template <typename U, IndexType TYPE>
    size_t SingleRange<U,TYPE>::size() const
    {
	return mSpace.size();
    }
    
    /******************
     *  SingleIndex   *	     
     ******************/
    
    template <typename U, IndexType TYPE>
    const U& SingleIndexBase<U,TYPE>::getMetaPos() const
    {
	return dynamic_cast<SingleRange*>( mRange )->get(mPos);
    }

    template <typename U, IndexType TYPE>
    size_t SingleIndexBase<U,TYPE>::dim() const
    {
	return 1;
    }
    
    template <typename U, IndexType TYPE>
    size_t SingleIndexBase<TYPE>::evaluate(const Index& in)
    {
	return in.mPos;
    }

    template <typename U, IndexType TYPE>
    void SingleIndexBase<U,TYPE>::linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
    }    

    template <typename U, IndexType TYPE>
    SingleIndex& SingleIndexBase<U,TYPE>::operator=(const U& upos)
    {
	setPos( dynamic_cast<SingleRange*>( mRange )->get(upos) );
    }
    
}
