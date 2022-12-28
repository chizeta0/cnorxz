
#ifndef __cxz_array_base_cc_h__
#define __cxz_array_base_cc_h__

#include "array_base.h"
#include "slice.h"

namespace CNORXZ
{
    /******************************
     *   CArrayBase (protected)   *
     ******************************/

    template <typename T>
    template <class Index>
    YIndex CArrayBase<T>::mkSliceIndex(const YIndex& yi, const Index& ind) const
    {
	// TODO: if ind.dim() < iy.dim: assume Null-Indices on missing positions;
	static_assert(has_sub<Index>::value, "got non-mutliple index");
	CXZ_ASSERT(yi.dim() == ind.dim(), "got index of incompatible dimension = "
		   << ind.dim() << ", expected: " << yi.dim());
	Vector<XIndexPtr> npack;
	Vector<SizeT> nbs;
	auto ypack = yi.pack();
	auto ipack = ind.pack();
	const auto& bs = yi.blockSizes();
	if constexpr(has_static_sub<Index>::value){
	    constexpr SizeT ID = index_dim<Index>::value;
	    npack.reserve(ID);
	    nbs.reserve(ID);
	    iter<0,ID>( [&](auto i) {
		const auto& ii1 = ypack[i];
		const auto& ii2 = std::get<i>(ipack);
		if(ii2->dim() != 0){
		    npack.push_back( mkSliceIndex(ii1, ii2) );
		    nbs.push_back(bs[i]);
		}
	    }, NoF {} );
	}
	else {
	    const SizeT idim = ind.dim();
	    npack.reserve(idim);
	    nbs.reserve(idim);
	    for(SizeT i = 0; i != idim; ++i){
		const auto& ii1 = ypack[i];
		const auto& ii2 = ipack[i];
		if(ii2->dim() != 0){
		    npack.push_back( mkSliceIndex(ii1, ii2) );
		    nbs.push_back(bs[i]);
		}
	    }
	}
	return yindex(npack);
    }
    
    template <typename T>
    template <class Index>
    XIndexPtr CArrayBase<T>::mkSliceIndex(const XIndexPtr& xi, const Sptr<Index>& ind) const
    {
	// TODO: if ind.dim() < iy.dim: assume Null-Indices on missing positions;
	CXZ_ASSERT(xi->dim() == ind->dim(), "got index of incompatible dimension = "
		   << ind->dim() << ", expected: " << xi->dim());
	Vector<XIndexPtr> npack;
	Vector<SizeT> nbs;
	auto xpack = xi->pack();
	const auto& bs = xi->blockSizes();
	if constexpr(has_static_sub<Index>::value){
	    auto ipack = ind->pack();
	    constexpr SizeT ID = index_dim<Index>::value;
	    npack.reserve(ID);
	    nbs.reserve(ID);
	    iter<0,ID>( [&](auto i) {
		const auto& ii1 = xpack[i];
		const auto& ii2 = std::get<i>(ipack);
		const XIndexPtr si = mkSliceIndex(ii1, ii2);
		if(si != nullptr){
		    npack.push_back( si );
		    nbs.push_back(bs[i]);
		}
	    }, NoF {} );
	}
	else if constexpr(has_static_sub<Index>::value){
	    auto ipack = ind->pack();
	    const SizeT idim = ind->dim();
	    const SizeT xdim = xi->dim();
	    const SizeT isize = ipack.size();
	    const SizeT xsize = xpack.size();
	    if(isize == 0 or xsize == 0){
		if(idim == 0){
		    return xi->copy();
		}
		else {
		    return nullptr;
		}
	    }
	    CXZ_ASSERT(isize == idim and xsize == xdim, "index error");
	    npack.reserve(idim);
	    nbs.reserve(idim);
	    for(SizeT i = 0; i != idim; ++i){
		const auto& ii1 = xpack[i];
		const auto& ii2 = ipack[i];
		const XIndexPtr si = mkSliceIndex(ii1, ii2);
		if(si != nullptr){
		    npack.push_back( si );
		    nbs.push_back(bs[i]);
		}
	    }
	}
	else {
	    const SizeT idim = ind->dim();
	    if(idim == 0){
		return xi->copy();
	    }
	    else {
		return nullptr;
	    }
	}
	if(npack.size() == 0){
	    return nullptr;
	}
	return xindexPtr(yindexPtr(npack));
    }

    /******************
     *   CArrayBase   *
     ******************/

    template <typename T>
    CArrayBase<T>::CArrayBase(const RangePtr& range) :
	mRange(rangeCast<YRange>(range))
    {}

    template <typename T>
    template <typename I, typename M>
    const T& CArrayBase<T>::operator[](const IndexInterface<I,M>& i) const
    {
	auto ai = this->begin() + i.lex();
	return *ai;
    }
    
    template <typename T>
    template <typename I, typename M>
    const T& CArrayBase<T>::at(const IndexInterface<I,M>& i) const
    {
	CXZ_ASSERT(i.lex() < this->size(), "index out of range");
	auto beg = this->begin();
	//CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	// check further compatibility of index/range format!!!
	auto ai = beg + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    Sptr<CArrayBase<T>> CArrayBase<T>::sl(const IndexInterface<I,M>& i) const
    {
	auto beg = this->begin();
	auto si = mkSliceIndex(beg, i);
	auto it = beg + i.lex();
	return std::make_shared<CSlice>(this, si, it.pos());
	//return std::make_shared<CSlice>();
    }

    template <typename T>
    SizeT CArrayBase<T>::size() const
    {
	return mRange->size();
    }
    
    template <typename T>
    RangePtr CArrayBase<T>::range() const
    {
	return mRange;
    }

    template <typename T>
    typename CArrayBase<T>::const_iterator CArrayBase<T>::begin() const
    {
	return this->cbegin();
    }
    
    template <typename T>
    typename CArrayBase<T>::const_iterator CArrayBase<T>::end() const
    {
	return this->cend();
    }
    
    template <typename T>
    template <class I, typename M>
    COpRoot<T,I> CArrayBase<T>::operator()(const IndexPtr<I,M>& i) const
    {
	CXZ_ASSERT(false, "BLOCKSIZES!!");
	return COpRoot<T,I>(*this, std::static_pointer_cast<I>(i));
    }

    template <typename T>
    template <class I, SizeT L>
    COpRoot<T,I> CArrayBase<T>::operator()(const LIndex<I,L>& i) const
    {
	CXZ_ASSERT(false, "BLOCKSIZES!!");
	return COpRoot<T,LIndex<I,L>>(*this, i);
    }

    template <typename T>
    template <class I, typename M>
    COpRoot<T,I> CArrayBase<T>::op(const IndexPtr<I,M>& i) const
    {
	CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	return COpRoot<T,I>(*this, std::static_pointer_cast<I>(i));
    }

    template <typename T>
    template <class I, SizeT L>
    COpRoot<T,I> CArrayBase<T>::op(const LIndex<I,L>& i) const
    {
	CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	return COpRoot<T,LIndex<I,L>>(*this, i);
    }

    /*****************
     *   ArrayBase   *
     *****************/

    template <typename T>
    ArrayBase<T>::ArrayBase(const RangePtr& range) :
	CArrayBase<T>(range)
    {}

    template <typename T>
    template <typename I, typename M>
    T& ArrayBase<T>::operator[](const IndexInterface<I,M>& i)
    {
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    T& ArrayBase<T>::at(const IndexInterface<I,M>& i)
    {
	CXZ_ASSERT(i.les() < this->size(), "index out of range");
	// check further compatibility of index/range format!!!
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    Sptr<ArrayBase<T>> ArrayBase<T>::sl(const IndexInterface<I,M>& i)
    {
	auto r = mkSliceRange(i);
	auto beg = this->begin();
	auto bs = mkSliceBlockSizes(i, beg);
	auto it = beg + i.lex();
	return std::make_shared<Slice>(r, this, bs, it.pos());
    }

    template <typename T>
    typename ArrayBase<T>::iterator ArrayBase<T>::begin()
    {
	return iterator(this->data(), this->cbegin());
    }

    template <typename T>
    typename ArrayBase<T>::iterator ArrayBase<T>::end()
    {
	return iterator(this->data(), this->cend());
    }

    template <typename T>
    template <class I, typename M>
    OpRoot<T,I> ArrayBase<T>::operator()(const IndexPtr<I,M>& i) const
    {
	CXZ_ASSERT(false, "BLOCKSIZES!!");
	return OpRoot<T,I>(*this, std::static_pointer_cast<I>(i));
    }

    template <typename T>
    template <class I, SizeT L>
    OpRoot<T,I> ArrayBase<T>::operator()(const LIndex<I,L>& i) const
    {
	CXZ_ASSERT(false, "BLOCKSIZES!!");
	return OpRoot<T,LIndex<I,L>>(*this, i);
    }

    template <typename T>
    template <class I, typename M>
    OpRoot<T,I> ArrayBase<T>::op(const IndexPtr<I,M>& i) const
    {
	CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	return OpRoot<T,I>(*this, std::static_pointer_cast<I>(i));
    }

    template <typename T>
    template <class I, SizeT L>
    OpRoot<T,I> ArrayBase<T>::op(const LIndex<I,L>& i) const
    {
	CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	return OpRoot<T,LIndex<I,L>>(*this, i);
    }
}

#endif
