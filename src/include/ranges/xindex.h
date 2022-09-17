
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
	typedef IndexInterface<XIndexBase,DType> IB;
	
	DEFAULT_MEMBERS(XIndexBase);
	XIndexBase(SizeT pos) : IndexInterface<XIndexBase,DType>(pos) {}
	
	virtual XIndexBase& operator=(SizeT pos) = 0;
	virtual XIndexBase& operator++() = 0;
	virtual XIndexBase& operator--() = 0;
	virtual Sptr<XIndexBase> operator+(Int n) const = 0;
	virtual Sptr<XIndexBase> operator-(Int n) const = 0;
	virtual XIndexBase& operator+=(Int n) = 0;
	virtual XIndexBase& operator-=(Int n) = 0;

	virtual DType operator*() const = 0;
	virtual DType operator->() const = 0;

	virtual Int pp(PtrId idxPtrNum) = 0;
	virtual Int mm(PtrId idxPtrNum) = 0;

	virtual SizeT dim() const = 0;
	virtual RangePtr range() const = 0;
	virtual SizeT getStepSize(SizeT n) const = 0;

	virtual String stringMeta() const = 0;
	virtual DType meta() const = 0;
	virtual XIndexBase& at(const DType& meta) = 0;

	//virtual DExpr ifor(SizeT step, DExpr ex) const = 0;
	//virtual DExpr iforh(SizeT step, DExpr ex) const = 0;
	// ...!!!
    };

    //Sptr<XIndexBase>& operator++(Sptr<XIndexBase>& i);
    //Sptr<XIndexBase>& operator--(Sptr<XIndexBase>& i);
    
    // MultiIndex Wrapper:
    template <class Index, typename Meta>
    class XIndex : public XIndexBase
    {
    public:
	typedef XIndexBase XIB;
	typedef XIB::IB IB;
	
	DEFAULT_MEMBERS(XIndex);
	XIndex(const IndexPtr<Index,Meta>& i);
	XIndex(const IndexInterface<Index,Meta>& i);

	virtual XIndex& operator=(SizeT pos) override final;
	virtual XIndex& operator++() override final;
	virtual XIndex& operator--() override final;
	virtual Sptr<XIndexBase> operator+(Int n) const override final;
	virtual Sptr<XIndexBase> operator-(Int n) const override final;
	virtual XIndex& operator+=(Int n) override final;
	virtual XIndex& operator-=(Int n) override final;

	virtual DType operator*() const override final;
	virtual DType operator->() const override final;

	virtual Int pp(PtrId idxPtrNum) override final;
	virtual Int mm(PtrId idxPtrNum) override final;

	virtual SizeT dim() const override final;
	virtual RangePtr range() const override final;
	virtual SizeT getStepSize(SizeT n) const override final;

	virtual String stringMeta() const override final;
	virtual DType meta() const override final;
	virtual XIndexBase& at(const DType& meta) override final;

	//virtual DExpr ifor(SizeT step, DExpr ex) const override final;
	//virtual DExpr iforh(SizeT step, DExpr ex) const override final;
	// ....!!!!

    private:
	IndexPtr<Index,Meta> mI;

    };

}

#endif
