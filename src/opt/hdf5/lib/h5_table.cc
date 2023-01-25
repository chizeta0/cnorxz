
#include <hdf5_hl.h>
#include "h5_table.h"

namespace CNORXZ
{
    namespace hdf5
    {
	Table::Table(const String& name, const ContentBase* _parent) :
	    ContentBase(name, _parent)
	{
	    if(H5Lexists(mParent->id(), mName.c_str(), H5P_DEFAULT)){
		hsize_t nfields = 0;
		hsize_t nrecords = 0;
		H5TBget_table_info(mParent->id(), mName.c_str(), &nfields, &nrecords);
		mRecords = CRangeFactory( nrecords ).create();
		Vector<char*> fieldsptr(nfields);
		Vector<SizeT> offsets(nfields);
		Vector<SizeT> sizes(nfields);
		SizeT typesize = 0;
		H5TBget_field_info(mParent->id(), mName.c_str(), fieldsptr.data(), sizes.data(),
				   offsets.data(), &typesize);
		Vector<String> fields(nfields);
		for(SizeT i = 0; i != nfields; ++i){
		    fields[i] = fieldsptr[i];
		}
		mFields = URangeFactory<String>( std::move(fields) ).create();
		mSizes = MArray<SizeT>(mFields, std::move(sizes));
		mOffsets = MArray<SizeT>(mFields, std::move(offsets));
		this->open();
		const SizeT n = H5Tget_nmembers(mType);
		CXZ_ASSERT(n == mFields->size(),
			   "number of dataset members does not match number of fields: "
			   << n << " vs " << mFields->size());
		Vector<hid_t> types(n);
		for(SizeT i = 0; i != n; ++i){
		    types[i] = H5Tget_member_class( mType, i );
		}
		mTypes = MArray<hid_t>(mFields, std::move(types));
		this->close();
	    }
	}

	Table::~Table()
	{
	    this->close();
	}

	ContentType Table::type() const
	{
	    return ContentType::TABLE;
	}
	
	bool Table::ro() const
	{
	    return mParent->ro();
	}
	
	Table& Table::open()
	{
	    if(mId == 0){
		mId = H5Dopen(mParent->id(), mName.c_str(), H5P_DEFAULT);
		mType = H5Dget_type(mId);
	    }
	    return *this;
	}
	
	Table& Table::close()
	{
	    if(mId != 0){
		H5Tclose(mType);
		H5Dclose(mId);
	    }
	    return *this;
	}
	
	String Table::path() const
	{
	    return mParent->path() + "/" + mName;
	}
	
	String Table::filename() const
	{
	    return mParent->filename();
	}

	Table& Table::initFieldNames(const Vector<String>& fnames)
	{
	    CXZ_ASSERT(mFields == nullptr, "fields already initialized");
	    mFields = URangeFactory<String>(fnames).create();
	    return *this;
	}

	Table& Table::initTable(SizeT n, const void* data, SizeT dsize, SizeT chunk_size)
	{
	    const Int compress = 0;
	    mRecords = CRangeFactory(n).create();
	    Vector<const char*> fields(mFields->size());
	    auto fr = std::dynamic_pointer_cast<URange<String>>(mFields);
	    for(auto fi = fr->begin(); fi != fr->end(); ++fi){
		fields[fi.lex()] = (*fi).c_str();
	    }
	    const herr_t err = H5TBmake_table
		(mName.c_str(), mParent->id(), mName.c_str(), mFields->size(), mRecords->size(), dsize,
		 fields.data(), mOffsets.data(), mTypes.data(), chunk_size, NULL, compress, data);
	    CXZ_ASSERT(err >= 0, "error while initialzing table: error code = " << err);
	    return *this;
	}
	
	Table& Table::appendRecord(SizeT n, const void* data, SizeT dsize)
	{
	    mRecords = mRecords->extend( CRangeFactory(1).create() );
	    const herr_t err = H5TBappend_records(mParent->id(), mName.c_str(), n, dsize,
						  mOffsets.data(), mSizes.data(), data);
	    CXZ_ASSERT(err >= 0, "error while appending record to table: error code = " << err);
	    return *this;
	}
	
	Table& Table::readRecord(SizeT pos, SizeT n, char* data)
	{
	    CXZ_ERROR("not implemented!!!");
	    return *this;
	}
	
	const RangePtr& Table::fields() const
	{
	    return mFields;
	}
	
	const RangePtr& Table::records() const
	{
	    return mRecords;
	}

    }
}
