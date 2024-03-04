// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.h
   @brief RRange and RIndex declaration

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_h__
#define __cxz_mpi_rrange_h__

namespace CNORXZ
{
    /** ****
	Specific index for RRanges.
     */
    class RIndex : public IndexInterface<RIndex<Vector<SizeT>>>
    {
    public:
	typedef IndexInterface<RIndex<Vector<SizeT>>> IB;
	typedef RRange RangeType;
	typedef Vector<SizeT> MetaType;

    private:
	Sptr<RRange> mRange;
	Vector<Sptr<CIndex>> mIs; // -> CPack!!!
	YFormat mLexFormat; // = mFormat (rank geometry is fixed and unique)
	UPos mPMax = 0; // = mLMax (same reason)
    }

    /** ****
	Specialization: RIndex is a multi-index.
	@see index_is_multi
     */
    template <>
    struct index_is_multi<RIndex>
    { static constexpr bool value = true; };

    /** ****
	Specialization: RIndex has sub-indices.
	@see has_sub
     */
    template <>
    struct has_sub<RIndex>
    { static constexpr bool value = true; };

    /** ****
	Specific factory for RRange.
     */
    class RRangeFactory : public RangeFactoryBase
    {
    public:
	/** Construct and setup factory.
	    @param geom SizeT vector specifying the rank geometry
	 */
	RRangeFactory(const Vector<SizeT>& geom);

    private:
	RRangeFactory() = default;
	virtual void make() override final;

	Vector<SizeT> mGeom;
    };

    /** ****
	Multi-dimensional range specifying the rank coordinate system
     */
    class RRange : public RangeInterface<RRange>
    {
    public:
	typedef RangeBase RB;
	typedef RIndex IndexType;

	friend RRangeFactory;
	
	virtual RangePtr sub(SizeT i) const override final;
	virtual MArray<RangePtr> sub() const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

    private:

	RRange() = default;
	RRange(const RRange& a) = delete;
	RRange(const Vector<RangePtr>& rvec);

	Vector<RangePtr> mRVec;

	virtual Vector<Uuid> key() const override final;
    };
}

#endif
