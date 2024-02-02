
#include <hdf5_hl.h>
#include "h5_table.h"

namespace CNORXZ
{
    namespace hdf5
    {
	Table::Table(const String& name, const ContentBase* _parent) :
	    ContentBase(name, _parent)
	{
	    if(exists()){
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
		mkTypes();
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

	bool Table::exists() const
	{
	    htri_t x = H5Lexists(mParent->id(), mName.c_str(), H5P_DEFAULT);
	    VCHECK(x);
	    return x;
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
	    mTypesize = dsize;
	    const herr_t err = H5TBmake_table
		(mName.c_str(), mParent->id(), mName.c_str(), mFields->size(), mRecords->size(), mTypesize,
		 fields.data(), mOffsets.data(), mTypes.data(), chunk_size, NULL, compress, data);
	    CXZ_ASSERT(err >= 0, "error while initialzing table: error code = " << err);
	    return *this;
	}
	
	Table& Table::appendRecords(SizeT n, const void* data)
	{
	    mRecords = mRecords->extend( CRangeFactory(n).create() );
	    const herr_t err = H5TBappend_records(mParent->id(), mName.c_str(), n, mTypesize,
						  mOffsets.data(), mSizes.data(), data);
	    CXZ_ASSERT(err >= 0, "error while appending record to table: error code = " << err);
	    return *this;
	}
	
	Table& Table::readRecords(SizeT pos, SizeT n, char* data)
	{
	    H5TBread_records(mParent->id(), mName.c_str(), pos, n, mTypesize, mOffsets.data(),
			     mSizes.data(), data);
	    return *this;
	}
	
	MArray<DType> Table::readRecord(SizeT pos) const
	{
	    Vector<char> buf(mTypesize);
	    H5TBread_records(mParent->id(), mName.c_str(), pos, 1, mTypesize, mOffsets.data(),
			     mSizes.data(), buf.data());
	    MArray<DType> out(mFields);
	    auto fi = std::make_shared<CIndex>(mFields);
	    
	    out(fi) = operation( [&](const SizeT& off, const std::function<DType(const char*)>& f){
		return f( buf.data() + off );
	    }, mOffsets(fi), mInterpret(fi) );

	    return out;
	}
	
	MArray<DType> Table::read() const
	{
	    Vector<char> buf(mTypesize*mRecords->size());
	    H5TBread_table(mParent->id(), mName.c_str(), mTypesize, mOffsets.data(),
			   mSizes.data(), buf.data());
	    MArray<DType> out(mRecords*mFields);
	    auto fi = std::make_shared<CIndex>(mFields);
	    auto ri = std::make_shared<CIndex>(mRecords);
	    
	    out(ri*fi) = operation( [&](const SizeT& off, const std::function<DType(const char*)>& f, const SizeT r){
		return f( buf.data() + r*mTypesize + off );
	    }, mOffsets(fi), mInterpret(fi), xpr(ri) );

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

	void Table::mkTypes()
	{
	    mTypes = MArray<hid_t>(mFields);
	    auto i = std::make_shared<CIndex>(mFields);
	    mTypes(i) = operation( [&](const SizeT n){ return H5Tget_member_type( mType, n ); } , xpr(i) );
	    mInterpret = MArray<std::function<DType(const char*)>>(mFields);
	    mInterpret(i) = operation( [](const hid_t tid){
		const hid_t tc = H5Tget_class(tid);
		const size_t ts = H5Tget_size(tid);
		const bool sig = H5Tget_sign(tid);
		std::function<DType(const char*)> of;
		switch(tc){
		case H5T_INTEGER:{
		    switch(ts){
		    case 1:{ // 8-bit
			of = sig ?
			    [](const char* d){ const int8_t x = *reinterpret_cast<const int8_t*>(d); return DType(static_cast<Int>(x)); } :
			    [](const char* d){ const uint8_t x = *reinterpret_cast<const uint8_t*>(d); return DType(static_cast<SizeT>(x)); };
			break;
		    }
		    case 2:{ // 16-bit
			of = sig ?
			    [](const char* d){ const int16_t x = *reinterpret_cast<const int16_t*>(d); return DType(static_cast<Int>(x)); } :
			    [](const char* d){ const uint16_t x = *reinterpret_cast<const uint16_t*>(d); return DType(static_cast<SizeT>(x)); };
			break;
		    }
		    case 4:{ // 32-bit
			of = sig ?
			    [](const char* d){ const int32_t x = *reinterpret_cast<const int32_t*>(d); return DType(static_cast<Int>(x)); } :
			    [](const char* d){ const uint32_t x = *reinterpret_cast<const uint32_t*>(d); return DType(static_cast<SizeT>(x)); };
			break;
		    }
		    case 8:{ // 64-bit
			of = sig ?
			    [](const char* d){ const int64_t x = *reinterpret_cast<const int64_t*>(d); return DType(static_cast<LInt>(x)); } :
			    [](const char* d){ const uint64_t x = *reinterpret_cast<const uint64_t*>(d); return DType(static_cast<SizeT>(x)); };
			break;
		    }
		    default:
			CXZ_ERROR("got integer of weird size: " << ts);
		    }
		    break;
		}
		case H5T_FLOAT:{
		    if(ts == 4) {
			of = [](const char* d){ const float x = *reinterpret_cast<const float*>(d); return DType(static_cast<Double>(x)); };
		    }
		    else {
			CXZ_ASSERT(ts == 8, "got float of weird size: " << ts);
			of = [](const char* d){ const double x = *reinterpret_cast<const double*>(d); return DType(static_cast<Double>(x)); };
		    }
		    break;
		}
		default:
		    CXZ_ERROR("got unsupported type");
		};
		return of;
	    }, mTypes(i) );
	}
    }
}
