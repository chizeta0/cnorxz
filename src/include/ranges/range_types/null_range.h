

#ifdef include_range_type
include_range_type(NUL,-2)
#else

#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

#ifndef __range_type_null_def__
#define __range_type_null_def__

namespace MultiArrayTools
{   
    typedef SingleIndex<size_t,SpaceType::NUL> NullIndex;

    std::shared_ptr<SingleRange<size_t,SpaceType::NUL> > nullr();
    std::shared_ptr<NullIndex> nulli();

    template <>
    class SingleRangeFactory<size_t,SpaceType::NUL> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<size_t,SpaceType::NUL> oType;

	SingleRangeFactory();
	std::shared_ptr<RangeBase> create();

	friend std::shared_ptr<oType> nullr();
	
    private:
	static std::shared_ptr<oType> mRInstance;
    };

    template <>
    class SingleRange<size_t,SpaceType::NUL> : public RangeInterface<NullIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<size_t,SpaceType::NUL> >::IndexType IndexType;
	typedef SingleRange<size_t,SpaceType::NUL> RangeType;
	typedef size_t MetaType;
        typedef SingleRangeFactory<size_t,SpaceType::NUL> FType; 
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	size_t get(size_t pos) const;
	size_t getMeta(size_t metapos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
	//virtual std::shared_ptr<VIWB> index() const final;
	
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
    
    std::shared_ptr<NullRF> mkNUL(const char* dp, size_t size);
	
}


#endif // #ifndef __range_type_null_def__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
