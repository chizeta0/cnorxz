// -*- C++ -*-

#ifndef __index_base_h__
#define __index_base_h__

#include <cstdlib>
#include <string>
#include <vector>

#include "base_def.h"
#include "range_base.h"
#include "name.h"

namespace MultiArrayTools
{
    size_t indexId()
    {
	static size_t id = 0;
	++id;
	return id;
    }
    
    class IndexBase
    {
    public:
	//DEFAULT_MEMBERS(IndexBase);

	IndexBase() { mId = indexId(); }
	IndexBase(IndexBase&& in) = default;
	IndexBase& operator=(IndexBase&& in) = default;
	
	IndexBase(const std::shared_ptr<RangeBase>& range, size_t pos);
	virtual ~IndexBase() = default; 
	
	virtual IndexBase& operator=(size_t pos) = 0;
	virtual IndexBase& operator++() = 0;
	virtual IndexBase& operator--() = 0;
	
	bool operator==(const IndexBase& in) const;
	bool operator!=(const IndexBase& in) const;
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const;
	virtual size_t max() const;

	virtual bool last() const = 0;
	virtual bool first() const = 0;
	
	virtual operator size_t() const;

	virtual std::string id() const { return std::to_string( mId ); }
    protected:

	std::shared_ptr<RangeBase> mRangePtr;
	size_t mPos;
	size_t mId;
    };

    template <typename MetaType>
    class IndexInterface : public IndexBase
    {
    public:

	DEFAULT_MEMBERS(IndexInterface);
	
	IndexInterface(const std::shared_ptr<RangeBase>& rangePtr, size_t pos);
	virtual MetaType meta() const = 0;
	virtual IndexInterface& at(const MetaType& meta) = 0;
    };
    
}

#include "index_base.cc"

#endif
