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

    
    template <typename U, SpaceType TYPE>
    class SingleIndex : public IndexInterface<SingleIndex<U,TYPE>,U>
    {
    public:

	typedef IndexInterface<SingleIndex<U,TYPE>,U> IB;
	typedef U MetaType;
	typedef SingleRange<U,TYPE> RangeType;
	typedef SingleIndex IType;

	//DEFAULT_MEMBERS_X(SingleIndex);
	
	SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range);

	static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }

	static constexpr SpaceType STYPE = TYPE;
	
	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	SingleIndex& operator=(size_t pos);
	SingleIndex& operator++();
	SingleIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	U meta() const;
	const U* metaPtr() const;
	SingleIndex& at(const U& metaPos);
	size_t posAt(const U& metaPos) const;
	
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
	    -> For<SingleIndex<U,TYPE>,Expr>;

	template <class Expr>
	auto iforh(size_t step, Expr ex) const
	    -> For<SingleIndex<U,TYPE>,Expr,ForType::HIDDEN>;

    private:
	std::shared_ptr<RangeType> mExplicitRangePtr;
	const U* mMetaPtr;
    };

    template <typename U, SpaceType TYPE>
    class SingleRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<U,TYPE> oType;

	SingleRangeFactory() = delete;
	SingleRangeFactory(const std::vector<U>& space);
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
    
    template <typename U, SpaceType TYPE>
    class SingleRange : public RangeInterface<SingleIndex<U,TYPE> >
    {
    public:
	typedef RangeBase RB;
	typedef SingleIndex<U,TYPE> IndexType;
	typedef SingleRange RangeType;
	typedef U MetaType;
	//typedef typename RangeInterface<SingleIndex<U,TYPE> >::IndexType IndexType;
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
	
	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;
	
	const U& get(size_t pos) const;
	size_t getMeta(const U& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
		
	friend SingleRangeFactory<U,TYPE>;

	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = true;
	
    protected:

	SingleRange() = delete;
	SingleRange(const SingleRange& in) = delete;
	
	SingleRange(const std::vector<U>& space);

	std::vector<U> mSpace;
	//std::map<U,size_t> mMSpace;
	MetaMap<U> mMSpace;
    };

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /******************
     *  SingleIndex   *	     
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

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range) :
	IndexInterface<SingleIndex<U,TYPE>,U>(range, 0),
	mExplicitRangePtr(std::dynamic_pointer_cast<RangeType>(IB::mRangePtr)),
	mMetaPtr(MetaPtrHandle<SingleIndex<U,TYPE>::RangeType::HASMETACONT>::set
		 ( dynamic_cast<RangeType*>(IB::mRangePtr.get() ) ) ) {}

    template <typename U, SpaceType TYPE>
    IndexType SingleIndex<U,TYPE>::type() const
    {
	return IndexType::SINGLE;
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename U, SpaceType TYPE>
    int SingleIndex<U,TYPE>::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <typename U, SpaceType TYPE>
    int SingleIndex<U,TYPE>::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    template <typename U, SpaceType TYPE>
    std::string SingleIndex<U,TYPE>::stringMeta() const
    {
	return std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <typename U, SpaceType TYPE>
    U SingleIndex<U,TYPE>::meta() const
    {
	return MetaPtrHandle<SingleIndex<U,TYPE>::RangeType::HASMETACONT>::getMeta
	    ( mMetaPtr, IB::mPos, mExplicitRangePtr );
    }

    template <typename U, SpaceType TYPE>
    const U* SingleIndex<U,TYPE>::metaPtr() const
    {
	return mMetaPtr;
    }
    
    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::at(const U& metaPos)
    {
	(*this) = std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->getMeta( metaPos );
	return *this;
    }

    template <typename U, SpaceType TYPE>
    size_t SingleIndex<U,TYPE>::posAt(const U& metaPos) const
    {
	return std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->getMeta( metaPos );
    }
    
    template <typename U, SpaceType TYPE>
    size_t SingleIndex<U,TYPE>::dim() // = 1
    {
	return 1;
    }

    template <typename U, SpaceType TYPE>
    bool SingleIndex<U,TYPE>::last()
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <typename U, SpaceType TYPE>
    bool SingleIndex<U,TYPE>::first()
    {
	return IB::mPos == 0;
    }

    template <typename U, SpaceType TYPE>
    std::shared_ptr<typename SingleIndex<U,TYPE>::RangeType> SingleIndex<U,TYPE>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <typename U, SpaceType TYPE>
    template <size_t N>
    void SingleIndex<U,TYPE>::getPtr() {}
    
    template <typename U, SpaceType TYPE>
    size_t SingleIndex<U,TYPE>::getStepSize(size_t n)
    {
	return 1;
    }

    template <typename U, SpaceType TYPE>
    std::string SingleIndex<U,TYPE>::id() const
    {
	return std::string("sin") + std::to_string(IB::mId);
    }

    template <typename U, SpaceType TYPE>
    void SingleIndex<U,TYPE>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " << meta() << std::endl;
    }

    template <typename U, SpaceType TYPE>
    template <class Expr>
    auto SingleIndex<U,TYPE>::ifor(size_t step, Expr ex) const
	-> For<SingleIndex<U,TYPE>,Expr>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<SingleIndex<U,TYPE>,Expr>(this, step, ex);
    }

    template <typename U, SpaceType TYPE>
    template <class Expr>
    auto SingleIndex<U,TYPE>::iforh(size_t step, Expr ex) const
	-> For<SingleIndex<U,TYPE>,Expr,ForType::HIDDEN>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<SingleIndex<U,TYPE>,Expr,ForType::HIDDEN>(this, step, ex);
    }

    
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, SpaceType TYPE>
    SingleRangeFactory<U,TYPE>::SingleRangeFactory(const std::vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new SingleRange<U,TYPE>( space ) );
    }

    template <typename U, SpaceType TYPE>
    std::shared_ptr<RangeBase> SingleRangeFactory<U,TYPE>::create()
    {
	setSelf();
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    template <typename U, SpaceType TYPE>
    SingleRange<U,TYPE>::SingleRange(const std::vector<U>& space) :
	RangeInterface<SingleIndex<U,TYPE> >(),
	mSpace(space), mMSpace(mSpace)
    {
	//for(size_t i = 0; i != mSpace.size(); ++i){
	//  mMSpace[mSpace[i]] = i;
	//}
    }
    
    template <typename U, SpaceType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, SpaceType TYPE>
    size_t SingleRange<U,TYPE>::getMeta(const U& metaPos) const
    {
	return mMSpace.at(metaPos);
    }

    template <typename U, SpaceType TYPE>
    size_t SingleRange<U,TYPE>::size() const
    {
	return mSpace.size();
    }

    template <typename U, SpaceType TYPE>
    size_t SingleRange<U,TYPE>::dim() const
    {
	return 1;
    }

    template <typename U, SpaceType TYPE>
    SpaceType SingleRange<U,TYPE>::spaceType() const
    {
	return TYPE;
    }
    
    template <typename U, SpaceType TYPE>
    std::string SingleRange<U,TYPE>::stringMeta(size_t pos) const
    {
	return xToString(get(pos));
    }

    template <typename U, SpaceType TYPE>
    std::vector<char> SingleRange<U,TYPE>::data() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( TYPE );
	h.metaSize = metaSize(mSpace);
	h.metaType = NumTypeMap<U>::num;
	h.multiple = 0;
	std::vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	stringCat(out, mSpace);
	//const char* scp = reinterpret_cast<const char*>(mSpace.data());
	//out.insert(out.end(), scp, scp + h.metaSize);
	return out;
    }
    
    template <typename U, SpaceType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::begin() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename U, SpaceType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::end() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

#include "range_types/header.h"

#endif
