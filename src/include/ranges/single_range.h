// -*- C++ -*-

#ifndef __single_range_h__
#define __single_range_h__

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
//#include "xfor/xfor.h"



namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    
    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleIndex : public IndexInterface<GenSingleIndex<U,TYPE,S>,U>
    {
    public:

	typedef IndexInterface<GenSingleIndex<U,TYPE,S>,U> IB;
	typedef U MetaType;
	typedef GenSingleRange<U,TYPE,S> RangeType;
	typedef GenSingleIndex IType;

	//DEFAULT_MEMBERS_X(GenSingleIndex);
	
	GenSingleIndex(const std::shared_ptr<GenSingleRange<U,TYPE,S> >& range);

	static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }

	static constexpr SpaceType STYPE = TYPE;
        static constexpr bool PARALLEL = true; 
        
	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	GenSingleIndex& operator=(size_t pos);
	GenSingleIndex& operator++();
	GenSingleIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	U meta() const;
	const U* metaPtr() const;
	GenSingleIndex& at(const U& metaPos);
	size_t posAt(const U& metaPos) const;

	bool isMeta(const U& metaPos) const;
	
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
	    -> For<GenSingleIndex<U,TYPE,S>,Expr>;

	template <class Expr>
	auto iforh(size_t step, Expr ex) const
	    -> For<GenSingleIndex<U,TYPE,S>,Expr,ForType::HIDDEN>;

        template <class Expr>
	auto pifor(size_t step, Expr ex) const
	    -> PFor<GenSingleIndex<U,TYPE,S>,Expr>;
        
    private:
	std::shared_ptr<RangeType> mExplicitRangePtr;
	const U* mMetaPtr;
    };

    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef GenSingleRange<U,TYPE,S> oType;

	GenSingleRangeFactory() = delete;
	GenSingleRangeFactory(const std::vector<U>& space);
	std::shared_ptr<RangeBase> create();
	
    };

    template <typename U>
    class MetaMap
    {
    private:
	std::map<U,size_t> mMap;
    public:
	MetaMap() = default;
	MetaMap(const MetaMap& in) = default;
	MetaMap(MetaMap&& in) = default;
	MetaMap& operator=(const MetaMap& in) = default;
	MetaMap& operator=(MetaMap&& in) = default;

	//MetaMap(const std::map<U,size_t>& in) : mMap(in) {}
	MetaMap(const std::vector<U>& in)
	{
	    for(size_t i = 0; i != in.size(); ++i){
		mMap[in[i]] = i;
	    }
	}
	
	size_t at(const U& in) const { return mMap.at(in); }
	size_t count(const U& in) const { return mMap.count(in); }
    };
    
    template <>
    class MetaMap<std::array<int,2> >
    {
    private:
	std::vector<size_t> mMap;
	int min1;
	int min2;
	int max1;
	int max2;
	size_t s1;
	size_t s2;
    public:
	typedef std::array<int,2> U;
	
	MetaMap() = default;
	MetaMap(const MetaMap& in) = default;
	MetaMap(MetaMap&& in) = default;
	MetaMap& operator=(const MetaMap& in) = default;
	MetaMap& operator=(MetaMap&& in) = default;

	MetaMap(const std::vector<U>& in) : min1(in[0][0]),
					    min2(in[0][1]),
					    max1(in[0][0]),
					    max2(in[0][1])
	{
	    for(auto& x: in){
		if(min1 > x[0]) min1 = x[0];
		if(min2 > x[1]) min2 = x[1];
		if(max1 < x[0]+1) max1 = x[0]+1;
		if(max2 < x[1]+1) max2 = x[1]+1;
	    }
	    s1 = max1 - min1;
	    s2 = max2 - min2;
	    mMap.resize(s1*s2,-1);
	    for(size_t i = 0; i != in.size(); ++i){
		const size_t mpos = (in[i][0] - min1) * s2 + (in[i][1] - min2);
		mMap[ mpos ] = i;
	    }
	}

	size_t at(const U& in) const
	{
	    //CHECK;
	    const size_t mpos = (in[0] - min1) * s2 + (in[1] - min2);
	    assert(mpos < mMap.size());
	    assert(mMap[ mpos ] != static_cast<size_t>( -1 ) );
	    return mMap[ mpos ];
	}
	
    };

    template <size_t S>
    struct CheckStatic
    {
        static constexpr size_t ISSTATIC = true;
        static constexpr size_t SIZE = S;
    };

    template <>
    struct CheckStatic<-1>
    {
        static constexpr size_t ISSTATIC = false;
        static constexpr size_t SIZE = -1;
    };

    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleRange : public RangeInterface<GenSingleIndex<U,TYPE,S> >
    {
    public:
	typedef RangeBase RB;
	typedef GenSingleIndex<U,TYPE,S> IndexType;
	typedef GenSingleRange RangeType;
	typedef U MetaType;
	typedef GenSingleRangeFactory<U,TYPE,S> FType; 
	//typedef typename RangeInterface<GenSingleIndex<U,TYPE,S> >::IndexType IndexType;
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;
	
	bool isMeta(const U& metaPos) const;
	
	const U& get(size_t pos) const;
	size_t getMeta(const U& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
		
	friend GenSingleRangeFactory<U,TYPE,S>;

	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = CheckStatic<S>::ISSTATIC;
	static constexpr size_t SIZE = CheckStatic<S>::SIZE;
	static constexpr bool HASMETACONT = true;
	
    protected:

	GenSingleRange() = delete;
	GenSingleRange(const GenSingleRange& in) = delete;
	
	GenSingleRange(const std::vector<U>& space);

	std::vector<U> mSpace;
	//std::map<U,size_t> mMSpace;
	MetaMap<U> mMSpace;
    };

    template <typename U, SpaceType TYPE>
    using SingleRange = GenSingleRange<U,TYPE,-1>;

    template <typename U, SpaceType TYPE>
    using SingleIndex = GenSingleIndex<U,TYPE,-1>;

    template <typename U, SpaceType TYPE>
    using SingleRangeFactory = GenSingleRangeFactory<U,TYPE,-1>;

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /******************
     *  GenSingleIndex   *	     
     ******************/

    template <bool HASMETACONT>
    struct MetaPtrHandle
    {
	template <class Range>
	static const typename Range::MetaType* set(Range* r)
	{
	    return &r->get(0);
	}

	template <typename U, class Range>
	static inline U getMeta(U* metaPtr, size_t pos, std::shared_ptr<Range> r)
	{
	    return metaPtr[pos];
	}
    };

    template <>
    struct MetaPtrHandle<false>
    {
	template <class Range>
	static const typename Range::MetaType* set(Range* r)
	{
	    return nullptr;
	}

	template <typename U, class Range>
	static inline U getMeta(U* metaPtr, size_t pos, std::shared_ptr<Range> r)
	{
	    return r->get(pos);
	}
    };

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleIndex<U,TYPE,S>::GenSingleIndex(const std::shared_ptr<GenSingleRange<U,TYPE,S> >& range) :
	IndexInterface<GenSingleIndex<U,TYPE,S>,U>(range, 0),
	mExplicitRangePtr(std::dynamic_pointer_cast<RangeType>(IB::mRangePtr)),
	mMetaPtr(MetaPtrHandle<GenSingleIndex<U,TYPE,S>::RangeType::HASMETACONT>::set
		 ( dynamic_cast<RangeType*>(IB::mRangePtr.get() ) ) ) {}

    template <typename U, SpaceType TYPE, size_t S>
    IndexType GenSingleIndex<U,TYPE,S>::type() const
    {
	return IndexType::SINGLE;
    }

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleIndex<U,TYPE,S>& GenSingleIndex<U,TYPE,S>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleIndex<U,TYPE,S>& GenSingleIndex<U,TYPE,S>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleIndex<U,TYPE,S>& GenSingleIndex<U,TYPE,S>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename U, SpaceType TYPE, size_t S>
    int GenSingleIndex<U,TYPE,S>::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <typename U, SpaceType TYPE, size_t S>
    int GenSingleIndex<U,TYPE,S>::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    template <typename U, SpaceType TYPE, size_t S>
    std::string GenSingleIndex<U,TYPE,S>::stringMeta() const
    {
	return std::dynamic_pointer_cast<GenSingleRange<U,TYPE,S> const>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    U GenSingleIndex<U,TYPE,S>::meta() const
    {
	return MetaPtrHandle<GenSingleIndex<U,TYPE,S>::RangeType::HASMETACONT>::getMeta
	    ( mMetaPtr, IB::mPos, mExplicitRangePtr );
    }

    template <typename U, SpaceType TYPE, size_t S>
    const U* GenSingleIndex<U,TYPE,S>::metaPtr() const
    {
	return mMetaPtr;
    }

    template <typename U, SpaceType TYPE, size_t S>
    bool GenSingleIndex<U,TYPE,S>::isMeta(const U& metaPos) const
    {
	return mExplicitRangePtr->isMeta(metaPos);
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    GenSingleIndex<U,TYPE,S>& GenSingleIndex<U,TYPE,S>::at(const U& metaPos)
    {
	(*this) = std::dynamic_pointer_cast<GenSingleRange<U,TYPE,S> const>( IB::mRangePtr )->getMeta( metaPos );
	return *this;
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleIndex<U,TYPE,S>::posAt(const U& metaPos) const
    {
	return std::dynamic_pointer_cast<GenSingleRange<U,TYPE,S> const>( IB::mRangePtr )->getMeta( metaPos );
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleIndex<U,TYPE,S>::dim() // = 1
    {
	return 1;
    }

    template <typename U, SpaceType TYPE, size_t S>
    bool GenSingleIndex<U,TYPE,S>::last()
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <typename U, SpaceType TYPE, size_t S>
    bool GenSingleIndex<U,TYPE,S>::first()
    {
	return IB::mPos == 0;
    }

    template <typename U, SpaceType TYPE, size_t S>
    std::shared_ptr<typename GenSingleIndex<U,TYPE,S>::RangeType> GenSingleIndex<U,TYPE,S>::range()
    {
	return mExplicitRangePtr;
    }

    template <typename U, SpaceType TYPE, size_t S>
    template <size_t N>
    void GenSingleIndex<U,TYPE,S>::getPtr() {}
    
    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleIndex<U,TYPE,S>::getStepSize(size_t n)
    {
	return 1;
    }

    template <typename U, SpaceType TYPE, size_t S>
    std::string GenSingleIndex<U,TYPE,S>::id() const
    {
	return std::string("sin") + std::to_string(IB::mId);
    }

    template <typename U, SpaceType TYPE, size_t S>
    void GenSingleIndex<U,TYPE,S>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " << meta() << std::endl;
    }

    template <typename U, SpaceType TYPE, size_t S>
    template <class Expr>
    auto GenSingleIndex<U,TYPE,S>::ifor(size_t step, Expr ex) const
	-> For<GenSingleIndex<U,TYPE,S>,Expr>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<GenSingleIndex<U,TYPE,S>,Expr>(this, step, ex);
    }

    template <typename U, SpaceType TYPE, size_t S>
    template <class Expr>
    auto GenSingleIndex<U,TYPE,S>::iforh(size_t step, Expr ex) const
	-> For<GenSingleIndex<U,TYPE,S>,Expr,ForType::HIDDEN>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<GenSingleIndex<U,TYPE,S>,Expr,ForType::HIDDEN>(this, step, ex);
    }

    template <typename U, SpaceType TYPE, size_t S>
    template <class Expr>
    auto GenSingleIndex<U,TYPE,S>::pifor(size_t step, Expr ex) const
	-> PFor<GenSingleIndex<U,TYPE,S>,Expr>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return PFor<GenSingleIndex<U,TYPE,S>,Expr>(this, step, ex);
    }

    
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleRangeFactory<U,TYPE,S>::GenSingleRangeFactory(const std::vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new GenSingleRange<U,TYPE,S>( space ) );
    }

    template <typename U, SpaceType TYPE, size_t S>
    std::shared_ptr<RangeBase> GenSingleRangeFactory<U,TYPE,S>::create()
    {
	setSelf();
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    template <typename U, SpaceType TYPE, size_t S>
    GenSingleRange<U,TYPE,S>::GenSingleRange(const std::vector<U>& space) :
	RangeInterface<GenSingleIndex<U,TYPE,S> >(),
	mSpace(space), mMSpace(mSpace)
    {
	//for(size_t i = 0; i != mSpace.size(); ++i){
	//  mMSpace[mSpace[i]] = i;
	//}
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    const U& GenSingleRange<U,TYPE,S>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::getMeta(const U& metaPos) const
    {
	return mMSpace.at(metaPos);
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::size() const
    {
	return mSpace.size();
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::dim() const
    {
	return 1;
    }

    template <typename U, SpaceType TYPE, size_t S>
    bool GenSingleRange<U,TYPE,S>::isMeta(const U& metaPos) const
    {
	return mMSpace.count(metaPos) != 0;
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    SpaceType GenSingleRange<U,TYPE,S>::spaceType() const
    {
	return TYPE;
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    std::string GenSingleRange<U,TYPE,S>::stringMeta(size_t pos) const
    {
	return xToString(get(pos));
    }

    template <typename U, SpaceType TYPE, size_t S>
    std::vector<char> GenSingleRange<U,TYPE,S>::data() const
    {
        DataHeader h = dataHeader();
	std::vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	stringCat(out, mSpace);
	//const char* scp = reinterpret_cast<const char*>(mSpace.data());
	//out.insert(out.end(), scp, scp + h.metaSize);
	return out;
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    DataHeader GenSingleRange<U,TYPE,S>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( TYPE );
	h.metaSize = metaSize(mSpace);
	h.metaType = NumTypeMap<U>::num;
	h.multiple = 0;
        return h;
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    typename GenSingleRange<U,TYPE,S>::IndexType GenSingleRange<U,TYPE,S>::begin() const
    {
	GenSingleIndex<U,TYPE,S> i( std::dynamic_pointer_cast<GenSingleRange<U,TYPE,S> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    typename GenSingleRange<U,TYPE,S>::IndexType GenSingleRange<U,TYPE,S>::end() const
    {
	GenSingleIndex<U,TYPE,S> i( std::dynamic_pointer_cast<GenSingleRange<U,TYPE,S> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

#include "range_types/header.h"

#endif
