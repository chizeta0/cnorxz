
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

	const RangePtr& Dataset::dataRange() const
	{
	    return mDataRange;
	}
	
    }
}
