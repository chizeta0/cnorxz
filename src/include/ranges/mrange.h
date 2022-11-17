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
	static constexpr SizeT NI = sizeof...(Indices);

	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	MIndex() = default;
	MIndex(MIndex&& i) = default;
	MIndex& operator=(MIndex&& i) = default;

	MIndex(const MIndex& i);
	MIndex& operator=(const MIndex& i);

	MIndex(const RangePtr& range, SizeT lexpos = 0);

	MIndex& operator=(SizeT pos);
	MIndex& operator++();
	MIndex& operator--();
	MIndex operator+(Int n) const;
	MIndex operator-(Int n) const;
	MIndex& operator+=(Int n);
	MIndex& operator-=(Int n);

	SizeT lex() const;
	SizeT pmax() const;
	SizeT lmax() const;
	IndexId<0> id() const;

	MetaType operator*() const;

	SizeT dim() const;
	Sptr<RangeType> range() const;

	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;
	
	String stringMeta() const;
	MetaType meta() const;
	MIndex& at(const MetaType& metaPos);
	
	template <class Xpr, class F>
	constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	// replace sub-index instances; only use if you know what you are doing!
	MIndex& operator()(const Sptr<MIndex>& mi);

	const IndexPack& pack() const;
	const auto& blockSizes() const;

    private:
	template <SizeT... Is>
	static constexpr decltype(auto) mkBlockSizes(const IndexPack& ipack, Isq<Is...> is);

	template <SizeT... Is>
	constexpr decltype(auto) mkIPack(Isq<Is...> is) const;

	template <SizeT I>
	inline void up();

	template <SizeT I>
	inline void down();

	template <SizeT I, class Xpr, class F>
	constexpr decltype(auto) mkIFor(const Xpr& xpr, F&& f) const;

	Sptr<RangeType> mRange;
	IndexPack mIPack;
	typedef RemoveRef<decltype(mkBlockSizes(mIPack,Isqr<0,NI-1>{}))> BlockTuple;
	BlockTuple mBlockSizes;
	SizeT mPMax = 0; // = LMax here, add new variable in GMIndex!
    };

    // modified blockSizes; to be used for Slices; can be created from MIndices
    template <class MIndexType, class BlockType>
    class GMIndex : public MIndexType
    {
    public:
	// override everything that modyfies IB::mPos or uses mBlockSizes!!!
	
	constexpr GMIndex(const MIndexType& mi, const BlockType& b);
	
	template <class Xpr, class F>
	constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	BlockType mLexBlockSizes;

	template <SizeT... Is>
	constexpr decltype(auto) mkPos(Isq<Is...> is) const;
    };

    // NOT THREAD SAVE
    template <class... Ranges>
    class MRangeFactory : public RangeFactoryBase
    {
    public:
	MRangeFactory(const Tuple<Sptr<Ranges>...>& rs);
	MRangeFactory(const Tuple<Sptr<Ranges>...>& rs, const RangePtr& ref);
	
    private:
	MRangeFactory() = default;
	virtual void make() override final;

	Tuple<Sptr<Ranges>...> mRs;
	RangePtr mRef;
    };
    
    template <class... Ranges>
    class MRange : public RangeInterface<MRange<Ranges...>>
    {
    public:
	typedef RangeBase RB;
	typedef MIndex<typename Ranges::IndexType...> IndexType;
        typedef Tuple<typename Ranges::IndexType::MetaType...> MetaType;
	static constexpr SizeT NR = sizeof...(Ranges);

	friend MRangeFactory<Ranges...>;

	virtual RangePtr sub(SizeT num) const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	
	decltype(auto) space() const;
	const MetaType get(SizeT pos) const;
	SizeT getMeta(const MetaType& metaPos) const;

    protected:
	MRange() = delete;
	MRange(const MRange& in) = delete;
	MRange& operator=(const MRange& in) = delete;
	MRange(const Tuple<Sptr<Ranges>...>& rs);
	
	Tuple<Sptr<Ranges>...> mRs;
	Arr<RangePtr,NR> mA;
    private:

	decltype(auto) mkA() const;
    };

    template <class... Ranges>
    RangePtr mrange(const Sptr<Ranges>&... rs);
}

#endif
