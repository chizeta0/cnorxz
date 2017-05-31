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
    
    class IndefinitIndexBase
    {
    public:
	DEFAULT_MEMBERS(IndefinitIndexBase);
	virtual ~IndefinitIndexBase() = default; 
	
	virtual IndefinitIndexBase& operator=(size_t pos) = 0;
	virtual IndefinitIndexBase& operator++() = 0;
	virtual IndefinitIndexBase& operator--() = 0;
	virtual IndefinitIndexBase& operator+=(int n) = 0;
	virtual IndefinitIndexBase& operator-=(int n) = 0;

	bool operator==(const IndefinitIndexBase& in) const;
	bool operator!=(const IndefinitIndexBase& in) const;
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const;
	
	virtual MultiRangeType rangeType() const = 0;

	virtual IndefinitIndexBase& setPos(size_t pos, IndefinitIndexBase* ctrlPtr = nullptr);
	virtual IndefinitIndexBase& setPosRel(int relPos, IndefinitIndexBase* ctrlPtr = nullptr);

	virtual IndefinitIndexBase& toFirst(IndefinitIndexBase* ctrlPtr = nullptr);
	virtual IndefinitIndexBase& toLast(IndefinitIndexBase* ctrlPtr = nullptr);
	
	virtual size_t max() const = 0;
	virtual int outOfRange() const;
	virtual bool atEdge() const;
	virtual bool master() const;

	virtual IndefinitIndexBase& subOrd(IndefinitIndexBase* major);
	virtual size_t giveSubStepSize(IndefinitIndexBase* subIndex) = 0;
	
    protected:

	int mPos;
	std::map<IndefinitIndexBase*,size_t> mMajor;
    };
    
    template <class Index>
    class IndexBase : public IndefinitIndexBase
    {
    public:

	typedef IndefinitIndexBase IIB;
	
	DEFAULT_MEMBERS(IndexBase);
	IndexBase(std::shared_ptr<const RangeBase<Index> >& rangePtr);
	
	//virtual size_t pos() const override; // = mPos; implement !!!
	virtual size_t max() const override;
	virtual void copyPos(const Index& in) = 0;
	
    protected:

	// translate index into position
	virtual size_t evaluate(const Index& in) const = 0;
	std::shared_ptr<const RangeBase<Index> > mRangePtr;
    };

}

#include "index_base.cc"

#endif
