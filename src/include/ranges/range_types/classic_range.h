
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
    typedef SingleIndex<size_t,SpaceType::NONE> ClassicIndex;

    template <>
    class SingleRangeFactory<size_t,SpaceType::NONE> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<size_t,SpaceType::NONE> oType;

	SingleRangeFactory(size_t size = 0);
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class SingleRange<size_t,SpaceType::NONE> : public RangeInterface<ClassicIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<size_t,SpaceType::NONE> >::IndexType IndexType;
	typedef SingleRange<size_t,SpaceType::NONE> RangeType;
	typedef size_t MetaType;
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
	//virtual std::shared_ptr<VIWB> index() const final;
	
	friend SingleRangeFactory<size_t,SpaceType::NONE>;

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	static SingleRangeFactory<size_t, SpaceType::NONE> factory(size_t size = 0)
	{ return SingleRangeFactory<size_t, SpaceType::NONE>(size); }
	
    protected:

	size_t mSize = 0; 
	
	SingleRange() = default;
	SingleRange(const SingleRange& in) = delete;

	SingleRange(size_t size);
    };

    typedef SingleRange<size_t,SpaceType::NONE> ClassicRange;
    typedef SingleRangeFactory<size_t,SpaceType::NONE> ClassicRF;
}


#endif // #ifndef __range_type_classic_def__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
