// -*- C++ -*-

#ifndef __cxz_mrange_h__
#define __cxz_mrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    
    template <class... Indices>
    class MIndex : public IndexInterface<MIndex<Indices...>,
					 Tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<MIndex<Indices...>,
			       Tuple<typename Indices::MetaType...> > IB;
	typedef Tuple<Sptr<Indices>...> IndexPack;
	typedef Tuple<typename Indices::MetaType...> MetaType;
	typedef MRange<typename Indices::RangeType...> RangeType;

	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	MIndex(const MIndex& i);
	MIndex(MIndex&& i);
	MIndex& operator=(const MIndex& i);
	MIndex& operator=(MIndex&& i);

	MIndex(const RangePtr& range, SizeT pos = 0);

	// replace sub-index instances; only use if you know what you are doing!
	MIndex& operator()(Sptr<Indices>&... indices);
	MIndex& operator()(const MIndex& indices);

	const IndexPack& pack() const { return mIPack; }
	const auto& getBlockSizes() const { return mBlockSizes; }
	
	MIndex& operator=(SizeT pos);
	MIndex& operator++();
	MIndex& operator--();
	MIndex operator+(Int n) const;
	MIndex operator-(Int n) const;
	MIndex& operator+=(Int n);
	MIndex& operator-=(Int n);
	
	SizeT max() const;
	decltype(auto) id() const;
	
	SizeT operator*() const;
	SizeT operator->() const;

	int pp(PtrId idxPtrNum);
	int mm(PtrId idxPtrNum);

	SizeT dim();
	Sptr<RangeType> range();

	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;

	String stringMeta() const;
	MetaType meta() const;
	MIndex& at(const MetaType& metaPos);

	template <class PosT, class Xpr>
	auto ifor(const PosT& step, const Xpr& xpr) const;

    private:
	MIndex() = default;
	
	IndexPack mIPack;
	Arr<SizeT,sizeof...(Indices)+1> mBlockSizes;
	Sptr<RangeType> mRange;

    };


    // NOT THREAD SAVE
    template <class... Ranges>
    class MRangeFactory : public RangeFactoryBase
    {
    public:
	MRangeFactory() = delete;
	MRangeFactory(const Sptr<Ranges>&... rs);
	MRangeFactory(const Tuple<Sptr<Ranges>...>& rs);
	
    private:
	virtual void make() override final;

	Tuple<Sptr<Ranges>...> mRs;
    };
    
    template <class... Ranges>
    class MRange : public RangeInterface<MIndex<typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef MIndex<typename Ranges::IndexType...> IndexType;
        typedef Tuple<typename Ranges::IndexType::MetaType...> MetaType;

	const Space& space() const;

	SizeT getMeta(const MetaType& metaPos) const;

	virtual Sptr<RangeBase> sub(SizeT num) const override;
	
	virtual SizeT dim() const final;
	virtual SizeT size() const final;

	virtual String stringMeta(SizeT pos) const final;
	
	friend MRangeFactory<Ranges...>;

    protected:
	MRange() = delete;
	MRange(const MRange& in) = delete;
	MRange& operator=(const MRange& in) = delete;
	
	Tuple<Sptr<Ranges>...> mRs;

    };
    
}

#endif
