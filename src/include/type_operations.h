
#ifndef __type_operations_h__
#define __type_operations_h__

#include <cstdlib>

#include "base_def.h"
#include "mbase_def.h"

#include "pack_num.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    template <typename T, class... Ranges>
    class operate
    {
    public:
	static constexpr bool FISSTATIC = false;

	operate(const std::shared_ptr<typename Ranges::IndexType>&... inds) : ituple(inds...) {}
	
	inline auto apply(const MultiArray<T,Ranges...>& ma)
	    -> OperationRoot<T,Ranges...>
	{
	    return PackNum<sizeof...(Ranges)-1>::mkElemOperation(ma, ituple);
	}

    private:

	// this is, why non-static
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> ituple;
    };

    template <class OperationClass, typename T, class... Ranges>
    class OperationTemplate<MultiArray<T,Ranges...>,OperationClass> : public OperationBase<MultiArray<T,Ranges...>,OperationClass>
    {
	typedef OperationBase<MultiArray<T,Ranges...>,OperationClass> OB;
	
	auto operator()(const std::shared_ptr<typename Ranges::IndexType>&... indices)
	    -> Operation<OperationRoot<T,Ranges...>,operate<T,Ranges...>,OperationClass>
	{
	    std::shared_ptr<operate<T,Ranges...> > ff(indices...);
	    return Operation<OperationRoot<T,Ranges...>,operate<T,Ranges...>,OperationClass>(ff, OB::THIS());
	}

    private:
	OperationTemplate() = default;
	friend OperationClass;

    };
    


} // namespace MultiArrayTools

#endif
