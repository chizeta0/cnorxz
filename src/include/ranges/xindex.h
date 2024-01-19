// -*- C++ -*-
/**
   
   @file include/ranges/xindex.h
   @brief XIndexBase and XIndex template declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_xindex_h__
#define __cxz_xindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	Abstract index wrapper base.
	Can be used for index polymorphism.
	Only use if absolutely necessary, indices should always reveal as much as possible
	to the compiler!
     */
    class XIndexBase
    {
    public:

	DEFAULT_MEMBERS(XIndexBase); /**< default constructors and assignments */

	/** Virtual default destructor */
	virtual ~XIndexBase() = default;

	/** Copy this index
	    @return Pointer to the copy.
	 */
	virtual XIndexPtr copy() const = 0;

	/** Current position.
	    @return Current position.
	 */
	virtual SizeT pos() const = 0;
	
	/** @copydoc IndexInterface::operator=(SizeT) */
	virtual XIndexBase& operator=(SizeT lexpos) = 0;

	/** @copydoc IndexInterface::operator++() */
	virtual XIndexBase& operator++() = 0;

	/** @copydoc IndexInterface::operator--() */
	virtual XIndexBase& operator--() = 0;

	/** @copydoc IndexInterface::operator+() */
	virtual XIndexPtr operator+(Int n) const = 0;

	/** @copydoc IndexInterface::operator-() */
	virtual XIndexPtr operator-(Int n) const = 0;

	/** @copydoc IndexInterface::operator-(UIndex) */
	virtual SizeT operator-(const XIndexBase& i) const = 0;

	/** @copydoc IndexInterface::operator+=() */
	virtual XIndexBase& operator+=(Int n) = 0;

	/** @copydoc IndexInterface::operator-=() */
	virtual XIndexBase& operator-=(Int n) = 0;

	/** @copydoc IndexInterface::lex() */
	virtual SizeT lex() const = 0;

	/** @copydoc IndexInterface::pmax() */
	virtual UPos pmax() const = 0;

	/** @copydoc IndexInterface::lmax() */
	virtual UPos lmax() const = 0;

	/** @copydoc IndexInterface::id() */
	virtual IndexId<0> id() const = 0;
	
	/** @copydoc IndexInterface::operator*() */
	virtual DType operator*() const = 0;

	/** @copydoc IndexInterface::dim() */
	virtual SizeT dim() const = 0;

	/** @copydoc IndexInterface::range() */
	virtual RangePtr range() const = 0;

	/** @copydoc IndexInterface::stepSize() */
	virtual UPos stepSize(const IndexId<0>& id) const = 0;

	/** @copydoc IndexInterface::stringMeta() */
	virtual String stringMeta() const = 0;

	/** @copydoc IndexInterface::meta() */
	virtual DType meta() const = 0;

	/** @copydoc IndexInterface::at() */
	virtual XIndexBase& at(const DType& meta) = 0;

	/** @copydoc IndexInterface::prange() */
	virtual RangePtr prange(const XIndexPtr& last) const = 0;

	/** @copydoc IndexInterface::deepFormat() */
	virtual Vector<SizeT> deepFormat() const = 0;

	/** @copydoc IndexInterface::deepMax() */
	virtual Vector<SizeT> deepMax() const = 0;

	/** @copydoc IndexInterface::reformat() */
	virtual XIndexBase& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) = 0;

	/** @copydoc IndexInterface::ifor() */
	virtual DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const = 0;

	/** @copydoc IndexInterface::formatIsTrivial() */
	virtual bool formatIsTrivial() const = 0;
    };

    /** ****
	Index Wrapper.
	@tparam Index Type of index to be wrapped.
	@tparam Meta Meta data type of wrapped index.
     */
    template <class Index, typename Meta>
    class XIndex : public XIndexBase
    {
    public:

	DEFAULT_C(XIndex); /** < default constructor. */

	/** Copy constructor.
	    No default: have to copy objects in shared ptr.
	 */
	XIndex(const XIndex& i);

	/** Move constructor.
	 */
	XIndex(XIndex&& i);

	/** Copy assignment.
	    No default: have to copy objects in shared ptr.
	 */
	XIndex& operator=(const XIndex& i);

	/** Move assignment.
	 */
	XIndex& operator=(XIndex&& i);

	/** Construct.
	    @param i Pointer to index to be wrapped.
	 */
	XIndex(const IndexPtr<Index,Meta>& i);

	/** Construct.
	    @param i Index to be wrapped.
	 */
	XIndex(const IndexInterface<Index,Meta>& i);

	virtual XIndexPtr copy() const override final;
	virtual SizeT pos() const override final;

	virtual XIndex& operator=(SizeT lexpos) override final;
	virtual XIndex& operator++() override final;
	virtual XIndex& operator--() override final;
	virtual XIndexPtr operator+(Int n) const override final;
	virtual XIndexPtr operator-(Int n) const override final;
	virtual SizeT operator-(const XIndexBase& i) const override final;
	virtual XIndex& operator+=(Int n) override final;
	virtual XIndex& operator-=(Int n) override final;

	virtual SizeT lex() const override final;
	virtual UPos pmax() const override final;
	virtual UPos lmax() const override final;
	virtual IndexId<0> id() const override final;
	
	virtual DType operator*() const override final;

	virtual SizeT dim() const override final;
	virtual RangePtr range() const override final;
	virtual UPos stepSize(const IndexId<0>& id) const override final;
	virtual String stringMeta() const override final;
	virtual DType meta() const override final;
	virtual XIndexBase& at(const DType& meta) override final;
	virtual RangePtr prange(const XIndexPtr& last) const override final;
	virtual Vector<SizeT> deepFormat() const override final;
	virtual Vector<SizeT> deepMax() const override final;
	virtual XIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) override final;
	virtual DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const override final;
	virtual bool formatIsTrivial() const override final;

	/** Get underlying index instance.
	    @return Reference to index.
	 */
	Index& get();

	/** Get underlying index instance (const).
	    @return Reference to index.
	 */
	const Index& get() const;

    private:
	IndexPtr<Index,Meta> mI;

    };

    /** ****
	Specialization: has_sub for XIndexBase.
	XIndexBase can have sub-indices.
	@see has_sub.
     */
    template <>
    struct has_sub<XIndexBase>
    { static constexpr bool value = true; };

    /** Create XIndex pointer.
	@param i Index to be wrapped.
	@return Pointer to created index wrapper.
     */
    template <class Index>
    inline XIndexPtr xindexPtr(const Sptr<Index>& i);

    /** Specialization of xindexPtr().
	If input index type is already a XIndex, the corresponding pointer is just passed.
	This is to avoid unwanted chains of index wrappers.
	@param i Input index.
	@return i.
     */
    template <>
    inline XIndexPtr xindexPtr<XIndexBase>(const Sptr<XIndexBase>& i);

}

#endif
