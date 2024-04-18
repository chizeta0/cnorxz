// -*- C++ -*-
/**
   
   @file include/ranges/wrange.h
   @brief WRange, WRangeFactory and WIndex declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_wrange_h__
#define __cxz_wrange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

#include <iterator>

namespace CNORXZ
{
    
    /** ****
	Specific index for WRange.
	@tparam MetaT Meta data type.
     */
    template <typename MetaT>
    class WIndex : public IndexInterface<WIndex<MetaT>,MetaT>
    {
    public:

	typedef IndexInterface<WIndex<MetaT>,MetaT> IB;
	typedef WRange<MetaT> RangeType;
	typedef MetaT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(WIndex); /**< default constructors and assignments */
	
	/** Construct index from range and position.
	    @param range Range to iterate over.
	    @param pos lexicographic position.
	 */
	WIndex(const RangePtr& range, SizeT pos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	WIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	WIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	WIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	WIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	WIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(WIndex) */
	SizeT operator-(const WIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	WIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	WIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	UPos pmax() const;

	/** @copydoc IndexInterface::lmax() */
	UPos lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	/** @copydoc IndexInterface::operator*() */
	const MetaT& operator*() const;
	
	/** @copydoc IndexInterface::dim() */
	SizeT dim() const; // = 1

	/** @copydoc IndexInterface::range() */
	Sptr<RangeType> range() const;

	/** @copydoc IndexInterface::stepSize() */
	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;

	/** @copydoc IndexInterface::stringMeta() */
	String stringMeta() const;

	/** @copydoc IndexInterface::meta() */
	const MetaT& meta() const;

	/** @copydoc IndexInterface::at() */
	WIndex& at(const MetaT& metaPos);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const WIndex<MetaType>& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	SizeT deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	SizeT deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	WIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** @copydoc IndexInterface::xpr() */
	decltype(auto) xpr(const Sptr<WIndex<MetaType>>& _this) const;

    private:
	Sptr<RangeType> mRangePtr;
	const MetaT* mMetaPtr;
	const SizeT* mSpaceToPos;
	const SizeT* mPosToSpace;
    };

    template <typename MetaT>
    void swap(WIndex<MetaT>& a, WIndex<MetaT>& b) { a.swap(b); } 
    
    /** Make index pack of a WIndex and another index.
	@param a pointer to WIndex.
	@param b pointer to another index.
     */
    template <typename MetaT, class I1>
    decltype(auto) operator*(const Sptr<WIndex<MetaT>>& a, const Sptr<I1>& b);
    
    /** ****
	Specific factory for WRange.
	@tparam MetaT Meta data type.
     */
    template <typename MetaT>
    class WRangeFactory : public RangeFactoryBase
    {
    public:
	WRangeFactory(const Vector<MetaT>& space);
        WRangeFactory(Vector<MetaT>&& space);
	WRangeFactory(const Vector<MetaT>& space, const RangePtr& ref);
        WRangeFactory(Vector<MetaT>&& space, const RangePtr& ref);
	
    private:
	WRangeFactory() = default;
	virtual void make() override final;

	Vector<MetaT> mSpace;
	RangePtr mRef;
    };

    /** ****
	Uni-(1-)dimensional range with non-trivial meta data space
	i.e. the parameter space can be arbitrary. In contrast to URange
	the meta data can have arbitrary ordering.
	Note: Internally, the meta data is still ordered for the sake of performance during look-up.
	The externally visible non-ordering is realized through maps between the internal (meta space)
	ordering, and the externally visible lexicographic ordering.
	@tparam MetaT Meta data type.
     */
    template <typename MetaT>
    class WRange : public RangeInterface<WRange<MetaT>>
    {
    public:
	typedef RangeBase RB;
	typedef WIndex<MetaT> IndexType;
	typedef MetaT MetaType;
	
	friend WRangeFactory<MetaType>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	/** Get meta data at given range position.
	    @param pos Integer indicating requested position.
	    @return Meta data at given postion.
	 */
	const MetaType& get(SizeT pos) const;

	/** Get range position for given meta data.
	    Returns size() if metaPos is not part of the range.
	    @param metaPos Meta data.
	    @return Position of the given meta data if it is contained by the range.
	 */
	SizeT getMeta(const MetaType& metaPos) const;

	/** Get meta data array.
	    @return Pointer to first element (memory ordering) of the underlying meta data array.
	 */
	const MetaType* get() const;

	/** Get map from meta space memory position to lexicographic position.
	    @return Pointer to first element of the map.
	 */ 
	const SizeT* spaceToPos() const;

	/** Get map from lexicographic position to meta space memory position.
	    @return Pointer to first element of the map.
	 */ 
	const SizeT* posToSpace() const;

    private:

	void setupSpace(const Vector<MetaT>& space);
	
	WRange() = default;
	WRange(const WRange& in) = delete;
	WRange(const Vector<MetaType>& space);
        WRange(Vector<MetaType>&& space);

	Vector<MetaType> mSpace;
	Vector<SizeT> mSpaceToPos;
	Vector<SizeT> mPosToSpace;

	virtual Vector<Uuid> key() const override final;
	
	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    /** ***
	Specialize RangeCast for casts to WRange
	@see RangeCast
     */    
    template <typename MetaType>
    struct RangeCast<WRange<MetaType>>
    {
	/** cast the range */
	static Sptr<WRange<MetaType>> func(const RangePtr& r);
    };

    /** Create an WRange, calls WRangeFactory.
	@param space Meta data space to create an WRange on.
	@return Created range.
     */
    template <typename MetaT>
    RangePtr wrange(const Vector<MetaT>& space);

    /** ***
	WIndex can be used as expression
	@see index_expression_exists
     */    
    template <typename MetaT>
    struct index_expression_exists<WIndex<MetaT>>
    {
	static constexpr bool value = true;
    };
}

#endif
