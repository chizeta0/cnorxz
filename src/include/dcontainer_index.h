
#ifndef __dcontainer_index_h__
#define __dcontainer_index_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "mbase_def.h"
#include "statics/static_for.h"
#include "ranges/range_helper.h"
#include "ranges/dynamic_meta.h"
#include "xfor/xfor.h"

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }
    
    // Future IndexWrapper
    class XIndexBase : public IndexInterface<XIndexBase,DType>
    {
    public:
	DEFAULT_MEMBERS(XIndexBase);

	constexpr IndexType type() const;

	virtual XIndexBase& operator=(size_t pos) = 0;
	virtual XIndexBase& operator++() = 0;
	virtual XIndexBase& operator--() = 0;
	virtual int pp(std::intptr_t idxPtrNum) = 0;
	virtual int mm(std::intptr_t idxPtrNum) = 0;
	virtual size_t dim() const = 0;
	virtual size_t getStepSize(size_t n) const = 0;
	virtual std::string stringMeta() const = 0;
	virtual DType meta() const = 0;
	virtual XIndexBase& at(const DType& meta) = 0;
	virtual DynamicExpression ifor(size_t step, DynamicExpression ex) const = 0;
	virtual DynamicExpression iforh(size_t step, DynamicExpression ex) const = 0;
	// ...!!!
    };

    typedef std::shared_ptr<XIndexBase> XIndexPtr;
    
    // MultiIndex Wrapper:
    template <class Index, typename Meta>
    class XIndex : public XIndexBase
    {
    private:
	IndexPtr<Index,Meta> mI;

    public:
	DEFAULT_MEMBERS(XIndex);
	XIndex(const IndexPtr<Index,Meta>& i);

	virtual XIndex& operator=(size_t pos) override;
	virtual XIndex& operator++() override;
	virtual XIndex& operator--() override;
	virtual int pp(std::intptr_t idxPtrNum) override;
	virtual int mm(std::intptr_t idxPtrNum) override;
	virtual size_t dim() const override;
	virtual size_t getStepSize(size_t n) const override;
	virtual std::string stringMeta() const override;
	virtual DType meta() const override;
	virtual XIndexBase& at(const DType& meta) override;
	virtual DynamicExpression ifor(size_t step, DynamicExpression ex) const override;
	virtual DynamicExpression iforh(size_t step, DynamicExpression ex) const override;
	// ....!!!!
    };

    // Future DynamicIndex
    //class YIndex : public IndexInterface<YIndex,DType>
    class YIndex : public XIndexBase
    {
    public:
	typedef IndexInterface<YIndex,DType> IB;
	
    private:
	vector<XIndexPtr> mIs;
	RangePtr mRange;
	std::vector<size_t> mBlockSizes; // dim() elements only!!!
	bool mExternalControl = false;

    public:
	constexpr IndexType type() const;

	DEFAULT_MEMBERS(YIndex);
	YIndex(const RangePtr& range);
	YIndex(const RangePtr& range, const std::vector<XIndexPtr>& is);

    	YIndex& operator=(size_t pos);
	YIndex& operator++();
	YIndex& operator--();
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);
	size_t dim() const;
	size_t getStepSize(size_t n) const;
	std::string stringMeta() const;
	DType meta() const;
	YIndex& at(const DType& meta);
	DynamicExpression ifor(size_t step, DynamicExpression ex) const;
	DynamicExpression iforh(size_t step, DynamicExpression ex) const;
    };

    typedef std::shared_ptr<YIndex> YIndexPtr;
    
    template <typename T>
    class DConstContainerIndex : public IndexInterface<DConstContainerIndex<T>,DType>
    {
    public:
	typedef IndexInterface<DConstContainerIndex<T>,DType> IB;

    protected:
	YIndexPtr mI;
	const T* mCData = nullptr;

    public:
	DEFAULT_MEMBERS(DConstContainerIndex);
	DConstContainerIndex(const T* data, const RangePtr& range);
	
    	DConstContainerIndex& operator=(size_t pos);
	DConstContainerIndex& operator++();
	DConstContainerIndex& operator--();

	template <class I, typename M> // fast but unsave
	DConstContainerIndex operator+(const IndexInterface<I,M>& i);
	
	template <class I, typename M> // fast but unsave
	DConstContainerIndex operator-(const IndexInterface<I,M>& i);
	
	template <class I, typename M> // save version of operator+
	DConstContainerIndex plus(const IndexInterface<I,M>& i);
	
	template <class I, typename M> // save version of operator-
	DConstContainerIndex minus(const IndexInterface<I,M>& i);
	
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);
	size_t dim() const;
	size_t getStepSize(size_t n) const;
	std::string stringMeta() const;
	DType meta() const;
	DType metaPtr() const;
	DConstContainerIndex& at(const DType& meta);
	DynamicExpression ifor(size_t step, DynamicExpression ex) const;
	DynamicExpression iforh(size_t step, DynamicExpression ex) const;

	const T& operator*() const;
	const T* operator->() const;

	DConstContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	DConstContainerIndex& operator()(const std::vector<XIndexPtr>& inds); // control via external indice
	DConstContainerIndex& operator()(); // -> sync; just to shorten the code
	
    };

    template <typename T>
    class DContainerIndex : public DConstContainerIndex<T>
    {
    public:
	typedef DConstContainerIndex<T> CCI;
	typedef typename CCI::IB IB;
	
    private:
	T* mData = nullptr;

    public:
	
	DEFAULT_MEMBERS(DContainerIndex);
	DContainerIndex(T* data, const RangePtr& range);
	DContainerIndex(T* data, const DConstContainerIndex<T>& cci);

	T& operator*();
	T* operator->();
    };
}

#endif
