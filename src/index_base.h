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
	
	virtual IndefinitIndexBase& operator=(size_t pos) = 0;
	virtual IndefinitIndexBase& operator++() = 0;
	virtual IndefinitIndexBase& operator--() = 0;
	virtual IndefinitIndexBase& operator+=(int n) = 0;
	virtual IndefinitIndexBase& operator-=(int n) = 0;

	// Make this somehow better... !!!
	//virtual bool operator==(const IndefinitIndexBase& i) = 0;
	//virtual bool operator!=(const IndefinitIndexBase& i) = 0;
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const; // = mPos; implement !!!
	
	virtual const std::string& name() const;
	virtual void name(const std::string& str);
 	virtual void name(const Name& nm);
	
	virtual MultiRangeType rangeType() const = 0;

	virtual bool link(IndefinitIndexBase* toLink);  
	virtual void freeLinked();
	virtual bool linked() const;
	virtual void linkTo(IndefinitIndexBase* target) = 0; 
	
	virtual void setPos(size_t pos);

	virtual size_t max() const;
	virtual size_t outOfRange() const;
	
    protected:

	DEFAULT_MEMBERS(IndefinitIndexBase);
	
	std::string mName;
	size_t mPos;

	IndefinitIndexBase* mLinked;
    };
    
    template <class Index>
    class IndexBase : public IndefinitIndexBase
    {
    public:
	
	//virtual size_t pos() const override; // = mPos; implement !!!
	virtual size_t max() const override;
	
    protected:

	DEFAULT_MEMBERS(IndexBase);
	
	// translate index into position
	virtual size_t evaluate(const Index& in) const = 0;
	RangeBase<Index>* mRange;
    };

}

#include "index_base.cc"

#endif
