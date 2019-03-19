
#ifndef __dynamic_range_h__
#define __dynamic_range_h__

#include <cassert>

#include "ranges/rbase_def.h"
#include "ranges/range_base.h"
#include "ranges/index_base.h"

#include "xfor/xfor.h"

#include <map>
//#include "ranges/rpheader.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"

#include "ranges/dynamic_meta.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }
    //using MultiArrayHelper::DynamicExpression;

    //using MultiArrayHelper::ExpressionHolder;

    class AbstractIW
    {
    public:
        AbstractIW() = default;
        AbstractIW(const AbstractIW& in) = default;
        AbstractIW(AbstractIW&& in) = default;
        AbstractIW& operator=(const AbstractIW& in) = default;
        AbstractIW& operator=(AbstractIW&& in) = default;

        virtual IndexType type() const = 0;
	
	virtual AbstractIW& operator=(size_t pos) = 0;
	virtual AbstractIW& operator++() = 0;
	virtual AbstractIW& operator--() = 0;

	virtual int pp(std::intptr_t idxPtrNum) = 0;
	virtual int mm(std::intptr_t idxPtrNum) = 0;

	virtual std::string stringMeta() const = 0;
	
        virtual size_t pos() const = 0;
        virtual size_t max() const = 0;
	virtual size_t dim() const = 0;
	virtual bool last() const = 0;
	virtual bool first() const = 0;

	virtual std::shared_ptr<RangeBase> range() const = 0;
        
        virtual size_t getStepSize(size_t n) const = 0;
        virtual size_t getStepSizeComp(std::intptr_t j) const = 0;
        
        virtual std::intptr_t get() const = 0;
	virtual std::intptr_t ptrNum() const = 0;

        //virtual DynamicMetaT meta() const = 0;
	//virtual const DynamicMetaT* metaPtr() const = 0;
	//virtual AbstractIW& at(const U& metaPos) = 0;
	//virtual size_t posAt(const U& metaPos) const = 0;

	//virtual bool isMeta(const U& metaPos) const = 0;
	inline AbstractIW& at(const std::string smeta)
	{
	    for((*this) = 0; this->pos() != this->max(); ++(*this)){
		if(this->stringMeta() == smeta){
		    break;
		}
	    }
	    return *this;
	}
    };
    
    template <class ExpressionCollection>
    class IndexWrapperBase : public AbstractIW
    {
    protected:
        std::shared_ptr<ExpressionCollection> mEc;
    public:

        IndexWrapperBase() = default;
        IndexWrapperBase(const IndexWrapperBase& in) = default;
        IndexWrapperBase(IndexWrapperBase&& in) = default;
        IndexWrapperBase& operator=(const IndexWrapperBase& in) = default;
        IndexWrapperBase& operator=(IndexWrapperBase&& in) = default;

        using AbstractIW::operator=;
        
        template <class Expr>
        ExpressionHolder<Expr> ifor(size_t step, ExpressionHolder<Expr> ex) const;

        template <class Expr>
        ExpressionHolder<Expr> iforh(size_t step, ExpressionHolder<Expr> ex) const;

	template <class Expr>
        ExpressionHolder<Expr> ifori(size_t step, Expr ex) const;

        template <class Expr>
        ExpressionHolder<Expr> iforhi(size_t step, Expr ex) const;

    };

    template <class EC>
    using IndexW = IndexWrapperBase<EC>;
    
    template <class Index, class ExpressionCollection>
    class IndexWrapper : public IndexWrapperBase<ExpressionCollection>
    {
    public:
        typedef IndexWrapperBase<ExpressionCollection> IWB;
	typedef typename Index::MetaType MetaType;

	static constexpr IndexType sType() { return IndexType::SINGLE; }
	
    protected:
        IndexWrapper() = default;

    private:
        std::shared_ptr<Index> mI;
    public:

        IndexWrapper(const IndexWrapper& in) = default;
        IndexWrapper(IndexWrapper&& in) = default;
        IndexWrapper& operator=(const IndexWrapper& in) = default;
        IndexWrapper& operator=(IndexWrapper&& in) = default;

        IndexWrapper(const std::shared_ptr<Index>& i) : mI(i)
        { IWB::mEc = ExpressionCollection::make(mI); }
        
        virtual IndexType type() const final { return mI->type(); }
	
	virtual IndexWrapper& operator=(size_t pos) override final { (*mI) = pos; return *this; }
	virtual IndexWrapper& operator++() override final { ++(*mI); return *this; }
	virtual IndexWrapper& operator--() override final { --(*mI); return *this; }

        virtual size_t pos() const override final { return mI->pos(); }
        virtual size_t max() const override final { return mI->max(); }
        
	virtual int pp(std::intptr_t idxPtrNum) override final { return mI->pp(idxPtrNum); }
	virtual int mm(std::intptr_t idxPtrNum) override final { return mI->mm(idxPtrNum); }

	virtual std::string stringMeta() const override final { return mI->stringMeta(); }
	//virtual DynamicMetaT meta() const final { return DynamicMetaT(mI->meta()); }
	//virtual const DynamicMetaT* metaPtr() const final { return nullptr; }
	IndexWrapper& at(const typename Index::MetaType& metaPos) { mI->at(metaPos); return *this; }
	size_t posAt(const typename Index::MetaType& metaPos) const { return mI->posAt(metaPos); }

	//virtual bool isMeta(const U& metaPos) const final { return mI->isMeta(); }
	
	virtual size_t dim() const override final { return mI->dim(); }
	virtual bool last() const override final { return mI->last(); }
	virtual bool first() const override final { return mI->first(); }

	virtual std::shared_ptr<RangeBase> range() const override final { return mI->range(); }
        
        virtual size_t getStepSize(size_t n) const override final { return mI->getStepSize(n); }
        virtual size_t getStepSizeComp(std::intptr_t j) const override final;
        
        virtual std::intptr_t get() const override final { return reinterpret_cast<std::intptr_t>(mI.get()); }
	virtual std::intptr_t ptrNum() const override final { return mI->ptrNum(); }
	
    };
    
    //typedef SingleRange<size_t,SpaceType::DYN> DynamicRange;

    template <class EC>
    class DynamicIndex : public IndexInterface<DynamicIndex<EC>,vector<char>>
    {
    private:
        typedef vector<std::pair<std::shared_ptr<IndexW<EC>>,size_t>> IVecT;
        
        IVecT mIVec;
        bool mIvecInit = false;
        
    public:
        typedef IndexInterface<DynamicIndex<EC>,vector<char>> IB;
	typedef vector<char> MetaType;
	typedef DynamicRange<EC> RangeType;
	typedef DynamicIndex IType;

        DynamicIndex(const std::shared_ptr<RangeType>& range);

        static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }

	static constexpr SpaceType STYPE = SpaceType::DYN;

        IndexType type() const;
	
	DynamicIndex& operator=(size_t pos);
	DynamicIndex& operator++();
	DynamicIndex& operator--();

        DynamicIndex& operator()(const IVecT& ivec);
        DynamicIndex& operator()(const vector<std::shared_ptr<IndexW<EC>>>& ivec);
		
	template <class... Indices>
	DynamicIndex& operator()(const std::shared_ptr<Indices>&... is);

	DynamicIndex<EC>& sync();
	
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	const MetaType* metaPtr() const;
	DynamicIndex& at(const MetaType& metaPos);
	size_t posAt(const MetaType& metaPos) const;

	//bool isMeta(const MetaType& metaPos) const;
	
	size_t dim() const;
	bool last() const;
	bool first() const;

        const IndexW<EC>& get(size_t n) const;

	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	void getPtr();

	size_t getStepSize(size_t n) const;
        
	std::string id() const;
	void print(size_t offset);

	template <class Expr>
	ExpressionHolder<Expr> ifor(size_t step, Expr ex) const;

	template <class Expr>
	ExpressionHolder<Expr> iforh(size_t step, Expr ex) const;

        template <class Expr>
	ExpressionHolder<Expr> pifor(size_t step, Expr ex) const;

    };    

    
    // NOT THREAD SAVE!!
    template <class EC>
    class DynamicRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef DynamicRange<EC> oType;

	DynamicRangeFactory();

	template <class... RangeTypes>
	DynamicRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	DynamicRangeFactory(std::shared_ptr<RangeTypes>... origs);

	DynamicRangeFactory(const vector<std::shared_ptr<RangeBase>>& origs);

	template <class Range>
	void append(std::shared_ptr<Range> r);
	
	std::shared_ptr<RangeBase> create();

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const vector<std::shared_ptr<RangeBase> >& pvec);
	
	static std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > mAleadyCreated;

	bool mProductCreated = false;
    };

    template <class EC>
    class DynamicRange : public RangeInterface<DynamicIndex<EC>>
    {
    public:
        static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	typedef RangeBase RB;
	typedef DynamicIndex<EC> IndexType;
	typedef DynamicRange RangeType;
	typedef vector<char> MetaType;
        typedef DynamicRangeFactory<EC> FType;
        
    private:
	DynamicRange() = default;
	DynamicRange(const DynamicRange& in) = default;

	template <class... RangeTypes>
	DynamicRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	DynamicRange(std::shared_ptr<RangeTypes>... origs);

        DynamicRange(const vector<std::shared_ptr<RangeBase>>& origs);

	size_t mSize = 1;
	bool mEmpty = true;
	
	vector<std::shared_ptr<RangeBase> > mOrig;

    public:
	virtual size_t size() const final;
	virtual size_t dim() const final;

	MetaType get(size_t pos) const;
	size_t getMeta(const MetaType& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	std::shared_ptr<RangeBase> sub(size_t num) const;

	template <class Range>
	std::shared_ptr<Range> fullsub(size_t num) const;

	template <class... Ranges>
	std::shared_ptr<MultiRange<Ranges...> > scast(SIZET<Ranges>... sizes) const; // save cast

        const vector<std::shared_ptr<RangeBase> >& orig() const;
        
	void sreplace(const std::shared_ptr<RangeBase> in, size_t num);
	
	bool isEmpty() const;
	
	friend DynamicRangeFactory<EC>;
	
	static DynamicRangeFactory<EC> factory()
	{ return DynamicRangeFactory<EC>(); }

    };

} // namespace MultiArrayTools


namespace MultiArrayHelper
{
    using namespace MultiArrayTools;

    template <class EC>
    inline void resolveSetRange(std::shared_ptr<DynamicRange<EC>>& rp,
                                const vector<std::shared_ptr<RangeBase> >& orig,
                                size_t origpos, size_t size)
    {
    	DynamicRangeFactory<EC> arf;
	for(size_t op = origpos; op != origpos + size; ++op){
	    //VCHECK(op);
	    arf.append(orig[op]);
	}
    	rp = std::dynamic_pointer_cast<DynamicRange<EC>>( arf.create() );
    }

    template <class EC>
    inline void setRangeToVec(vector<std::shared_ptr<RangeBase> >& v,
                              std::shared_ptr<DynamicRange<EC>> r)
    {
	if(not r->isEmpty()){
	    for(size_t i = r->dim(); i != 0; --i){
		v.insert(v.begin(), r->sub(i-1));
	    }
	}
    }

    template <class EC>
    inline size_t getStepSize(const DynamicIndex<EC>& ii, std::intptr_t j)
    {
        size_t ss = 0;
        size_t sx = 1;
        for(size_t k = ii.dim(); k != 0; --k){
	    const size_t i = k-1;
            const auto& ni = ii.get(i);
            const size_t max = ni.max();
            const size_t tmp = ni.getStepSizeComp(j);
            ss += tmp * ii.getStepSize(i);
            sx *= max;
        }
        return ss;
    }

}

//#include "dynamic_range.cc.h"

#endif
