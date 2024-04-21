// -*- C++ -*-
/**
   
   @file opt/mpi/include/rarray.cc.h
   @brief RArray template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rarray_cc_h__
#define __cxz_mpi_rarray_cc_h__

#include "rarray.h"
#include "raindex.h"
#include "rop_types.h"

namespace CNORXZ
{
    namespace mpi
    {

	template <typename T>
	RCArray<T>::RCArray(const RCArray& a) :
	    mA(a.mA->copy()),
	    mGeom(a.mGeom),
	    mGlobal(a.mGlobal)
	{}

	template <typename T>
	RCArray<T>& RCArray<T>::operator=(const RCArray& a)
	{
	    mA = ObjHandle<CArrayBase<T>>(a.mA->copy());
	    mGeom = a.mGeom;
	    mGlobal = a.mGlobal;
	}

	template <typename T>
	RCArray<T>::RCArray(const CArrayBase<T>& a, const RangePtr& geom) :
	    mA(a.copy()),
	    mGeom(geom),
	    mGlobal(RRangeFactory(rangeCast<YRange>(a.range()),rangeCast<YRange>(mGeom)).create())
	{}

	template <typename T>
	template <typename I, typename M>
	T RCArray<T>::operator[](const IndexInterface<I,M>& i) const
	{
	    if constexpr(is_rank_index<I>::value){
		return *(begin() + i.lex());
	    }
	    else {
		return (*mA)[i];
	    }
	}

	template <typename T>
	template <typename I, typename M>
	T RCArray<T>::at(const IndexInterface<I,M>& i) const
	{
	    if constexpr(is_rank_index<I>::value){
		CXZ_ASSERT(i.rank() == getNumRanks(), "rank number out of scope");
		// consistency checks!!!
		return *(begin() + i.lex());
	    }
	    else {
		return mA->at(i);
	    }
	}

	template <typename T>
	template <class... Indices>
	T RCArray<T>::operator[](const SPack<Indices...>& pack) const
	{
	    typedef typename std::remove_reference<decltype(*pack[CSizeT<0>{}])>::type I0;
	    if constexpr(is_rank_index<I0>::value){
		return *(begin() + pack.lex());
	    }
	    else {
		return (*mA)[pack];
	    }
	}

	template <typename T>
	template <class... Indices>
	T RCArray<T>::at(const SPack<Indices...>& pack) const
	{
	    typedef typename std::remove_reference<decltype(*pack[CSizeT<0>{}])>::type I0;
	    if constexpr(is_rank_index<I0>::value){
		CXZ_ASSERT(pack[CSizeT<0>{}]->rank() == getNumRanks(),
			   "rank number out of scope");
		return *(begin() + pack.lex());
	    }
	    else {
		return mA->at(pack);
	    }
	}

	template <typename T>
	T RCArray<T>::operator[](const DPack& pack) const
	{
	    // TODO: assert that none of the indices is rank index
	    return (*mA)[pack];
	}

	template <typename T>
	T RCArray<T>::at(const DPack& pack) const
	{
	    // TODO: assert that none of the indices is rank index
	    return mA->at(pack);
	}

	template <typename T>
	template <typename I, typename M>
	Sptr<CArrayBase<T>> RCArray<T>::sl(const IndexInterface<I,M>& begin,
					   const IndexInterface<I,M>& end) const
	{
	    if constexpr(is_rank_index<I>::value){
		CXZ_ERROR("not implemented");
		return nullptr;
	    }
	    else {
		return mA->sl(begin, end);
	    }
	}

	template <typename T>
	template <class Index>
	COpRoot<T,Index> RCArray<T>::operator()(const Sptr<Index>& i) const
	{
	    CXZ_ERROR("not implemented");
	    return COpRoot<T,Index>();
	}

	template <typename T>
	template <class... Indices>
	inline decltype(auto) RCArray<T>::operator()(const SPack<Indices...>& pack) const
	{
	    typedef typename std::remove_reference<decltype(*pack[CSizeT<0>{}])>::type I0;
	    if constexpr(is_rank_index<I0>::value){
		// preliminary:
		CXZ_ASSERT(this->formatIsTrivial(),
			   "array has non-trivial format, rank operations require trivial format");
		auto ri = pack[CSizeT<0>{}];
		auto li = iter<1,sizeof...(Indices)>
		    ( [&](auto i) { return pack[CSizeT<i>{}]; },
		      [](const auto&... x) { return mindexPtr( (x * ...) ); } );
		return croproot(*this, ri, li);
	    }
	    else {
		return (*mA)(pack);
	    }
	}

	template <typename T>
	inline decltype(auto) RCArray<T>::operator()(const DPack& pack) const
	{
	    // TODO: assert that none of the indices is rank index
	    return (*mA)(pack);
	}

	template <typename T>
	const T* RCArray<T>::data() const
	{
	    return mA->data();
	}

	template <typename T>
	SizeT RCArray<T>::size() const
	{
	    return mA->size() * mGeom->size();
	}

	template <typename T>
	RangePtr RCArray<T>::range() const
	{
	    return mGlobal;
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::begin() const
	{
	    return const_iterator(mA->data(), mGlobal);
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::end() const
	{
	    return const_iterator(mA->data(), mGlobal, mGlobal->size());
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::cbegin() const
	{
	    return const_iterator(mA->data(), mGlobal);
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::cend() const
	{
	    return const_iterator(mA->data(), mGlobal, mGlobal->size());
	}

	template <typename T>
	bool RCArray<T>::isView() const
	{
	    return mA->isView();
	}

	template <typename T>
	const CArrayBase<T>& RCArray<T>::local() const
	{
	    return *mA;
	}

	template <typename T>
	RangePtr RCArray<T>::geom() const
	{
	    return mGeom;
	}

	template <typename T>
	const Vector<const T*>& RCArray<T>::buffermap() const
	{
	    return mBuf;
	}
	
	template <typename T>
	template <class TarI, class RTarI, class SrcI, class RSrcI, class I>
	void RCArray<T>::load(const Sptr<RIndex<TarI,RTarI>>& lpi, const Sptr<RIndex<SrcI,RSrcI>>& ai,
			      const Sptr<I>& i, const Sptr<Vector<SizeT>>& imap) const
	{
	    mA->checkFormatCompatibility(ai->local()*i);
	    const SizeT blocks = i->pmax().val();
	    setupBuffer(lpi, ai, imap, *mA, mBuf, mMap, blocks);
	}

	template <typename T>
	template <class Index, class F>
	Sptr<Vector<SizeT>> RCArray<T>::load(const Sptr<Index>& i, const F& f) const
	{
	    Sptr<Vector<SizeT>> imap = std::make_shared<Vector<SizeT>>();

	    //load(i, /**/, imap);
	    return imap;
	}

	template <class TarI, class RTarI, class SrcI, class RSrcI, typename T>
	void setupBuffer(const Sptr<RIndex<TarI,RTarI>>& rgi, const Sptr<RIndex<SrcI,RSrcI>>& rgj,
			 const Sptr<Vector<SizeT>>& imap, const CArrayBase<T>& data,
			 Vector<T>& buf, Vector<const T*>& map, const SizeT blocks)
	{
	    const SizeT myrank = getRankNumber();
	    const SizeT Nranks = getNumRanks();

	    const SizeT mapsize = rgj->range()->size();
	    map = Vector<const T*>(mapsize,nullptr);
	    Vector<Vector<T>> sendbuf(Nranks);
	    for(auto& sb: sendbuf){
		sb.reserve(data.size());
	    }
	    Vector<Vector<SizeT>> request(Nranks);
	    const SizeT locsz = rgi->local()->lmax().val();

	    // First loop: setup send buffer
	    rgi->ifor( mapXpr(rgj, rgi, imap,
			      operation
			      ( [&](SizeT p, SizeT q) {
				  const SizeT r = p / locsz;
				  if(myrank != r){
				      request[r].push_back(p % locsz);
				  }
			      } , posop(rgj), posop(rgi) ) ) ,
		       NoF {} )();

	    // transfer:
	    Vector<SizeT> reqsizes(Nranks);
	    SizeT bufsize = 0;
	    Vector<Vector<SizeT>> ext(Nranks);
	    for(auto& e: ext){
		e.resize(Nranks);
	    }
	    for(SizeT i = 0; i != Nranks; ++i){
		reqsizes[i] = request[i].size();
		bufsize += reqsizes[i]*blocks;
		ext[myrank][i] = reqsizes[i];
	    }
	    buf.resize(bufsize);
	    MPI_Status stat;

	    // transfer requests:
	    for(SizeT o = 1; o != Nranks; ++o){
		const SizeT dstr = (myrank + o) % Nranks;
		const SizeT srcr = (myrank - o + Nranks) % Nranks;
		SizeT sendsize = 0;
		MPI_Sendrecv(reqsizes.data()+dstr, 1, MPI_UNSIGNED_LONG, dstr, 0,
			     &sendsize, 1, MPI_UNSIGNED_LONG, srcr, 0, MPI_COMM_WORLD, &stat);
		ext[srcr][myrank] = sendsize;
		Vector<SizeT> sendpos(sendsize);
		MPI_Sendrecv(request[dstr].data(), reqsizes[dstr], MPI_UNSIGNED_LONG, dstr, 0,
			     sendpos.data(), sendsize, MPI_UNSIGNED_LONG, srcr, 0, MPI_COMM_WORLD, &stat);
		sendbuf[srcr].resize(sendsize*blocks);
		for(SizeT i = 0; i != sendsize; ++i){
		    std::memcpy( sendbuf[srcr].data()+i*blocks, data.data()+sendpos[i]*blocks, blocks*sizeof(T) );
		}
	    }

	    const MPI_Datatype dt = Typemap<T>::value();
	
	    // transfer data:
	    for(SizeT o = 1; o != Nranks; ++o){
		const SizeT dstr = (myrank + o) % Nranks;
		const SizeT srcr = (myrank - o + Nranks) % Nranks;
		SizeT off = 0;
		for(SizeT p = 0; p != srcr; ++p){
		    off += ext[myrank][p];
		}
	
		MPI_Sendrecv(sendbuf[dstr].data(), ext[dstr][myrank]*blocks, dt, dstr, 0,
			     buf.data()+off*blocks, ext[myrank][srcr]*blocks, dt, srcr, 0,
			     MPI_COMM_WORLD, &stat);
	
	    }

	    // Second loop: Assign map to target buffer positions:
	    Vector<SizeT> cnt(Nranks);
	    rgi->ifor( mapXpr(rgj, rgi, imap,
			      operation
			      ( [&](SizeT p, SizeT q) {
				  const SizeT r = p / locsz;
				  if(myrank != r){
				      SizeT off = 0;
				      for(SizeT s = 0; s != r; ++s){
					  off += ext[myrank][s];
				      }
				      map[p] = buf.data() + off*blocks + cnt[r]*blocks;
				      ++cnt[r];
				  }
				  map[q + myrank*locsz] = data.data() + q*blocks;
			      } , posop(rgj), posop(rgi) ) ), NoF {} )();
	}

	
    } // namespace mpi
} // namespace CNORXZ

#endif
