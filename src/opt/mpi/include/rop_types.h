// -*- C++ -*-
/**
   
   @file opt/include/rop_types.h
   @brief Ranked Operation types declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

**/

#ifndef __cxz_mpi_rop_types_h__
#define __cxz_mpi_rop_types_h__

#include "operation/operation.h"

namespace CNORXZ
{
    namespace mpi
    {
	template <typename T, class RIndexT, class IndexT>
	class CROpRoot : public COpInterface<CROpRoot<T,RIndexT,IndexT>>
	{
	public:
	    typedef COpInterface<CROpRoot<T,RIndexT,IndexT>> OI;

	    constexpr CROpRoot() = default;
	    constexpr CROpRoot(const RCArray<T>& a, const Sptr<RIndexT>& ri,
			       const Sptr<IndexT>& li);
	    
	    template <class PosT>
	    constexpr decltype(auto) operator()(const PosT& pos) const;

	    constexpr decltype(auto) operator()() const;

	    template <SizeT I>
	    constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

	private:

	    const T* const* mData;
	    Sptr<RIndexT> mRIndex;
	    Sptr<IndexT> mIndex;
	};


	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) croproot(const RCArray<T>& a, const Sptr<RIndexT>& ri,
					  const Sptr<IndexT>& li);

	template <typename T, class RIndexT, class IndexT>
	class ROpRoot : public OpInterface<ROpRoot<T,RIndexT,IndexT>>
	{
	public:
	    typedef OpInterface<ROpRoot<T,RIndexT,IndexT>> OI;

	    constexpr ROpRoot() = default;
	    constexpr ROpRoot(RArray<T>& a, const Sptr<RIndexT>& ri,
			      const Sptr<IndexT>& li);

	    template <class Op>
	    constexpr ROpRoot& operator=(const Op& in);

	    template <class Op>
	    constexpr ROpRoot& operator+=(const Op& in);

	    constexpr ROpRoot& operator=(const ROpRoot& in);

	    template <class PosT>
	    constexpr decltype(auto) operator()(const PosT& pos) const;

	    constexpr decltype(auto) operator()() const;

	    template <SizeT I>
	    constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

	private:

	    ArrayBase<T>* mLocal;
	    const T* const* mData;
	    Sptr<RIndexT> mRIndex;
	    Sptr<IndexT> mIndex;
	};

	template <typename T, class RIndexT, class IndexT>
	constexpr decltype(auto) roproot(RArray<T>& a, const Sptr<RIndexT>& ri,
					 const Sptr<IndexT>& li);

	/*
	template <class CXpr>
	class RContraction : public OpInterfacte<RContraction<CXpr>>
	{
	public:
	    typedef OpInterfacte<RContraction<CXpr>> OI;

	    constexpr RContraction() = default;
	    constexpr RContraction(CXpr&& cxpr);

	    template <class PosT>
	    constexpr decltype(auto) operator()(const PosT& pos) const;

	    constexpr decltype(auto) operator()() const;

	    template <SizeT I>
	    constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

	private:
	    CXpr mCXpr;
	};
	*/
    } // namespace mpi

    template <typename T, class RIndexT, class IndexT>
    struct op_size<mpi::CROpRoot<T,RIndexT,IndexT>>
    {
	static constexpr SizeT value = 2;
    };
    
} // namespace CNORXZ

#endif
