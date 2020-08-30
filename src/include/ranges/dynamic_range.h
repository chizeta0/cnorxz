
#ifndef __dynamic_range_h__
#define __dynamic_range_h__

#include <cassert>
#include <map>

#include "ranges/rbase_def.h"
#include "ranges/range_base.h"
#include "ranges/index_base.h"

#include "xfor/xfor.h"

//#include "ranges/rpheader.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"
#include "ranges/dynamic_meta.h"

#include "index_wrapper.h"
#include "rpack_num.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    class DynamicIndex : public IndexInterface<DynamicIndex,vector<char>>
    {
    private:
        typedef vector<std::pair<std::shared_ptr<IndexW>,size_t>> IVecT;
	typedef std::map<std::string,std::shared_ptr<IndexW>> IMapT;

	static IMapT sIMap;
	
        IVecT mIVec;
        bool mIvecInit = false;
        
    public:
        typedef IndexInterface<DynamicIndex,vector<char>> IB;
	typedef vector<char> MetaType;
	typedef DynamicRange RangeType;
	typedef DynamicIndex IType;

        DynamicIndex(const std::shared_ptr<RangeType>& range);

	static void clearIMap() { sIMap.clear(); }

	template <class Index>
	static std::shared_ptr<Index> getIndexFromMap(const std::string& name)
	{
	    auto tmp = std::dynamic_pointer_cast<IndexWrapper<Index>>(sIMap.at(name));
	    assert(tmp);
	    return tmp->getIndex();
	}
	
        static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }

	static constexpr SpaceType STYPE = SpaceType::DYN;

        IndexType type() const;
	
	DynamicIndex& operator=(size_t pos);
	DynamicIndex& operator++();
	DynamicIndex& operator--();

        DynamicIndex& operator()(const IVecT& ivec);
        DynamicIndex& operator()(const vector<std::shared_ptr<IndexW>>& ivec);
        DynamicIndex& operator()(const vector<std::string>& inames);
		
	template <class... Indices>
	DynamicIndex& operator()(const std::shared_ptr<Indices>&... is);

	DynamicIndex& sync();
	
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	const MetaType* metaPtr() const;
	DynamicIndex& at(const MetaType& metaPos);
	size_t posAt(const MetaType& metaPos) const;

	//bool isMeta(const MetaType& metaPos) const;
	
	size_t dim() const;
	bool last() const;
	bool first() const;

        const IndexW& get(size_t n) const;

	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	void getPtr();

	size_t getStepSize(size_t n) const;
        
	std::string id() const;
	void print(size_t offset);

	template <class Expr>
	DynamicExpression ifor(size_t step, Expr ex) const;

	template <class Expr>
	DynamicExpression iforh(size_t step, Expr ex) const;

        template <class Expr>
	DynamicExpression pifor(size_t step, Expr ex) const;

    };    

    
    // NOT THREAD SAVE!!
    class DynamicRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef DynamicRange oType;

	DynamicRangeFactory();

	template <class... RangeTypes>
	DynamicRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	DynamicRangeFactory(std::shared_ptr<RangeTypes>... origs);

	DynamicRangeFactory(const vector<std::shared_ptr<RangeBase>>& origs);

	template <class Range>
	void append(std::shared_ptr<Range> r);

        std::shared_ptr<RangeBase> create();

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const vector<std::shared_ptr<RangeBase> >& pvec);
	
	static std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > mAleadyCreated;

	bool mProductCreated = false;
    };

    class DynamicRange : public RangeInterface<DynamicIndex>
    {
    public:
        static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	typedef RangeBase RB;
	typedef DynamicIndex IndexType;
	typedef DynamicRange RangeType;
	typedef vector<char> MetaType;
        typedef DynamicRangeFactory FType;
        
    private:
	DynamicRange() = default;
	DynamicRange(const DynamicRange& in) = default;

	template <class... RangeTypes>
	DynamicRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	DynamicRange(std::shared_ptr<RangeTypes>... origs);

        DynamicRange(const vector<std::shared_ptr<RangeBase>>& origs);

	size_t mSize = 1;
	bool mEmpty = true;
	
	vector<std::shared_ptr<RangeBase> > mOrig;

    public:
	virtual size_t size() const final;
	virtual size_t dim() const final;

	MetaType get(size_t pos) const;
	size_t getMeta(const MetaType& metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	std::shared_ptr<RangeBase> sub(size_t num) const;

	template <class Range>
	std::shared_ptr<Range> fullsub(size_t num) const;

	template <class... Ranges>
	std::shared_ptr<MultiRange<Ranges...> > scast(SIZET<Ranges>... sizes) const; // save cast

        const vector<std::shared_ptr<RangeBase> >& orig() const;
        
	void sreplace(const std::shared_ptr<RangeBase> in, size_t num);
	
	bool isEmpty() const;
	
	friend DynamicRangeFactory;
	
	static DynamicRangeFactory factory()
	{ return DynamicRangeFactory(); }

    };

} // namespace MultiArrayTools


namespace MultiArrayHelper
{
    namespace
    {
        using namespace MultiArrayTools;
    }

    template <>
    inline void resolveSetRange<DynamicRange>(std::shared_ptr<DynamicRange>& rp,
                                              const vector<std::shared_ptr<RangeBase> >& orig,
                                              size_t origpos, size_t size);

    template <>
    inline void setRangeToVec<DynamicRange>(vector<std::shared_ptr<RangeBase> >& v,
                                            std::shared_ptr<DynamicRange> r);

    template <>
    inline size_t getStepSize<DynamicIndex>(const DynamicIndex& ii, std::intptr_t j);

}

//#include "dynamic_range.cc.h"

#endif
