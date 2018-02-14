

#ifdef include_range_type
include_range_type(SPIN,2)
#else

#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

//#ifndef __spin_range_h__
//#define __spin_range_h__

namespace MultiArrayTools
{
    typedef SingleIndex<size_t,SpaceType::SPIN> SpinIndex;

    template <>
    class SingleRangeFactory<size_t,SpaceType::SPIN> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<size_t,SpaceType::SPIN> oType;

	SingleRangeFactory();
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class SingleRange<size_t,SpaceType::SPIN> : public RangeInterface<SpinIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<size_t,SpaceType::SPIN> >::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	virtual std::shared_ptr<VIWB> index() const override;
	
	friend SingleRangeFactory<size_t,SpaceType::SPIN>;

	static constexpr bool defaultable = true;
	static constexpr size_t mSpinNum = 4;

	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = mSpinNum;
	
	static SingleRangeFactory<size_t, SpaceType::SPIN> factory()
	{ return SingleRangeFactory<size_t, SpaceType::SPIN>(); }
	
    protected:

	SingleRange() = default;
	SingleRange(const SingleRange& in) = delete;

	//SingleRange(size_t spinNum);
    };

    typedef SingleRange<size_t,SpaceType::SPIN> SpinRange;
    typedef SingleRangeFactory<size_t,SpaceType::SPIN> SpinRF;
}


//#endif // #ifndef __spin_range_h__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
