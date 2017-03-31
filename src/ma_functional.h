// -*- C++ -*-

#ifndef __ma_functional_h__
#define __ma_functional_h__

#include <cstdlib>
#include <tuple>
#include <utility>

#include "base_def.h"

namespace MultiArrayTools
{

    // Map is a ConstMultiArrayOperationRoot where the corresponding MultiArray defines the map
    template <class InRange, class OutRange>
    class IndexMapFunction
    {
    public:

	typedef OutRange OR;
	typedef typename OutRange::IndexType OutIndex;

	//IndexMapFunction() = default;
	
	IndexMapFunction(const MultiArrayBase<typename OutRange::IndexType,InRange>& ma,
			 const OutRange& outRange,
			 const Name& inName, const Name& outName);

	IndexMapFunction(const IndexMapFunction& in) = default;
	
	void linkIndicesTo(IndefinitIndexBase* target);
	void eval() const;
	
	IndefinitIndexBase& index() const;
	
    private:
	
	ConstMultiArrayOperationRoot<typename OutRange::IndexType,InRange> mMap;
	std::shared_ptr<OutRange> mOutRange;
	mutable typename OutRange::IndexType mOIndex;	
    };

    class vec3d2Function
    {
    public:

	typedef SingleIndex<int,RangeType::SPACE> CoordIndex;
	typedef MultiIndex<CoordIndex,CoordIndex,CoordIndex> InIndex;
	typedef SingleIndex<size_t,RangeType::DISTANCE> OutSubIndex;
	typedef MultiIndex<OutSubIndex> OutIndex;
	typedef MultiRange<SingleRange<size_t,RangeType::DISTANCE> > OutRange;

	DEFAULT_MEMBERS(vec3d2Function);

	vec3d2Function(const OutRange& outRange);
	//vec3d2Function& operator=(const vec3d2Function& in) = default;
	
	OutIndex operator()(const InIndex& i) const;
	
    private:
	std::shared_ptr<OutRange> mOutRange;
	mutable OutIndex out;
    };

    class pyProjFunction
    {
    public:
	typedef SingleIndex<int,RangeType::SPACE> CoordIndex;
	typedef SingleIndex<std::array<int,3>,RangeType::MOMENTUM> MomIndex;
	typedef MultiIndex<MomIndex,CoordIndex,CoordIndex,CoordIndex> InIndex;
	typedef SingleIndex<size_t,RangeType::DISTANCE> DistIndex;
	typedef SingleIndex<int,RangeType::SPACE> ScalProdIndex;
	typedef MultiIndex<DistIndex,ScalProdIndex> OutIndex;
	typedef MultiRange<SingleRange<size_t,RangeType::DISTANCE>,
			   SingleRange<int,RangeType::SPACE> > OutRange;

	DEFAULT_MEMBERS(pyProjFunction);

	pyProjFunction(const OutRange& outRange);
	OutIndex operator()(const InIndex& i) const;

    private:
	std::shared_ptr<OutRange> mOutRange;
	mutable OutIndex out;
    };
    
}

#include "ma_functional.cc"

#endif
