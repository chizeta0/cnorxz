
#ifdef include_range_type
include_range_type(PSPACE,3) // Periodic 1dim space
#else

#ifdef __ranges_header__

#ifndef __range_type_space_def__
#define __range_type_space_def__

namespace MultiArrayTools
{
     // Periodic 1dim space
    typedef GenSingleIndex<int,SpaceType::PSPACE,-1> XSpaceIndex;

    template <>
    class GenSingleRangeFactory<int,SpaceType::PSPACE,-1> : public RangeFactoryBase
    {
    public:
	
	typedef GenSingleRange<int,SpaceType::PSPACE,-1> oType;

	GenSingleRangeFactory(size_t size = 0);
	std::shared_ptr<RangeBase> create();
	
    };
    
    template <>
    class GenSingleRange<int,SpaceType::PSPACE,-1> : public RangeInterface<XSpaceIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<GenSingleIndex<int,SpaceType::PSPACE,-1> >::IndexType IndexType;
	typedef GenSingleRange<int,SpaceType::PSPACE,-1> RangeType;
	typedef int MetaType;
        typedef GenSingleRangeFactory<int,SpaceType::PSPACE,-1> FType; 
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	int get(size_t pos) const;
	size_t getMeta(int metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
	
	friend GenSingleRangeFactory<int,SpaceType::PSPACE,-1>;

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	static GenSingleRangeFactory<int,SpaceType::PSPACE,-1> factory(size_t size = 0)
	{ return GenSingleRangeFactory<int,SpaceType::PSPACE,-1>(size); }
	
    protected:

	size_t mSize = 0;
	
	GenSingleRange() = default;
	GenSingleRange(const GenSingleRange& in) = delete;
	
	GenSingleRange(size_t size);
    };

    typedef GenSingleRange<int,SpaceType::PSPACE,-1> PSpaceRange;
    typedef GenSingleRangeFactory<int,SpaceType::PSPACE,-1> PSpaceRF;

    std::shared_ptr<PSpaceRF> mkPSPACE(const char* dp, size_t size);
    
    template <class SpaceRange>
    struct PromoteMSpaceRange
    {
	template <class... SpaceRanges>
	static auto mk(const MultiRange<SpaceRanges...>&)
	    -> MultiRange<SpaceRange,SpaceRanges...>;

	template <class... SpaceRanges>
	static auto mkf(const MultiRangeFactory<SpaceRanges...>&)
	    -> MultiRangeFactory<SpaceRange,SpaceRanges...>;

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
