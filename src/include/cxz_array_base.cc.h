
#include "cxz_array_base.h"

namespace CNORXZ
{
        
    /**********************
     *  ArrayBase    *	     
     **********************/

    template <typename T, class... SRanges>
    ArrayBase<T,SRanges...>::ArrayBase(const ArrayBase& in) :
	mInit(in.mInit),
	mRange(in.mRange)
    {
	if(mRange){
	    mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
	}
    }
	

    template <typename T, class... SRanges>
    ArrayBase<T,SRanges...>::ArrayBase(ArrayBase&& in) :
	mInit(in.mInit),
	mRange(in.mRange)
    {
	if(mRange){
	    mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
	}
    }

    template <typename T, class... SRanges>
    ArrayBase<T,SRanges...>& ArrayBase<T,SRanges...>::operator=(const ArrayBase& in)
    {
	mInit = in.mInit;
	mRange = in.mRange;
	if(mRange){
	    mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
	}
	return *this;
    }

    template <typename T, class... SRanges>
    ArrayBase<T,SRanges...>& ArrayBase<T,SRanges...>::operator=(ArrayBase&& in)
    {
	mInit = in.mInit;
	mRange = in.mRange;
	if(mRange){
	    mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
	}
	return *this;
    }
    
    template <typename T, class... SRanges>
    ArrayBase<T,SRanges...>::ArrayBase(const std::shared_ptr<SRanges>&... ranges)
    {
	ContainerRangeFactory<SRanges...> crf(ranges...);
	mRange = std::dynamic_pointer_cast<ContainerRange<SRanges...> >( crf.create() );
	mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
    }

    template <typename T, class... SRanges>
    ArrayBase<T,SRanges...>::ArrayBase(const typename CRange::Space& space)
    {
	ContainerRangeFactory<SRanges...> crf(space);
	mRange = std::dynamic_pointer_cast<ContainerRange<SRanges...> >( crf.create() );
	mProtoI = std::make_shared<IndexType>( mRange, reinterpret_cast<std::intptr_t>(this) );
    }

    template <typename T, class... SRanges>
    template <typename X>
    const T& ArrayBase<T,SRanges...>::operator[](const ConstContainerIndex<X,typename SRanges::IndexType...>& i)
    {
	IndexType ii(*mProtoI);
	ii = i;
	return (*this)[ii];
    }

    template <typename T, class... SRanges>
    const T& ArrayBase<T,SRanges...>::operator[](const std::tuple<IPTR<typename SRanges::IndexType>...>& is) const
    {
	IndexType ii(*mProtoI);
	ii(is);
	return (*this)[ii];
    }
    
    template <typename T, class... SRanges>
    size_t ArrayBase<T,SRanges...>::size() const
    {
	return mRange->size();
    }

    template <typename T, class... SRanges>
    typename ArrayBase<T,SRanges...>::CIndexType ArrayBase<T,SRanges...>::begin() const
    {
	return cbegin();
    }
    
    template <typename T, class... SRanges>
    typename ArrayBase<T,SRanges...>::CIndexType ArrayBase<T,SRanges...>::end() const
    {
	return end();
    }

    template <typename T, class... SRanges>
    typename ArrayBase<T,SRanges...>::CIndexType ArrayBase<T,SRanges...>::cbegin() const
    {
	CIndexType i(*mProtoI,true);
	i = 0;
	return i.setData(data());
    }
    
    template <typename T, class... SRanges>
    typename ArrayBase<T,SRanges...>::CIndexType ArrayBase<T,SRanges...>::cend() const
    {
	CIndexType i(*mProtoI,true);
	i = i.max();
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    const std::shared_ptr<typename ArrayBase<T,SRanges...>::CRange>&
    ArrayBase<T,SRanges...>::range() const
    {
	return mRange;
    }

    template <typename T, class... SRanges>
    bool ArrayBase<T,SRanges...>::isConst() const
    {
	return true;
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    ArrayBase<T,SRanges...>::operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    ArrayBase<T,SRanges...>::op(const std::shared_ptr<CIndexType>& ind) const
    {
	return ConstOperationRoot<T,SRanges...>(data(), *ind);
    }

    template <typename T, class... SRanges>
    template <class... MappedRanges>
    ConstOperationRoot<T,MappedRanges...>
    ArrayBase<T,SRanges...>::m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds) const
    {
	static_assert(sizeof...(SRanges) == sizeof...(MappedRanges),
		      "number of mapped ranges must be equal to number of original ranges");
	return ConstOperationRoot<T,MappedRanges...>(*this, inds...);
    }
    
    template <typename T, class... SRanges>
    bool ArrayBase<T,SRanges...>::isInit() const
    {
	return mInit;
    }
    
    template <typename T, class... SRanges>
    template <size_t N>
    auto ArrayBase<T,SRanges...>::getRangePtr() const
	-> decltype(mRange->template getPtr<N>())
    {
	return mRange->template getPtr<N>();
    }

    
    /******************************
     *  MutableArrayBase     *	     
     ******************************/

    template <typename T, class... SRanges>
    MutableArrayBase<T,SRanges...>::MutableArrayBase(const std::shared_ptr<SRanges>&... ranges) :
	ArrayBase<T,SRanges...>(ranges...) {}

    template <typename T, class... SRanges>
    MutableArrayBase<T,SRanges...>::MutableArrayBase(const typename CRange::Space& space) :
	ArrayBase<T,SRanges...>(space) {}

    template <typename T, class... SRanges>
    template <typename X>
    T& MutableArrayBase<T,SRanges...>::operator[](const ConstContainerIndex<X,typename SRanges::IndexType...>& i)
    {
	IndexType ii(*MAB::mProtoI);
	ii = i;
	return (*this)[ii];
    }

    template <typename T, class... SRanges>
    T& MutableArrayBase<T,SRanges...>::operator[](const std::tuple<IPTR<typename SRanges::IndexType>...>& is)
    {
	IndexType ii(*MAB::mProtoI,this->data());
	ii(is);
	return (*this)[ii];
    }

    template <typename T, class... SRanges>
    typename MutableArrayBase<T,SRanges...>::IndexType MutableArrayBase<T,SRanges...>::begin()
    {
	IndexType i(*MAB::mProtoI,this->data(),true);
	i = 0;
	return i.setData(data());
    }
    
    template <typename T, class... SRanges>
    typename MutableArrayBase<T,SRanges...>::IndexType MutableArrayBase<T,SRanges...>::end()
    {
	IndexType i(*MAB::mProtoI,this->data(),true);
	i = i.max();
	return i.setData(data());
    }

    
    template <typename T, class... SRanges>
    bool MutableArrayBase<T,SRanges...>::isConst() const
    {
	return false;
    }

    template <typename T, class... SRanges>
    OperationRoot<T,SRanges...>
    MutableArrayBase<T,SRanges...>::operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds)
    {
	return OperationRoot<T,SRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    OperationRoot<T,SRanges...>
    MutableArrayBase<T,SRanges...>::op(const std::shared_ptr<CIndexType>& ind)
    {
	return OperationRoot<T,SRanges...>(data(), *ind);
    }
    
    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MutableArrayBase<T,SRanges...>::operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
    {
	return ConstOperationRoot<T,SRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    ConstOperationRoot<T,SRanges...>
    MutableArrayBase<T,SRanges...>::op(const std::shared_ptr<CIndexType>& ind) const
    {
	return ConstOperationRoot<T,SRanges...>(data(), *ind);
    }

    template <typename T, class... SRanges>
    template <class... MappedRanges>
    OperationRoot<T,MappedRanges...>
    MutableArrayBase<T,SRanges...>::m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds)
    {
	static_assert(sizeof...(SRanges) == sizeof...(MappedRanges),
		      "number of mapped ranges must be equal to number of original ranges");
	return OperationRoot<T,MappedRanges...>(*this, inds...);
    }

    template <typename T, class... SRanges>
    template <class... MappedRanges>
    ConstOperationRoot<T,MappedRanges...>
    MutableArrayBase<T,SRanges...>::m(const std::shared_ptr<typename MappedRanges::IndexType>&... inds) const
    {
	static_assert(sizeof...(SRanges) == sizeof...(MappedRanges),
		      "number of mapped ranges must be equal to number of original ranges");
	return ConstOperationRoot<T,MappedRanges...>(*this, inds...);
    }

} // end namespace CNORXZ

