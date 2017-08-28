// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>

#include "base_def.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }
    
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

    void seekIndexInst(std::shared_ptr<const IndexBase> i, std::vector<std::shared_ptr<const IndexBase> >& ivec);

    BlockType getBlockType(std::shared_ptr<const IndexBase> i,
			   std::shared_ptr<const IndexBase> j, bool first);
    
    template <typename T>
    class OperationBase
    {
    public:

	typedef T value_type;
	
	OperationBase() = default;
	virtual ~OperationBase() = default;

	 // init block, return resulting type (BLOCK, VALUE, SPLIT)
	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const = 0;
	virtual OperationBase& block() const = 0; // update block
	
	//virtual size_t argNum() const = 0;
	virtual const BlockBase<T>& get() const = 0;

    protected:
 	mutable std::shared_ptr<BlockBase> mBlockPtr;
    };
   
    template <typename T>
    class MutableOperationBase : public OperationBase<T>
    {
    public:
	typedef T value_type;
	
	MutableOperationBase() = default;
	virtual BlockBase<T>& get() = 0;
    };

    template <class OperationClass>
    class OperationTemplate
    {
    public:

	typedef typename OperationClass::value_type value_type;
	
	OperationTemplate(OperationClass* oc);
	
	template <class Second>
	auto operator+(const Second& in) const
	    -> Operation<value_type,std::plus<value_type>,OperationClass,Second>;
	
	template <class Second>
	auto operator-(const Second& in) const
	    -> Operation<value_type,std::minus<value_type>,OperationClass,Second>;
	
	template <class Second>
	auto operator*(const Second& in) const
	    -> Operation<value_type,std::multiplies<value_type>,OperationClass,Second>;
	
	template <class Second>
	auto operator/(const Second& in) const
	    -> Operation<value_type,std::divides<value_type>,OperationClass,Second>;
	
    private:
	OperationClass* mOc;
    };
    
    template <typename T, class... Ranges>
    class OperationMaster : public MutableOperationBase<T>
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename MultiRange<Ranges...>::IndexType IndexType;

	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OperationBase<T>& second,
			std::shared_ptr<typename CRange::IndexType>& index);
		
	virtual BlockBase<T>& get() override;
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const override;
	virtual OperationMaster& block() const override;
	
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

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const override;
	virtual ConstOperationRoot& block() const override;
	
    protected:
	
	MultiArrayBase<T,Ranges...> const& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    template <typename T, class... Ranges>
    class OperationRoot : public MutableOperationBase<T>,
			  public OperationTemplate<OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationMaster<T,Ranges...> operator=(const OperationBase<T>& in);
	
	virtual const BlockBase<T>& get() const override;
	virtual BlockBase<T>& get() override;

	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const override;
	virtual OperationRoot& block() const override;
	
    protected:
	
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
    };

    template <typename T, class OpFunction, class... Ops>
    class Operation : public OperationBase<T>,
		      public OperationTemplate<Operation<T,OpFunction,Ops...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	Operation(const Ops&... ops);
	
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const override;
	virtual Operation& block() const override;
	
    protected:
	std::tuple<Ops...> mOps;
	mutable BlockResult<T> mRes;
    };

}

#include "multi_array_operation.cc"

#endif
