// -*- C++ -*-

#ifndef __multi_array_h__
#define __multi_array_h__

#include <cstdlib>

#include "base_def.h"
#include "multi_array_operation.h"

namespace MultiArrayTools
{

    template <typename T, class Range>
    class MultiArray
    {
    public:

	DEFAULT_MEMBERS(MultiArray);
	MultiArray(const Range& range); // !!!!
	
	template <class... NameTypes>
	MultiArrayOperation<T,Range>& operator()(const NameTypes&... str) const;
	
	T& operator()(const typename Range::indexType& i);
	const T& operator()(const typename Range::indexType& i) const;
	
    private:
	bool mInit = false;
	std::vector<T> mCont;
	std::shared_ptr<Range> mRange;
    };

}

#include "multi_array.cc"

#endif
