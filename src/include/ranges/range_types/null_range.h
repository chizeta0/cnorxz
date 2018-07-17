

#ifdef include_range_type
include_range_type(NUL,-2)
#else

#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

//#ifndef __spin_range_h__
//#define __spin_range_h__

namespace MultiArrayTools
{   
    typedef SingleIndex<size_t,SpaceType::NUL> NullIndex;

    template <>
    class SingleRangeFactory<size_t,SpaceType::NUL> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<size_t,SpaceType::NUL> oType;

	SingleRangeFactory();
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class SingleRange<size_t,SpaceType::NUL> : public RangeInterface<NullIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<size_t,SpaceType::NUL> >::IndexType IndexType;
	typedef SingleRange<size_t,SpaceType::NUL> RangeType;
	typedef size_t MetaType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metapos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	//virtual std::shared_ptr<VIWB> index() const override;
	
	friend SingleRangeFactory<size_t,SpaceType::NUL>;

	static constexpr bool defaultable = true;

	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = 1;
	static constexpr bool HASMETACONT = false;
	
	static SingleRangeFactory<size_t,SpaceType::NUL> factory()
	{ return SingleRangeFactory<size_t,SpaceType::NUL>(); }
	
    protected:

	SingleRange() = default;
	SingleRange(const SingleRange& in) = delete;

	//SingleRange(size_t spinNum);
    };

    typedef SingleRange<size_t,SpaceType::NUL> NullRange;
    typedef SingleRangeFactory<size_t,SpaceType::NUL> NullRF;
}


//#endif // #ifndef __spin_range_h__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
