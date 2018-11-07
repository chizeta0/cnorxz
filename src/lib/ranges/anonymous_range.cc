
#include "ranges/anonymous_range.h"

namespace MultiArrayTools
{
    /******************************
     *   AnonymousRangeFactory    *
     ******************************/

    AnonymousRangeFactory::AnonymousRangeFactory()
    {
	mProd = std::shared_ptr<oType>( new AnonymousRange() );
    }

    
    std::map<std::shared_ptr<RangeBase>,std::vector<std::intptr_t> > AnonymousRangeFactory::mAleadyCreated;

    AnonymousRangeFactory::AnonymousRangeFactory(const std::vector<std::shared_ptr<RangeBase>>& origs)
    {
        mProd = std::shared_ptr<oType>( new AnonymousRange( origs ) );
    }
    
    std::shared_ptr<RangeBase> AnonymousRangeFactory::checkIfCreated(const std::vector<std::shared_ptr<RangeBase> >& pvec)
    {
	std::shared_ptr<RangeBase> out;
	bool check = false;
	for(auto& x: mAleadyCreated){
	    if(x.second.size() == pvec.size()){
		check = true;
		for(size_t i = 0; i != x.second.size(); ++i){
		    if(x.second[i] != reinterpret_cast<std::intptr_t>( pvec[i].get() ) ){
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
		app[i] = reinterpret_cast<std::intptr_t>( pvec[i].get() );
	    }
	    mAleadyCreated[mProd] = app;
	    out = mProd;
	}
	return out;
    }

    
    std::shared_ptr<RangeBase> AnonymousRangeFactory::create()
    {
	mProd = checkIfCreated(std::dynamic_pointer_cast<AnonymousRange>(mProd)->mOrig); 
	setSelf();
	mProductCreated = true;
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

    SpaceType AnonymousRange::spaceType() const
    {
	return SpaceType::ANON;
    }

    bool AnonymousRange::isEmpty() const
    {
	return mEmpty;
    }
    
    std::string AnonymousRange::stringMeta(size_t pos) const
    {
	std::string out = "[ ";
	//size_t xpos = pos;
	for(size_t i = mOrig.size(); i != 0; --i) {
	    auto& x = mOrig[i-1];
	    const size_t redpos = pos % x->size();
	    out = ( (i == mOrig.size()) ? out : out + " , " ) + x->stringMeta(redpos);
	    pos -= redpos;
	    pos /= x->size();
	}
	out += " ]";
	return out;
    }

    std::vector<char> AnonymousRange::data() const
    {
	DataHeader h = dataHeader();
	std::vector<char> out;
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	for(auto& x: mOrig){
	    auto part = x->data();
	    out.insert(out.end(), part.begin(), part.end());
	}
	return out;
    }

    DataHeader AnonymousRange::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::ANON );
	h.metaSize = mOrig.size();
	h.multiple = 1;
        return h;
    }
        
    size_t AnonymousRange::anonymousDim() const
    {
	return mOrig.size();
    }
    
    typename AnonymousRange::IndexType AnonymousRange::begin() const
    {
	AnonymousIndex i
	    (std::dynamic_pointer_cast<AnonymousRange>
	     ( std::shared_ptr<RangeBase>(RB::mThis) ) );
	i = 0;
	return i;
    }
    
    typename AnonymousRange::IndexType AnonymousRange::end() const
    {
	AnonymousIndex i
	    (std::dynamic_pointer_cast<AnonymousRange>
	     ( std::shared_ptr<RangeBase>(RB::mThis) ) );
	i = size();
	return i;
    }

    
    std::shared_ptr<RangeBase> AnonymousRange::sub(size_t num) const
    {
	return mOrig.at(num);
    }

    void AnonymousRange::sreplace(const std::shared_ptr<RangeBase> in, size_t num)
    {
	assert(mOrig[num]->size() == in->size());
	mOrig[num] = in;
    }

    const std::vector<std::shared_ptr<RangeBase> >& AnonymousRange::orig() const
    {
        return mOrig;
    }
    
    SingleRange<size_t,SpaceType::ANON>::SingleRange(const std::vector<std::shared_ptr<RangeBase>>& origs) :
        RangeInterface<AnonymousIndex>(),
        mOrig(origs)
    {
        mSize = 1;
        for(auto& x: mOrig){
            mSize *= x->size();
        }
        if(mOrig.size()){
            mEmpty = false;
        }
    }

    
    /*****************
     *   Functions   *
     *****************/

    std::shared_ptr<AnonymousRange> defaultRange(size_t size )
    {
	AnonymousRangeFactory arf
	    ( std::dynamic_pointer_cast<AnonymousRange>
	      (AnonymousRange::factory().create() ) );
	return std::dynamic_pointer_cast<AnonymousRange>( arf.create() );
    }


    
} // end namespace MultiArrayTools
