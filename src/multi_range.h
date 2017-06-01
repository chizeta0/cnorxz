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
    class MultiIndex : public IndexBase<MultiIndex<Indices...> >
    {
    public:
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef IndefinitIndexBase IIB;
	typedef IndexBase<MultiIndex<std::shared_ptr<Indices>...> > IB;
	typedef std::tuple<decltype(Indices().getMetaPos())...> MetaType;
	
    protected:
	
	virtual bool linkLower(IndefinitIndexBase* toLink);
	virtual size_t evaluate(const MultiIndex& in) const override;
	
	IndexPack mIPack;

    public:

	MultiIndex() = default;
	// NO DEFAULT HERE !!!
	// ( have to subord sub-indices (mMajor) correctly, and not only copy their mMajor pointer to 'in'
	// which is not major any more in copies!! )
	MultiIndex(const MultiIndex& in);
	MultiIndex& operator=(const MultiIndex& in);
	
	MultiIndex(RangeBase<MultiIndex<std::shared_ptr<Indices>...> > const* range);
	
	MultiIndex(RangeBase<MultiIndex<std::shared_ptr<Indices>...> > const* range,
		   Indices&&... inds);
	
	MultiIndex(RangeBase<MultiIndex<std::shared_ptr<Indices>...> > const* range,
		   const IndexPack& ipack);

	MultiIndex(std::vector<std::shared_ptr<IndefinitIndexBase> >& indexList);
	
	virtual MultiIndex& operator++() override;
	virtual MultiIndex& operator--() override;
	virtual MultiIndex& operator+=(int n) override;
	virtual MultiIndex& operator-=(int n) override;

	bool operator==(const MultiIndex& in);
	bool operator!=(const MultiIndex& in);
	
	virtual MultiIndex& operator=(size_t pos) override;
	virtual MultiRangeType rangeType() const override;
	
	template <size_t N>
	auto getIndex() -> decltype(*std::get<N>(mIPack))&;
	//typename std::tuple_element<N, std::tuple<std::shared_ptr<Indices>...> >::type& getIndex();

	template <size_t N>
	auto getIndex() const -> decltype(*std::get<N>(mIPack))&;
	//typename std::tuple_element<N, std::tuple<std::shared_ptr<Indices>...> >::type const& getIndex() const;
	
	IndefinitIndexBase& get(size_t n);
	const IndefinitIndexBase& get(size_t n) const;

        MetaType getMetaPos() const;
	MultiIndex& atMeta(const MetaType& metaPos);
	
	MultiIndex& operator()(Indices&&... inds);
	MultiIndex& operator()(const Indices&... inds);
	
	// dimension of MultiRange; includes ALL degrees of freedom
	virtual size_t dim() const override;
	virtual size_t giveSubStepSize(IndefinitIndexBase* subIndex) override;
    };

    /*****************************
     *   IndexGetter Functions   *
     ****************************/
    
  
    template <class... Ranges>
    class MultiRange : public RangeBase<MultiIndex<typename Ranges::IndexType...> >
    {
    public:

	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef typename RangeBase<MultiIndex<typename Ranges::IndexType...> >::IndexType
	IndexType;

	static const size_t dim = sizeof...(Ranges);
	
	template <size_t N> // !!! return ref to range, not the corresp. shared ptr
	auto getRange() -> ;
	//typename std::tuple_element<N, std::tuple<std::shared_ptr<Ranges>...> >::type& getRange();

	template <size_t N> // !!!
	auto getRange() const ->;
	//typename std::tuple_element<N, std::tuple<std::shared_ptr<Ranges>...> >::type const& getRange() const;
	
	size_t size() const override;
	const SpaceType& space() const;
	
	virtual MultiRangeType type() const override; 
	
	virtual MultiIndex<typename Ranges::IndexType...> begin() const override;
	virtual MultiIndex<typename Ranges::IndexType...> end() const override;

	virtual std::shared_ptr<IndefinitIndexBase> indexInstance() const override;
	
    protected:

	MultiRange() = delete;
	MultiRange(const MultiRange& in) = delete;
	MultiRange& operator=(const MultiRange& in) = delete;
	
	MultiRange(const std::shared_ptr<Ranges>&... rs);
	MultiRange(const SpaceType& space);
	
	SpaceType mSpace;
    };
    
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& os,
			 const std::tuple<Ts...>& meta);


#include "multi_range.cc"

#endif
