
#ifndef __cxz_xindex_h__
#define __cxz_xindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"

namespace CNORXZ
{
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

}

#endif
