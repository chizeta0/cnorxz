// -*- C++ -*-

#ifndef __cxz_urange_h__
#define __cxz_urange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xfor/for_type.h"

namespace CNORXZ
{
    
    template <typename MetaType>
    class UIndex : public IndexInterface<UIndex<MetaType>,MetaType>
    {
    public:

	typedef IndexInterface<UIndex<MetaType>,MetaType> IB;
	typedef URange<MetaType> RangeType;

	UIndex(const RangePtr& range);
	
	UIndex& operator=(SizeT pos);
	UIndex& operator++();
	UIndex& operator--();
	UIndex operator+(Int n) const;
	UIndex operator-(Int n) const;
	UIndex& operator+=(Int n);
	UIndex& operator-=(Int n);

	const MetaType& operator*() const;
	const MetaType* operator->() const;
	
	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;
	SizeT getStepSize(SizeT n) const;

	String stringMeta() const;
	MetaType meta() const;
	UIndex& at(const MetaType& metaPos);

	template <class Expr>
	auto ifor(SizeT step, Expr ex) const
	    -> For<UIndex<MetaType>,Expr>;

	template <class Expr>
	auto iforh(SizeT step, Expr ex) const
	    -> For<UIndex<MetaType>,Expr,ForType::HIDDEN>;

        template <class Expr>
	auto pifor(SizeT step, Expr ex) const
	    -> PFor<UIndex<MetaType>,Expr>;
        
    private:
	Sptr<RangeType> mRangePtr;
	const MetaType* mMetaPtr;
    };

    template <typename MetaType>
    class URangeFactory : public RangeFactoryBase
    {
    public:
	URangeFactory() = delete;
	URangeFactory(const Vector<U>& space);
        URangeFactory(Vector<U>&& space);

    protected:
	void make();
    };

    template <typename MetaType>
    class URange : public RangeInterface<UIndex<MetaType>>
    {
    public:
	typedef RangeBase RB;
	typedef UIndex<MetaType> IndexType;
	typedef URangeFactory<MetaType> FType; 
	
	virtual SizeT size() const final;
	virtual SizeT dim() const final;

	virtual std::string stringMeta(SizeT pos) const final;
	const MetaType& get(SizeT pos) const;
	SizeT getMeta(const MetaType& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
		
	friend URangeFactory<MetaType>;

    protected:

	URange() = delete;
	URange(const URange& in) = delete;
	URange(const vector<MetaType>& space);
        URange(vector<MetaType>&& space);

	Vector<MetaType> mSpace; // SORTED!!!
    };

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZ
{
    /*****************
     *    UIndex     *	     
     *****************/

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
