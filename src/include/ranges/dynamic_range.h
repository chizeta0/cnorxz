
#ifndef __dynamic_range_h__
#define __dynamic_range_h__

#include "ranges/rbase_def.h"
#include "ranges/range_base.h"
#include "ranges/index_base.h"

namespace MultiArrayTools
{
    typedef std::pair<const char*,size_t> DynamicMetaElem;
    
    class DynamicMetaT
    {
    private:
        std::vector<DynamicMetaElem> mMeta;

    public:
        DynamicMetaT() = default;
        DynamicMetaT(const DynamicMetaT& in) = default;
        DynamicMetaT(DynamicMetaT&& in) = default;
        DynamicMetaT& operator=(const DynamicMetaT& in) = default;
        DynamicMetaT& operator=(DynamicMetaT&& in) = default;

        template <typename... Us>
        DynamicMetaT(const std::tuple<Us...>& meta);
        
        bool operator==(const DynamicMetaT& in) const;
        bool operator!=(const DynamicMetaT& in) const;

        DynamicMetaElem& operator[](size_t pos);
        const DynamicMetaElem& operator[](size_t pos) const;
    };


    class IndexWrapperBase
    {
    public:

        IndexWrapperBase() = default;
        IndexWrapperBase(const IndexWrapperBase& in) = default;
        IndexWrapperBase(IndexWrapperBase&& in) = default;
        IndexWrapperBase& operator=(const IndexWrapperBase& in) = default;
        IndexWrapperBase& operator=(IndexWrapperBase&& in) = default;
        
        virtual IndexType type() const = 0;
	
	virtual IndexWrapperBase& operator=(size_t pos) = 0;
	virtual IndexWrapperBase& operator++() = 0;
	virtual IndexWrapperBase& operator--() = 0;

	virtual int pp(std::intptr_t idxPtrNum) = 0;
	virtual int mm(std::intptr_t idxPtrNum) = 0;

	virtual std::string stringMeta() const = 0;
	virtual DynamicMetaT meta() const = 0;
	virtual const DynamicMetaT* metaPtr() const = 0;
	//virtual IndexWrapperBase& at(const U& metaPos) = 0;
	//virtual size_t posAt(const U& metaPos) const = 0;

	//virtual bool isMeta(const U& metaPos) const = 0;
	
	virtual size_t dim() const = 0;
	virtual bool last() const = 0;
	virtual bool first() const = 0;

	virtual std::shared_ptr<RangeBase> range() const = 0;
        
        virtual size_t getStepSize(size_t n) const = 0;

        virtual std::intptr_t get() const = 0;

	virtual DynamicalExpression ifor(size_t step, DynamicalExpression ex) const = 0;
	virtual DynamicalExpression iforh(size_t step, DynamicalExpression ex) const = 0;
    };

    typedef IndexWrapperBase IndexW;
    
    template <class Index, class Expr>
    class IndexWrapper : public IndexWrapperBase
    {
    protected:
        IndexWrapper() = default;

    private:
        std::shared_ptr<Index> mI;
    public:

        IndexWrapper(const IndexWrapper& in) = default;
        IndexWrapper(IndexWrapper&& in) = default;
        IndexWrapper& operator=(const IndexWrapper& in) = default;
        IndexWrapper& operator=(IndexWrapper&& in) = default;

        IndexWrapper(const std::shared_ptr<Index>& i) : mI(i) {}
        
        virtual IndexType type() const final { return mI->type(); }
	
	virtual IndexWrapperBase& operator=(size_t pos) final { (*mI) = pos; return *this; }
	virtual IndexWrapperBase& operator++() final { ++(*mI); return *this; }
	virtual IndexWrapperBase& operator--() final { --(*mI); return *this; }

	virtual int pp(std::intptr_t idxPtrNum) final { return mI->pp(idxPtrNum); }
	virtual int mm(std::intptr_t idxPtrNum) final { return mI->mm(idxPtrNum); }

	virtual std::string stringMeta() const final { return mI->stringMeta(); }
	virtual DynamicMetaT meta() const final { return DynamicMetaT(mI->meta()); }
	virtual const DynamicMetaT* metaPtr() const final { return nullptr; }
	IndexWrapperBase& at(const typename Index::MetaType& metaPos) { mI->at(metaPos); return *this; }
	size_t posAt(const typename Index::MetaType& metaPos) const { return mI->posAt(metaPos); }

	//virtual bool isMeta(const U& metaPos) const final { return mI->isMeta(); }
	
	virtual size_t dim() const final { return mI->dim(); }
	virtual bool last() const final { return mI->last(); }
	virtual bool first() const final { return mI->first(); }

	virtual std::shared_ptr<RangeBase> range() const final { return mI->range(); }
        
        virtual size_t getStepSize(size_t n) const final { return mI->getStepSize(n); }

        virtual std::intptr_t get() const final { return reinterpret_cast<std::intptr_t>(mI.get()); }
	
	virtual DynamicalExpression ifor(size_t step, DynamicalExpression ex) const final { return mI->ifor(step, ex); }
	virtual DynamicalExpression iforh(size_t step, DynamicalExpression ex) const final { return mI->iforh(step, ex); }
	
    };
    
    typedef SingleRange<size_t,SpaceType::DYN> DynamicRange;
    
    class DynamicIndex : public IndexInterface<DynamicIndex,DynamicMetaT>
    {
    private:
        std::vector<std::pair<std::shared_ptr<IndexW>,size_t>> mIVec;

	inline DynamicalExpression mkFor(size_t i, size_t step,
					 DynamicalExpression ex, bool hidden = false) const;	

    public:
        typedef IndexInterface<DynamicIndex,DynamicMetaT> IB;
	typedef DynamicMetaT MetaType;
	typedef DynamicRange RangeType;
	typedef DynamicIndex IType;

        DynamicIndex(const std::shared_ptr<DynamicRange>& range);

        static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }

	static constexpr SpaceType STYPE = SpaceType::DYN;

        IndexType type() const;
	
	DynamicIndex& operator=(size_t pos);
	DynamicIndex& operator++();
	DynamicIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	const MetaType* metaPtr() const;
	DynamicIndex& at(const MetaType& metaPos);
	size_t posAt(const MetaType& metaPos) const;

	//bool isMeta(const MetaType& metaPos) const;
	
	size_t dim();
	bool last();
	bool first();

	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	void getPtr();

	size_t getStepSize(size_t n);
	
	std::string id() const;
	void print(size_t offset);

	template <class Expr>
	auto ifor(size_t step, Expr ex) const
	    -> DynamicalExpression;

	template <class Expr>
	auto iforh(size_t step, Expr ex) const
	    -> DynamicalExpression;

    };    

    
    // NOT THREAD SAVE!!
    class DynamicRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef DynamicRange oType;

	DynamicRangeFactory();

	template <class... RangeTypes>
	DynamicRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	DynamicRangeFactory(std::shared_ptr<RangeTypes>... origs);

	template <class Range>
	void append(std::shared_ptr<Range> r);
	
	std::shared_ptr<RangeBase> create();

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const std::vector<std::shared_ptr<RangeBase> >& pvec);
	
	static std::map<std::shared_ptr<RangeBase>,std::vector<std::intptr_t> > mAleadyCreated;

	bool mProductCreated = false;
    };

    template <>
    class SingleRange<size_t,SpaceType::DYN> : public RangeInterface<DynamicIndex>
    {
    public:
        static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	typedef RangeBase RB;
	typedef DynamicIndex IndexType;
	typedef DynamicRange RangeType;
	typedef DynamicMetaT MetaType;

    private:
	SingleRange() = default;
	SingleRange(const SingleRange& in) = default;

	template <class... RangeTypes>
	SingleRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	SingleRange(std::shared_ptr<RangeTypes>... origs);

	size_t mSize = 1;
	bool mEmpty = true;
	
	std::vector<std::shared_ptr<RangeBase> > mOrig;

    public:
	virtual size_t size() const final;
	virtual size_t dim() const final;

	MetaType get(size_t pos) const;
	size_t getMeta(const MetaType& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	virtual SpaceType spaceType() const final;
	
	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;
	
	std::shared_ptr<RangeBase> sub(size_t num) const;

	template <class Range>
	std::shared_ptr<Range> fullsub(size_t num) const;

	template <class... Ranges>
	std::shared_ptr<MultiRange<Ranges...> > scast(SIZET<Ranges>... sizes) const; // save cast

	void sreplace(const std::shared_ptr<RangeBase> in, size_t num);
	
	bool isEmpty() const;
	
	friend DynamicRangeFactory;
	
	static DynamicRangeFactory factory()
	{ return DynamicRangeFactory(); }

    };

} // namespace MultiArrayTools


/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
       
    /***********************
     *   DynamicRange    *
     ***********************/
    
    template <class... RangeTypes>
    DynamicRangeFactory::DynamicRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs)
    {
	mProd = std::shared_ptr<oType>( new DynamicRange( origs ) );
    }

    template <class... RangeTypes>
    DynamicRangeFactory::DynamicRangeFactory(std::shared_ptr<RangeTypes>... origs)
    {
	mProd = std::shared_ptr<oType>( new DynamicRange( origs... ) );
    }

    template <class Range>
    void DynamicRangeFactory::append(std::shared_ptr<Range> r)
    {
	if(mProductCreated){
	    
	    mProd = std::shared_ptr<oType>( new DynamicRange( *std::dynamic_pointer_cast<oType>(mProd) ) );
	    mProductCreated = false;
	}
	std::dynamic_pointer_cast<oType>(mProd)->mOrig.push_back(r);
	std::dynamic_pointer_cast<oType>(mProd)->mSize *= r->size();
	std::dynamic_pointer_cast<oType>(mProd)->mEmpty = false;
    }

    /*****************
     *   Functions   *
     *****************/

    //std::shared_ptr<DynamicRange> defaultRange(size_t size = 0);
}

namespace MultiArrayHelper
{
    using namespace MultiArrayTools;

    template <>
    inline void resolveSetRange<DynamicRange>(std::shared_ptr<DynamicRange>& rp,
						const std::vector<std::shared_ptr<RangeBase> >& orig,
						size_t origpos, size_t size)
    {
    	DynamicRangeFactory arf;
	for(size_t op = origpos; op != origpos + size; ++op){
	    //VCHECK(op);
	    arf.append(orig[op]);
	}
    	rp = std::dynamic_pointer_cast<DynamicRange>( arf.create() );
    }

    template <>
    inline void setRangeToVec<DynamicRange>(std::vector<std::shared_ptr<RangeBase> >& v,
					      std::shared_ptr<DynamicRange> r)
    {
	if(not r->isEmpty()){
	    for(size_t i = r->dim(); i != 0; --i){
		v.insert(v.begin(), r->sub(i-1));
	    }
	}
    }

}

namespace MultiArrayTools
{
    /***********************
     *   DynamicRange    *
     ***********************/

    template <class... RangeTypes>
    SingleRange<size_t,SpaceType::DYN>::SingleRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<DynamicIndex>()
    {
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( origs, mOrig );
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( origs );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }
    
    template <class... RangeTypes>
    SingleRange<size_t,SpaceType::DYN>::SingleRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<DynamicIndex>()
    {
	auto rst = std::make_tuple(origs...);
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( rst, mOrig );
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( rst );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }

    template <class Range>
    std::shared_ptr<Range> SingleRange<size_t,SpaceType::DYN>::fullsub(size_t num) const
    {
	return std::dynamic_pointer_cast<Range>( mOrig.at(num) );
    }

    template <class... Ranges>
    std::shared_ptr<MultiRange<Ranges...> > SingleRange<size_t,SpaceType::DYN>::scast(SIZET<Ranges>... sizes) const
    {
	std::tuple<std::shared_ptr<Ranges>...> rtp;
	RPackNum<sizeof...(Ranges)-1>::resolveRangeType(mOrig, rtp, 0, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
    }

    inline DynamicalExpression DynamicIndex::mkFor(size_t i, size_t step,
						   DynamicalExpression ex, bool hidden) const
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
    
    template <class Expr>
    auto DynamicIndex::ifor(size_t step, Expr ex) const
        -> DynamicalExpression
    {
	DynamicalExpression expr(std::make_shared<Expr>(ex));
	return mkFor(mIVec.size()-1, step, expr);
    }
    
    template <class Expr>
    auto DynamicIndex::iforh(size_t step, Expr ex) const
        -> DynamicalExpression
    {
	DynamicalExpression expr(std::make_shared<Expr>(ex));
	return mkFor(mIVec.size()-1, step, expr, true);
    }
    
}

#endif
