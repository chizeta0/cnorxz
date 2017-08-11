// -*- C++ -*-

#ifndef __container_range_h__
#define __container_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "base_def.h"
#include "range_base.h"
#include "index_base.h"

namespace MultiArrayTools
{

    template <class... Indices>
    class ContainerIndex : public IndexInterface<std::tuple<typename Indices::MetaType...> >
    {
    public:

	typedef IndexBase IB;
	typedef std::tuple<typename Indices::MetaType...>  MetaType;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef IndexInterface<std::tuple<typename Indices::MetaType...> > IndexI;
	typedef ContainerRange<typename Indices::RangeType...> RangeType;

    protected:
	bool mExternControl = false;
	IndexPack mIPack;
    
    public:
	ContainerIndex() = delete;

	ContainerIndex(const ContainerIndex& in);
	ContainerIndex& operator=(const ContainerIndex& in);

	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range);

	virtual ContainerIndex& operator++() override;
	virtual ContainerIndex& operator--() override;
	virtual ContainerIndex& operator=(size_t pos) override;

	virtual MetaType meta() const override;
	virtual ContainerIndex& at(const MetaType& metaPos) override;

	virtual bool first() const override;
	virtual bool last() const override;
	
	virtual size_t dim() const override;

	ContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	
	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;
	
	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;
	
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices

	ContainerIndex& operator()(); // -> sync; just to shorten the code

	const std::shared_ptr<RangeType>& range() const;
    };

    
    template <class... Ranges>
    class ContainerRangeFactory : public RangeFactoryBase
    {
    public:

	typedef ContainerRange<Ranges...> oType;
	
	ContainerRangeFactory() = delete;
	ContainerRangeFactory(const std::shared_ptr<Ranges>&... rs);
	ContainerRangeFactory(const typename ContainerRange<Ranges...>::SpaceType& space);
	
	virtual std::shared_ptr<RangeBase> create() override; 
	
    protected:
	
    };
    
    template <class... Ranges>
    class ContainerRange : public RangeInterface<ContainerIndex<typename Ranges::IndexType...> >
    {
    public:

	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef typename RangeInterface<ContainerIndex<typename Ranges::IndexType...> >::IndexType IndexType;
	
    protected:
	ContainerRange() = default;
	ContainerRange(const ContainerRange& in) = delete;
	ContainerRange& operator=(const ContainerRange& in) = delete;
	
	ContainerRange(const std::shared_ptr<Ranges>&... rs);
	ContainerRange(const SpaceType& space);

	SpaceType mSpace;	

    public:	
	static const size_t sdim = sizeof...(Ranges);

	virtual size_t dim() const override;
	virtual size_t size() const override;

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	const SpaceType& space() const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	
	virtual std::shared_ptr<IndexBase> index() const override;

	friend ContainerRangeFactory<Ranges...>;
	
    };
   
} // end namespace MultiArrayTools

#include "container_range.cc"

#endif
