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

    void seekIndexInst(std::shared_ptr<const IndexBase> i, std::vector<std::shared_ptr<const IndexBase> >& ivec);

    
    // <block type, step size within actual instance>
    typedef std::pair<BlockType,size_t> BTSS;
    
    BTSS getBlockType(std::shared_ptr<const IndexBase> i,
		      std::shared_ptr<const IndexBase> j, bool first);

    template <typename T>
    std::shared_ptr<BlockBase<T> > makeBlock(const std::vector<T>& vec, size_t stepSize, size_t blockSize);

    template <typename T>
    std::shared_ptr<MutableBlockBase<T> > makeBlock(std::vector<T>& vec, size_t stepSize, size_t blockSize);

    size_t getBTNum(const std::vector<BTSS>& mp, BlockType bt)
    {
	size_t out = 0;
	for(auto& xx: mp){
	    if(xx.first == bt){
		++out;
	    }
	}
	return out;
    }

    void minimizeAppearanceOfType(std::map<std::shared_ptr<const IndexBase>, std::vector<BTSS> > mp,
				  BlockType bt)
    {
	size_t minNum = getBTNum( *mp.begin(), bt );
	for(auto& mm: mp){
	    size_t tmp = getBTNum( mm.second, bt );
	    if(tmp < minNum){
		minNum = tmp;
	    }
	}

	for(auto mit = mp.begin(); mit != mp.end(); ){
	    size_t tmp = getBTNum( mit->second, bt );
	    if(tmp > minNum){
		mit = mp.erase(mit);
	    }
	    else {
		++mit;
	    }
	}

    }
    
    template <typename T>
    std::shared_ptr<const IndexBase> seekBlockIndex(std::shared_ptr<const IndexBase>& ownIdx,
						    const OperationBase<T>& second)
    {
	std::vector<std::shared_ptr<const IndexBase> > ivec;
	seekIndexInst(ownIdx, ivec);
	std::map<std::shared_ptr<const IndexBase>, std::vector<BTSS> > mp;
	
	for(auto& xx: ivec){
	    mp[xx] = second.block(xx);
	}

	// seek minimal number of VALUEs => guarantees absence of conflicting blocks
	minimizeAppearanceOfType(mp, BlockType::VALUE);

	// seek mininmal number of SPLITs => maximal vectorization possible
	minimizeAppearanceOfType(mp, BlockType::SPLIT);
	
	return *mp.begin();
    }
    
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
		
	virtual MutableBlockBase<T>& get() override;
	virtual const BlockBase<T>& get() const override;

	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const override;
	virtual OperationMaster& block() const override;
	
    protected:

	//void performAssignment(const OperationBase<T>& in);
	OperationBase<T> const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	mutable std::shared_ptr<MutableBlockBase> mBlockPtr;
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
 	mutable std::shared_ptr<BlockBase> mBlockPtr;
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
	virtual MutableBlockBase<T>& get() override;

	virtual std::vector<BlockType> block(const std::shared_ptr<IndexBase>& blockIndex) const override;
	virtual OperationRoot& block() const override;
	
    protected:
	
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	mutable std::shared_ptr<MutableBlockBase> mBlockPtr;
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
