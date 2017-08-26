// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>

#include "base_def.h"

namespace MultiArrayTools
{

    /*
     * OperationBase
     * MutableOperationBase
     * 
     * OperationMaster : MutableOperationBase
     * 
     * OperationTemplate<...>
     * ConstOperationRoot : OperationBase, OperationTemplate<...>
     * OperationRoot : MutableOperationBase, 
     *                 OperationTemplate<...>
     * 
     */
    
    template <typename T>
    class OperationBase
    {
    public:

	typedef T value_type;
	
	OperationBase() = default;
	virtual ~OperationBase() = default;

	//virtual size_t argNum() const = 0;
	virtual const T& get() const = 0;
    };
   
    template <typename T>
    class MutableOperationBase : public OperationBase<T>
    {
    public:

	MutableOperationBase() = default;
	virtual T& get() = 0;
    };

    template <class OperationClass>
    class OperationTemplate
    {
    public:

	OperationTemplate(OperationClass* oc);
	
	template <class Second>
	auto operator+(const Second& in) const
	    -> Operation<double,std::plus<double>,OperationClass,Second>;
	
	template <class Second>
	auto operator-(const Second& in) const
	    -> Operation<double,std::minus<double>,OperationClass,Second>;
	
	template <class Second>
	auto operator*(const Second& in) const
	    -> Operation<double,std::multiplies<double>,OperationClass,Second>;
	
	template <class Second>
	auto operator/(const Second& in) const
	    -> Operation<double,std::divides<double>,OperationClass,Second>;
	
    private:
	OperationClass* mOc;
    };
    
    template <typename T, class... Ranges>
    class OperationMaster : public MutableOperationBase<T>
    {
    public:

	typedef OperationBase<T> OB;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename MultiRange<Ranges...>::IndexType IndexType;

	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OperationBase<T>& second,
			std::shared_ptr<typename CRange::IndexType>& index);
		
	virtual T& get() override;
	virtual const T& get() const override;

    protected:

	//void performAssignment(const OperationBase<T>& in);
	OperationBase<T> const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    
    template <typename T, class... Ranges>
    class ConstOperationRoot : public OperationBase<T>,
			       public OperationTemplate<ConstOperationRoot<T,Ranges...> >
    {
    public:
	
	typedef OperationBase<T> OB;
	typedef OperationTemplate<ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	virtual const T& get() const override;
	
    protected:
	
	MultiArrayBase<T,Ranges...> const& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    template <typename T, class... Ranges>
    class OperationRoot : public MutableOperationBase<T>,
			  public OperationTemplate<OperationRoot<T,Ranges...> >
    {
    public:
	
	typedef OperationBase<T> OB;
	typedef OperationTemplate<OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationMaster<T,Ranges...> operator=(const OperationBase<T>& in);
	
	virtual const T& get() const override;
	virtual T& get() override;
	
    protected:
	
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    template <typename T, class OpFunction, class... Ops>
    class Operation : public OperationBase<T>,
		      public OperationTemplate<Operation<T,OpFunction,Ops...> >
    {
    public:
	
	typedef OperationBase<T> OB;
	typedef OperationTemplate<Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	Operation(const Ops&... ops);
	
	virtual const T& get() const override;
		
    protected:
	std::tuple<Ops...> mOps;
	mutable T mRes;
    };

}

#include "multi_array_operation.cc"

#endif
