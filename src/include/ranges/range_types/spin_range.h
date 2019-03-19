

#ifdef include_range_type
include_range_type(SPIN,2)
#else

#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

#ifndef __range_type_spin_def__
#define __range_type_spin_def__

namespace MultiArrayTools
{
    typedef GenSingleIndex<size_t,SpaceType::SPIN,4> SpinIndex;

    template <>
    class GenSingleRangeFactory<size_t,SpaceType::SPIN,4> : public RangeFactoryBase
    {
    public:
	
	typedef GenSingleRange<size_t,SpaceType::SPIN,4> oType;

	GenSingleRangeFactory();
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class GenSingleRange<size_t,SpaceType::SPIN,4> : public RangeInterface<SpinIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<GenSingleIndex<size_t,SpaceType::SPIN,4> >::IndexType IndexType;
	typedef GenSingleRange<size_t,SpaceType::SPIN,4> RangeType;
	typedef size_t MetaType;
        typedef GenSingleRangeFactory<size_t,SpaceType::SPIN,4> FType; 
        
	virtual size_t size() const final;
	virtual size_t dim() const final;

        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
	//virtual std::shared_ptr<VIWB> index() const final;
	
	friend GenSingleRangeFactory<size_t,SpaceType::SPIN,4>;

	static constexpr bool defaultable = true;
	static constexpr size_t mSpinNum = 4;

	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = mSpinNum;
	static constexpr bool HASMETACONT = false;
	
	static GenSingleRangeFactory<size_t,SpaceType::SPIN,4> factory()
	{ return GenSingleRangeFactory<size_t,SpaceType::SPIN,4>(); }
	
    protected:

	GenSingleRange() = default;
	GenSingleRange(const GenSingleRange& in) = delete;

	//GenSingleRange(size_t spinNum);
    };

    typedef GenSingleRange<size_t,SpaceType::SPIN,4> SpinRange;
    typedef GenSingleRangeFactory<size_t,SpaceType::SPIN,4> SpinRF;

    std::shared_ptr<SpinRF> mkSPIN(const char* dp, size_t size);
}


#endif // #ifndef __range_type_spin_def__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
