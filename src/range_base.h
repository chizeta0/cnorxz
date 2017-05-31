// -*- C++ -*-

#ifndef __range_base_h__
#define __range_base_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"

namespace MultiArrayTools
{
    
    enum class RangeType
    {
	NIL = 0,
	ANY = 1,
	SPACE = 2,
	MOMENTUM = 3,
	LORENTZ = 4,
	SPIN = 5,
	ENSEMBLE = 6,
	VALUE_ERROR = 7,
	DISTANCE = 8
    };

    class MultiRangeType
    {
    public:

	DEFAULT_MEMBERS(MultiRangeType);

	MultiRangeType(const RangeType& type);
	MultiRangeType(const std::vector<MultiRangeType>& multiType);

	~MultiRangeType();
	
	MultiRangeType& operator=(const RangeType& type);
	MultiRangeType& operator=(const std::vector<MultiRangeType>& multiType);

	MultiRangeType& operator[](size_t num);
	const MultiRangeType& operator[](size_t num) const;
	
	bool multi() const;

	bool operator==(const MultiRangeType& in) const;
	bool operator!=(const MultiRangeType& in) const;
	
    private:
	void setType(RangeType type);
	void setMultiType(const std::vector<MultiRangeType>& multiType);
	
	RangeType mType;
	std::vector<MultiRangeType>* mMultiType;
    };

    class IndefinitRangeBase
    {
    public:

	virtual ~IndefinitRangeBase() = default;

	static std::shared_ptr<IndefinitRangeBase> makeInstance(const MultiRangeType& mrt); // !!!
	
	virtual size_t size() const = 0;
	virtual bool isSubRange() const = 0;
	virtual MultiRangeType type() const = 0;
	virtual std::shared_ptr<IndefinitIndexBase> indexInstance() const = 0;
    };
    
    template <class Index>
    class RangeBase : public IndefinitRangeBase
    {
    public:
	typedef Index IndexType;

	virtual IndexType begin() const = 0;
	virtual IndexType end() const = 0;
	virtual RangeBase<Index>* base();
	virtual bool isSubRange() const override;

	virtual std::shared_ptr<IndefinitIndexBase> indexInstance() const override;
    };
  
    //template <class Range>
    //auto cross(const Range& r1, const Range& r2) -> /**/;
    
    //template <class Range1, class Range2>
    //auto cross(const Range1& r1, const Range2& r2) -> /**/;
    
    template <class Index>
    class SubRangeBase : public RangeBase<Index>
    {
    public:
	virtual bool isSubRange() const override;
	virtual RangeBase<Index>* base() override;
    protected:
	DEFAULT_MEMBERS(SubRangeBase);
	RangeBase<Index>* mBase;
	std::vector<bool> mOccupation;
    };

}

#include "range_base.cc"

#endif
