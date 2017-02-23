
// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"
#include "multi_array_operation.h"
#include "name.h"

namespace MultiArrayTools
{
   
    template <typename T, class Range>
    class MultiArray
    {
    public:

	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const Range& range);
	MultiArray(const Range& range, const std::vector<T>& vec);
	MultiArray(const Range& range, std::vector<T>&& vec);
	
	template <class... NameTypes>
	MultiArrayOperationBase<T,Range> operator()(const NameTypes&... str);
	
	T& operator[](const typename Range::IndexType& i);
	const T& operator[](const typename Range::IndexType& i) const;

	size_t size() const; 

	virtual bool isSlice() const;
	
	auto begin() -> decltype(Range().begin());
	auto end() -> decltype(Range().end());

    protected:

	std::shared_ptr<Range> mRange;
	
    private:
	bool mInit = false;
	std::vector<T> mCont;
    };

}

#include "multi_array.cc"

#endif
