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
	MIndex& operator()(const Sptr<MIndex>& mi);

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

	SizeT dim();
	Sptr<RangeType> range();

	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;

	String stringMeta() const;
	MetaType meta() const;
	MIndex& at(const MetaType& metaPos);

	template <class Xpr, class F>
	constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	MIndex() = default;
	
	IndexPack mIPack;
	//Arr<SizeT,sizeof...(Indices)> mBlockSizes;
	typedef decltype(mkBlockSizes(std::make_index_sequence<sizeof...(Indices)-1>{})) BlockTuple;
	BlockTuple mBlockSizes;
	Sptr<RangeType> mRange;

	// shift to utils:
	template <SizeT P, SizeT... Is>
	constexpr decltype(auto) indexSequencePlus(std::index_sequence<Is...> is) const;

	// shift to utils:
	template <SizeT B, SizeT E>
	constexpr decltype(auto) mkIndexSequence() const;

	
	template <class G, class F, SizeT... Is>
	constexpr decltype(auto) accumulatei(const G& g, const F& f, std::index_sequence<Is...> is) const;

	template <class G, class F, SizeT... Is>
	constexpr decltype(auto) accumulate2i(const G& g, const F& f, std::index_sequence<Is...> is) const;

	template <SizeT B, SizeT E, class G, class F>
	constexpr decltype(auto) accumulate(const G& g, const F& f) const;

	template <SizeT B, SizeT E, class G, class F>
	constexpr decltype(auto) accumulate2(const G& g, const F& f) const;

	template <SizeT... Is>
	constexpr decltype(auto) mkIPack(SizeT pos, std::index_sequence<Is...> is) const;

	template <SizeT... Is>
	inline void setIPack(std::index_sequence<Is...> is) const;

	template <SizeT... Is>
	constexpr decltype(auto) mkBlockSizes(std::index_sequence<Is...> is) const;

	template <SizeT... Is>
	constexpr decltype(auto) mkPos(std::index_sequence<Is...> is) const;

	template <SizeT I>
	inline void up();

	template <SizeT I>
	inline void down();

	template <SizeT... Is>
	inline String mkStringMeta(std::index_sequence<Is...> is) const;

	template <SizeT... Is>
	inline MetaType mkMeta(std::index_sequence<Is...> is) const;

	template <SizeT I, class Xpr, class F>
	constexpr decltype(auto) mkIFor(const Xpr& xpr, F&& f) const;

	template <SizeT... Is>
	inline void ati(const MetaType& meta, std::index_sequence<Is...> is) const;
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
	BlockType mBlockSizes;

	template <SizeT... Is>
	constexpr decltype(auto) mkPos(std::index_sequence<Is...> is) const;
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
    class MRange : public RangeInterface<MIndex<typename Ranges::IndexType...>,
					 Tuple<typename Ranges::IndexType::MetaType...>>
    {
    public:
	typedef RangeBase RB;
	typedef MIndex<typename Ranges::IndexType...> IndexType;
        typedef Tuple<typename Ranges::IndexType::MetaType...> MetaType;

	friend MRangeFactory<Ranges...>;

	virtual RangePtr sub(SizeT num) const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual IndexType begin() const override final;
	virtual IndexType end() const override final;
	
	decltype(auto) space() const;
	const MetaType get(SizeT pos) const;
	SizeT getMeta(const MetaType& metaPos) const;

    protected:
	MRange() = delete;
	MRange(const MRange& in) = delete;
	MRange& operator=(const MRange& in) = delete;
	MRange(const Tuple<Sptr<Ranges>...>& rs);
	
	Tuple<Sptr<Ranges>...> mRs;
	Arr<RangePtr,sizeof...(Ranges)> mA;
    private:

	template <SizeT... Is>
	decltype(auto) mkA(std::index_sequence<Is...> is) const;

	template <SizeT... Is>
	SizeT sizei(std::index_sequence<Is...> is) const;
    };
    
}

#endif
