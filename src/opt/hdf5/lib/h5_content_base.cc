// -*- C++ -*-
/**

   @file opt/hdf5/lib/h5_content_base.cc
   @brief Content base member function implementation.

   Implementation of all non-virtual member functions of ContentBase.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	ContentBase::ContentBase(const String& _name, const ContentBase* _parent) :
	    mName(_name), mParent(_parent)
	{}

	const String& ContentBase::name() const
	{
	    return mName;
	}

	const ContentBase* ContentBase::parent() const
	{
	    return mParent;
	}

	hid_t ContentBase::id() const
	{
	    return mId;
	}

	bool isOpen() const
	{
	    return mId != 0;
	}

	DType ContentBase::getAttribute(const String& name) const
	{
	    CXZ_ASSERT(attributeExists(name), "no attribute with name '" << name
		       << "' in object " << mName);
	    const hid_t attr = H5Aopen(mId, name.c_str(), H5P_DEFAULT);
	    CXZ_ASSERT(attr != H5I_INVALID_HID, "error while reading attribute " << name);
	    const hid_t atype_id = H5Aget_type(attr);
	    CXZ_ASSERT(atype_id != H5I_INVALID_HID,
		       "error while determining type of attribute " << name);

	    const H5T_class_t tc = H5Tget_class(atype_id);
	    DType o;
	    switch(tc){
	    case H5T_INTEGER: {
		Int v;
		H5Aread(attr, atype_id, &v);
		o = DType(v);
		break;
	    }
	    case H5T_FLOAT: {
		Double v;
		H5Aread(attr, atype_id, &v);
		o = DType(v);
		break;
	    }
	    case H5T_STRING: {
		const SizeT asize = H5Tget_size(atype_id);
		Vector<char> v(asize+1);
		H5Aread(attr, atype_id, v.data());
		o = DType( String( v.data() ) );
		break;
	    }
	    case H5T_ARRAY: {
		const int ndims = H5Tget_array_ndims(atype_id);
		CXZ_ASSERT(ndims, "array of dimension " << ndims << " not supported");
		hsize_t dims;
		H5Tget_array_dims(atype_id, &dims);
		const hid_t att_id = H5Tget_super(atype_id);
		const H5T_class_t atc = H5Tget_class(att_id);
		switch(atc){
		case H5T_INTEGER: {
		    Vector<Int> v(dims);
		    H5Aread(attr, atype_id, v.data());
		    o = DType(v);
		    break;
		}
		case H5T_FLOAT: {
		    Vector<Double> v(dims);
		    H5Aread(attr, atype_id, v.data());
		    o = DType(v);
		    break;
		}
		default:
		    CXZ_ERROR("attribute type id " << atype_id << " not supported");
		}
		H5Tclose(att_id);
		break;
	    }
	    default:
		CXZ_ERROR("attribute type id " << atype_id << " not supported");
	    }
	    
	    H5Tclose(atype_id);
	    H5Aclose(attr);
	    return o;
	}
	
	bool ContentBase::attributeExists(const String& name) const
	{
	    CXZ_ASSERT(isOpen(), "tried to get attribute of closed object");
	    const htri_t ret = H5Aexists(mId, name.c_str());
	    CXZ_ASSERT(ret > 0, "error while reading attribute " << name);
	    return ret;
	}
	
	std::map<String,DType> ContentBase::getAttributes() const
	{
	    CXZ_ASSERT(isOpen(), "tried to get attribute of closed object");
	    struct ItData
	    {
		std::map<String,DType> d;
		const ContentBase* _this;
	    } itdata;
	    itdata._this = this;
	    auto itop = [](hid_t loc_id, const char *attr_name,
			   const H5A_info_t* ainfo, void *op_data)
	    {
		ItData* x = reinterpret_cast<ItData*>(op_data);
		const String n = attr_name;
		x->d[n] = x->_this->getAttribute( String(attr_name) );
		return static_cast<herr_t>(0);
	    };
	    H5Aiterate(id(), H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
		       itop, &itdata);
	    return itdata.d;
	}
	
	std::map<String,DType> ContentBase::getRecursiveAttributes() const
	{
	    std::map<String,DType> out = getAttributes();
	    if(mParent){
		std::map<String,DType> par = mParent->getRecursiveAttributes();
		out.insert(par.begin(), par.end());
	    }
	    return out;
	}
    }
}
