// -*- C++ -*-

#ifndef __range_base_h__
#define __range_base_h__

#include <cstdlib>
#include <vector>
#include "base_def.h"

namespace MultiArrayTools
{
    
    enum class RangeType
    {
	NIL = 0,
	SPACE = 1,
	MOMENTUM = 2,
	LORENTZ = 3,
	SPIN = 4
    };

    class MultiRangeType
    {
    public:

	DEFAULT_MEMBERS(MultiRangeType);
	
	MultiRangeType& operator=(RangeType& type);
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
    
    template <class Index>
    class RangeBase
    {
    public:
	typedef Index IndexType;
	
	virtual size_t size() const = 0;
	virtual Index begin() = 0;
	virtual Index end() = 0;
	virtual RangeBase<Index>* base() = 0;
	virtual bool isSubRange() const;

    protected:
	DEFAULT_MEMBERS(RangeBase);
	
    };

    template <class Range>
    auto cross(const Range& r1, const Range& r2) -> /**/;
    
    template <class Range1, class Range2>
    auto cross(const Range1& r1, const Range2& r2) -> /**/;
    
    template <class Index>
    class SubRangeBase : public RangeBase<Index>
    {
    public:
	virtual bool isSubRange() const override;
    protected:
	DEFAULT_MEMBERS(SubRangeBase);
	RangeBase<Index>* mBase;
	std::vector<bool> mOccupation;
    };

}

#include "range_base.cc"

#endif
