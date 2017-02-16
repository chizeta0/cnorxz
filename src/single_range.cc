
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


    
    /******************
     *  SingleIndex   *	     
     ******************/
    
    template <typename U, IndexType TYPE>
    const U& SingleIndexBase<U,TYPE>::getMetaPos() const
    {
	return dynamic_cast<SingleRange*>( mRange )->get(mPos);
    }

    template <typename U, IndexType TYPE>
    size_t dim() const
    {
	return 1;
    }
    
    template <typename U, IndexType TYPE>
    size_t SingleIndexBase<TYPE>::evaluate(const Index& in)
    {
	return in.mPos;
    }

    template <typename U, IndexType TYPE>
    void linkTo(IndefinitIndexBase* target)
    {
	target->link(this);
    }    

}
