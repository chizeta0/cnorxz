
#ifndef __cxz_xindex_h__
#define __cxz_xindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    // Future IndexWrapper
    class XIndexBase
    {
    public:
	DEFAULT_MEMBERS(XIndexBase);
	virtual XIndexPtr copy() const = 0;
	virtual SizeT pos() const = 0;
	
	virtual XIndexBase& operator=(SizeT pos) = 0;
	virtual XIndexBase& operator++() = 0;
	virtual XIndexBase& operator--() = 0;
	virtual XIndexPtr operator+(Int n) const = 0;
	virtual XIndexPtr operator-(Int n) const = 0;
	virtual XIndexBase& operator+=(Int n) = 0;
	virtual XIndexBase& operator-=(Int n) = 0;

	virtual SizeT max() const = 0;
	virtual IndexId<0> id() const = 0;
	
	virtual DType operator*() const = 0;
	virtual DType operator->() const = 0;

	virtual Int pp(PtrId idxPtrNum) = 0;
	virtual Int mm(PtrId idxPtrNum) = 0;

	virtual SizeT dim() const = 0;
	virtual RangePtr range() const = 0;
	virtual UPos stepSize(const IndexId<0>& id) const = 0;

	virtual String stringMeta() const = 0;
	virtual DType meta() const = 0;
	virtual XIndexBase& at(const DType& meta) = 0;

	virtual DXpr<SizeT> ifor(const DXpr<SizeT>& xpr) const = 0;
    };

    //Sptr<XIndexBase>& operator++(Sptr<XIndexBase>& i);
    //Sptr<XIndexBase>& operator--(Sptr<XIndexBase>& i);
    
    // MultiIndex Wrapper:
    template <class Index, typename Meta>
    class XIndex : public XIndexBase
    {
    public:

	DEFAULT_C(XIndex);
	// no default copy/assignment (have to copy objects in shared ptr)
	XIndex(const XIndex& i);
	XIndex(XIndex&& i);
	XIndex& operator=(const XIndex& i);
	XIndex& operator=(XIndex&& i);
	XIndex(const IndexPtr<Index,Meta>& i);
	XIndex(const IndexInterface<Index,Meta>& i);

	virtual XIndexPtr copy() const override final;
	virtual SizeT pos() const override final;

	virtual XIndex& operator=(SizeT pos) override final;
	virtual XIndex& operator++() override final;
	virtual XIndex& operator--() override final;
	virtual XIndexPtr operator+(Int n) const override final;
	virtual XIndexPtr operator-(Int n) const override final;
	virtual XIndex& operator+=(Int n) override final;
	virtual XIndex& operator-=(Int n) override final;

	virtual SizeT max() const override final;
	virtual IndexId<0> id() const override final;
	
	virtual DType operator*() const override final;
	virtual DType operator->() const override final;

	virtual Int pp(PtrId idxPtrNum) override final;
	virtual Int mm(PtrId idxPtrNum) override final;

	virtual SizeT dim() const override final;
	virtual RangePtr range() const override final;
	virtual UPos stepSize(const IndexId<0>& id) const override final;

	virtual String stringMeta() const override final;
	virtual DType meta() const override final;
	virtual XIndexBase& at(const DType& meta) override final;

	virtual DXpr<SizeT> ifor(const DXpr<SizeT>& xpr) const override final;

    private:
	IndexPtr<Index,Meta> mI;

    };

}

#endif
