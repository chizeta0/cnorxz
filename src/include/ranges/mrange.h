// -*- C++ -*-

#ifndef __cxz_mrange_h__
#define __cxz_mrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "index_format.h"
#include "index_pack.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    // template <class FormatT, class... Indices>
    // -> Format + IndexTuple
    template <class FormatT, class... Indices>
    class GMIndex : public IndexInterface<GMIndex<FormatT,Indices...>,
					  Tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<GMIndex<FormatT,Indices...>,
			       Tuple<typename Indices::MetaType...>> IB;
	//typedef Tuple<Sptr<Indices>...> IndexPack;
	typedef Tuple<typename Indices::MetaType...> MetaType;
	typedef MRange<typename Indices::RangeType...> RangeType;
	static constexpr SizeT NI = sizeof...(Indices);

	constexpr GMIndex() = default;
	constexpr GMIndex(GMIndex&& i) = default;
	constexpr GMIndex& operator=(GMIndex&& i) = default;

	constexpr GMIndex(const GMIndex& i);
	constexpr GMIndex& operator=(const GMIndex& i);

	constexpr GMIndex(const SPack<Indices...>& pack);
	constexpr GMIndex(const FormatT& format, const SPack<Indices...>& pack);
	constexpr GMIndex(const Sptr<Indices>&... is);
	constexpr GMIndex(const FormatT& format, const Sptr<Indices>&... is);
	constexpr GMIndex(const RangePtr& range, SizeT lexpos = 0);
	constexpr GMIndex(const RangePtr& range, const FormatT& format, SizeT lexpos = 0);

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
	decltype(auto) xpr(const Sptr<MIndex<Indices...>>& _this) const;
	
	template <class Xpr, class F>
	constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	// replace sub-index instances; only use if you know what you are doing!
	GMIndex& operator()(const Sptr<MIndex<Indices...>>& mi);
	GMIndex& operator()();

	const SPack<Indices...>& pack() const;
	const auto& format() const;
	const auto& lexFormat() const;
	GMIndex& setFormat(const FormatT& bs);

    private:
	template <SizeT... Is>
	static constexpr decltype(auto) mkLexFormat(const SPack<Indices...>& ipack, Isq<Is...> is);

	static constexpr decltype(auto) mkLMax(const SPack<Indices...>& ipack);

	static constexpr decltype(auto) mkPMax(const SPack<Indices...>& ipack, const FormatT& format);

	inline void mkPos();
	
	template <SizeT... Is>
	constexpr decltype(auto) mkIPack(Isq<Is...> is) const;

	template <SizeT I>
	inline void up();

	template <SizeT I>
	inline void down();

	template <SizeT I, class Xpr, class F>
	constexpr decltype(auto) mkIFor(const Xpr& xpr, F&& f) const;

	Sptr<RangeType> mRange;
	SPack<Indices...> mIPack;
	typedef RemoveRef<decltype(mkLexFormat(mIPack,Isqr<0,NI-1>{}))> LexFormatT;
	LexFormatT mLexFormat;
	FormatT mFormat;
	SizeT mLex;
	typedef RemoveRef<decltype(mkLMax(mIPack))> LMaxT;
	LMaxT mLMax;
	typedef RemoveRef<decltype(mkPMax(mIPack,mFormat))> PMaxT;
	PMaxT mPMax;
    };

    template <class BT1, class BT2, class... Indices>
    decltype(auto) replaceFormat(const BT1& bs1, const Sptr<GMIndex<BT2,Indices...>>& gmi);

    template <class... Indices>
    struct index_has_const_size<MIndex<Indices...>>
    { static constexpr bool value = (index_has_const_size<Indices>::value and ...); };

    template <class... Indices>
    struct index_const_size<MIndex<Indices...>>
    { static constexpr SizeT value = (index_const_size<Indices>::value * ...); };

    template <class... Indices>
    struct index_dim<MIndex<Indices...>>
    { static constexpr SizeT value = sizeof...(Indices); };

    template <class... Indices>
    struct has_sub<MIndex<Indices...>>
    { static constexpr bool value = true; };

    template <class... Indices>
    struct has_static_sub<MIndex<Indices...>>
    { static constexpr bool value = true; };

    template <class... Indices>
    constexpr decltype(auto) mindex(const Sptr<Indices>&... is);

    template <class... Indices>
    constexpr decltype(auto) mindex(const SPack<Indices...>& pack);

    template <class... Indices>
    constexpr decltype(auto) mindexPtr(const SPack<Indices...>& pack);

    template <class FormatT, class... Indices>
    constexpr decltype(auto) gmindexPtr(const FormatT& bs, const Sptr<Indices>&... is);
    
    template <class I1, class FormatT, class... Indices>
    decltype(auto) operator*(const Sptr<GMIndex<FormatT,Indices...>>& a, const Sptr<I1>& b);

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
	virtual MArray<RangePtr> sub() const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;
	
	decltype(auto) space() const;
	const MetaType get(SizeT pos) const;
	SizeT getMeta(const MetaType& metaPos) const;

    protected:
	MRange() = default;
	MRange(const MRange& in) = delete;
	MRange& operator=(const MRange& in) = delete;
	MRange(const Tuple<Sptr<Ranges>...>& rs);
	
	Tuple<Sptr<Ranges>...> mRs;
	Arr<RangePtr,NR> mA;

	virtual Vector<Uuid> key() const override final;
    private:

	decltype(auto) mkA() const;
    };

    template <class... Ranges>
    RangePtr mrange(const Sptr<Ranges>&... rs);

    template <class... Ranges>
    struct RangeCast<MRange<Ranges...>>
    {
	static Sptr<MRange<Ranges...>> func(const RangePtr& r);
    };
}

#endif
