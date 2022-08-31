
#ifndef __dcontainer_index_h__
#define __dcontainer_index_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "mbase_def.h"
#include "statics/static_for.h"
#include "ranges/range_helper.h"

namespace CNORXZ
{
    // Future DynamicIndex
    class XIndexBase : public IndexInterface<XIndexBase,DType>
    {
    public:
	DEFAULT_MEMBERS(XIndexBase);
	// ...!!!
    };

    typedef std::shared_ptr<XIndexBase> XIndexPtr;
    
    // MultiIndex Wrapper:
    template <class... Indices>
    class XIndex
    {
    private:
	std::shared_ptr<MultiIndex<Indices...>> mI;

    public:
	DEFAULT_MEMBERS(XIndex);
	XIndex(const std::shared_ptr<MultiIndex<Indices...>>& i);

	// ....!!!!
    };
    
    template <typename T>
    class DConstContainerIndex : public IndexInterface<DConstContainerIndex<T>,DType>
    {
    protected:
	XIndexPtr mI;
	const T* mCData = nullptr;
	size_t mCPos = 0;
    };

    template <typename T>
    class MDConstContainerIndex : public DConstContainerIndex<T>
    {
    private:
	T* mData = nullptr;
    };
}

#endif
