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
	
	virtual const std::string& name() const;
	virtual void name(const std::string& str);
 	virtual void name(const Name& nm);
	
	virtual MultiRangeType rangeType() const = 0;

	virtual bool link(IndefinitIndexBase* toLink);  
	virtual void freeLinked();
	virtual bool linked() const;
	virtual void linkTo(IndefinitIndexBase* target) = 0; 
	virtual IndefinitIndexBase* getLinked(const std::string& name) = 0;

	// include sub-index update routine
	virtual void setPos(size_t pos);

	// does NOT include sub-index update routine !!
	// relative position to current
	virtual void setPos(int relPos, IndefinitIndexBase* subIndex) = 0;

	virtual size_t max() const = 0;
	virtual size_t outOfRange() const;

	virtual bool toNull() const;

	virtual void eval() = 0;
	virtual void evalMajor();
	virtual void evalMajor(size_t stepSize, int num);
	virtual bool master();

	virtual void subOrd(IndefinitIndexBase* major);
	virtual size_t giveSubStepSize(IndefinitIndexBase* subIndex) = 0;

	virtual size_t majorStep() const;
	
	//virtual bool virt() const { return true; }
	
    protected:

	std::string mName;
	size_t mPos;
	size_t mMajorStep;
	
	IndefinitIndexBase* mLinked = nullptr;
	IndefinitIndexBase* mMajor = nullptr;
	IndefinitIndexBase* mSoftLinked = nullptr;
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
