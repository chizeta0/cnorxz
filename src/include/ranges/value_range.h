
#ifndef __value_range_h__
#define __value_range_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <map>

#include "rbase_def.h"
//#include "base_def.h"
#include "ranges/rpheader.h"
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
    
    template <typename U>
    class ValueIndex : public IndexInterface<ValueIndex<U>,U>
    {
    public:

	typedef IndexInterface<ValueIndex<U>,U> IB;
	typedef U MetaType;
	typedef ValueRange<U> RangeType;
	typedef ValueIndex IType;

	ValueIndex(const std::shared_ptr<RangeType>& rptr);

	static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }

	static constexpr SpaceType STYPE = SpaceType::NONE;
	
	IndexType type() const;
	
	ValueIndex& operator=(size_t pos);
	ValueIndex& operator++();
	ValueIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	const U& meta() const;
	const U* metaPtr() const;
	ValueIndex& at(const U& metaPos);
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
	    -> For<ValueIndex<U>,Expr>;

	template <class Expr>
	auto iforh(size_t step, Expr ex) const
	    -> For<ValueIndex<U>,Expr,ForType::HIDDEN>;
	
    private:
	std::shared_ptr<RangeType> mExplicitRangePtr;
	U mMeta;
    };

    template <typename U>
    class ValueRangeFactory : public RangeFactoryBase
    {
    public:

	typedef ValueRange<U> oType;
	
	ValueRangeFactory();
	std::shared_ptr<RangeBase> create();
    };

    template <typename U>
    class ValueRange : public RangeInterface<ValueIndex<U> >
    {
    public:
	typedef RangeBase RB;
	typedef ValueIndex<U> IndexType;
	typedef ValueRange RangeType;
	typedef U MetaType;
        typedef ValueRangeFactory<U> FType;
        
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
        virtual size_t typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;

        U get(size_t pos) const;
        
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
		
	friend ValueRangeFactory<U>;

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = 1;
	static constexpr bool HASMETACONT = false;

	static std::shared_ptr<ValueRange> Default()
	{
	    ValueRangeFactory<U> vrf;
	    return std::dynamic_pointer_cast<ValueRange>( vrf.create() );
	}
	
    protected:
        mutable U const* mMeta;
	ValueRange() = default;
    };
    
} // namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /*****************
     *  ValueIndex   *	     
     *****************/

    namespace
    {
	template<typename U>
	std::shared_ptr<RangeBase> mkDefaultValueRange()
	{
	    ValueRangeFactory<U> vrf;
	    return vrf.create();
	}
    }
    
    template <typename U>
    ValueIndex<U>::ValueIndex(const std::shared_ptr<RangeType>& rptr) :
	IndexInterface<ValueIndex<U>,U>(rptr, 0),
	mExplicitRangePtr(std::dynamic_pointer_cast<RangeType>(IB::mRangePtr))
    {}

    template <typename U>
    IndexType ValueIndex<U>::type() const
    {
	return IndexType::SINGLE;
    }

    template <typename U>
    ValueIndex<U>& ValueIndex<U>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    template <typename U>
    ValueIndex<U>& ValueIndex<U>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename U>
    ValueIndex<U>& ValueIndex<U>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename U>
    int ValueIndex<U>::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <typename U>
    int ValueIndex<U>::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    template <typename U>
    std::string ValueIndex<U>::stringMeta() const
    {
	return xToString(mMeta);
    }
    
    template <typename U>
    const U& ValueIndex<U>::meta() const
    {
	return mMeta;
    }

    template <typename U>
    const U* ValueIndex<U>::metaPtr() const
    {
	return &mMeta;
    }
    
    template <typename U>
    ValueIndex<U>& ValueIndex<U>::at(const U& metaPos)
    {
	mMeta = metaPos;
	return *this;
    }

    template <typename U>
    size_t ValueIndex<U>::posAt(const U& metaPos) const
    {
	return 0;
    }
    
    template <typename U>
    size_t ValueIndex<U>::dim() // = 1
    {
	return 1;
    }

    template <typename U>
    bool ValueIndex<U>::last()
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <typename U>
    bool ValueIndex<U>::first()
    {
	return IB::mPos == 0;
    }

    template <typename U>
    std::shared_ptr<typename ValueIndex<U>::RangeType> ValueIndex<U>::range()
    {
	return mExplicitRangePtr;
    }

    template <typename U>
    template <size_t N>
    void ValueIndex<U>::getPtr() {}
    
    template <typename U>
    size_t ValueIndex<U>::getStepSize(size_t n)
    {
	return 1;
    }

    template <typename U>
    std::string ValueIndex<U>::id() const
    {
	return std::string("val") + std::to_string(IB::mId);
    }

    template <typename U>
    void ValueIndex<U>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " << meta() << std::endl;
    }

    template <typename U>
    template <class Expr>
    auto ValueIndex<U>::ifor(size_t step, Expr ex) const
	-> For<ValueIndex<U>,Expr>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<ValueIndex<U>,Expr>(this, step, ex);
    }

    template <typename U>
    template <class Expr>
    auto ValueIndex<U>::iforh(size_t step, Expr ex) const
	-> For<ValueIndex<U>,Expr,ForType::HIDDEN>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<ValueIndex<U>,Expr,ForType::HIDDEN>(this, step, ex);
    }


    /**************************
     *   ValueRangeFactory    *
     **************************/


    template <typename U>
    ValueRangeFactory<U>::ValueRangeFactory()
    {
	mProd = std::shared_ptr<oType>( new ValueRange<U>() );
    }

    template <typename U>
    std::shared_ptr<RangeBase> ValueRangeFactory<U>::create()
    {
	setSelf();
	return mProd;
    }


    /*******************
     *   ValueRange    *
     *******************/

    template <typename U>
    size_t ValueRange<U>::size() const
    {
	return 1;
    }

    template <typename U>
    size_t ValueRange<U>::dim() const
    {
	return 1;
    }

    template <typename U>
    SpaceType ValueRange<U>::spaceType() const
    {
	return SpaceType::NONE;
    }

    template <typename U>
    size_t ValueRange<U>::typeNum() const
    {
        return NumTypeMap<U>::num;
    }
        
    template <typename U>
    size_t ValueRange<U>::cmeta(char* target, size_t pos) const
    {
        if(target){
            *reinterpret_cast<U*>(target) = *mMeta;
        }
        return sizeof(U);
    }

    template <typename U>
    std::string ValueRange<U>::stringMeta(size_t pos) const
    {
	return "";
    }

    template <typename U>
    U ValueRange<U>::get(size_t pos) const
    {
        return *mMeta;
    }
    
    template <typename U>
    vector<char> ValueRange<U>::data() const
    {
	assert(0);
	DataHeader h = dataHeader();
	vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	
	//const char* scp = reinterpret_cast<const char*>(mSpace.data());
	//out.insert(out.end(), scp, scp + h.metaSize);
	return out;
    }

    template <typename U>
    DataHeader ValueRange<U>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::NONE );
	h.metaSize = 0;
	h.metaType = NumTypeMap<U>::num;
	h.multiple = 0;
        return h;
    }

    template <typename U>
    typename ValueRange<U>::IndexType ValueRange<U>::begin() const
    {
	ValueIndex<U> i( std::dynamic_pointer_cast<ValueRange<U> >
			 ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
        mMeta = &i.meta();
	return i;
    }
    
    template <typename U>
    typename ValueRange<U>::IndexType ValueRange<U>::end() const
    {
	ValueIndex<U> i( std::dynamic_pointer_cast<ValueRange<U> >
			 ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
        mMeta = &i.meta();
	return i;
    }


} // namespace MultiArrayTools

#endif
