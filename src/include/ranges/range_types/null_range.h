

#ifdef include_range_type
include_range_type(NUL,-2)
#else

#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

#ifndef __range_type_null_def__
#define __range_type_null_def__

namespace CNORXZ
{   
    typedef GenSingleIndex<size_t,SpaceType::NUL,0> NullIndex;

    std::shared_ptr<GenSingleRange<size_t,SpaceType::NUL,0> > nullr();
    std::shared_ptr<NullIndex> nulli();

    template <>
    class GenSingleRangeFactory<size_t,SpaceType::NUL,0> : public RangeFactoryBase
    {
    public:
	
	typedef GenSingleRange<size_t,SpaceType::NUL,0> oType;

	GenSingleRangeFactory();
	std::shared_ptr<RangeBase> create();

	friend std::shared_ptr<oType> nullr();
	
    private:
	static std::shared_ptr<oType> mRInstance;
    };

    template <>
    class GenSingleRange<size_t,SpaceType::NUL,0> : public RangeInterface<NullIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<GenSingleIndex<size_t,SpaceType::NUL,0> >::IndexType IndexType;
	typedef GenSingleRange<size_t,SpaceType::NUL,0> RangeType;
	typedef size_t MetaType;
        typedef GenSingleRangeFactory<size_t,SpaceType::NUL,0> FType; 
	
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
	size_t getMeta(size_t metapos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;
	//virtual std::shared_ptr<VIWB> index() const final;
	
	friend GenSingleRangeFactory<size_t,SpaceType::NUL,0>;

	static constexpr bool defaultable = true;

	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = 1;
	static constexpr bool HASMETACONT = false;
	
	static GenSingleRangeFactory<size_t,SpaceType::NUL,0> factory()
	{ return GenSingleRangeFactory<size_t,SpaceType::NUL,0>(); }
	
    protected:

	GenSingleRange() = default;
	GenSingleRange(const GenSingleRange& in) = delete;

	//GenSingleRange(size_t spinNum);
    };

    typedef GenSingleRange<size_t,SpaceType::NUL,0> NullRange;
    typedef GenSingleRangeFactory<size_t,SpaceType::NUL,0> NullRF;
    
    std::shared_ptr<NullRF> mkNUL(const char* dp, size_t size);
	
}


#endif // #ifndef __range_type_null_def__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
