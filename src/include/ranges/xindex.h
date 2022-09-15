
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

	virtual XIndexBase& operator=(SizeT pos) = 0;
	virtual XIndexBase& operator++() = 0;
	virtual XIndexBase& operator--() = 0;
	virtual Int pp(PtrId idxPtrNum) = 0;
	virtual Int mm(PtrId idxPtrNum) = 0;
	virtual size_t dim() const = 0;
	virtual size_t getStepSize(SizeT n) const = 0;
	virtual String stringMeta() const = 0;
	virtual DType meta() const = 0;
	virtual XIndexBase& at(const DType& meta) = 0;
	//virtual DExpr ifor(SizeT step, DExpr ex) const = 0;
	//virtual DExpr iforh(SizeT step, DExpr ex) const = 0;
	// ...!!!
    };

    // MultiIndex Wrapper:
    template <class Index, typename Meta>
    class XIndex : public XIndexBase
    {
    private:
	IndexPtr<Index,Meta> mI;

    public:
	DEFAULT_MEMBERS(XIndex);
	XIndex(const IndexPtr<Index,Meta>& i);
	XIndex(const IndexInterface<Index,Meta>& i);

	virtual XIndex& operator=(SizeT pos) override;
	virtual XIndex& operator++() override;
	virtual XIndex& operator--() override;
	virtual Int pp(PtrId idxPtrNum) override;
	virtual Int mm(PtrId idxPtrNum) override;
	virtual SizeT dim() const override;
	virtual SizeT getStepSize(SizeT n) const override;
	virtual String stringMeta() const override;
	virtual DType meta() const override;
	virtual XIndexBase& at(const DType& meta) override;
	//virtual DExpr ifor(SizeT step, DExpr ex) const override;
	//virtual DExpr iforh(SizeT step, DExpr ex) const override;
	// ....!!!!
    };

}

#endif
