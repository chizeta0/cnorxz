
#include "ranges/anonymous_range.h"

namespace MultiArrayTools
{
    /******************************
     *   AnonymousRangeFactory    *
     ******************************/

    std::shared_ptr<RangeBase> checkIfCreated(const std::vector<std::shared_ptr<RangeBase> >& pvec)
    {
	std::shared_ptr<RangeBase> out;
	bool check = false;
	for(auto& x: mAleadyCreated){
	    if(x.second.size() == pvec.size()){
		check = true;
		for(size_t i = 0; i != x.size(); ++i){
		    if(x.second[i] != static_cast<std::intptr_t>( pvec[i].get() ) ){
			check = false;
			break;
		    }
		}
		if(check == true){
		    out = x.first;
		    break;
		}
	    }
	}
	if(not check){
	    std::vector<std::intptr_t> app(pvec.size());
	    for(size_t i = 0; i != app.size(); ++i){
		app[i] = static_cast<std::intptr_t>( pvec[i].get() );
	    }
	    mAleadyCreated[mProd] = app;
	    out = mProd;
	}
	return out;
    }

    
    std::shared_ptr<RangeBase> AnonymousRangeFactory::create()
    {
	mProd = checkIfCreated(); 
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

    
    std::shared_ptr<RangeBase> AnonymousRange::sub(size_t num) const
    {
	return mOrig.at(num);
    }
    
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
