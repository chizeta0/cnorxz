
#include "index_wrapper.h"
#include "range_helper.h"

namespace CNORXZ
{

    template <class Index>
    IndexWrapper<Index>::IndexWrapper(const std::shared_ptr<Index>& i) : mI(i)
    {
	ClassicRF crf(mI->max());
	mCI = std::make_shared<ClassicIndex>
	    ( std::dynamic_pointer_cast<ClassicRange>( crf.create() ) );
	(*mCI) = mI->pos();
    }
        
    template <class Index>
    IndexType IndexWrapper<Index>::type() const
    {
        return mI->type();
    }
	
    template <class Index>
    IndexWrapper<Index>& IndexWrapper<Index>::operator=(size_t pos)
    {
        (*mI) = pos;
        return *this;
    }

    template <class Index>
    IndexWrapper<Index>& IndexWrapper<Index>::operator++()
    {
        ++(*mI);
        return *this;
    }
    
    template <class Index>
    IndexWrapper<Index>& IndexWrapper<Index>::operator--()
    {
        --(*mI);
        return *this;
    }

    template <class Index>
    size_t IndexWrapper<Index>::pos() const
    {
        return mI->pos();
    }
    
    template <class Index>
    size_t IndexWrapper<Index>::max() const
    {
        return mI->max();
    }
        
    template <class Index>
    int IndexWrapper<Index>::pp(std::intptr_t idxPtrNum)
    {
        return mI->pp(idxPtrNum);
    }
    
    template <class Index>
    int IndexWrapper<Index>::mm(std::intptr_t idxPtrNum)
    {
        return mI->mm(idxPtrNum);
    }

    template <class Index>
    std::string IndexWrapper<Index>::stringMeta() const
    {
        return mI->stringMeta();
    }

    template <class Index>
    IndexWrapper<Index>& IndexWrapper<Index>::at(const typename Index::MetaType& metaPos)
    {
        mI->at(metaPos);
        return *this;
    }
    
    template <class Index>
    size_t IndexWrapper<Index>::posAt(const typename Index::MetaType& metaPos)
    {
        return mI->posAt(metaPos);
    }

    template <class Index>
    size_t IndexWrapper<Index>::dim() const
    {
        return mI->dim();
    }
    
    template <class Index>
    bool IndexWrapper<Index>::last() const
    {
        return mI->last();
    }
    
    template <class Index>
    bool IndexWrapper<Index>::first() const
    {
        return mI->first();
    }

    template <class Index>
    std::shared_ptr<RangeBase> IndexWrapper<Index>::range() const
    {
        return mI->range();
    }
        
    template <class Index>
    size_t IndexWrapper<Index>::getStepSize(size_t n) const
    {
        return mI->getStepSize(n);
    }
    
    template <class Index>
    size_t IndexWrapper<Index>::getStepSizeComp(std::intptr_t j) const
    {
        size_t out = RangeHelper::getStepSize(*mI, j);
	if(out == 0){
	    out = RangeHelper::getStepSize(*mCI, j);
	}
	return out;
    }
        
    template <class Index>
    std::intptr_t IndexWrapper<Index>::get() const
    {
        return reinterpret_cast<std::intptr_t>(mI.get());
    }

    template <class Index>
    std::intptr_t IndexWrapper<Index>::ptrNum() const
    {
        return mI->ptrNum();
    }

    template <class Index>
    DynamicExpression IndexWrapper<Index>::ifor(size_t step, DynamicExpression ex) const
    {
        return mI->ifor(step, ex);
    }
    
    template <class Index>
    DynamicExpression IndexWrapper<Index>::iforh(size_t step, DynamicExpression ex) const
    {
        return mI->iforh(step, ex);
    }

    template <class Index>
    std::shared_ptr<IndexWrapperBase> IndexWrapper<Index>::duplicate() const
    {
        return std::make_shared<IndexWrapper>( std::make_shared<Index>( *mI ) );
    }

    template <class Index>
    std::shared_ptr<Index> IndexWrapper<Index>::getIndex() const
    {
	return mI;
    }

    template <class Index>
    std::shared_ptr<ClassicIndex> IndexWrapper<Index>::reduced() const
    {
	(*mCI) = mI->pos();
	return mCI;
    }

    template <class Index>
    inline std::shared_ptr<IndexWrapperBase> mkIndexWrapper(const Index& i)
    {
	return std::make_shared<IndexWrapper<Index>>(std::make_shared<Index>(i));
    }

}
