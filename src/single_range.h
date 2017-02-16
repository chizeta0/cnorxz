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

	DEFAULT_MEMBERS(SingleIndex);

	virtual SingleIndex& operator=(const U& upos);
	virtual const U& getMetaPos() const;

	virtual size_t dim() const override; // = 1
	virtual void linkTo(IndefinitIndexBase* target) override;
	
    protected:
	virtual size_t evaluate(const Index& in) const override;
    };

    template <typename U, RangeType TYPE>
    class SingleRange : public RangeBase<SingleIndex<U,TYPE> >
    {
    public:
	DEFAULT_MEMBERS(SingleRange);

	virtual size_t size() const override;
	
	const U& get(size_t pos) const;
	size_t get(const U& metaPos) const;
	
    protected:
	std::vector<U> mSpace;
    };

}

#include "single_range.cc"

#endif
