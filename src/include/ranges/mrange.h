// -*- C++ -*-

#ifndef __cxz_mrange_h__
#define __cxz_mrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    
    template <class BlockType, class... Indices>
    class GMIndex : public IndexInterface<GMIndex<BlockType,Indices...>,
					  Tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<GMIndex<BlockType,Indices...>,
			       Tuple<typename Indices::MetaType...>> IB;
	typedef Tuple<Sptr<Indices>...> IndexPack;
	typedef Tuple<typename Indices::MetaType...> MetaType;
	typedef MRange<typename Indices::RangeType...> RangeType;
	static constexpr SizeT NI = sizeof...(Indices);

	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	constexpr GMIndex() = default;
	constexpr GMIndex(GMIndex&& i) = default;
	constexpr GMIndex& operator=(GMIndex&& i) = default;

	constexpr GMIndex(const GMIndex& i);
	constexpr GMIndex& operator=(const GMIndex& i);

	constexpr GMIndex(const RangePtr& range, SizeT lexpos = 0);
	constexpr GMIndex(const RangePtr& range, const BlockType& blockSizes, SizeT lexpos = 0);

	GMIndex& operator=(SizeT pos);
	GMIndex& operator++();
	GMIndex& operator--();
	GMIndex operator+(Int n) const;
	GMIndex operator-(Int n) const;
	GMIndex& operator+=(Int n);
	GMIndex& operator-=(Int n);

	SizeT lex() const;
	constexpr decltype(auto) pmax() const;
	constexpr decltype(auto) lmax() const;
	IndexId<0> id() const;

	MetaType operator*() const;
	
	constexpr SizeT dim() const;
	Sptr<RangeType> range() const;

	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;
	
	String stringMeta() const;
	MetaType meta() const;
	GMIndex& at(const MetaType& metaPos);
	
	template <class Xpr, class F>
	constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	// replace sub-index instances; only use if you know what you are doing!
	GMIndex& operator()(const Sptr<GMIndex>& mi);

	const IndexPack& pack() const;
	const auto& blockSizes() const;
	const auto& lexBlockSizes() const;

    private:
	template <SizeT... Is>
	static constexpr decltype(auto) mkLexBlockSizes(const IndexPack& ipack, Isq<Is...> is);

	static constexpr decltype(auto) mkLMax(const IndexPack& ipack);

	static constexpr decltype(auto) mkPMax(const IndexPack& ipack, const BlockType& blockSizes);

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
	typedef RemoveRef<decltype(mkLexBlockSizes(mIPack,Isqr<0,NI-1>{}))> LexBlockType;
	LexBlockType mLexBlockSizes;
	BlockType mBlockSizes;
	SizeT mLex;
	typedef RemoveRef<decltype(mkLMax(mIPack))> LMaxT;
	LMaxT mLMax;
	typedef RemoveRef<decltype(mkPMax(mIPack,mBlockSizes))> PMaxT;
	PMaxT mPMax;
    };

    template <class... Indices>
    using MIndex = GMIndex<None,Indices...>;
    
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
