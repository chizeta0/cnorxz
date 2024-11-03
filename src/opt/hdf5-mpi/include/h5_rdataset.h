// -*- C++ -*-
/**

   @file opt/hdf5-mpi/include/h5_rdataset.h
   @brief RDataset declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_rdataset_h__
#define __cxz_h5_rdataset_h__

//#include "hdf5/h5_types.h"
//#include "hdf5/h5_content_base.h"
//#include "hdf5/h5_dataset.h"
//#include "mpi/mpi_base.h"

#include "h5_mpi_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 datasets on multiple ranks.
	 */
	class RDataset : public Dataset
	{
	public:
	    DEFAULT_MEMBERS(RDataset); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param name Dataset name.
		@param _parent Parent content object.
	     */
	    RDataset(const String& name, const ContentBase* _parent);
	    //virtual ~RDataset();

	    virtual RDataset& initbase(const RangePtr& fileRange, hid_t type) override;
	    virtual RDataset& writebase(const RangePtr& writeRange, Sptr<YIndex> pos,
					const void* data) override;
	    virtual void readbase(void* dest, RangePtr readrange, Sptr<YIndex> beg) const override;

	    /** Initalize the dataset.
		@param data Array containing the dataset.
	    */
	    template <typename T>
	    RDataset& init(const mpi::RArray<T>& data);

	private:
	    bool checkHaveParallel() const;
	};

	/** ****
	    Class to handle hdf5 datasets on multiple ranks,
	    the value type is assumed to be known at compile time.
	    @tparam T Dataset value type.
	 */
	template <typename T>
	class SRDataset : public RDataset
	{
	public:
	    DEFAULT_MEMBERS(SRDataset); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param name Dataset name.
		@param _parent Parent content object.
	     */
	    SRDataset(const String& name, const ContentBase* _parent);

	    /** Read the dataset.
		@param geom Geometry of the created array.
		@return Array containing the dataset values.
	     */
	    mpi::RArray<T> read(const RangePtr& geom) const;
	};	
    }
}
#endif
