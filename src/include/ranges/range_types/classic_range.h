
//#ifdef include_range_type
//include_range_type(NONE,0)
//#else

#ifndef include_range_type
#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

#ifndef __range_type_classic_def__
#define __range_type_classic_def__

namespace MultiArrayTools
{
    typedef GenSingleIndex<size_t,SpaceType::NONE,-1> ClassicIndex;

    template <>
    class GenSingleRangeFactory<size_t,SpaceType::NONE,-1> : public RangeFactoryBase
    {
    public:
	
	typedef GenSingleRange<size_t,SpaceType::NONE,-1> oType;

	GenSingleRangeFactory(size_t size = 0);
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class GenSingleRange<size_t,SpaceType::NONE,-1> : public RangeInterface<ClassicIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<GenSingleIndex<size_t,SpaceType::NONE,-1> >::IndexType IndexType;
	typedef GenSingleRange<size_t,SpaceType::NONE,-1> RangeType;
	typedef size_t MetaType;
        typedef GenSingleRangeFactory<size_t,SpaceType::NONE,-1> FType; 
        
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
	//virtual std::shared_ptr<VIWB> index() const final;
	
	friend GenSingleRangeFactory<size_t,SpaceType::NONE,-1>;

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	static GenSingleRangeFactory<size_t,SpaceType::NONE,-1> factory(size_t size = 0)
	{ return GenSingleRangeFactory<size_t,SpaceType::NONE,-1>(size); }
	
    protected:

	size_t mSize = 0; 
	
	GenSingleRange() = default;
	GenSingleRange(const GenSingleRange& in) = delete;

	GenSingleRange(size_t size);
    };

    typedef GenSingleRange<size_t,SpaceType::NONE,-1> ClassicRange;
    typedef GenSingleRangeFactory<size_t,SpaceType::NONE,-1> ClassicRF;
}


#endif // #ifndef __range_type_classic_def__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
