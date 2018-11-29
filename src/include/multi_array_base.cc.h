
#include "multi_array_base.h"

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
    MultiArrayBase<T,SRanges...>::MultiArrayBase(const typename CRange::Space& space)
    {
	ContainerRangeFactory<T,SRanges...> crf(space);
	mRange = std::dynamic_pointer_cast<ContainerRange<T,SRanges...> >( crf.create() );
	mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
    }

    template <typename T, class... SRanges>
    template <typename X>
    const T& MultiArrayBase<T,SRanges...>::operator[](const ContainerIndex<X,typename SRanges::IndexType...>& i)
    {
	IndexType ii(*mProtoI);
	ii = i;
	return (*this)[ii];
    }

    template <typename T, class... SRanges>
    const T& MultiArrayBase<T,SRanges...>::operator[](const std::tuple<IPTR<typename SRanges::IndexType>...>& is) const
    {
	IndexType ii(*mProtoI);
	ii(is);
	return (*this)[ii];
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
    MultiArrayBase<T,SRanges...>::operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MultiArrayBase<T,SRanges...>::op(const std::shared_ptr<IndexType>& ind) const
    {
	return ConstOperationRoot<T,SRanges...>(data(), *ind);
    }

    template <typename T, class... SRanges>
    template <class... MappedRanges>
    ConstOperationRoot<T,MappedRanges...>
    MultiArrayBase<T,SRanges...>::m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds) const
    {
	static_assert(sizeof...(SRanges) == sizeof...(MappedRanges),
		      "number of mapped ranges must be equal to number of original ranges");
	return ConstOperationRoot<T,MappedRanges...>(*this, inds...);
    }
    
    template <typename T, class... SRanges>
    bool MultiArrayBase<T,SRanges...>::isInit() const
    {
	return mInit;
    }
    
    template <typename T, class... SRanges>
    template <size_t N>
    auto MultiArrayBase<T,SRanges...>::getRangePtr() const
	-> decltype(mRange->template getPtr<N>())
    {
	return mRange->template getPtr<N>();
    }

    
    /******************************
     *  MutableMultiArrayBase     *	     
     ******************************/

    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::MutableMultiArrayBase(const std::shared_ptr<SRanges>&... ranges) :
	MultiArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class... SRanges>
    MutableMultiArrayBase<T,SRanges...>::MutableMultiArrayBase(const typename CRange::Space& space) :
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
    template <typename X>
    T& MutableMultiArrayBase<T,SRanges...>::operator[](const ContainerIndex<X,typename SRanges::IndexType...>& i)
    {
	IndexType ii(*MAB::mProtoI);
	ii = i;
	return (*this)[ii];
    }

    template <typename T, class... SRanges>
    T& MutableMultiArrayBase<T,SRanges...>::operator[](const std::tuple<IPTR<typename SRanges::IndexType>...>& is)
    {
	IndexType ii(*MAB::mProtoI);
	ii(is);
	return (*this)[ii];
    }

    
    template <typename T, class... SRanges>
    bool MutableMultiArrayBase<T,SRanges...>::isConst() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    OperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds)
    {
	return OperationRoot<T,SRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    OperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::op(const std::shared_ptr<IndexType>& ind)
    {
	return OperationRoot<T,SRanges...>(data(), *ind);
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MutableMultiArrayBase<T,SRanges...>::op(const std::shared_ptr<IndexType>& ind) const
    {
	return ConstOperationRoot<T,SRanges...>(data(), *ind);
    }

    template <typename T, class... SRanges>
    template <class... MappedRanges>
    OperationRoot<T,MappedRanges...>
    MutableMultiArrayBase<T,SRanges...>::m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds)
    {
	static_assert(sizeof...(SRanges) == sizeof...(MappedRanges),
		      "number of mapped ranges must be equal to number of original ranges");
	return OperationRoot<T,MappedRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    template <class... MappedRanges>
    ConstOperationRoot<T,MappedRanges...>
    MutableMultiArrayBase<T,SRanges...>::m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds) const
    {
	static_assert(sizeof...(SRanges) == sizeof...(MappedRanges),
		      "number of mapped ranges must be equal to number of original ranges");
	return ConstOperationRoot<T,MappedRanges...>(*this, inds...);
    }

} // end namespace MultiArrayTools

