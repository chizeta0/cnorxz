
#ifndef __operation_helper_h__
#define __operation_helper_h__

#include "multi_array_operation.h"

namespace MultiArrayTools
{

    template <class BaseArray, class... Ranges>
    class PseudoArray
    {
	size_t mThreadNum;
	Slice<> mSl;
	mutable ConstOperationRoot<T,Range> mOp;
	
    public:

	template <class ET>
	const SrcHolder<ConstOperationRoot<T,Range>> operator[](ET pos) const;
    };
    
    template <class Operation>
    class SrcHolder
    {

	TempHolder operator+(SrcHolder in) const;
	// aso
    };

    template <class Operation>
    class TempHolder
    {
	TempHolder operator+(SrcHolder in) const;
	TempHolder operator+(TempHolder in) const;
    };

    template <class Operation>
    class TarHolder
    {
	TarHolder& operator=(TempHolder in);
    };
    
} // namespace MultiArrayTools

#endif
