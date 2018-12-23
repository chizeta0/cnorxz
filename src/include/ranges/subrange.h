
#ifndef __subrange_h__
#define __subrange_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <map>

#include "base_def.h"
//#include "ranges/rpack_num.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"

#include "xfor/for_type.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    template <class Index>
    class SubIndex : public IndexInterface<SubIndex<Index>,typename Index::MetaType>
    {
    public:
        
        typedef IndexInterface<SubIndex<Index>,typename Index::MetaType> IB;
        typedef typename Index::MetaType MetaType;
        typedef SubRange<typename Index::RangeType> RangeType;
        typedef SubIndex IType;

        SubIndex(const std::shared_ptr<RangeType>& range);

        static constexpr IndexType sType() { return IndexType::SINGLE; }
        static constexpr size_t totalDim() { return 1; }
        static constexpr size_t sDim() { return 1; }

        static constexpr SpaceType STYPE = Index::STYPE;

        IndexType type() const;
        
	SubIndex& operator=(size_t pos);
	SubIndex& operator++();
	SubIndex& operator--();

        SubIndex& operator()(const std::shared_ptr<Index>& ind); // set full index
        
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	const MetaType* metaPtr() const;
	SubIndex& at(const MetaType& metaPos);
	size_t posAt(const MetaType& metaPos) const;

	bool isMeta(const MetaType& metaPos) const;
	
	size_t dim(); // = 1
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
	    -> For<SubIndex<Index>,SubExpr<Index,Expr>>;

	template <class Expr>
	auto iforh(size_t step, Expr ex) const
	    -> For<SubIndex<Index>,SubExpr<Index,Expr>,ForType::HIDDEN>;

    private:
	std::shared_ptr<RangeType> mExplicitRangePtr;
	//const U* mMetaPtr;
        std::shared_ptr<Index> mFullIndex;
    };

    template <class Range>
    class SubRangeFactory : public RangeFactoryBase
    {
    public:
        typedef SubRange<Range> oType;

        SubRangeFactory() = delete;
        SubRangeFactory(const std::shared_ptr<Range>& fullRange,
                        const std::vector<size_t>& subset);
        std::shared_ptr<RangeBase> create();
    };
    
    template <class Range>
    class SubRange : public RangeInterface<SubIndex<typename Range::IndexType>>
    {
    public:
        typedef RangeBase RB;
        typedef SubIndex<typename Range::IndexType> IndexType;
        typedef SubRange RangeType;
        typedef typename IndexType::MetaType MetaType;
        typedef SubRangeFactory<Range> FType;

        virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;
	
	bool isMeta(const MetaType& metaPos) const;
	
	const MetaType& get(size_t pos) const;
	size_t getMeta(const MetaType& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

        std::shared_ptr<Range> fullRange() const;
        const std::vector<size_t>& subset() const;
        std::shared_ptr<SingleRange<MetaType,SpaceType::ANY>> outRange() const;
        
        friend SubRangeFactory<Range>;

        static constexpr bool defaultable = false;
        static constexpr size_t ISSTATIC = 0;
        static constexpr size_t SIZE = -1;
        static constexpr bool HASMETACONT = false;
        
    private:

        SubRange() = delete;
        SubRange(const SubRange& in) = delete;

        SubRange(const std::shared_ptr<Range>& fullRange, const std::vector<size_t>& subset);
        
        std::shared_ptr<Range> mFullRange;
        std::vector<size_t> mSubSet;
    };
    
} // namespace MultiArrayTools

namespace MultiArrayTools
{
    
    /*****************
     *   SubIndex    *
     *****************/

    template <class Index>
    SubIndex<Index>::SubIndex(const std::shared_ptr<RangeType>& range) :
        IndexInterface<SubIndex<Index>,typename Index::MetaType>(range, 0),
        mExplicitRangePtr(std::dynamic_pointer_cast<RangeType>(IB::mRangePtr))
    {
        mFullIndex = std::make_shared<Index>(mExplicitRangePtr->fullRange());
    }


    template <class Index>
    IndexType SubIndex<Index>::type() const
    {
        return IndexType::SINGLE;
    }

    template <class Index>
    SubIndex<Index>& SubIndex<Index>::operator=(size_t pos)
    {
        IB::mPos = pos;
        (*mFullIndex) = mExplicitRangePtr->subset()[IB::mPos];
        return *this;
    }

    template <class Index>
    SubIndex<Index>& SubIndex<Index>::operator++()
    {
        ++IB::mPos;
        (*mFullIndex) = mExplicitRangePtr->subset()[IB::mPos];
        return *this;
    }

    template <class Index>
    SubIndex<Index>& SubIndex<Index>::operator--()
    {
        --IB::mPos;
        (*mFullIndex) = mExplicitRangePtr->subset()[IB::mPos];
        return *this;
    }

    template <class Index>
    SubIndex<Index>& SubIndex<Index>::operator()(const std::shared_ptr<Index>& ind)
    {
        assert(mFullIndex->range() == ind->range());
        mFullIndex = ind;
        return *this;
    }
    
    template <class Index>
    int SubIndex<Index>::pp(std::intptr_t idxPtrNum)
    {
        ++(*this);
        return 1;
    }

    template <class Index>
    int SubIndex<Index>::mm(std::intptr_t idxPtrNum)
    {
        --(*this);
        return 1;
    }

    template <class Index>
    std::string SubIndex<Index>::stringMeta() const
    {
        return std::dynamic_pointer_cast<SingleRange<MetaType,STYPE> const>( IB::mRangePtr )->stringMeta(IB::mPos);
    }

    template <class Index>
    typename SubIndex<Index>::MetaType SubIndex<Index>::meta() const
    {
        MetaType* x = nullptr;
        return MetaPtrHandle<SubIndex<Index>::RangeType::HASMETACONT>::getMeta
	    ( x, IB::mPos, mExplicitRangePtr );
    }

    template <class Index>
    const typename  SubIndex<Index>::MetaType* SubIndex<Index>::metaPtr() const
    {
        assert(0); // not sure where it is used
        return mFullIndex->metaPtr();
    }

    template <class Index>
    SubIndex<Index>& SubIndex<Index>::at(const MetaType& metaPos)
    {
        (*this) = mExplicitRangePtr->getMeta( metaPos );
        return *this;
    }

    template <class Index>
    size_t SubIndex<Index>::posAt(const MetaType& metaPos) const
    {
        return mExplicitRangePtr->getMeta( metaPos );
    }

    template <class Index>
    bool SubIndex<Index>::isMeta(const MetaType& metaPos) const
    {
        return mExplicitRangePtr->isMeta( metaPos );
    }

    template <class Index>
    size_t SubIndex<Index>::dim()
    {
        return 1;
    }

    template <class Index>
    bool SubIndex<Index>::last()
    {
        return IB::mPos == IB::mMax - 1;
    }

    template <class Index>
    bool SubIndex<Index>::first()
    {
        return IB::mPos == 0;
    }

    template <class Index>
    std::shared_ptr<typename SubIndex<Index>::RangeType> SubIndex<Index>::range()
    {
        return mExplicitRangePtr;
    }

    template <class Index>
    template <size_t N>
    void SubIndex<Index>::getPtr() {}

    template <class Index>
    size_t SubIndex<Index>::getStepSize(size_t n)
    {
        return 1;
    }

    template <class Index>
    std::string SubIndex<Index>::id() const
    {
        return std::string("sub") + std::to_string(IB::mId);
    }

    template <class Index>
    void SubIndex<Index>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " << meta() << std::endl;
    }

    template <class Index>
    template <class Expr>
    auto SubIndex<Index>::ifor(size_t step, Expr ex) const
        -> For<SubIndex<Index>,SubExpr<Index,Expr>>
    {
        return For<SubIndex<Index>,SubExpr<Index,Expr>>
            (this, step, SubExpr<Index,Expr>
	     ( mFullIndex, reinterpret_cast<std::intptr_t>(this),
	       &mExplicitRangePtr->subset(), ex ) );
    }

    template <class Index>
    template <class Expr>
    auto SubIndex<Index>::iforh(size_t step, Expr ex) const
	-> For<SubIndex<Index>,SubExpr<Index,Expr>,ForType::HIDDEN>
    {
        return For<SubIndex<Index>,SubExpr<Index,Expr>,ForType::HIDDEN>
            (this, step, SubExpr<Index,Expr>
	     ( mFullIndex, reinterpret_cast<std::intptr_t>(this),
	       &mExplicitRangePtr->subset(), ex ) );
    }

    
    /************************
     *   SubRangeFactory    *
     ************************/

    template <class Range>
    SubRangeFactory<Range>::SubRangeFactory(const std::shared_ptr<Range>& fullRange,
                                            const std::vector<size_t>& subset)
    {
        mProd = std::shared_ptr<oType>( new SubRange<Range>( fullRange, subset ) );
    }

    template <class Range>
    std::shared_ptr<RangeBase> SubRangeFactory<Range>::create()
    {
        setSelf();
        return mProd;
    }

    /*****************
     *   SubRange    *
     *****************/

    template <class Range>
    SubRange<Range>::SubRange(const std::shared_ptr<Range>& fullRange,
                              const std::vector<size_t>& subset) :
        RangeInterface<SubIndex<typename Range::IndexType>>(),
        mFullRange(fullRange), mSubSet(subset) {}

    template <class Range>
    size_t SubRange<Range>::size() const
    {
        return mSubSet.size();
    }

    template <class Range>
    size_t SubRange<Range>::dim() const
    {
        return 1;
    }

    template <class Range>
    SpaceType SubRange<Range>::spaceType() const
    {
        return SpaceType::ANY;
    }

    template <class Range>
    DataHeader SubRange<Range>::dataHeader() const
    {
        DataHeader h;
        h.spaceType = static_cast<int>( SpaceType::ANY );
        h.metaSize = metaSize(mSubSet);
        h.metaType = NumTypeMap<MetaType>::num;
        h.multiple = 0;
        return h;
    }

    template <class Range>
    std::string SubRange<Range>::stringMeta(size_t pos) const
    {
        return xToString(get(pos));
    }

    template <class Range>
    std::vector<char> SubRange<Range>::data() const
    {
        DataHeader h = dataHeader();
        std::vector<char> out;
        out.reserve(h.metaSize + sizeof(DataHeader));
        char* hcp = reinterpret_cast<char*>(&h);
        out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
        std::vector<MetaType> subvec(mSubSet.size());
        size_t i = 0;
        for(auto& x: mSubSet){
            subvec[i++] = mFullRange->get(x);
        }
        stringCat(out, subvec);
        return out;
    }

    template <class Range>
    bool SubRange<Range>::isMeta(const MetaType& metaPos) const
    {
        for(size_t i = 0; i != size(); ++i){
            if(get(i) == metaPos){
                return true;
            }
        }
        return false;
    }

    template <class Range>
    const typename SubRange<Range>::MetaType& SubRange<Range>::get(size_t pos) const
    {
        return mFullRange->get( mSubSet[pos] );
    }

    template <class Range>
    size_t SubRange<Range>::getMeta(const MetaType& metaPos) const
    {
        for(size_t i = 0; i != size(); ++i){
            if(get(i) == metaPos){
                return i;
            }
        }
        return -1;
    }

    template <class Range>
    typename SubRange<Range>::IndexType SubRange<Range>::begin() const
    {
        SubRange<Range>::IndexType i( std::dynamic_pointer_cast<SubRange<Range>>
                                      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
        i = 0;
        return i;
    }

    template <class Range>
    typename SubRange<Range>::IndexType SubRange<Range>::end() const
    {
        SubRange<Range>::IndexType i( std::dynamic_pointer_cast<SubRange<Range>>
                                      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
        i = size();
        return i;
    }

    template <class Range>
    std::shared_ptr<Range> SubRange<Range>::fullRange() const
    {
        return mFullRange;
    }

    template <class Range>
    const std::vector<size_t>& SubRange<Range>::subset() const
    {
        return mSubSet;
    }

    template <class Range>
    std::shared_ptr<SingleRange<typename SubRange<Range>::MetaType,SpaceType::ANY>> SubRange<Range>::outRange() const
    {
        std::vector<MetaType> ometa(mSubSet.size());
        size_t i = 0;
        for(auto& x: mSubSet){
            ometa[i++] = mFullRange->get(x);
        }
        SingleRangeFactory<MetaType,SpaceType::ANY> srf(ometa);
        return std::dynamic_pointer_cast<SingleRange<MetaType,SpaceType::ANY>>( srf.create() );
    }
}

#endif
