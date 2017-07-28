// -*- C++ -*-

#ifndef __container_range_h__
#define __container_range_h__

#include <cstdlib>
#include "base_def.h"

namespace MultiArrayTools
{
    
    template <class... Ranges>
    class ContainerRangeFactory : public RangeFactoryBase
    {
    public:

	typedef ContainerRange<Ranges...> oType;
	
	ContainerRangeFactory() = delete;
	ContainerRangeFactory(const std::shared_ptr<Ranges>&... rs);
	ContainerRangeFactory(const ContainerRange<Ranges...>::SpaceType& space);
	
	virtual std::shared_ptr<RangeBase> create() override; 
	
    protected:
	
    };
    
    template <class... Ranges>
    class ContainerRange : public RangeInterface<ContainerIndex<typename Ranges::IndexType...> >
    {
    public:

	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef typename RangeInterface<ContainerIndex<typename Ranges::IndexType...> >::IndexType IndexType;

	static const size_t dim = sizeof...(Ranges);

	virtual size_t dim() const override;
	virtual size_t size() const override;

	virtual typename IndexType begin() const override;
	virtual typename IndexType end() const override;

	virtual std::shared_ptr<IndexBase> index() const override;

	friend ContainerRangeFactory<Ranges...>;
	
    protected:

	ContainerRange() = default;
	ContainerRange(const ContainerRange& in) = delete;

	ContainerRange(const std::shared_ptr<Ranges>&... rs);
	ContainerRange(const SpaceType& space);

	SpaceType mSpace;	
    };
    
    template <class... Indices>
    class ContainerIndex : public IndexInterface<std::tuple<decltype(Indices().meta())...> >
    {
    public:

	typedef std::tuple<decltype(Indices().meta())...> MetaType;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	
	ContainerIndex() = default;

	ContainerIndex(const ContainerIndex& in);
	ContainerIndex& operator=(const ContainerIndex& in);

	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range);

	virtual ContainerIndex& operator++() override;
	virtual ContainerIndex& operator--() override;
	virtual ContainerIndex& operator=(size_t pos) override;

	virtual MetaType meta() const override;
	virtual ContainerIndex& at(const MetaType& metaPos) override;

	virtual size_t dim() const override;
	virtual size_t pos() const override; // recalculate when externalControl == true
	
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
	
    protected:
	
	bool mExternControl = false;
	IndexPack mIPack;
    };
    
} // end namespace MultiArrayTools

#include "container_range.cc"

#endif
