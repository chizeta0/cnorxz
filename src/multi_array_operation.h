// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>
#include <map>
#include <utility>

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

    void seekIndexInst(std::shared_ptr<IndexBase> i, std::vector<std::shared_ptr<IndexBase> >& ivec);

    
    // <block type, step size within actual instance>
    typedef std::pair<BlockType,size_t> BTSS;
    
    BTSS getBlockType(std::shared_ptr<IndexBase> i,
		      std::shared_ptr<IndexBase> j, bool first);

    template <typename T>
    std::shared_ptr<BlockBase<T> > makeBlock(const std::vector<T>& vec, size_t stepSize, size_t blockSize);

    template <typename T>
    std::shared_ptr<MutableBlockBase<T> > makeBlock(std::vector<T>& vec, size_t stepSize, size_t blockSize);

    size_t getBTNum(const std::vector<BTSS>& mp, BlockType bt);

    void minimizeAppearanceOfType(std::map<std::shared_ptr<IndexBase>, std::vector<BTSS> > mp,
				  BlockType bt);

    template <typename T>
    std::shared_ptr<IndexBase> seekBlockIndex(std::shared_ptr<IndexBase> ownIdx,
					      const OperationBase<T>& second);
    
    template <typename T>
    class OperationBase
    {
    public:
	typedef T value_type;

	OperationBase() = default;
	virtual ~OperationBase() = default;

	 // init block, return resulting type (BLOCK, VALUE, SPLIT)
	virtual std::vector<BTSS> block(const std::shared_ptr<IndexBase> blockIndex) const = 0;
	virtual OperationBase& block() const = 0; // update block
	
	//virtual size_t argNum() const = 0;
	virtual const BlockBase<T>& get() const = 0;
    };
   
    template <typename T>
    class MutableOperationBase : public OperationBase<T>
    {
    public:
	typedef T value_type;
	
	MutableOperationBase() = default;

	virtual MutableBlockBase<T>& get() = 0;

    };

    template <typename T, class OperationClass>
    class OperationTemplate
    {
    public:
	
	OperationTemplate(OperationClass* oc);
	
	template <class Second>
	auto operator+(const Second& in) const
	    -> Operation<T,std::plus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator-(const Second& in) const
	    -> Operation<T,std::minus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator*(const Second& in) const
	    -> Operation<T,std::multiplies<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator/(const Second& in) const
	    -> Operation<T,std::divides<T>,OperationClass,Second>;
	
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
		
	virtual MutableBlockBase<T>& get() override;
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BTSS> block(const std::shared_ptr<IndexBase> blockIndex) const override;
	virtual OperationMaster& block() const override;
	
    protected:

	//void performAssignment(const OperationBase<T>& in);
	OperationBase<T> const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	mutable std::shared_ptr<MutableBlockBase<T> > mBlockPtr;
    };

    
    template <typename T, class... Ranges>
    class ConstOperationRoot : public OperationBase<T>,
			       public OperationTemplate<T,ConstOperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BTSS> block(const std::shared_ptr<IndexBase> blockIndex) const override;
	virtual ConstOperationRoot& block() const override;
	
    protected:
	
	MultiArrayBase<T,Ranges...> const& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
 	mutable std::shared_ptr<BlockBase<T> > mBlockPtr;
    };

    template <typename T, class... Ranges>
    class OperationRoot : public MutableOperationBase<T>,
			  public OperationTemplate<T,OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationMaster<T,Ranges...> operator=(const OperationBase<T>& in);
	
	virtual const BlockBase<T>& get() const override;
	virtual MutableBlockBase<T>& get() override;

	virtual std::vector<BTSS> block(const std::shared_ptr<IndexBase> blockIndex) const override;
	virtual OperationRoot& block() const override;
	
    protected:
	
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	mutable std::shared_ptr<MutableBlockBase<T> > mBlockPtr;
    };

    template <typename T, class OpFunction, class... Ops>
    class Operation : public OperationBase<T>,
		      public OperationTemplate<T,Operation<T,OpFunction,Ops...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	Operation(const Ops&... ops);
	
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BTSS> block(const std::shared_ptr<IndexBase> blockIndex) const override;
	virtual Operation& block() const override;
	
    protected:
	std::tuple<Ops...> mOps;
	mutable BlockResult<T> mRes;
    };

}

#include "multi_array_operation.cc"

#endif
