
#ifndef __cxz_yrange_h__
#define __cxz_yrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xindex.h"
#include "xpr/xpr.h"
#include "index_format.h"
#include "index_pack.h"

namespace CNORXZ
{
    class YIndex : public IndexInterface<YIndex,Vector<DType>>
    {
    public:
	typedef IndexInterface<YIndex,Vector<DType>> IB;
	typedef YRange RangeType;
	typedef Vector<DType> MetaType;
	
	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	YIndex() = default;
	YIndex(YIndex&& i) = default;
	YIndex& operator=(YIndex&& i) = default;
	// no defaults:
	YIndex(const YIndex& i);
	YIndex& operator=(const YIndex& i);

	YIndex(const Vector<XIndexPtr>& is);
	YIndex(const YFormat& bs, const Vector<XIndexPtr>& is);
	YIndex(const RangePtr& range, SizeT lexpos = 0);
	YIndex(const RangePtr& range, const YFormat& bs, SizeT lexpos = 0);

	YIndex& operator=(SizeT lexpos);
	YIndex& operator++();
	YIndex& operator--();
	YIndex operator+(Int n) const; // equivalent to applying n times ++
	YIndex operator-(Int n) const;
	YIndex& operator+=(Int n);
	YIndex& operator-=(Int n);

	SizeT lex() const;
	UPos pmax() const;
	UPos lmax() const;
	IndexId<0> id() const;
	
	Vector<DType> operator*() const;

	SizeT dim() const;
	Sptr<YRange> range() const;
	UPos stepSize(const IndexId<0> id) const;

	template <class Index>
	YIndex formatFrom(const Index& ind) const;

	String stringMeta() const;
	Vector<DType> meta() const;
	YIndex& at(const Vector<DType>& meta);
	DXpr<SizeT> xpr(const Sptr<YIndex>& _this) const;

	DXpr<SizeT> ifor(const DXpr<SizeT>& xpr, std::function<SizeT(SizeT,SizeT)>&& f) const;

	YIndex& operator()(const Sptr<YIndex>& i);
	YIndex& operator()();

	const DPack& pack() const;
	Vector<SizeT> deepFormat() const;
	const YFormat& format() const;
	const YFormat& lexFormat() const;
	YIndex& setFormat(const YFormat& bs);
	
    private:
	inline Vector<SizeT> mkFormat() const;
	inline Vector<SizeT> mkLexFormat() const;
	inline Vector<RangePtr> mkRangeVec(const Vector<XIndexPtr>& is) const;
	inline void mkPos();
	inline DPack mkIndices() const;
	inline void up(SizeT i);
	inline void down(SizeT i);
	inline decltype(auto) mkIFor(SizeT i, const DXpr<SizeT>& xpr,
				     std::function<SizeT(SizeT,SizeT)>&& f) const;

	inline SizeT mkPMax() const;
	inline SizeT mkLMax() const;
	
	Sptr<YRange> mRange;
	DPack mIs;
	YFormat mFormat; // dim() elements only!!!
	YFormat mLexFormat; // dim() elements only!!!
	SizeT mLex = 0;
	UPos mPMax = 0;
	UPos mLMax = 0;
    };

    template <>
    struct index_is_multi<YIndex>
    { static constexpr bool value = true; };

    YIndex yindex(const DPack& pack);
    YIndex yindex(const Vector<XIndexPtr>& is);
    Sptr<YIndex> yindexPtr(const DPack& is);
    Sptr<YIndex> yindexPtr(const Vector<XIndexPtr>& is);
    Sptr<YIndex> yindexPtr(const Vector<SizeT>& bs, const Vector<XIndexPtr>& is);

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
	virtual MArray<RangePtr> sub() const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

    private:

	YRange() = default;
	YRange(const YRange& a) = delete;
	YRange(const Vector<RangePtr>& rvec);
	YRange(Vector<RangePtr>&& rvec);

	Vector<RangePtr> mRVec;

	virtual Vector<Uuid> key() const override final;
	
	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    RangePtr yrange(const Vector<RangePtr>& rs);
    
    template <>
    struct RangeCast<YRange>
    {
	static Sptr<YRange> func(const RangePtr& r);
    };

}

#endif
