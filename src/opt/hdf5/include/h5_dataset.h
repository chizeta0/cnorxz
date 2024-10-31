// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_dataset.h
   @brief Dataset declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_dataset_h__
#define __cxz_h5_dataset_h__

#include "h5_types.h"
#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 datasets.
	 */
	class Dataset : public ContentBase
	{
	public:
	    DEFAULT_MEMBERS(Dataset); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param name Dataset name.
		@param _parent Parent content object.
	     */
	    Dataset(const String& name, const ContentBase* _parent);

	    /** Destructor. Release all involved hdf5 ids. */
	    ~Dataset();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual Dataset& open() override final;
	    virtual Dataset& close() override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;
	    virtual bool exists() const override final;

	    /** Initalize the dataset.
		@param dataRange A potentially multi-dimensional range characterizing the dataset.
		@param type Data type id.
	     */
	    virtual Dataset& initbase(const RangePtr& dataRange, hid_t type);

	    /** Initalize the dataset.
		@param dataRange A potentially multi-dimensional range characterizing the dataset.
		@param type Data type id.
		@param data Pointer to raw data.
	     */
	    virtual Dataset& initbase(const RangePtr& dataRange, hid_t type, const void* data);

	    /** Write data into dataset.
		@param dataRange A potentially multi-dimensional range characterizing the format of the data to be written.
		@param pos Position on target.
		@param data Pointer to raw data to be writte.
	     */
	    virtual Dataset& writebase(const RangePtr& dataRange, Sptr<YIndex> pos, const void* data);
	    
	    /** Read the dataset.
		@param dest Pointer to destination.
		@param readrange Range of the destination data.
		@param beg Position within the file space.
	     */
	    virtual void readbase(void* dest, RangePtr readrange, Sptr<YIndex> beg) const;
	    
	    /** Initalize the dataset.
		@param data Array containing the dataset.
	    */
	    template <typename T>
	    Dataset& init(const ArrayBase<T>& data);

	    /** Get the data range.
		@return Pointer to the range.
	     */
	    const RangePtr& dataRange() const;
	    
	protected:
	    Vector<hsize_t> mkOff(const Sptr<YIndex>& beg) const;
	    
	    RangePtr mFileRange; /**< The range of the dataset. */
	    hid_t mType; /**< The data type identifier. */
	    hid_t mFilespace; /**< The hdf5 file space identifier. */
	    
	};

	/** ****
	    Class to handle hdf5 datasets, the value type is assumed to be known at compile time.
	    @tparam T Dataset value type.
	 */
	template <typename T>
	class SDataset : public Dataset
	{
	public:
	    DEFAULT_MEMBERS(SDataset); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param name Dataset name.
		@param _parent Parent content object.
	     */
	    SDataset(const String& name, const ContentBase* _parent);

	    /** Read the dataset.
		@return Array containing the dataset values.
	     */
	    MArray<T> read() const;

	    /** Read the dataset using range of given index.
		The index position is ignored.
		@param idx Index specifying the range type.
		@return Array containing the dataset values.
	     */
	    template <class I, typename M>
	    MArray<T> read(const IndexInterface<I,M>& idx) const;

	    /** Read a given subset of the dataset.
		The subset needs to be hypercubic.
		@param beg Index indicating the begin edge of the hypercube.
		@param end Index indicating the end edge of the hypercube (inclusive).
		@return Array containing the dataset values.
	     */
	    template <class I, typename M>
	    MArray<T> read(const IndexInterface<I,M>& beg, const IndexInterface<I,M>& end) const;

	private:

	    //template <class I, typename M>
	    //Vector<hsize_t> mkFPos(const IndexInterface<I,M>& beg) const;
	    template <class I, typename M>
	    Sptr<YIndex> toYptr(const IndexInterface<I,M>& beg) const;
	};
    }
}

#endif
