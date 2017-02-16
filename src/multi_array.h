
// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>
#include <vector>

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
	MultiArrayOperation<T,Range>& operator()(const NameTypes&... str) const;
	
	T& operator()(const typename Range::indexType& i);
	const T& operator()(const typename Range::indexType& i) const;
	
    private:
	bool mInit = false;
	std::shared_ptr<Range> mRange;
	std::vector<T> mCont;
    };

}

#include "multi_array.cc"

#endif
