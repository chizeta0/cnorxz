// -*- C++ -*-

#ifndef __multi_range_h__
#define __multi_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "base_def.h"
#include "range_base.h"
#include "index_base.h"

namespace MultiArrayTools
{
    
    template <class... Indices>
    class MultiIndex : public IndexInterface<std::tuple<decltype(Indices().meta())...> >
    {
    public:
	
	typedef IndexBase IB;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef std::tuple<decltype(Indices().meta())...> MetaType;
	typedef IndexInterface<MetaType> IndexI;	

    protected:
	IndexPack mIPack;

    public:
	MultiIndex() = default;
	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	MultiIndex(const MultiIndex& in);
	MultiIndex& operator=(const MultiIndex& in);

	template <class MRange>
	MultiIndex(const std::shared_ptr<MRange>& range);
	
	virtual MultiIndex& operator++() override;
	virtual MultiIndex& operator--() override;
	virtual MultiIndex& operator=(size_t pos) override;

	template <size_t DIR>
	MultiIndex& up();

	template <size_t DIR>
	MultiIndex& down();
	
	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;
		
	const IndexBase& get(size_t n) const;
	
        virtual MetaType meta() const override;
	virtual MultiIndex& at(const MetaType& metaPos) override;

	virtual bool first() const override;
	virtual bool last() const override;
	
	virtual size_t dim() const override;
    };

    /*************************
     *   MultiRangeFactory   *
     *************************/

    template <class... Ranges>
    class MultiRangeFactory : public RangeFactoryBase
    {
    public:
	typedef MultiRange<Ranges...> oType;
	
	MultiRangeFactory() = delete;
	MultiRangeFactory(const std::shared_ptr<Ranges>&... rs);
	MultiRangeFactory(const typename MultiRange<Ranges...>::SpaceType& space);
	
	virtual std::shared_ptr<RangeBase> create() override;
    };
    
    /******************
     *   MultiRange   *
     ******************/
      
    template <class... Ranges>
    class MultiRange : public RangeInterface<MultiIndex<typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef typename RangeInterface<MultiIndex<typename Ranges::IndexType...> >::IndexType IndexType;

    protected:
	MultiRange() = delete;
	MultiRange(const MultiRange& in) = delete;
	MultiRange& operator=(const MultiRange& in) = delete;
	
	MultiRange(const std::shared_ptr<Ranges>&... rs);
	MultiRange(const SpaceType& space);
	
	SpaceType mSpace;
	
    public:
	
	static const size_t sdim = sizeof...(Ranges);

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	virtual size_t dim() const override;
	virtual size_t size() const override;
	
	const SpaceType& space() const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;

	virtual std::shared_ptr<IndexBase> index() const override;

	friend MultiRangeFactory<Ranges...>;
	
    };
    
}

#include "multi_range.cc"

#endif
