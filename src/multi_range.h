// -*- C++ -*-

#ifndef __multi_range_h__
#define __multi_range_h__

#include <cstdlib>
#include <tuple>

#include "base_def.h"
#include "range_base.h"
#include "index_base.h"

namespace MultiArrayTools
{
    
    template <class... Indices>
    class MultiIndex : public IndexBase<MultiIndex<Indices...> >
    {
    public:

	DEFAULT_MEMBERS(MultiIndex);

	MultiIndex(Indices&&... inds);
	
	typedef std::tuple<Indices...> IndexPack;
	typedef IndefinitIndexBase IIB;
	
	virtual MultiIndex& operator++() override;
	virtual MultiIndex& operator--() override;
	virtual MultiIndex& operator+=(int n) override;
	virtual MultiIndex& operator-=(int n) override;
	
	template <size_t N>
	auto getIndex(size_t x) -> decltype(std::get<N>(IndexPack()));

	template <size_t N>
	auto getIndex(size_t x) const -> decltype(std::get<N>(IndexPack()));

	IndefinitIndexBase& get(size_t n);
	const IndefinitIndexBase& get(size_t n) const;

	virtual void name(const Name& nm) override;
	
	// dimension of MultiRange; includes ALL degrees of freedom
	virtual size_t dim() const override;

	virtual bool link(IndefinitIndexBase* toLink) override;
	virtual void linkTo(IndefinitIndexBase* target) override;
	
    protected:
	
	virtual bool linkLower(IndefinitIndexBase* toLink);
	virtual size_t evaluate(const MultiIndex& in) const override;
	
	IndexPack mIPack;
    };

    template <class... Ranges>
    class MultiRange : public RangeBase<MultiIndex<typename Ranges::IndexType...> >
    {
    public:

	typedef std::tuple<Ranges...> SpaceType;
	
	DEFAULT_MEMBERS(MultiRange);
	static const size_t dim = sizeof...(Ranges);

	template <size_t N>
	auto get() -> decltype( std::get<N>(SpaceType()) );

	template <size_t N>
	auto get() const -> decltype( std::get<N>(SpaceType()) );

	
	virtual MultiIndex<typename Ranges::IndexType...> begin() const override;
	virtual MultiIndex<typename Ranges::IndexType...> end() const override;
	
    protected:
	SpaceType mSpace;
    };
    
}

#include "multi_range.cc"

#endif
