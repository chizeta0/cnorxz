// -*- C++ -*-
/**
   
   @file include/ranges/urange.h
   @brief URange, URangeFactory and UIndex declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_urange_h__
#define __cxz_urange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

#include <iterator>

namespace CNORXZ
{
    
    /** ****
	Specific index for URange.
	@tparam MetaT Meta data type.
     */
    template <typename MetaT>
    class UIndex : public IndexInterface<UIndex<MetaT>,MetaT>
    {
    public:

	typedef IndexInterface<UIndex<MetaT>,MetaT> IB;
	typedef URange<MetaT> RangeType;
	typedef MetaT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(UIndex); /**< default constructors and assignments */
	
	/** Construct index from range and position.
	    @param range Range to iterate over.
	    @param pos lexicographic position.
	 */
	UIndex(const RangePtr& range, SizeT pos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	UIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	UIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	UIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	UIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	UIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(UIndex) */
	SizeT operator-(const UIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	UIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	UIndex& operator-=(Int n);

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
	UIndex& at(const MetaT& metaPos);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const UIndex<MetaType>& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	SizeT deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	SizeT deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	UIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** @copydoc IndexInterface::xpr() */
	decltype(auto) xpr(const Sptr<UIndex<MetaType>>& _this) const;

    private:
	Sptr<RangeType> mRangePtr;
	const MetaT* mMetaPtr;
    };

    template <typename MetaT>
    void swap(UIndex<MetaT>& a, UIndex<MetaT>& b) { a.swap(b); } 
    
    /** Make index pack of a UIndex and another index.
	@param a pointer to UIndex.
	@param b pointer to another index.
     */
    template <typename MetaT, class I1>
    decltype(auto) operator*(const Sptr<UIndex<MetaT>>& a, const Sptr<I1>& b);
    
    /** ****
	Specific factory for URange.
	@tparam MetaT Meta data type.
     */
    template <typename MetaT>
    class URangeFactory : public RangeFactoryBase
    {
    public:
	URangeFactory(const Vector<MetaT>& space);
        URangeFactory(Vector<MetaT>&& space);
	URangeFactory(const Vector<MetaT>& space, const RangePtr& ref);
        URangeFactory(Vector<MetaT>&& space, const RangePtr& ref);
	
    private:
	URangeFactory() = default;
	virtual void make() override final;

	Vector<MetaT> mSpace;
	RangePtr mRef;
    };

    /** ****
	Uni-(1-)dimensional range with non-trivial meta data space
	i.e. the parameter space can be arbitrary. However, the parameter space
	is assumed to be ordered, i.e. the i-1-th element is assumed to be smaller
	(according to std::less) than the i-th element.
	(In the future, there will be another range type, where random ordering is possible.)
	@tparam MetaT Meta data type.
     */
    template <typename MetaT>
    class URange : public RangeInterface<URange<MetaT>>
    {
    public:
	typedef RangeBase RB;
	typedef UIndex<MetaT> IndexType;
	typedef MetaT MetaType;
	
	friend URangeFactory<MetaType>;

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

	/** Get meta data array.
	    @return Pointer to first element of the underlying meta data array.
	 */
	const MetaType* get() const;

	/** Get range position for given meta data.
	    Returns size() if metaPos is not part of the range.
	    @param metaPos Meta data.
	    @return Position of the given meta data if it is contained by the range.
	 */
	SizeT getMeta(const MetaType& metaPos) const;
	
    private:

	URange() = default;
	URange(const URange& in) = delete;
	URange(const Vector<MetaType>& space);
        URange(Vector<MetaType>&& space);

	Vector<MetaType> mSpace;

	virtual Vector<Uuid> key() const override final;
	
	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    /** ***
	Specialize RangeCast for casts to URange
	@see RangeCast
     */    
    template <typename MetaType>
    struct RangeCast<URange<MetaType>>
    {
	/** cast the range */
	static Sptr<URange<MetaType>> func(const RangePtr& r);
    };

    /** Create an URange, calls URangeFactory.
	@param space Meta data space to create an URange on.
	@return Created range.
     */
    template <typename MetaT>
    RangePtr urange(const Vector<MetaT>& space);

    /** ***
	UIndex can be used as expression
	@see index_expression_exists
     */    
    template <typename MetaT>
    struct index_expression_exists<UIndex<MetaT>>
    {
	static constexpr bool value = true;
    };
}

#endif
