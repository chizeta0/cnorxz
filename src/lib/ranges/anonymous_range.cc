
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

    
    std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > AnonymousRangeFactory::mAleadyCreated;

    AnonymousRangeFactory::AnonymousRangeFactory(const vector<std::shared_ptr<RangeBase>>& origs)
    {
        mProd = std::shared_ptr<oType>( new AnonymousRange( origs ) );
    }
    
    std::shared_ptr<RangeBase> AnonymousRangeFactory::checkIfCreated(const vector<std::shared_ptr<RangeBase> >& pvec)
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
	    vector<std::intptr_t> app(pvec.size());
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

    vector<size_t> AnonymousRange::typeNum() const
    {
        vector<size_t> o;
        for(auto& x: mOrig){
            auto tn = x->typeNum();
            o.insert(o.end(), tn.begin(), tn.end());
        }
        return o;
    }
    
    size_t AnonymousRange::cmeta(char* target, size_t pos) const
    {
        size_t out = 0;
        size_t off = cmetaSize();
	for(size_t i = mOrig.size(); i != 0; --i) {
	    auto& x = mOrig[i-1];
	    const size_t redpos = pos % x->size();
            const size_t s = x->cmetaSize();
            out += s;
            off -= s;
	    pos -= redpos;
	    pos /= x->size();
            x->cmeta(target+off,redpos);
	}
        return out;
    }

    size_t AnonymousRange::cmetaSize() const
    {
        size_t out = 0;
	for(size_t i = mOrig.size(); i != 0; --i) {
            auto& x = mOrig[i-1];
            out += x->cmetaSize();
	}
        return out;
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

    vector<char> AnonymousRange::data() const
    {
	DataHeader h = dataHeader();
	vector<char> out;
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

    std::shared_ptr<AnonymousRange> AnonymousRange::sreplace(const std::shared_ptr<RangeBase> in, size_t num) const
    {
	assert(mOrig[num]->size() == in->size());
        auto tmp = mOrig;
	tmp[num] = in;
        AnonymousRangeFactory arf(tmp);
        return std::dynamic_pointer_cast<AnonymousRange>(arf.create());
    }

    std::shared_ptr<AnonymousRange> AnonymousRange::sreplace(const vector<std::shared_ptr<RangeBase>>& in, size_t num) const
    {
        size_t nsize = 1;
        for(auto& x: in){
            nsize *= x->size();
        }
	assert(mOrig[num]->size() == nsize);
        auto norig = mOrig;
        norig.resize(mOrig.size() + in.size() - 1);
        for(size_t i = 0; i != num; ++i){
            norig[i] = mOrig[i];
        }
        for(size_t i = 0; i != in.size(); ++i){
            norig[num+i] = in[i];
        }
        for(size_t i = num + 1 ; i < mOrig.size(); ++i){
            norig[in.size()+i-1] = mOrig[i];
        }
        //mOrig = std::move( norig );
        AnonymousRangeFactory arf(norig);
        return std::dynamic_pointer_cast<AnonymousRange>(arf.create());
    }

    std::shared_ptr<AnonymousRange> AnonymousRange::sreplace(const std::shared_ptr<RangeBase>& in,
                                                             const vector<size_t>& num) const
    {
        if(num.size() != 0){
            size_t cnt = num[0];
            size_t rep_size = 1;
            // assert continuous ordering or replaced ranges:
            for(auto& x: num){
                assert(cnt++ == x);
                rep_size *= mOrig[x]->size();
            }
            assert(rep_size == in->size());
            vector<std::shared_ptr<RangeBase>> norig;
            norig.reserve(mOrig.size()-num.size()+1);
            norig.insert(norig.end(),mOrig.begin(),mOrig.begin()+num[0]);
            norig.push_back(in);
            norig.insert(norig.end(),mOrig.begin()+num.back()+1,mOrig.end());
            AnonymousRangeFactory arf(norig);
            return std::dynamic_pointer_cast<AnonymousRange>(arf.create());
        }
        else {
            return std::dynamic_pointer_cast<AnonymousRange>( std::shared_ptr<RangeBase>(RB::mThis) );
        }
    }
    
    const vector<std::shared_ptr<RangeBase> >& AnonymousRange::orig() const
    {
        return mOrig;
    }
    
    GenSingleRange<size_t,SpaceType::ANON,MUI>::GenSingleRange(const vector<std::shared_ptr<RangeBase>>& origs) :
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
