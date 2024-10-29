// -*- C++ -*-
/**

   @file opt/hdf5/lib/h5_dataset.cc
   @brief Dataset implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#include "h5_dataset.h"

namespace CNORXZ
{
    namespace hdf5
    {
	Dataset::Dataset(const String& name, const ContentBase* _parent) :
	    ContentBase(name, _parent)
	{
	    if(exists()){
		open();
	    }
	}
	
	Dataset::~Dataset()
	{
	    this->close();
	}

	ContentType Dataset::type() const
	{
	    return ContentType::DSET;
	}
	
	bool Dataset::ro() const
	{
	    return mParent->ro();
	}
	
	Dataset& Dataset::open()
	{
	    if(mId == 0 and exists()){
		mId = H5Dopen(mParent->id(), mName.c_str(), H5P_DEFAULT);
		mType = H5Dget_type(mId);

		mFilespace = H5Dget_space(mId);
		SizeT ndims = H5Sget_simple_extent_ndims(mFilespace);
		Vector<hsize_t> dims(ndims);
		H5Sget_simple_extent_dims(mFilespace, dims.data(), nullptr);

		Vector<RangePtr> rs(ndims);
		for(SizeT i = 0; i != ndims; ++i){
		    rs[i] = CRangeFactory(dims[i]).create();
		}
		mDataRange = yrange(rs);
	    }
	    return *this;
	}
	
	Dataset& Dataset::close()
	{
	    if(mId != 0){
		H5Sclose(mFilespace);
		H5Tclose(mType);
		H5Dclose(mId);
		mId = 0;
	    }
	    return *this;
	}
	
	String Dataset::path() const
	{
	    return mParent->path() + "/" + mName;
	}
	
	String Dataset::filename() const
	{
	    return mParent->filename();
	}

	bool Dataset::exists() const
	{
	    return H5Lexists(mParent->id(), mName.c_str(), H5P_DEFAULT) > 0;
	}

	Dataset& Dataset::init(const RangePtr& dataRange, hid_t type)
	{
	    CXZ_ASSERT(not isOpen(), "tried to initialize dataset that is already extisting");
	    mDataRange = dataRange;
	    const H5T_class_t tc = H5Tget_class(type);
	    CXZ_ASSERT(tc != H5T_NO_CLASS, "id does not correspond to a data type"); // (did not found anythng better to check if type id is valid)...
	    const hid_t dcpl_id = H5Pcreate(H5P_DATASET_CREATE);
	    // TODO: all sub-ranges explicity!!!:
	    const SizeT ndim = dataRange->dim();
	    Vector<hsize_t> exts(ndim);
	    for(SizeT i = 0; i != ndim; ++i){
		exts[i] = static_cast<hsize_t>( dataRange->sub(i)->size() );
	    }
	    mFilespace = H5Screate_simple(ndim, exts.data(), NULL);
	    mType = type;
	    mId = H5Dcreate(mParent->id(), mName.c_str(), mType, mFilespace,
			    H5P_DEFAULT, dcpl_id, H5P_DEFAULT);
	    H5Pclose(dcpl_id);
	    return *this;
	}

	Dataset& Dataset::init(const RangePtr& dataRange, hid_t type, const void* data)
	{
	    init(dataRange, type);
	    Vector<hsize_t> dims(mDataRange->dim());
	    for(SizeT i = 0; i != dims.size(); ++i){
		dims[i] = mDataRange->sub(i)->size();
	    }
	    const hid_t memspace = H5Screate_simple(dims.size(), dims.data(), NULL);
	    H5Dwrite(mId, mType, memspace, mFilespace, H5P_DEFAULT, data);
	    H5Sclose(memspace);
	    return *this;
	}

	void Dataset::readbase(void* dest, RangePtr readrange, Sptr<YIndex> beg) const
	{
	    // TODO: Check if readrange is compatible with mDataRange!!!
	    if(not readrange){
		readrange = mDataRange;
	    }
	    Vector<hsize_t> dims(readrange->dim());
	    for(SizeT i = 0; i != dims.size(); ++i){
		dims[i] = readrange->sub(i)->size();
	    }
	    if(beg){
		const Vector<hsize_t> fpos = mkOff(beg);
		H5Sselect_hyperslab(mFilespace, H5S_SELECT_SET, fpos.data(), NULL, dims.data(), NULL);
	    }
	    const hid_t mem_space_id = H5Screate_simple(static_cast<hsize_t>(dims.size()),
							dims.data(), nullptr);
	    const hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
	    //MArray<T> out(readrange);
	    const herr_t err = H5Dread(mId, mType, mem_space_id, mFilespace, xfer_plist_id, dest);
	    CXZ_ASSERT(err >= 0, "error while reading dataset '" << mName
		       << "', errorcode :" << err);
	    H5Pclose(xfer_plist_id);
	    H5Sclose(mem_space_id);
	}

	const RangePtr& Dataset::dataRange() const
	{
	    return mDataRange;
	}
	
        Vector<hsize_t> Dataset::mkOff(const Sptr<YIndex>& beg) const
	{
	    Vector<hsize_t> off(beg->dim());
	    for(SizeT i = 0; i != beg->dim(); ++i){
		off[i] = beg->pack().get(i)->lex();
	    }
	    return off;
	}
	
    }
}
