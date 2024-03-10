// -*- C++ -*-
/**
   
   @file opt/include/rop_types.h
   @brief Ranked Operation types declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

**/

#ifndef __cxz_mpi_rop_types_h__
#define __cxz_mpi_rop_types_h__

namespace CNORXZ
{
    template <typename T, class IndexT>
    class CROpRoot : public COpInterface<CROpRoot<T,IndexT>>
    {
    public:
	typedef COpInterface<CROpRoot<T,IndexT>> OI;

	constexpr CROpRoot() = default;

    	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	const T* mData;
	COpRoot
    };

    template <typename T, class IndexT>
    class ROpRoot : public OpInterface<ROpRoot<T,IndexT>>
    {
    public:
	typedef OpInterface<ROpRoot<T,IndexT>> OI;

	constexpr ROpRoot() = default;

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	
    };

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
}

#endif
