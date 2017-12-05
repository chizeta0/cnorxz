

#ifdef include_range_type
include_range_type(SPIN,2)
#else

#ifdef __ranges_header__
// assert, that this is only used within range_types/header.h

#ifndef __spin_range_h__
#define __spin_range_h__

namespace MultiArrayTools
{
    typedef SingleIndex<size_t,RangeType::SPIN> SpinIndex;

    template <>
    class SingleRangeFactory<size_t,RangeType::SPIN> : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<U,TYPE> oType;

	SingleRangeFactory() = delete;
	SingleRangeFactory(size_t spinNum); // = 4 :)
	std::shared_ptr<RangeBase> create();
	
    };

    template <>
    class SingleRange<size_t,RangeType::SPIN> : public RangeInterface<SpinIndex>
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<U,TYPE> >::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	virtual std::shared_ptr<IndexBase> index() const override;
	
	friend SingleRangeFactory<size_t,RangeType::SPIN>;
	
    protected:

	SingleRange() = default;
	SingleRange(const SingleRange& in) = delete;

	SingleRange(size_t spinNum);

	size_t mSpinNum = 4;
    };

    typedef SingleRange<size_t,RangeType::SPIN> SpinRange;
    typedef SingleRangeFactory<size_t,RangeType::SPIN> SpinRF;
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /********************
     *   SingleRange    *
     ********************/
    
    SingleRangeFactory<size_t,RangeType::SPIN>::SingleRangeFactory(const std::vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new SingleRange<size_t,RangeType::SPIN>( space ) );
    }

    std::shared_ptr<RangeBase> SingleRangeFactory<size_t,RangeType::SPIN>::create()
    {
	setSelf();
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    SingleRange<size_t,RangeType::SPIN>::SingleRange(size_t spinNum) :
	RangeInterface<SingleIndex<size_t,RangeType::SPIN> >()
    {
	mSpinNum = spinNum;
    }
        
    size_t SingleRange<size_t,RangeType::SPIN>::get(size_t pos) const
    {
	return pos;
    }
    
    size_t SingleRange<size_t,RangeType::SPIN>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t SingleRange<size_t,RangeType::SPIN>::size() const
    {
	return mSpinNum;
    }
    
    size_t SingleRange<size_t,RangeType::SPIN>::dim() const
    {
	return 1;
    }
    
    typename SingleRange<size_t,RangeType::SPIN>::IndexType SingleRange<size_t,RangeType::SPIN>::begin() const
    {
	SingleIndex<size_t,RangeType::SPIN> i( std::dynamic_pointer_cast<SingleRange<size_t,RangeType::SPIN> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename SingleRange<size_t,RangeType::SPIN>::IndexType SingleRange<size_t,RangeType::SPIN>::end() const
    {
	SingleIndex<size_t,RangeType::SPIN> i( std::dynamic_pointer_cast<SingleRange<size_t,RangeType::SPIN> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    std::shared_ptr<IndexBase> SingleRange<size_t,RangeType::SPIN>::index() const
    {
	return std::make_shared<SingleIndex<size_t,RangeType::SPIN> >
	    ( std::dynamic_pointer_cast<SingleRange<size_t,RangeType::SPIN> >
	      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
    }
}

#endif // #ifndef __spin_range_h__

#endif // #ifdef __ranges_header__

#endif // #ifdef include_range_type
