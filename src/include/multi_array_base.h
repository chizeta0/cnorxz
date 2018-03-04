
#ifndef __multi_array_base_h__
#define __multi_array_base_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>

#include "base_def.h"
#include "mbase_def.h"

#include "ranges/rheader.h"

namespace MultiArrayTools
{

    // Explicitely specify subranges in template argument !!!
    template <typename T, class... SRanges>
    class MultiArrayBase
    {
    public:

	typedef T value_type;
	typedef ContainerRange<T,SRanges...> CRange;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;

	DEFAULT_MEMBERS(MultiArrayBase);
	MultiArrayBase(const std::shared_ptr<SRanges>&... ranges);
	MultiArrayBase(const typename CRange::SpaceType& space);

	virtual ~MultiArrayBase() = default;

	virtual const T& operator[](const IndexType& i) const = 0;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const = 0;

	virtual const T* data() const = 0;
	
	virtual size_t size() const; 
	virtual bool isSlice() const = 0;

	virtual IndexType begin() const;
	virtual IndexType end() const;
	
	virtual IndexType beginIndex() const;
	virtual IndexType endIndex() const;

	virtual const std::shared_ptr<CRange>& range() const;

	virtual bool isConst() const;

	virtual ConstOperationRoot<T,SRanges...>
	operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const;
	
	virtual bool isInit() const;
	
    protected:
	bool mInit = false;
	std::shared_ptr<CRange> mRange;
	std::shared_ptr<IndexType> mProtoI;
    };

    template <typename T, class... SRanges>
    class MutableMultiArrayBase : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IndexType;
	
	using MultiArrayBase<T,SRanges...>::operator[];
	using MultiArrayBase<T,SRanges...>::at;
	using MultiArrayBase<T,SRanges...>::data;
	using MultiArrayBase<T,SRanges...>::begin;
	using MultiArrayBase<T,SRanges...>::end;
	
	DEFAULT_MEMBERS(MutableMultiArrayBase);
	MutableMultiArrayBase(const std::shared_ptr<SRanges>&... ranges);
	MutableMultiArrayBase(const typename CRange::SpaceType& space);
	
	virtual T& operator[](const IndexType& i) = 0;
	virtual T& at(const typename CRange::IndexType::MetaType& meta) = 0;
	
	virtual T* data() = 0;
	
	//virtual IndexType begin();
	//virtual IndexType end();

	virtual bool isConst() const override;

	virtual ConstOperationRoot<T,SRanges...>
	operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const override;
	virtual OperationRoot<T,SRanges...> operator()(std::shared_ptr<typename SRanges::IndexType>&... inds);
    };

    
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
        
    /**********************
     *  MultiArrayBase    *	     
     **********************/

    template <typename T, class... SRanges>
    MultiArrayBase<T,SRanges...>::MultiArrayBase(const std::shared_ptr<SRanges>&... ranges)
    {
	ContainerRangeFactory<T,SRanges...> crf(ranges...);
	mRange = std::dynamic_pointer_cast<ContainerRange<T,SRanges...> >( crf.create() );
	mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
    }

    template <typename T, class... SRanges>
    MultiArrayBase<T,SRanges...>::MultiArrayBase(const typename CRange::SpaceType& space)
    {
	ContainerRangeFactory<T,SRanges...> crf(space);
	mRange = std::dynamic_pointer_cast<ContainerRange<T,SRanges...> >( crf.create() );
	mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
    }
    
    template <typename T, class... SRanges>
    size_t MultiArrayBase<T,SRanges...>::size() const
    {
	return mRange->size();
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType MultiArrayBase<T,SRanges...>::begin() const
    {
	IndexType i(*mProtoI);
	i = 0;
	return i.setData(data());
    }
    
    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType MultiArrayBase<T,SRanges...>::end() const
    {
	IndexType i(*mProtoI);
	i = i.max();
	//i = mRange->size();
	return i.setData(data());
    }
    
    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType
    MultiArrayBase<T,SRanges...>::beginIndex() const
    {
	IndexType i(*mProtoI);
	i = 0;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    typename MultiArrayBase<T,SRanges...>::IndexType
    MultiArrayBase<T,SRanges...>::endIndex() const
    {
	IndexType i(*mProtoI);
	i = i.max();
	//i = mRange->size();
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    const std::shared_ptr<typename MultiArrayBase<T,SRanges...>::CRange>&
    MultiArrayBase<T,SRanges...>::range() const
    {
	return mRange;
    }

    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::isConst() const
    {
	return true;
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MultiArrayBase<T,SRanges...>::operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }
    
    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::isInit() const
    {
	return mInit;
    }
    
    
    /******************************
     *  MutableMultiArrayBase     *	     
     ******************************/

    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::MutableMultiArrayBase(const std::shared_ptr<SRanges>&... ranges) :
	MultiArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::MutableMultiArrayBase(const typename CRange::SpaceType& space) :
	MultiArrayBase<T,SRanges...>(space) {}
    /*
    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::IndexType MutableMultiArrayBase<T,SRanges...>::begin()
    {
	auto i = mRange->begin();
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    typename MutableMultiArrayBase<T,SRanges...>::IndexType MutableMultiArrayBase<T,SRanges...>::end()
    {
	auto i = mRange->end();
	return i.setData(data());
    }
    */
    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::isConst() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    OperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::operator()(std::shared_ptr<typename SRanges::IndexType>&... inds)
    {
	return OperationRoot<T,SRanges...>(*this, inds...);
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }

} // end namespace MultiArrayTools
    
#endif
