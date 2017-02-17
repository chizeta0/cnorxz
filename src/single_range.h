// -*- C++ -*-

#ifndef __single_range_h__
#define __single_range_h__

#include <cstdlib>
#include <vector>

#include "base_def.h"
#include "index_base.h"
#include "range_base.h"

namespace MultiArrayTools
{

    template <typename U, RangeType TYPE>
    class SingleIndex : public IndexBase<SingleIndex<U,TYPE> >
    {
    public:

	typedef IndexBase<SingleIndex<U,TYPE> > IB;
	typedef IndefinitIndexBase IIB;
	
	DEFAULT_MEMBERS(SingleIndex);

	// find better solution !!!
	SingleIndex(const U& upos, size_t disambig);
	SingleIndex(size_t pos);

	virtual SingleIndex& operator=(const U& upos);

	virtual SingleIndex& operator=(size_t pos) override;
	virtual SingleIndex& operator++() override;
	virtual SingleIndex& operator--() override;
	virtual SingleIndex& operator+=(int n) override;
	virtual SingleIndex& operator-=(int n) override;

	virtual bool operator==(const SingleIndex& i);
	virtual bool operator!=(const SingleIndex& i);

	virtual MultiRangeType rangeType() const override;

	virtual const U& getMetaPos() const;

	virtual size_t dim() const override; // = 1
	virtual void linkTo(IndefinitIndexBase* target) override;
	
    protected:
	virtual size_t evaluate(const SingleIndex& in) const override;
    };

    template <typename U, RangeType TYPE>
    class SingleRange : public RangeBase<SingleIndex<U,TYPE> >
    {
    public:
	DEFAULT_MEMBERS(SingleRange);

	SingleRange(const std::vector<U>& space);
	
	virtual size_t size() const override;
	
	const U& get(size_t pos) const;
	size_t get(const U& metaPos) const;

	SingleIndex<U,TYPE> begin() const override;
	SingleIndex<U,TYPE> end() const override;
	
    protected:
	std::vector<U> mSpace;
    };

}

#include "single_range.cc"

#endif
