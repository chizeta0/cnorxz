
#include "ranges/dynamic_range.h"

namespace MultiArrayTools
{
    /**********************
     *   DynamicIndex    *
     **********************/

    
    DynamicIndex::DynamicIndex(const std::shared_ptr<DynamicRange >& range) :
	IndexInterface<DynamicIndex,MetaType>(range, 0),
	mExplicitRangePtr(std::dynamic_pointer_cast<RangeType>(IB::mRangePtr)),
    {}

    
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
        // !!!
	return *this;
    }

    
    DynamicIndex& DynamicIndex::operator--()
    {
        // !!!
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
    
    
    MetaType DynamicIndex::meta() const
    {
	return mExplicitRangePtr->meta(IB::mPos);
    }

    
    const MetaType* DynamicIndex::metaPtr() const
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

    
    size_t DynamicIndex::posAt(const U& metaPos) const
    {
	return std::dynamic_pointer_cast<DynamicRange const>( IB::mRangePtr )->getMeta( metaPos );
    }

    
    size_t DynamicIndex::dim() // = 1
    {
	return 1;
    }

    
    bool DynamicIndex::last()
    {
	return IB::mPos == IB::mMax - 1;
    }

    
    bool DynamicIndex::first()
    {
	return IB::mPos == 0;
    }

    
    std::shared_ptr<typename DynamicIndex::RangeType> DynamicIndex::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    
    template <size_t N>
    void DynamicIndex::getPtr() {}
    
    
    size_t DynamicIndex::getStepSize(size_t n)
    {
	return 1;
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
		  << "](" << IB::mRangePtr << "): " << meta() << std::endl;
    }
    
    
    /******************************
     *   DynamicRangeFactory    *
     ******************************/

    DynamicRangeFactory::DynamicRangeFactory()
    {
	mProd = std::shared_ptr<oType>( new DynamicRange() );
    }
    
    std::map<std::shared_ptr<RangeBase>,std::vector<std::intptr_t> > DynamicRangeFactory::mAleadyCreated;
    
    std::shared_ptr<RangeBase> DynamicRangeFactory::checkIfCreated(const std::vector<std::shared_ptr<RangeBase> >& pvec)
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

    
    std::shared_ptr<RangeBase> DynamicRangeFactory::create()
    {
	mProd = checkIfCreated(std::dynamic_pointer_cast<DynamicRange>(mProd)->mOrig); 
	setSelf();
	mProductCreated = true;
	return mProd;
    }

    /***********************
     *   DynamicRange    *
     ***********************/
    
    size_t DynamicRange::get(size_t pos) const
    {
	return pos;
    }
        
    size_t DynamicRange::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
        
    size_t DynamicRange::size() const
    {
	return mSize;
    }
        
    size_t DynamicRange::dim() const
    {
	return 1;
    }

    SpaceType DynamicRange::spaceType() const
    {
	return SpaceType::DYN;
    }

    bool DynamicRange::isEmpty() const
    {
	return mEmpty;
    }
    
    std::string DynamicRange::stringMeta(size_t pos) const
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

    std::vector<char> DynamicRange::data() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::DYN );
	h.metaSize = mOrig.size();
	h.multiple = 1;
	std::vector<char> out;
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	for(auto& x: mOrig){
	    auto part = x->data();
	    out.insert(out.end(), part.begin(), part.end());
	}
	return out;
    }
    
    size_t DynamicRange::anonymousDim() const
    {
	return mOrig.size();
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
    
    /*****************
     *   Functions   *
     *****************/

    std::shared_ptr<DynamicRange> defaultRange(size_t size )
    {
	DynamicRangeFactory arf
	    ( std::dynamic_pointer_cast<DynamicRange>
	      (DynamicRange::factory().create() ) );
	return std::dynamic_pointer_cast<DynamicRange>( arf.create() );
    }


    
} // end namespace MultiArrayTools
