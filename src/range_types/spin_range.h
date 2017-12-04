
#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

#ifndef __spin_range_h__
#define __spin_range_h__

namespace MultiArrayTools
{
    typedef SingleIndex<size_t,RangeType::SPIN> SpinIndex;

    template <>
    class SingleRangeFactory<size_t,RangeType::SPIN> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<U,TYPE> oType;

	SingleRangeFactory() = delete;
	SingleRangeFactory(size_t spinNum); // = 4 :)
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class SingleRange<size_t,RangeType::SPIN> : public RangeInterface<SpinIndex>
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
	
	friend SingleRangeFactory<size_t,RangeType::SPIN>;
	
    protected:

	SingleRange() = delete;
	SingleRange(const SingleRange& in) = delete;

	SingleRange(size_t spinNum);

	size_t mSpinNum = 4;
    };
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    // ... !!!!
}

#endif
#endif
