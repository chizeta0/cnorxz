// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>

#include "base_def.h"
#include "index_base.h"

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
    
    
    typedef std::map<Name,std::shared_ptr<IndexBase> > IndexList;
    
    template <typename T>
    class OperationBase
    {
    public:

	typedef T value_type;
	
	OperationBase() = default;
	virtual ~OperationBase();

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
	
	template <class Second>
	Operation<OperationClass,Second> operator+(const Second& in) const;
    };
    
    template <typename T, class... Ranges>
    class OperationMaster : public MutableOperationBase<T>
    {
    public:

	typedef OperationBase<T> OB;
	typedef typename MultiRange<Ranges...>::IndexType IndexType;

	OperationMaster(MutableMultiArrayBase& ma, OperationBase<T>& second,
			const ContainerRange<Ranges...>::IndexType& index);
		
	virtual T& get() override;
	virtual const T& get() const override;

    protected:

	//void performAssignment(const OperationBase<T>& in);
	OperationBase<T> const& mSecond;
	MutableMultiArrayBase<T,CRange>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    
    template <typename T, class... Ranges>
    class ConstOperationRoot : public OperationBase<T>,
			       public OperationTemplate<ConstOperationRoot<T,CRange> >
    {
    public:
	
	typedef OperationBase<T> OB;
	typedef OperationTemplate<ConstOperationRoot<T,CRange> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	ConstOperationRoot(const MultiArrayBase<T,CRange>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	virtual const T& get() const override;
	
    protected:
	
	MultiArrayBase<T,CRange> const& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    template <typename T, class... Ranges>
    class OperationRoot : public MutableOperationBase<T>,
			  public OperationTemplate<OperationRoot<T,CRange> >
    {
    public:
	
	typedef OperationBase<T> OB;
	typedef OperationTemplate<OperationRoot<T,CRange> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	OperationRoot(MutableMultiArrayBase<T,CRange>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationMaster<T,Ranges...> operator=(const OperationBase<T>& in);
	
	virtual const T& get() const override;
	virtual T& get() override;
	
    protected:
	
	MutableMultiArrayBase<T,CRange>& mArrayRef;
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
	
	Operation(Ops&&... ops);
	
	virtual const T& get() const override;
		
    protected:
	std::tuple<Ops...> mOps;
	T res;
    };


#include "multi_array_operation.cc"

#endif
