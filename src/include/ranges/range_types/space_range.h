
#ifdef include_range_type
include_range_type(PSPACE,3) // Periodic 1dim space
#else

#ifdef __ranges_header__

#ifndef __range_type_space_def__
#define __range_type_space_def__

namespace MultiArrayTools
{
     // Periodic 1dim space
    typedef SingleIndex<int,SpaceType::PSPACE> XSpaceIndex;

    template <>
    class SingleRangeFactory<int,SpaceType::PSPACE> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<int,SpaceType::PSPACE> oType;

	SingleRangeFactory(size_t size = 0);
	std::shared_ptr<RangeBase> create();
	
    };
    
    template <>
    class SingleRange<int,SpaceType::PSPACE> : public RangeInterface<XSpaceIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<int,SpaceType::PSPACE> >::IndexType IndexType;
	typedef SingleRange<int,SpaceType::PSPACE> RangeType;
	typedef int MetaType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;

	virtual std::string stringMeta(size_t pos) const override;
	virtual std::vector<char> data() const override;

	int get(size_t pos) const;
	size_t getMeta(int metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	
	friend SingleRangeFactory<int,SpaceType::PSPACE>;

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	static SingleRangeFactory<int, SpaceType::PSPACE> factory(size_t size = 0)
	{ return SingleRangeFactory<int, SpaceType::PSPACE>(size); }
	
    protected:

	size_t mSize = 0;
	
	SingleRange() = default;
	SingleRange(const SingleRange& in) = delete;
	
	SingleRange(size_t size);
    };

    typedef SingleRange<int,SpaceType::PSPACE> PSpaceRange;
    typedef SingleRangeFactory<int,SpaceType::PSPACE> PSpaceRF;

    std::shared_ptr<PSpaceRF> mkPSPACE(char* dp, size_t size);
    
    template <class SpaceRange>
    struct PromoteMSpaceRange
    {
	template <class... SpaceRanges>
	static auto mk(MultiRange<SpaceRanges...>) -> MultiRange<SpaceRange,SpaceRanges...>;

	template <class... SpaceRanges>
	static auto mkf(MultiRangeFactory<SpaceRanges...>) -> MultiRangeFactory<SpaceRange,SpaceRanges...>;

    };

    template <size_t N>
    struct CreateNDimSpaceRange
    {
	template <class SpaceRange>
	static auto mk()
	    -> decltype(PromoteMSpaceRange<SpaceRange>::
			template mk(CreateNDimSpaceRange<N-1>::
				    template mk<SpaceRange>()));

	template <class SpaceRange>
	static auto mkf()
	    -> decltype(PromoteMSpaceRange<SpaceRange>::
			template mkf(CreateNDimSpaceRange<N-1>::
				     template mkf<SpaceRange>()));

    };

    template <>
    struct CreateNDimSpaceRange<1>
    {
	template <class SpaceRange>
	static auto mk()
	    -> MultiRange<SpaceRange>;

	template <class SpaceRange>
	static auto mkf()
	    -> MultiRangeFactory<SpaceRange>;

    };

    template <class SpaceRange, size_t N>
    using MSpaceRange = decltype(CreateNDimSpaceRange<N>::template mk<SpaceRange>());

    template <class SpaceRange, size_t N>
    using MSpaceRF = decltype(CreateNDimSpaceRange<N>::template mkf<SpaceRange>());
}

#endif // #ifndef __range_type_space_def__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
