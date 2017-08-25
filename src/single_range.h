// -*- C++ -*-

#ifndef __single_range_h__
#define __single_range_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"
#include "index_base.h"
#include "range_base.h"

namespace MultiArrayTools
{

    template <typename U, RangeType TYPE>
    class SingleIndex : public IndexInterface<U>
    {
    public:

	typedef IndexBase IB;
	typedef U MetaType;
	typedef SingleRange<U,TYPE> RangeType;
	
	//DEFAULT_MEMBERS_X(SingleIndex);
	
	SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range);
	
	virtual SingleIndex& operator=(size_t pos) override;
	virtual SingleIndex& operator++() override;
	virtual SingleIndex& operator--() override;

	virtual U meta() const override;
	virtual SingleIndex& at(const U& metaPos) override;
	
	virtual size_t dim() const override; // = 1
	virtual bool last() const override;
	virtual bool first() const override;

	virtual std::string id() const override { return std::string("sin") + std::to_string(IB::mId); }
    };

    template <typename U, RangeType TYPE>
    class SingleRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<U,TYPE> oType;

	SingleRangeFactory() = delete;
	SingleRangeFactory(const std::vector<U>& space);
	std::shared_ptr<RangeBase> create();
	
    };
    
    template <typename U, RangeType TYPE>
    class SingleRange : public RangeInterface<SingleIndex<U,TYPE> >
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<U,TYPE> >::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	const U& get(size_t pos) const;
	size_t getMeta(const U& metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	virtual std::shared_ptr<IndexBase> index() const override;
	
	friend SingleRangeFactory<U,TYPE>;
	
    protected:

	SingleRange() = delete;
	SingleRange(const SingleRange& in) = delete;
	
	SingleRange(const std::vector<U>& space);

	std::vector<U> mSpace;
    };
    /*
    // specializaions

    template <>
    class SingleRange<int,RangeType::SPACE> : public RangeBase<SingleIndex<int,RangeType::SPACE> >
    {
    public:
	typedef typename RangeBase<SingleIndex<int,RangeType::SPACE> >::IndexType IndexType;

	static SingleRange<int,RangeType::SPACE> oType() { return SingleRange<int,RangeType::SPACE>(); }
	
	virtual size_t size() const override;
	
	int get(size_t pos) const;
	size_t getMeta(int metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<int,RangeType::SPACE> begin() const override;
	SingleIndex<int,RangeType::SPACE> end() const override;
	
    protected:
	
	SingleRange(size_t ext);

	size_t mExt;
    };

    template <>
    class SingleRange<size_t,RangeType::DISTANCE> : public RangeBase<SingleIndex<size_t,RangeType::DISTANCE> >
    {
    public:
	typedef typename RangeBase<SingleIndex<size_t,RangeType::DISTANCE> >::IndexType IndexType;

	static SingleRange<size_t,RangeType::DISTANCE> oType() { return SingleRange<size_t,RangeType::DISTANCE>(); }
	
	virtual size_t size() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<size_t,RangeType::DISTANCE> begin() const override;
	SingleIndex<size_t,RangeType::DISTANCE> end() const override;
	
    protected:
	
	SingleRange(size_t ext);

	size_t mExt;
    };

    template <>
    class SingleRange<size_t,RangeType::ENSEMBLE> : public RangeBase<SingleIndex<size_t,RangeType::ENSEMBLE> >
    {
    public:
	typedef typename RangeBase<SingleIndex<size_t,RangeType::ENSEMBLE> >::IndexType IndexType;

	static SingleRange<size_t,RangeType::ENSEMBLE> oType() { return SingleRange<size_t,RangeType::ENSEMBLE>(); }
	
	virtual size_t size() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<size_t,RangeType::ENSEMBLE> begin() const override;
	SingleIndex<size_t,RangeType::ENSEMBLE> end() const override;
	
    protected:
	
	SingleRange(size_t num);

	size_t mNum;
    };

    enum class VET
    {
	VALUE = 0,
	ERROR = 1
    };

    std::ostream& operator<<(std::ostream& os, VET vet);
    
    template <>
    class SingleRange<VET,RangeType::VALUE_ERROR> : public RangeBase<SingleIndex<VET,RangeType::VALUE_ERROR> >
    {
    public:
	typedef typename RangeBase<SingleIndex<VET,RangeType::VALUE_ERROR> >::IndexType IndexType;

	static SingleRange<VET,RangeType::VALUE_ERROR> oType()
	{ return SingleRange<VET,RangeType::VALUE_ERROR>(); }
	
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
	typedef typename RangeBase<SingleIndex<size_t,RangeType::LORENTZ> >::IndexType IndexType;

	static SingleRange<size_t,RangeType::LORENTZ> oType() { return SingleRange<size_t,RangeType::LORENTZ>(); }
	
	virtual size_t size() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;

	virtual MultiRangeType type() const override;
	
	SingleIndex<size_t,RangeType::LORENTZ> begin() const override;
	SingleIndex<size_t,RangeType::LORENTZ> end() const override;
    };
    */
}

#include "single_range.cc"

#endif
