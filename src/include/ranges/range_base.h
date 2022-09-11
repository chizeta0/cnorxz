// -*- C++ -*-

#ifndef __cxz_range_base_h__
#define __cxz_range_base_h__

#include "base/base.h"

namespace CNORXZ
{

    SizeT indexId();
    
    enum class SpaceType
	{
	 NONE = 0, // meta data is that of a classic range, i.e. 0,1,2,...,N-1
	 ANY = 1, // meta data is arbitrary, i.e. explicitly stored; range could be multiple
#define include_range_type(x,n) x = n,
#include "range_types/header.h"
#undef include_range_type
	 ANON = -1, // anonymous content
         DYN = -3 // dynamic content
	};

    struct DataHeader
    {
    public:
	static constexpr SizeT VERSION = 1; // fixed by version of this repository !
    private:
	SizeT version = VERSION;
    public:
	int spaceType = static_cast<int>( SpaceType::NONE );
	SizeT metaSize = 0; // size of meta data
	int multiple = 0; // = 1 if multi range
	int metaType = 0; // type of meta data
	inline SizeT v() const { return version; } 
    };

    typedef Sptr<RangeBase> RangePtr;

    class RangeFactoryBase
    {
    public:

	RangeFactoryBase() = default;
	virtual ~RangeFactoryBase() = default;

	RangePtr create();
	
    protected:

	virtual void make() = 0;

	RangePtr fromCreated(const TypeInfo& info, const Vector<PtrId>& rids) const;
	void addToCreated(const TypeInfo& info, const Vector<PtrId>& rids, const RangePtr& r);

	RangePtr mProd;

    private:
	// also add single ranges here (PtrId -> own)
	static Map<TypeInfo,Map<Vector<PtrId>,RangePtr>> sCreated;
	
    };

    Sptr<RangeFactoryBase> createRangeFactory(const char** dp);
    Sptr<RangeFactoryBase> createSingleRangeFactory(const Vector<char>*& d, int metaType, SizeT size);

    class RangeBase
    {
    public:
	
	virtual ~RangeBase() = default;
	    
	virtual SizeT size() const = 0;
	virtual SizeT dim() const = 0;
	
	bool operator==(const RangeBase& in) const;
	bool operator!=(const RangeBase& in) const;

	virtual TypeInfo type() const = 0;
        PtrId id() const;

	XIndexPtr beginX() const;
	XIndexPtr endX() const;
	
        virtual Vector<SizeT> typeNum() const = 0;
        virtual SizeT cmeta(char* target, SizeT pos) const = 0;
        virtual SizeT cmetaSize() const = 0;
        virtual String stringMeta(SizeT pos) const = 0;
	virtual Vector<char> data() const = 0; // usefull when writing to files, etc...

	virtual SpaceType spaceType() const = 0;
        virtual DataHeader dataHeader() const = 0;

	virtual RangePtr sub(SizeT num) const { return RangePtr(); }
	
	friend RangeFactoryBase;
	
    protected:
	
	RangeBase() = default;
	std::weak_ptr<RangeBase> mThis;
    };

    template <class Index>
    inline Sptr<IndexWrapperBase> mkIndexWrapper(const Index& i);
    
    template <class Index>
    class RangeInterface : public RangeBase
    {
    public:

	//typedef typename Index::MetaType MetaType;
	typedef Index IndexType;

	static constexpr SpaceType STYPE = IndexType::STYPE;
	
	virtual Index begin() const = 0;
	virtual Index end() const = 0;
	Sptr<Index> beginPtr() const { return std::make_shared<Index>(this->begin()); }
	Sptr<Index> endPtr() const { return std::make_shared<Index>(this->end()); }
	virtual Sptr<IndexWrapperBase> aindex() const override final
        { return mkIndexWrapper(this->begin()); }

    protected:
	RangeInterface() = default;
    };


    RangePtr operator*(const RangePtr& a, const RangePtr& b); // -> Ptr to MultiRange
}

#endif
