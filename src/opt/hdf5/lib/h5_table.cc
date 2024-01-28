
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
		Vector<Arr<char,256>> fieldnames(nfields);
		Vector<char*> fieldsptr(nfields);
		for(SizeT i = 0; i != nfields; ++i){
		    fieldsptr[i] = fieldnames[i].data();
		}
		Vector<SizeT> offsets(nfields);
		Vector<SizeT> sizes(nfields);
		H5TBget_field_info(mParent->id(), mName.c_str(), fieldsptr.data(), sizes.data(),
				   offsets.data(), &mTypesize);
		Vector<FieldID> fields(nfields);
		for(SizeT i = 0; i != nfields; ++i){
		    fields[i].first = i;
		    fields[i].second = fieldsptr[i];
		}
		mFields = URangeFactory<FieldID>( std::move(fields) ).create();
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
		mId = 0;
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
	    Vector<FieldID> fields(fnames.size());
	    for(SizeT i = 0; i != fields.size(); ++i){
		fields[i].first = i;
		fields[i].second = fnames[i];
	    }
	    mFields = URangeFactory<FieldID>(fields).create();
	    return *this;
	}

	Table& Table::initTable(SizeT n, const void* data, SizeT dsize, SizeT chunk_size)
	{
	    const Int compress = 0;
	    mRecords = CRangeFactory(n).create();
	    Vector<const char*> fields(mFields->size());
	    auto fr = std::dynamic_pointer_cast<URange<FieldID>>(mFields);
	    for(auto fi = fr->begin(); fi != fr->end(); ++fi){
		fields[fi.lex()] = (*fi).second.c_str();
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

	MArray<DType> Table::read() const
	{
	    Vector<char> buf(mTypesize*mRecords->size());
	    H5TBread_table(mParent->id(), mName.c_str(), mTypesize, mOffsets.data(),
			   mSizes.data(), buf.data());
	    const hid_t dset_id = H5Dopen(mParent->id(), mName.c_str(), H5P_DEFAULT);
	    const hid_t type_id = H5Dget_type(dset_id);
	    MArray<DType> out(mRecords*mFields);
	    CIndex fi(mFields);
	    CIndex ri(mRecords);
	    const char* b = nullptr;
	    for(fi = 0; fi.lex() != mFields->size(); ++fi){
		const SizeT off = mOffsets[fi];
		const hid_t tp = H5Tget_member_type( type_id, fi.lex() );
		const hid_t tc = H5Tget_class(tp);
		const size_t ts = H5Tget_size(tp);
		//const bool sig = H5Tget_sign(tp);
		DType x;
		switch(tc){
		case H5T_INTEGER: {
		    if(ts == 4){
			for(ri = 0, b = buf.data(); ri.lex() != mRecords->size(); ++ri, b += mTypesize){
			    const Int xi = *reinterpret_cast<const Int*>(b+off);
			    out[ri*fi] = DType(xi);
			}
			break;
		    }
		    else if(ts == 8){
 			for(ri = 0, b = buf.data(); ri.lex() != mRecords->size(); ++ri, b += mTypesize){
			    const LInt xi = *reinterpret_cast<const LInt*>(b+off);
			    out[ri*fi] = DType(xi);
			}
			break;
		    }
		}
		case H5T_FLOAT: {
		    if(ts == 8){
 			for(ri = 0, b = buf.data(); ri.lex() != mRecords->size(); ++ri, b += mTypesize){
			    const Double xi = *reinterpret_cast<const Double*>(b+off);
			    out[ri*fi] = DType(xi);
			}
			break;
		    }
		}
		default:
		    CXZ_ERROR("type " << tp << " not supported");
		}
	    }
	    H5Tclose(type_id);
	    H5Dclose(dset_id);
	    return out;
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
