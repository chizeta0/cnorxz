
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

	RangePtr ContentBase::range() const
	{
	    return mRange;
	}

	hid_t ContentBase::id() const
	{
	    return mId;
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
		Vector<char> v(asize);
		H5Aread(attr, atype_id, v.data());
		o = DType( String( v.data() ) );
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
	
	Vector<DType> ContentBase::getAttributes() const
	{
	    CXZ_ASSERT(isOpen(), "tried to get attribute of closed object");
	    struct ItData
	    {
		Vector<DType> d;
		const ContentBase* _this;
	    } itdata;
	    itdata._this = this;
	    auto itop = [](hid_t loc_id, const char *attr_name,
			   const H5A_info_t* ainfo, void *op_data)
	    {
		ItData* x = reinterpret_cast<ItData*>(op_data);
		x->d.push_back( x->_this->getAttribute( String(attr_name) ) );
		return static_cast<herr_t>(0);
	    };
	    H5Aiterate(id(), H5_INDEX_NAME, H5_ITER_NATIVE, nullptr,
		       itop, &itdata);
	    return itdata.d;
	}
	
	Vector<DType> ContentBase::getRecursiveAttributes() const
	{
	    Vector<DType> out = getAttributes();
	    if(mParent){
		Vector<DType> par = mParent->getRecursiveAttributes();
		out.insert(out.begin(), par.begin(), par.end());
	    }
	    return out;
	}
    }
}
