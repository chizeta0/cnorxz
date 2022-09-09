// -*- C++ -*-

#ifndef __cxz_single_range_h__
#define __cxz_single_range_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <map>

#include "base_def.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"

#include "xfor/for_type.h"

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }

    
    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleIndex : public IndexInterface<GenSingleIndex<U,TYPE,S>,U>
    {
    public:

	typedef IndexInterface<GenSingleIndex<U,TYPE,S>,U> IB;
	typedef U MetaType;
	typedef GenSingleRange<U,TYPE,S> RangeType;
	typedef GenSingleIndex IType;

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
	GenSingleRangeFactory(const vector<U>& space);
        GenSingleRangeFactory(vector<U>&& space);
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

	MetaMap(const vector<U>& in)
	{
	    for(size_t i = 0; i != in.size(); ++i){
		mMap[in[i]] = i;
	    }
	}
	
	size_t at(const U& in) const
        {
            auto x = mMap.find(in);
            if(x != mMap.end()){
                return x->second;
            }
            else {
                return mMap.size();
            }
        }
	size_t count(const U& in) const { return mMap.count(in); }
    };

    template <size_t S>
    struct CheckStatic
    {
        static constexpr size_t ISSTATIC = true;
        static constexpr size_t SIZE = S;
    };

    template <>
    struct CheckStatic<MUI>
    {
        static constexpr size_t ISSTATIC = false;
        static constexpr size_t SIZE = MUI;
    };

    template <SpaceType TYPE>
    struct CheckDefault
    {
	static constexpr size_t ISDEFAULT = false;
	static constexpr size_t HASMETACONT = true; 
    };

    template <>
    struct CheckDefault<SpaceType::NONE>
    {
	static constexpr size_t ISDEFAULT = true;
	static constexpr size_t HASMETACONT = false; 
    };

    template <typename U>
    struct ToCMeta
    {
        static inline size_t apply(char* target, const U& elem)
        {
            *reinterpret_cast<U*>(target) = elem;
            return sizeof(U);
        }

        static inline size_t size(const U& elem)
        {
            return sizeof(U);
        }
    };

    template <typename V>
    struct ToCMeta<vector<V>>
    {
        static inline size_t apply(char* target, const vector<V>& elem)
        {
            size_t o = 0;
            for(auto& e: elem){
                o += ToCMeta<V>::apply(target+o, e);
            }
            return o;
        }

        static inline size_t size(const vector<V>& elem)
        {
            size_t out = 0;
            for(auto& x: elem){
                out += ToCMeta<V>::size(x);
            }
            return out;
        }
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
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;

        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	bool isMeta(const U& metaPos) const;
	
	const U& get(size_t pos) const;
	size_t getMeta(const U& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
		
	friend GenSingleRangeFactory<U,TYPE,S>;

	static GenSingleRangeFactory<U,TYPE,S> factory()
	{
	    static_assert( not CheckDefault<TYPE>::HASMETACONT,
			   "asked for default factory for meta data containing range" );
	    return GenSingleRangeFactory<U,TYPE,S>(vector<U>());
	}
	
	static constexpr bool defaultable = CheckDefault<TYPE>::ISDEFAULT;
	static constexpr size_t ISSTATIC = CheckStatic<S>::ISSTATIC;
	static constexpr size_t SIZE = CheckStatic<S>::SIZE;
	static constexpr bool HASMETACONT = CheckDefault<TYPE>::HASMETACONT;
	
    protected:

	GenSingleRange() = delete;
	GenSingleRange(const GenSingleRange& in) = delete;
	
	GenSingleRange(const vector<U>& space);
        GenSingleRange(vector<U>&& space);

	vector<U> mSpace;
	MetaMap<U> mMSpace;
    };

    template <typename U, SpaceType TYPE>
    using SingleRange = GenSingleRange<U,TYPE,MUI>;

    template <typename U, SpaceType TYPE>
    using SingleIndex = GenSingleIndex<U,TYPE,MUI>;

    template <typename U, SpaceType TYPE>
    using SingleRangeFactory = GenSingleRangeFactory<U,TYPE,MUI>;

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZ
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
    template <class Expr>
    auto GenSingleIndex<U,TYPE,S>::ifor(size_t step, Expr ex) const
	-> For<GenSingleIndex<U,TYPE,S>,Expr>
    {
	return For<GenSingleIndex<U,TYPE,S>,Expr>(this, step, ex);
    }

    template <typename U, SpaceType TYPE, size_t S>
    template <class Expr>
    auto GenSingleIndex<U,TYPE,S>::iforh(size_t step, Expr ex) const
	-> For<GenSingleIndex<U,TYPE,S>,Expr,ForType::HIDDEN>
    {
	return For<GenSingleIndex<U,TYPE,S>,Expr,ForType::HIDDEN>(this, step, ex);
    }

    template <typename U, SpaceType TYPE, size_t S>
    template <class Expr>
    auto GenSingleIndex<U,TYPE,S>::pifor(size_t step, Expr ex) const
	-> PFor<GenSingleIndex<U,TYPE,S>,Expr>
    {
	return PFor<GenSingleIndex<U,TYPE,S>,Expr>(this, step, ex);
    }

    
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleRangeFactory<U,TYPE,S>::GenSingleRangeFactory(const vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new GenSingleRange<U,TYPE,S>( space ) );
    }

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleRangeFactory<U,TYPE,S>::GenSingleRangeFactory(vector<U>&& space)
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
    GenSingleRange<U,TYPE,S>::GenSingleRange(const vector<U>& space) :
	RangeInterface<GenSingleIndex<U,TYPE,S> >(),
	mSpace(space), mMSpace(mSpace)
    {
	//for(size_t i = 0; i != mSpace.size(); ++i){
	//  mMSpace[mSpace[i]] = i;
	//}
    }

    template <typename U, SpaceType TYPE, size_t S>
    GenSingleRange<U,TYPE,S>::GenSingleRange(vector<U>&& space) :
	RangeInterface<GenSingleIndex<U,TYPE,S> >(),
	mSpace(space), mMSpace(mSpace) {}

        
    template <typename U, SpaceType TYPE, size_t S>
    const U& GenSingleRange<U,TYPE,S>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <size_t SIZE>
    inline size_t getStatSizeOrDyn(size_t size)
    {
	return SIZE;
    }

    template <>
    inline size_t getStatSizeOrDyn<MUI>(size_t size)
    {
	return size;
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::getMeta(const U& metaPos) const
    {
        return mMSpace.at(metaPos);
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::size() const
    {
	return getStatSizeOrDyn<S>(mSpace.size());
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
    vector<size_t> GenSingleRange<U,TYPE,S>::typeNum() const
    {
        return {NumTypeMap<U>::num()};
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::cmeta(char* target, size_t pos) const
    {
        return ToCMeta<U>::apply(target, mSpace[pos]);
    }

    template <typename U, SpaceType TYPE, size_t S>
    size_t GenSingleRange<U,TYPE,S>::cmetaSize() const
    {
        return ToCMeta<U>::size(mSpace[0]);
    }

    template <typename U, SpaceType TYPE, size_t S>
    std::string GenSingleRange<U,TYPE,S>::stringMeta(size_t pos) const
    {
	return xToString(get(pos));
    }

    template <typename U, SpaceType TYPE, size_t S>
    vector<char> GenSingleRange<U,TYPE,S>::data() const
    {
        DataHeader h = dataHeader();
	vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	stringCat(out, mSpace);
	return out;
    }
    
    template <typename U, SpaceType TYPE, size_t S>
    DataHeader GenSingleRange<U,TYPE,S>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( TYPE );
	h.metaSize = metaSize(mSpace);
	h.metaType = NumTypeMap<U>::num();
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
