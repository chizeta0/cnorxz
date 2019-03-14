
//#include "ranges/dynamic_range.h"
//#include "ranges/dynamic_meta.h"

#include "rpack_num.h"

namespace MultiArrayTools
{
    namespace
    {
        using namespace MultiArrayHelper;
    }

    /*************************
     *   IndexWrapperBase    *
     *************************/

    template <class ExpressionCollection>
    template <class Expr>
    ExpressionHolder<Expr>
    IndexWrapperBase<ExpressionCollection>::ifor(size_t step, ExpressionHolder<Expr> ex) const
    {
	return mEc->ifor(step, ex);
    }

    template <class ExpressionCollection>
    template <class Expr>
    ExpressionHolder<Expr>
    IndexWrapperBase<ExpressionCollection>::iforh(size_t step, ExpressionHolder<Expr> ex) const
    {
	return mEc->iforh(step, ex);
    }

    template <class ExpressionCollection>
    template <class Expr>
    ExpressionHolder<Expr>
    IndexWrapperBase<ExpressionCollection>::ifori(size_t step, Expr ex) const
    {
	return mEc->ifori(step, ex);
    }

    template <class ExpressionCollection>
    template <class Expr>
    ExpressionHolder<Expr>
    IndexWrapperBase<ExpressionCollection>::iforhi(size_t step, Expr ex) const
    {
	return mEc->iforhi(step, ex);
    }

    template <class Index, class ExpressionCollection>
    size_t IndexWrapper<Index,ExpressionCollection>::getStepSizeComp(std::intptr_t j) const
    {
	return MultiArrayHelper::getStepSize(*mI, j);
    }
    
    /****************************
     *   DynamicRangeFactory    *
     ****************************/

    template <class EC>
    template <class... RangeTypes>
    DynamicRangeFactory<EC>::DynamicRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs)
    {
	mProd = std::shared_ptr<oType>( new DynamicRange<EC>( origs ) );
    }

    template <class EC>
    template <class... RangeTypes>
    DynamicRangeFactory<EC>::DynamicRangeFactory(std::shared_ptr<RangeTypes>... origs)
    {
	mProd = std::shared_ptr<oType>( new DynamicRange<EC>( origs... ) );
    }

    template <class EC>
    DynamicRangeFactory<EC>::DynamicRangeFactory(const vector<std::shared_ptr<RangeBase>>& origs)
    {
        mProd = std::shared_ptr<oType>( new DynamicRange<EC>( origs ) );
    }
    
    template <class EC>
    template <class Range>
    void DynamicRangeFactory<EC>::append(std::shared_ptr<Range> r)
    {
	if(mProductCreated){
	    
	    mProd = std::shared_ptr<oType>( new DynamicRange<EC>( *std::dynamic_pointer_cast<oType>(mProd) ) );
	    mProductCreated = false;
	}
	std::dynamic_pointer_cast<oType>(mProd)->mOrig.push_back(r);
	std::dynamic_pointer_cast<oType>(mProd)->mSize *= r->size();
	std::dynamic_pointer_cast<oType>(mProd)->mEmpty = false;
    }

    template <class EC>
    DynamicRangeFactory<EC>::DynamicRangeFactory()
    {
	mProd = std::shared_ptr<oType>( new DynamicRange<EC>() );
    }

    // INSTANCIATE IF NEEDED!!
    template <class EC>
    std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > DynamicRangeFactory<EC>::mAleadyCreated;

    template <class EC>
    std::shared_ptr<RangeBase> DynamicRangeFactory<EC>::checkIfCreated(const vector<std::shared_ptr<RangeBase> >& pvec)
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


    template <class EC>
    std::shared_ptr<RangeBase> DynamicRangeFactory<EC>::create()
    {
	mProd = checkIfCreated(std::dynamic_pointer_cast<DynamicRange<EC>>(mProd)->mOrig); 
	setSelf();
	mProductCreated = true;
	return mProd;
    }

    
    /*********************
     *   DynamicIndex    *
     *********************/

    template <class EC>
    DynamicIndex<EC>::DynamicIndex(const std::shared_ptr<DynamicRange<EC> >& range) :
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

    template <class EC>
    IndexType DynamicIndex<EC>::type() const
    {
	return IndexType::SINGLE;
    }

    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::operator++()
    {
        size_t ipos = mIVec.size()-1;
        auto& ii = mIVec[ipos].first;
        auto& jj = mIVec[ipos-1].first;
        while(ii->pos() == ii->max()-1 and ipos != 0) {
            (*ii) = 0;
            ++(*jj);
            --ipos;
        }
	return *this;
    }

    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::operator--()
    {
        size_t ipos = mIVec.size()-1;
        auto& ii = mIVec[ipos].first;
        auto& jj = mIVec[ipos-1].first;
        while(ii->pos() == 0 and ipos != 0) {
            (*ii) = ii->max()-1;
            --(*jj);
            --ipos;
        }
	return *this;
    }

    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::sync()
    {
	size_t sv = 1;
	IB::mPos = 0;
	for(size_t i = 0; i != mIVec.size(); ++i){
	    auto& x = mIVec[mIVec.size()-i-1];
	    IB::mPos += x.first->pos() * sv;
	    sv *= x.first->max();
	}
	return *this;
    }
    
    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::operator()(const IVecT& ivec)
    {
        mIVec = ivec;
	sync();
        return *this;
    }

    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::operator()(const vector<std::shared_ptr<IndexW<EC>>>& ivec)
    {
        assert(mIVec.size() == ivec.size());
	for(size_t i = 0; i != mIVec.size(); ++i){
	    mIVec[i].first = ivec[i];
	}
	sync();
        return *this;
    }
    
    template <class EC>
    template <class... Indices>
    DynamicIndex<EC>& DynamicIndex<EC>::operator()(const std::shared_ptr<Indices>&... is)
    {
	vector<std::shared_ptr<IndexW<EC>>> tmp =
	    { std::make_shared<IndexWrapper<Indices,EC>>(is)... };
	
	assert(mIVec.size() == tmp.size());
	for(size_t i = 0; i != mIVec.size(); ++i){
	    mIVec[i].first = tmp[i];
	}
	sync();
	return *this;
    }
    
    template <class EC>
    int DynamicIndex<EC>::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <class EC>
    int DynamicIndex<EC>::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    template <class EC>
    std::string DynamicIndex<EC>::stringMeta() const
    {
	return std::dynamic_pointer_cast<DynamicRange<EC> const>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class EC>
    typename DynamicIndex<EC>::MetaType DynamicIndex<EC>::meta() const
    {
	return std::dynamic_pointer_cast<DynamicRange<EC> const>( IB::mRangePtr )->get(IB::mPos);
    }
    
    template <class EC>
    const typename DynamicIndex<EC>::MetaType* DynamicIndex<EC>::metaPtr() const
    {
	return nullptr;
    }

    /*
    bool DynamicIndex<EC>::isMeta(const MetaType& metaPos) const
    {
	return mExplicitRangePtr->isMeta(metaPos);
        }*/
    
    template <class EC>
    DynamicIndex<EC>& DynamicIndex<EC>::at(const MetaType& metaPos)
    {
	(*this) = std::dynamic_pointer_cast<DynamicRange<EC> const>( IB::mRangePtr )->getMeta( metaPos );
	return *this;
    }

    template <class EC>
    size_t DynamicIndex<EC>::posAt(const MetaType& metaPos) const
    {
	return std::dynamic_pointer_cast<DynamicRange<EC> const>( IB::mRangePtr )->getMeta( metaPos );
    }

    template <class EC>
    size_t DynamicIndex<EC>::dim() const // = 1
    {
	return mIVec.size();
    }

    template <class EC>
    bool DynamicIndex<EC>::last() const
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <class EC>
    bool DynamicIndex<EC>::first() const
    {
	return IB::mPos == 0;
    }

    template <class EC>
    const IndexW<EC>& DynamicIndex<EC>::get(size_t n) const
    {
        return *mIVec[n].first;
    }

    template <class EC>
    std::shared_ptr<typename DynamicIndex<EC>::RangeType> DynamicIndex<EC>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }


    template <class EC>
    template <size_t N>
    void DynamicIndex<EC>::getPtr() {}
    
    template <class EC>
    size_t DynamicIndex<EC>::getStepSize(size_t n) const
    {
	return mIVec[n].second;
    }

    template <class EC>
    std::string DynamicIndex<EC>::id() const
    {
	return std::string("dyn") + std::to_string(IB::mId);
    }

    template <class EC>    
    void DynamicIndex<EC>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " /*<< meta().first*/ << std::endl;
    }

    template <class Expr>
    struct ForMaker
    {
	template <class IVecT>
	static inline auto mk(size_t i, size_t step, ExpressionHolder<Expr> ex, 
			      const IVecT& ivec, bool hidden = false)
	    -> ExpressionHolder<Expr>
	{
	    if(i == 0) {
		auto& ii = *ivec[0].first;
		return hidden ? ii.iforh(step*ivec[i].second, ex) :
		    ii.ifor(step*ivec[i].second, ex);
	    }
	    else {
		auto& ii = *ivec[i].first;
		return mk(i-1, step,
			  (hidden ? ii.iforh(step*ivec[i].second, ex) :
			   ii.ifor(step*ivec[i].second, ex)),
			  ivec, hidden);

	    }
	}
    };

    /*
    template <class EC>
    template <class Expr>
    inline auto DynamicIndex<EC>::mkFor(size_t i, size_t step,
					ExpressionHolder<Expr> ex,
					bool hidden) const
	-> ExpressionHolder<Expr>
    {
	if(i != 0){
	    auto& ii = *mIVec[i].first;
	    return mkFor(i-1, step,  hidden ? ii.iforh(step, ex) : ii.ifor(step, ex));
	}
	else {
	    auto& ii = *mIVec[0].first;
	    return hidden ? ii.iforh(step, ex) : ii.ifor(step, ex);
	}		
    }
    */
    template <class EC>
    template <class Expr>
    ExpressionHolder<Expr> DynamicIndex<EC>::ifor(size_t step, Expr ex) const
    {
	if(mIVec.size() == 1){
	    return mIVec.back().first->ifori(step,ex);
	}
	else {
	    return ForMaker<Expr>::mk(mIVec.size()-2, step, mIVec.back().first->ifori(step,ex),
				      mIVec);
	}
    }

    template <class EC>
    template <class Expr>
    ExpressionHolder<Expr> DynamicIndex<EC>::iforh(size_t step, Expr ex) const
    {
	if(mIVec.size() == 1){
	    return mIVec.back().first->iforhi(step,ex);
	}
	else {
	    return ForMaker<Expr>::mk(mIVec.size()-2, step, mIVec.back().first->iforhi(step,ex),
				      mIVec, true);
	}
    }

    template <class EC>
    template <class Expr>
    ExpressionHolder<Expr> DynamicIndex<EC>::pifor(size_t step, Expr ex) const
    {
        return ifor(step, ex); // no multithreading here at the moment...
    }

    /***********************
     *   DynamicRange    *
     ***********************/

    template <class EC>
    typename DynamicRange<EC>::MetaType DynamicRange<EC>::get(size_t pos) const
    {
        std::vector<char> out(cmeta(nullptr,0));
        cmeta(out.data(),pos);
        return out;
    }

    template <class EC>
    size_t DynamicRange<EC>::getMeta(const MetaType& metaPos) const
    {
	return 0; // !!!
    }

    template <class EC>
    size_t DynamicRange<EC>::size() const
    {
	return mSize;
    }

    template <class EC>
    size_t DynamicRange<EC>::dim() const
    {
        return mOrig.size();
    }

    template <class EC>
    SpaceType DynamicRange<EC>::spaceType() const
    {
	return SpaceType::DYN;
    }

    template <class EC>
    bool DynamicRange<EC>::isEmpty() const
    {
	return mEmpty;
    }

    template <class EC>
    size_t DynamicRange<EC>::typeNum() const
    {
        return 0;
    }

    template <class EC>
    size_t DynamicRange<EC>::cmeta(char* target, size_t pos) const
    {
        size_t out = 0;
	for(size_t i = mOrig.size(); i != 0; --i) {
	    auto& x = mOrig[i-1];
	    const size_t redpos = pos % x->size();
            const size_t offset = x->cmeta(target+out,redpos);
            out += offset;
	    pos -= redpos;
	    pos /= x->size();
	}
        return out;
    }

    template <class EC>
    std::string DynamicRange<EC>::stringMeta(size_t pos) const
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

    template <class EC>
    vector<char> DynamicRange<EC>::data() const
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

    template <class EC>
    DataHeader DynamicRange<EC>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::DYN );
	h.metaSize = mOrig.size();
	h.multiple = 1;
        return h;
    }
    
    template <class EC>
    typename DynamicRange<EC>::IndexType DynamicRange<EC>::begin() const
    {
	DynamicIndex<EC> i
	    (std::dynamic_pointer_cast<DynamicRange>
	     ( std::shared_ptr<RangeBase>(RB::mThis) ) );
	i = 0;
	return i;
    }

    template <class EC>
    typename DynamicRange<EC>::IndexType DynamicRange<EC>::end() const
    {
	DynamicIndex<EC> i
	    (std::dynamic_pointer_cast<DynamicRange>
	     ( std::shared_ptr<RangeBase>(RB::mThis) ) );
	i = size();
	return i;
    }

    template <class EC>
    std::shared_ptr<RangeBase> DynamicRange<EC>::sub(size_t num) const
    {
	return mOrig.at(num);
    }

    template <class EC>
    void DynamicRange<EC>::sreplace(const std::shared_ptr<RangeBase> in, size_t num)
    {
	assert(mOrig[num]->size() == in->size());
	mOrig[num] = in;
    }

    template <class EC>
    template <class... RangeTypes>
    DynamicRange<EC>::DynamicRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<DynamicIndex<EC>>()
    {
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( origs, mOrig );
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( origs );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }

    template <class EC>
    template <class... RangeTypes>
    DynamicRange<EC>::DynamicRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<DynamicIndex<EC>>()
    {
	auto rst = std::make_tuple(origs...);
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( rst, mOrig );
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( rst );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }

    template <class EC>
    DynamicRange<EC>::DynamicRange(const vector<std::shared_ptr<RangeBase>>& origs) :
        RangeInterface<DynamicIndex<EC>>(),
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

    template <class EC>
    template <class Range>
    std::shared_ptr<Range> DynamicRange<EC>::fullsub(size_t num) const
    {
	return std::dynamic_pointer_cast<Range>( mOrig.at(num) );
    }

    template <class EC>
    template <class... Ranges>
    std::shared_ptr<MultiRange<Ranges...> > DynamicRange<EC>::scast(SIZET<Ranges>... sizes) const
    {
	std::tuple<std::shared_ptr<Ranges>...> rtp;
	RPackNum<sizeof...(Ranges)-1>::resolveRangeType(mOrig, rtp, 0, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
    }

    template <class EC>
    const vector<std::shared_ptr<RangeBase> >& DynamicRange<EC>::orig() const
    {
        return mOrig;
    }
    
} // end namespace MultiArrayTools
