// -*- C++ -*-

#ifndef __cxz_op_types_h__
#define __cxz_op_types_h__

#include "base/base.h"
#include "xpr/xpr_base.h"
#include "array/darray_base.h"

namespace CNORXZ
{


    template <class OpT>
    class COpInterface : public XprInterface<OpT>
    {
    public:
	constexpr COpInterface() = default;
	
	OpT& THIS() { return static_cast<OpT&>(*this); }
	const OpT& THIS() const { return static_cast<const OpT&>(*this); }

	constexpr decltype(auto) r() { return THIS(); }
	constexpr decltype(auto) r() const { return THIS(); }
	
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

	constexpr OpInterface() = default;
	
	OpT& THIS() { return static_cast<OpT&>(*this); }
	const OpT& THIS() const { return static_cast<const OpT&>(*this); }

	template <class IndexT, class F, class... Args>
	constexpr decltype(auto) ax(const Sptr<IndexT>& ind, F&& f, const Args&... args);

	template <class IndexT, class F, class... Args>
	inline void a(const Sptr<IndexT>& ind, F&& f, const Args&... args);
    };

    template <class T>
    struct is_operation
    { static constexpr bool value = std::is_base_of<COpInterface<T>,T>::value; };

    template <class T>
    struct is_mutable_operation
    { static constexpr bool value = std::is_base_of<OpInterface<T>,T>::value; };

    template <class T>
    struct op_size
    { static constexpr SizeT value = is_operation<T>::value ? 1 : 0; };
    
    template <typename T, class IndexT>
    class COpRoot : public COpInterface<COpRoot<T,IndexT>>
    {
    public:
	typedef OpInterface<COpRoot<T,IndexT>> OI;

	constexpr COpRoot() = default;
	
	constexpr COpRoot(const DArrayBase<T>& a, const Sptr<IndexT>& ind);
	constexpr COpRoot(const T* data, const Sptr<IndexT>& ind);
	constexpr COpRoot& init(const T* data, const Sptr<IndexT>& ind);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
	
	const T* data() const;
	
    private:
	const T* mData = nullptr;
	Sptr<IndexT> mIndex;
    };

    template <typename T, class IndexT>
    class OpCont : public OpInterface<OpCont<T,IndexT>>
    {
    public:
	typedef OpInterface<OpCont<T,IndexT>> OI;
	typedef typename Container<T,index_const_size<IndexT>::value,
				   index_has_const_size<IndexT>::value>::type CT;
	
	constexpr OpCont() = default;
	
	constexpr OpCont(const Sptr<IndexT>& ind);
	constexpr OpCont& init(const Sptr<IndexT>& ind);
	constexpr OpCont& init(const Sptr<IndexT>& ind, const Vector<T>& c);

	constexpr decltype(auto) r();
	constexpr decltype(auto) r() const;
	
        template <class Op>
        constexpr OpCont& operator=(const Op& in);

        template <class Op>
        constexpr OpCont& operator+=(const Op& in);

        constexpr OpCont& operator=(const OpCont& in);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

	T* data();
	const T* data() const;
	
    private:
	Sptr<IndexT> mIndex;
	CT mC;
    };
    
    
    template <typename T, class IndexT>
    class OpRoot : public OpInterface<OpRoot<T,IndexT>>
    {
    public:
	typedef OpInterface<OpRoot<T,IndexT>> OI;

	constexpr OpRoot() = default;
	
	constexpr OpRoot(MDArrayBase<T>& a, const Sptr<IndexT>& ind);
	constexpr OpRoot(T* data, const Sptr<IndexT>& ind);
	constexpr OpRoot& init(T* data, const Sptr<IndexT>& ind);
            
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
	typedef OpInterface<Operation<F,Ops...>> OI;

	constexpr Operation() = default;
	
	constexpr Operation(F&& f, const Ops&... ops);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	template <SizeT... Is>
	constexpr decltype(auto) exec(std::index_sequence<Is...> is) const;

	template <SizeT I, SizeT... Is>
	constexpr decltype(auto) rootStepsi(const IndexId<I>& id,
					    std::index_sequence<Is...> is) const;
     
	Tuple<Ops...> mOps;
	F mF;
	
    };

    template <class F, class... Ops>
    constexpr decltype(auto) mkOperation(F&& f, const Ops&... ops);

    template <class F, class... Ops>
    struct op_size<Operation<F,Ops...>>
    { static constexpr SizeT value = sizeof...(Ops); };
    
    template <class CXpr>
    class Contraction : public OpInterface<Contraction<CXpr>>
    {
    public:
	typedef OpInterface<Contraction<CXpr>> OI;

	constexpr Contraction() = default;
	
	constexpr Contraction(CXpr&& cxpr);

	template <class PosT>
	constexpr decltype(auto) operator()(const PosT& pos) const;

	constexpr decltype(auto) operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	CXpr mCXpr;
    };

    template <class F, class Op, class IndexT>
    constexpr decltype(auto) mkContracion(F&& f, Op&& op, const Sptr<IndexT>& i);
}

#endif