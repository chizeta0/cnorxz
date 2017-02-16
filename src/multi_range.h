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
	
	typedef std::tuple<Indices...> IndexPack;
	static size_t sMult = sizeof...(Indices);
	
	virtual MultiIndex& operator++() override;
	virtual MultiIndex& operator--() override;
	virtual MultiIndex& operator+=(int n) override;
	virtual MultiIndex& operator-=(int n) override;
	
	template <size_t N>
	auto& getIndex() -> decltype(std::get<N>(mIPack));

	template <size_t N>
	const auto& getIndex() const -> decltype(std::get<N>(mIPack));

	IndefinitIndexBase& getIndex(size_t n);
	const IndefinitIndexBase& getIndex(size_t n) const;
	
	// dimension of MultiRange
	virtual size_t dim() const override; // implement !!!

	virtual bool link(IndefinitIndexBase* toLink) override;
	virtual void linkTo(IndefinitIndexBase* target) override;
	
    protected:
	
	virtual bool linkLower(IndefinitIndexBase* toLink);
	virtual size_t evaluate(const MultiIndex& in) const override;
	
	IndexPack mIPack;
    };

    template <class... Ranges>
    class MultiRange : public RangeBase<MultiIndex<typename Ranges::indexType...> >
    {
    public:

	DEFAULT_MEMBERS(MultiRange);
	static size_t dim = sizeof...(Ranges);

	template <size_t N>
	auto get() -> decltype( std::get<N>(mSpace) );
	
    protected:
	std::tuple<Ranges...> mSpace;
    };
    
}

#include "multi_range.cc"

#endif
