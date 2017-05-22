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
	virtual ~IndefinitIndexBase();
	
	virtual IndefinitIndexBase& operator=(size_t pos) = 0;
	virtual IndefinitIndexBase& operator++() = 0;
	virtual IndefinitIndexBase& operator--() = 0;
	virtual IndefinitIndexBase& operator+=(int n) = 0;
	virtual IndefinitIndexBase& operator-=(int n) = 0;

	// Make this somehow better... !!!
	//virtual bool operator==(const IndefinitIndexBase& i) = 0;
	//virtual bool operator!=(const IndefinitIndexBase& i) = 0;
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const;
	
	virtual MultiRangeType rangeType() const = 0;

	virtual void setPos(size_t pos);
	virtual void setPosRel(int relPos);

	virtual IndefinitIndexBase& toFirst();
	virtual IndefinitIndexBase& toLast();
	
	virtual size_t max() const = 0;
	virtual int outOfRange() const;
	virtual bool atEdge() const;
	virtual bool toNull() const;

	virtual void eval() = 0;
	virtual void evalMajor();
	virtual void evalMajor(int num);
	virtual bool master();

	virtual void subOrd(IndefinitIndexBase* major);
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
	IndexBase(RangeBase<Index> const* range);
	
	//virtual size_t pos() const override; // = mPos; implement !!!
	virtual size_t max() const override;
	virtual bool toNull() const override;

	virtual void assignRange(RangeBase<Index> const* range);

	virtual void eval() override;
	virtual void copyPos(const Index& in) = 0;
	
    protected:

	// translate index into position
	virtual size_t evaluate(const Index& in) const = 0;
	RangeBase<Index> const* mRange;
    };

}

#include "index_base.cc"

#endif
