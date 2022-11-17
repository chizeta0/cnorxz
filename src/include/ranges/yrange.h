
#ifndef __cxz_yrange_h__
#define __cxz_yrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xindex.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    class YIndex : public IndexInterface<YIndex,DType>
    {
    public:
	typedef IndexInterface<YIndex,DType> IB;
	typedef YRange RangeType;
	typedef DType MetaType;
	
	YIndex() = default;
	YIndex(YIndex&& i) = default;
	YIndex& operator=(YIndex&& i) = default;

	YIndex(const YIndex& i);
	YIndex& operator=(const YIndex& i);

	YIndex(const RangePtr& range, SizeT lexpos);

	YIndex& operator=(SizeT lexpos);
	YIndex& operator++();
	YIndex& operator--();
	YIndex operator+(Int n) const; // equivalent to applying n times ++
	YIndex operator-(Int n) const;
	YIndex& operator+=(Int n);
	YIndex& operator-=(Int n);

	SizeT lex() const;
	SizeT pmax() const;
	SizeT lmax() const;
	IndexId<0> id() const;
	
	DType operator*() const;

	SizeT dim() const;
	Sptr<YRange> range() const;
	UPos stepSize(const IndexId<0> id) const;

	String stringMeta() const;
	DType meta() const;
	YIndex& at(const DType& meta);

	DXpr<SizeT> ifor(const DXpr<SizeT>& xpr, const std::function<SizeT(SizeT,SizeT)>& f) const;

    private:
	inline Vector<SizeT> mkBlockSizes() const;
	inline Vector<SizeT> mkLexBlockSizes() const;
	inline Vector<XIndexPtr> mkIndices() const;
	inline void up(SizeT i);
	inline void down(SizeT i);
	inline decltype(auto) mkIFor(SizeT i, const DXpr<SizeT>& xpr,
				     const std::function<SizeT(SizeT,SizeT)>& f) const;

	inline SizeT mkPMax() const;
	inline SizeT mkLMax() const;
	
	SizeT mLPos = 0;
	Sptr<YRange> mRange;
	Vector<XIndexPtr> mIs;
	Vector<SizeT> mBlockSizes; // dim() elements only!!!
	Vector<SizeT> mLexBlockSizes; // dim() elements only!!!
	SizeT mLex = 0;
	SizeT mPMax = 0;
	SizeT mLMax = 0;
    };

    class YRangeFactory : public RangeFactoryBase
    {
    public:
	YRangeFactory(const Vector<RangePtr>& rvec);
	YRangeFactory(Vector<RangePtr>&& rvec);
	YRangeFactory(const Vector<RangePtr>& rvec, const RangePtr& ref);
	YRangeFactory(Vector<RangePtr>&& rvec, const RangePtr& ref);

    private:
	YRangeFactory() = default;
	virtual void make() override final;

	Vector<RangePtr> mRVec;
	RangePtr mRef;
    };

    class YRange : public RangeInterface<YRange>
    {
    public:
	typedef RangeBase RB;
	typedef YIndex IndexType;

	friend YRangeFactory;

	virtual RangePtr sub(SizeT i) const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;

    private:

	YRange() = delete;
	YRange(const YRange& a) = delete;
	YRange(const Vector<RangePtr>& rvec);
	YRange(Vector<RangePtr>&& rvec);

	Vector<RangePtr> mRVec;
    };
}

#endif
