
#include "ranges/dynamic_range.h"

namespace MultiArrayTools
{
    namespace
    {
        using namespace MultiArrayHelper;
    }


    
    DynamicRangeFactory::DynamicRangeFactory(const vector<std::shared_ptr<RangeBase>>& origs)
    {
        mProd = std::shared_ptr<oType>( new DynamicRange( origs ) );
    }
    
    

    
    DynamicRangeFactory::DynamicRangeFactory()
    {
	mProd = std::shared_ptr<oType>( new DynamicRange() );
    }

    // INSTANCIATE IF NEEDED!!
    
    std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > DynamicRangeFactory::mAleadyCreated;

    
    std::shared_ptr<RangeBase> DynamicRangeFactory::checkIfCreated(const vector<std::shared_ptr<RangeBase> >& pvec)
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


    
    std::shared_ptr<RangeBase> DynamicRangeFactory::create()
    {
	mProd = checkIfCreated(std::dynamic_pointer_cast<DynamicRange>(mProd)->mOrig); 
	setSelf();
	mProductCreated = true;
	return mProd;
    }

    
    /*********************
     *   DynamicIndex    *
     *********************/

    
    DynamicIndex::DynamicIndex(const std::shared_ptr<DynamicRange >& range) :
	IndexInterface<DynamicIndex,MetaType>(range, 0),
	mIVec(range->dim())
    {
	size_t xx = 1;
	for(size_t i = mIVec.size()-1; i != 0; --i){
	    mIVec[i].second = xx;
	    xx *= range->sub(i)->size();
	}
	mIVec[0].second = xx;
    }

    
    IndexType DynamicIndex::type() const
    {
	return IndexType::SINGLE;
    }

    
    DynamicIndex& DynamicIndex::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    
    DynamicIndex& DynamicIndex::operator++()
    {
        ++IB::mPos;
        if(mIvecInit){
            size_t ipos = mIVec.size()-1;
            auto& ii = mIVec[ipos].first;
            auto& jj = mIVec[ipos-1].first;
            while(ii->pos() == ii->max()-1 and ipos != 0) {
                (*ii) = 0;
                ++(*jj);
                --ipos;
            }
        }
	return *this;
    }

    
    DynamicIndex& DynamicIndex::operator--()
    {
        --IB::mPos;
        if(mIvecInit){
            size_t ipos = mIVec.size()-1;
            auto& ii = mIVec[ipos].first;
            auto& jj = mIVec[ipos-1].first;
            while(ii->pos() == 0 and ipos != 0) {
                (*ii) = ii->max()-1;
                --(*jj);
                --ipos;
            }
        }
	return *this;
    }

    
    DynamicIndex& DynamicIndex::sync()
    {
        assert(mIvecInit);
	size_t sv = 1;
	IB::mPos = 0;
	for(size_t i = 0; i != mIVec.size(); ++i){
	    auto& x = mIVec[mIVec.size()-i-1];
	    IB::mPos += x.first->pos() * sv;
	    sv *= x.first->max();
	}
	return *this;
    }
    
    
    DynamicIndex& DynamicIndex::operator()(const IVecT& ivec)
    {
        mIvecInit = true;
        mIVec = ivec;
	sync();
        return *this;
    }

    
    DynamicIndex& DynamicIndex::operator()(const vector<std::shared_ptr<IndexW>>& ivec)
    {
        mIvecInit = true;
        assert(mIVec.size() == ivec.size());
	for(size_t i = 0; i != mIVec.size(); ++i){
	    mIVec[i].first = ivec[i];
	}
	sync();
        return *this;
    }
    
    
    
    int DynamicIndex::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    
    int DynamicIndex::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    
    std::string DynamicIndex::stringMeta() const
    {
	return std::dynamic_pointer_cast<DynamicRange const>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    
    typename DynamicIndex::MetaType DynamicIndex::meta() const
    {
	return std::dynamic_pointer_cast<DynamicRange const>( IB::mRangePtr )->get(IB::mPos);
    }
    
    
    const typename DynamicIndex::MetaType* DynamicIndex::metaPtr() const
    {
	return nullptr;
    }

    /*
    bool DynamicIndex::isMeta(const MetaType& metaPos) const
    {
	return mExplicitRangePtr->isMeta(metaPos);
        }*/
    
    
    DynamicIndex& DynamicIndex::at(const MetaType& metaPos)
    {
	(*this) = std::dynamic_pointer_cast<DynamicRange const>( IB::mRangePtr )->getMeta( metaPos );
	return *this;
    }

    
    size_t DynamicIndex::posAt(const MetaType& metaPos) const
    {
	return std::dynamic_pointer_cast<DynamicRange const>( IB::mRangePtr )->getMeta( metaPos );
    }

    
    size_t DynamicIndex::dim() const // = 1
    {
	return mIVec.size();
    }

    
    bool DynamicIndex::last() const
    {
	return IB::mPos == IB::mMax - 1;
    }

    
    bool DynamicIndex::first() const
    {
	return IB::mPos == 0;
    }

    
    const IndexW& DynamicIndex::get(size_t n) const
    {
        assert(mIvecInit);
        return *mIVec[n].first;
    }

    
    std::shared_ptr<typename DynamicIndex::RangeType> DynamicIndex::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }


    
    
    
    size_t DynamicIndex::getStepSize(size_t n) const
    {
	return mIVec[n].second;
    }

    
    std::string DynamicIndex::id() const
    {
	return std::string("dyn") + std::to_string(IB::mId);
    }

        
    void DynamicIndex::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " /*<< meta().first*/ << std::endl;
    }


    /***********************
     *   DynamicRange    *
     ***********************/

    
    typename DynamicRange::MetaType DynamicRange::get(size_t pos) const
    {
        vector<char> out(cmetaSize());
        cmeta(out.data(),pos);
        return out;
    }

    
    size_t DynamicRange::getMeta(const MetaType& metaPos) const
    {
	return 0; // !!!
    }

    
    size_t DynamicRange::size() const
    {
	return mSize;
    }

    
    size_t DynamicRange::dim() const
    {
        return mOrig.size();
    }

    
    SpaceType DynamicRange::spaceType() const
    {
	return SpaceType::DYN;
    }

    
    bool DynamicRange::isEmpty() const
    {
	return mEmpty;
    }

    
    vector<size_t> DynamicRange::typeNum() const
    {
        vector<size_t> o;
        for(auto& x: mOrig){
            auto tn = x->typeNum();
            o.insert(o.end(), tn.begin(), tn.end());
        }
        return o;
    }

    
    size_t DynamicRange::cmeta(char* target, size_t pos) const
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
        assert(off == 0);
        return out;
    }

    
    size_t DynamicRange::cmetaSize() const
    {
        size_t out = 0;
	for(size_t i = mOrig.size(); i != 0; --i) {
            auto& x = mOrig[i-1];
            out += x->cmetaSize();
	}
        return out;
    }

    
    std::string DynamicRange::stringMeta(size_t pos) const
    {
	std::string out = "";
	//size_t xpos = pos;
	for(size_t i = mOrig.size(); i != 0; --i) {
	    auto& x = mOrig[i-1];
	    const size_t redpos = pos % x->size();
	    out = x->stringMeta(redpos) + ( (i == mOrig.size()) ? out : ", " + out );
	    pos -= redpos;
	    pos /= x->size();
	}
	out = "[" + out + "]";
	return out;
    }

    
    vector<char> DynamicRange::data() const
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

    
    DataHeader DynamicRange::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::DYN );
	h.metaSize = mOrig.size();
	h.multiple = 1;
        return h;
    }
    
    
    typename DynamicRange::IndexType DynamicRange::begin() const
    {
	DynamicIndex i
	    (std::dynamic_pointer_cast<DynamicRange>
	     ( std::shared_ptr<RangeBase>(RB::mThis) ) );
	i = 0;
	return i;
    }

    
    typename DynamicRange::IndexType DynamicRange::end() const
    {
	DynamicIndex i
	    (std::dynamic_pointer_cast<DynamicRange>
	     ( std::shared_ptr<RangeBase>(RB::mThis) ) );
	i = size();
	return i;
    }

    
    std::shared_ptr<RangeBase> DynamicRange::sub(size_t num) const
    {
	return mOrig.at(num);
    }

    
    void DynamicRange::sreplace(const std::shared_ptr<RangeBase> in, size_t num)
    {
	assert(mOrig[num]->size() == in->size());
	mOrig[num] = in;
    }

    

    
    DynamicRange::DynamicRange(const vector<std::shared_ptr<RangeBase>>& origs) :
        RangeInterface<DynamicIndex>(),
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

    
    
    const vector<std::shared_ptr<RangeBase> >& DynamicRange::orig() const
    {
        return mOrig;
    }
    
} // end namespace MultiArrayTools
