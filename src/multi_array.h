// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"
#include "multi_array_operation.h"
#include "manipulator.h"
#include "name.h"

namespace MultiArrayTools
{

    template <typename T, class Range>
    class MultiArrayBase
    {
    public:

	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const Range& range);
	
	virtual T& operator[](const typename Range::IndexType& i) = 0;
	virtual const T& operator[](const typename Range::IndexType& i) const = 0;

	virtual size_t size() const; 
	virtual bool isSlice() const = 0;
	
	virtual auto begin() -> decltype(Range().begin());
	virtual auto end() -> decltype(Range().end());

	virtual const Range& range() const;

	template <class... NameTypes>
	MultiArrayOperationRoot<T,Range> operator()(const NameTypes&... str);

	virtual void manipulate(ManipulatorBase<T>& mb, size_t manBegin, size_t manEnd) = 0;
	
    protected:
	std::shared_ptr<Range> mRange;

    };
    
    template <typename T, class Range>
    class MultiArray : public MultiArrayBase<T,Range>
    {
    public:

	typedef MultiArrayBase<T,Range> MAB;

	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const Range& range);
	MultiArray(const Range& range, const std::vector<T>& vec);
	MultiArray(const Range& range, std::vector<T>&& vec);
	
	T& operator[](const typename Range::IndexType& i) override;
	const T& operator[](const typename Range::IndexType& i) const override;
	
	virtual bool isSlice() const override;

	virtual void manipulate(ManipulatorBase<T>& mb, size_t manBegin, size_t manEnd) override;
	
	template <typename U, class RangeX>
	friend class MultiArray;
	
    private:
	std::vector<T> mCont;
    };

}

#include "multi_array.cc"

#endif
