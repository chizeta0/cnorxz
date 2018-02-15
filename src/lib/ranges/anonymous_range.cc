
#include "ranges/anonymous_range.h"

namespace MultiArrayTools
{
    /******************************
     *   AnonymousRangeFactory    *
     ******************************/
        
    std::shared_ptr<RangeBase> AnonymousRangeFactory::create()
    {
	setSelf();
	return mProd;
    }

    /***********************
     *   AnonymousRange    *
     ***********************/
    
    size_t AnonymousRange::get(size_t pos) const
    {
	return pos;
    }
        
    size_t AnonymousRange::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
        
    size_t AnonymousRange::size() const
    {
	return mSize;
    }
        
    size_t AnonymousRange::dim() const
    {
	return 1;
    }
        
    typename AnonymousRange::IndexType AnonymousRange::begin() const
    {
	AnonymousIndex i
	    (std::dynamic_pointer_cast<ClassicRange>
	     ( ClassicRange::factory(size()).create() ) ); // !!!
	i = 0;
	return i;
    }
    
    typename AnonymousRange::IndexType AnonymousRange::end() const
    {
	AnonymousIndex i
	    ( std::dynamic_pointer_cast<ClassicRange>
	      ( ClassicRange::factory(size()).create() ));// !!!
	i = size();
	return i;
    }

    // put this in the interface class !!!
    /*
    std::shared_ptr<VIWB> AnonymousRange::index() const
    {
	//typedef IndexWrapper<IndexType> IW;
	return std::shared_ptr<VIWB>();
		    std::make_shared<IW>
	    (std::make_shared<IndexType>
	     ( std::dynamic_pointer_cast<AnonymousRange>
	     ( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );  //!!!!
    }
    */
    
    /*****************
     *   Functions   *
     *****************/

    std::shared_ptr<AnonymousRange> defaultRange(size_t size )
    {
	AnonymousRangeFactory arf
	    ( std::dynamic_pointer_cast<ClassicRange>
	      (ClassicRange::factory(size).create() ) );
	return std::dynamic_pointer_cast<AnonymousRange>( arf.create() );
    }
	
} // end namespace MultiArrayTools
