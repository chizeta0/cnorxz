
#ifndef __vindex_wrapper_h__
#define __vindex_wrapper_h__

#include "ranges/vindex_base.h"

namespace MultiArrayTools
{
    
    template <class I>
    std::shared_ptr<IndexWrapper<I> > make_viwb(std::shared_ptr<I> idxPtr)
    {
	return std::make_shared<IndexWrapper<I> >(idxPtr);
    }
    
    template <class I>
    class IndexWrapper : public VirtualIndexWrapperBase
    {
    public:
	
	DEFAULT_MEMBERS(IndexWrapper);

	IndexWrapper(std::shared_ptr<I> idxPtr) : mIdxPtr(idxPtr) {}

	virtual IndexType type() const override { return mIdxPtr->type(); }
	virtual size_t dim() const override { return mIdxPtr->dim(); }
	virtual size_t pos() const override { return mIdxPtr->pos(); }
	virtual size_t max() const override { return mIdxPtr->max(); }
	virtual std::shared_ptr<RangeBase> rangePtr() const override { return mIdxPtr->vrange(); }
	virtual std::shared_ptr<VirtualIndexWrapperBase> getPtr(size_t n) const override
	{ return mIdxPtr->getVPtr(n); }
	virtual std::intptr_t getPtrNum() const override { return reinterpret_cast<std::intptr_t>( mIdxPtr.get() ); }
	virtual size_t getStepSize(size_t n) const override { return mIdxPtr->getStepSize(n); }

	std::shared_ptr<I> get() const { return mIdxPtr; } // unwrap
	
    private:
	std::shared_ptr<I> mIdxPtr;
    };

}

#endif
