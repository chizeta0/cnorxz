// -*- C++ -*-

#ifndef __range_base_h__
#define __range_base_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "rbase_def.h"

namespace MultiArrayTools
{
    class RangeBase;
}

namespace MultiArrayTools
{

    size_t indexId();
    
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
	static constexpr size_t VERSION = 1; // fixed by version of this repository !
    private:
	size_t version = VERSION;
    public:
	int spaceType = static_cast<int>( SpaceType::NONE );
	size_t metaSize = 0; // size of meta data
	int multiple = 0; // = 1 if multi range
	int metaType = 0; // type of meta data
	inline size_t v() const { return version; } 
    };
    
    class RangeFactoryBase
    {
    public:

	RangeFactoryBase() = default;
	virtual ~RangeFactoryBase() = default;

	// should return mProd !!
	virtual std::shared_ptr<RangeBase> create() = 0;
	
    protected:

	std::shared_ptr<RangeBase> mProd;
	
	// call this function before returning product !!
	void setSelf();
    };

    std::shared_ptr<RangeFactoryBase> createRangeFactory(const char** dp);
    
    class RangeBase
    {
    public:
	
	static constexpr bool ISINDEX = false;
	
	virtual ~RangeBase() = default;
	    
	virtual size_t size() const = 0;
	virtual size_t dim() const = 0;
	
	bool operator==(const RangeBase& in) const;
	bool operator!=(const RangeBase& in) const;

        std::intptr_t id() const;
        
	virtual std::string stringMeta(size_t pos) const = 0;
	virtual std::vector<char> data() const = 0; // usefull when writing to files, etc...

	virtual SpaceType spaceType() const = 0;
        virtual DataHeader dataHeader() const = 0;
	//virtual bool regular() const = 0; // integer distance (e.g. 2,3,4,...)
	//virtual bool linear() const = 0; // 1dim valuable (e.g. 2.45, 3.12, 3.56,...)
	//virtual bool multi() const = 0; // mdim
	//virtual bool maplike() const = 0; // meta type is ~ MultiArray<T,...>
	
	friend RangeFactoryBase;
	
    protected:
	
	RangeBase() = default;
	std::weak_ptr<RangeBase> mThis;
    };
    
    template <class Index>
    class RangeInterface : public RangeBase
    {
    public:

	//typedef typename Index::MetaType MetaType;
	typedef Index IndexType;

	static constexpr SpaceType STYPE = IndexType::STYPE;
	
	virtual Index begin() const = 0;
	virtual Index end() const = 0;

    protected:
	RangeInterface() = default;
    };
    
}

#endif
