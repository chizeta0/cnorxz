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
	SingleIndex(RangeBase<SingleIndex<U,TYPE> > const* range,
		    const U& upos, size_t disambig);

	SingleIndex(RangeBase<SingleIndex<U,TYPE> > const* range,
		    size_t pos);

	//virtual SingleIndex& operator=(const U& upos);

	virtual SingleIndex& operator=(size_t pos) override;
	virtual SingleIndex& operator++() override;
	virtual SingleIndex& operator--() override;
	virtual SingleIndex& operator+=(int n) override;
	virtual SingleIndex& operator-=(int n) override;

	virtual bool operator==(const SingleIndex& i);
	virtual bool operator!=(const SingleIndex& i);

	virtual MultiRangeType rangeType() const override;

	virtual const U& getMetaPos() const;
	virtual SingleIndex& atMeta(const U& metaPos);
	
	virtual size_t dim() const override; // = 1
	virtual void linkTo(IndefinitIndexBase* target) override;

	virtual void copyPos(const SingleIndex& in) override;

	virtual IndefinitIndexBase* getLinked(const std::string& name) override;
	virtual size_t giveSubStepSize(IndefinitIndexBase* subIndex) override;
	
	//virtual void eval() override;
	//virtual bool virt() const override { return false; }
	
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
	size_t getMeta(const U& metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<U,TYPE> begin() const override;
	SingleIndex<U,TYPE> end() const override;
	
    protected:
	std::vector<U> mSpace;
    };

    // specializaions

    template <>
    class SingleRange<int,RangeType::SPACE> : public RangeBase<SingleIndex<int,RangeType::SPACE> >
    {
    public:
	DEFAULT_MEMBERS(SingleRange);

	SingleRange(int begin, int end);
	
	virtual size_t size() const override;
	
	int get(size_t pos) const;
	size_t getMeta(int metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<int,RangeType::SPACE> begin() const override;
	SingleIndex<int,RangeType::SPACE> end() const override;
	
    protected:
	int mBegin;
	int mEnd;
    };

    template <>
    class SingleRange<size_t,RangeType::ENSEMBLE> : public RangeBase<SingleIndex<size_t,RangeType::ENSEMBLE> >
    {
    public:
	DEFAULT_MEMBERS(SingleRange);

	SingleRange(size_t num);
	
	virtual size_t size() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<size_t,RangeType::ENSEMBLE> begin() const override;
	SingleIndex<size_t,RangeType::ENSEMBLE> end() const override;
	
    protected:
	size_t mNum;
    };

    enum class VET
    {
	VALUE = 0,
	ERROR = 1
    };

    template <>
    class SingleRange<VET,RangeType::VALUE_ERROR> : public RangeBase<SingleIndex<VET,RangeType::VALUE_ERROR> >
    {
    public:
	DEFAULT_MEMBERS(SingleRange);

	virtual size_t size() const override;
	
	VET get(size_t pos) const;
	size_t getMeta(VET metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<VET,RangeType::VALUE_ERROR> begin() const override;
	SingleIndex<VET,RangeType::VALUE_ERROR> end() const override;
    };

    template <>
    class SingleRange<size_t,RangeType::LORENTZ> : public RangeBase<SingleIndex<size_t,RangeType::LORENTZ> >
    {
    public:
	DEFAULT_MEMBERS(SingleRange);

	virtual size_t size() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<size_t,RangeType::LORENTZ> begin() const override;
	SingleIndex<size_t,RangeType::LORENTZ> end() const override;
    };
    
}

#include "single_range.cc"

#endif
