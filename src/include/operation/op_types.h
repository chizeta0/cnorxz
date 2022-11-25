// -*- C++ -*-

#ifndef __cxz_op_types_h__
#define __cxz_op_types_h__

#include "base/base.h"

namespace CNORXZ
{


    template <class OpT>
    class COpInterface : public XprInterface<OpT>
    {
    public:
	
	OpT& THIS() { return static_cast<OpT&>(*this); }
	const OpT& THIS() const { return static_cast<const OpT&>(*this); }

	template <class F, class IndexT>
	constexpr decltype(auto) c(F&& f, const Sptr<IndexT>& ind) const;

	template <class IndexT>
	constexpr decltype(auto) c(const Sptr<IndexT>& ind) const;

	template <class F, class... Args>
	constexpr decltype(auto) o(F&& f, Args&&... args) const;

    };

    template <class OpT>
    class OpInterface : public COpInterface<OpT>
    {
    public:
	typedef COpInterface<OpT> OI;

	OpT& THIS() { return static_cast<OpT&>(*this); }
	const OpT& THIS() const { return static_cast<const OpT&>(*this); }

	template <class IndexT, class F, class... Args>
	constexpr decltype(auto) ax(const Sptr<IndexT>& ind, F&& f, Args&&... args);

	template <class IndexT, class F, class... Args>
	inline SizeT a(const Sptr<IndexT>& ind, F&& f, Args&&... args);
    };

    template <class T>
    struct is_operation
    { static constexpr bool value = std::is_base_of<COpInterface,T>::value };

    template <class T>
    struct is_mutable_operation
    { static constexpr bool value = std::is_base_of<OpInterface,T>::value };

    template <typename T, class IndexT>
    class COpRoot : public COpInterface<COpRoot<T,IndexT>>
    {
    public:
	typedef OpInterface<T,COpRoot<T,IndexT>> OI;
	
	constexpr COpRoot(const DArrayBase<T>& a, const Sptr<IndexT>& ind);
	constexpr COpRoot(const T* data, const Sptr<IndexT>& ind);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
	
	const T* data() const;
	
    private:
	const T* mData = nullptr;
	Sptr<IndexType> mIndex;
    };

    template <typename T, class IndexT>
    class OpCont : public OpInterface<OpCont<T,IndexT>>
    {
    public:
	typedef OpInterface<OpCont<T,IndexT>> OI;

	constexpr OpCont(const Sptr<Index>& ind);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

	T* data();
	const T* data() const;
	
    private:
	Sptr<IndexT> mIndex;
	Vector<T> mC;
    };
    
    
    template <typename T, class IndexT>
    class OpRoot : public OpInterface<OpRoot<T,IndexT>>
    {
    public:
	typedef OpInterface<OpRoot<T,IndexT>> OI;

	constexpr OpRoot(MDArrayBase<T>& a, const Sptr<IndexT>& ind);
	constexpr OpRoot(T* data, const Sptr<IndexT>& ind);
            
        template <class Op>
        constexpr OpRoot& operator=(const Op& in);

        template <class Op>
        constexpr OpRoot& operator+=(const Op& in);

        constexpr OpRoot& operator=(const OpRoot& in);
        
	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

	T* data() const;

    private:
	T* mData = nullptr;
	Sptr<IndexT> mIndex;
	
    };
    

    template <class F, class... Ops>
    class Operation : public OpInterface<Operation<F,Ops...>>
    {
    public:
	typedef OpInterface<T,Operation<T,F,Ops...>> OI;

	constexpr Operation(F&& f, Ops&&... ops);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	template <SizeT... Is>
	constexpr decltype(auto) exec(std::index_sequence<Is...> is) const;
     
	Tuple<Ops...> mOps;
	F mF;
	
    };
    
    template <class CXpr>
    class Contraction : public OpInterface<Contraction<CXpr>>
    {
    public:
	typedef OpInterface<Contraction<CXpr>> OI;

	constexpr Contraction(CXpr&& cxpr);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	CXpr mCXpr;
	Sptr<IndexType> mInd;
    };

    template <class F, class Op, class IndexT>
    constexpr decltype(auto) mkContracion(F&& f, Op&& op, const Sptr<IndexT>& i);
}

#endif
