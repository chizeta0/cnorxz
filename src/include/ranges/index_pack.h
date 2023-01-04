
#ifndef __cxz_index_pack_h__
#define __cxz_index_pack_h__

#include "base/base.h"
#include "xindex.h"

namespace CNORXZ
{
    template <class... Indices>
    class SPack
    {
    public:
	SP_DEFAULT_MEMBERS(constexpr,SPack);
	constexpr SPack(const Sptr<Indices>&... is);
	constexpr SPack(const Tuple<Sptr<Indices>...>& is);

	constexpr const Tuple<Sptr<Indices>...>& all() const;

	constexpr SizeT size() const;
	
	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;

	template <class Index>
	constexpr decltype(auto) rmul(const Sptr<Index>& i) const;

	template <class Index>
	constexpr decltype(auto) lmul(const Sptr<Index>& i) const;

	template <class... Indices2>
	constexpr decltype(auto) mul(const SPack<Indices2...>& p) const;
	
    private:
	Tuple<Sptr<Indices>...> mIs;
    };

    class DPack
    {
    public:
	DEFAULT_MEMBERS(DPack);
	DPack(const Vector<XIndexPtr>& is);
	DPack(Vector<XIndexPtr>&& is);

	const Vector<XIndexPtr>& all() const;
	SizeT size() const;
	const XIndexPtr& get(SizeT i) const;
	const XIndexPtr& operator[](SizeT i) const;
	DPack rmul(const XIndexPtr& i) const;
	DPack lmul(const XIndexPtr& i) const;
	DPack mul(const DPack& p) const;
	
    private:
	Vector<XIndexPtr> mIs;
    };
}

#endif
