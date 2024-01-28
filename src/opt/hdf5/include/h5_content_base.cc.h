
#ifndef __cxz_h5_content_base_cc_h__
#define __cxz_h5_content_base_cc_h__

#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	namespace
	{

	    template <typename T>
	    struct CreateAttribute
	    {
		static inline herr_t write(hid_t id, const String& name, const T& v)
		{
		    CXZ_ERROR("type " << typeid(v).name() << " not supported (name "
			      << name << ", id = " << id << ")");
		    return 0;
		}
	    };

	    template <>
	    struct CreateAttribute<Int>
	    {
		static inline herr_t write(hid_t id, const String& name, const Int& v)
		{
		    const hsize_t dim = 1;
		    const hid_t type_id =  H5Tarray_create(H5T_NATIVE_INT, 1, &dim);
		    const hid_t space_id = H5Screate(H5S_SCALAR);
		    const hid_t attr_id = H5Acreate(id, name.c_str(), type_id, space_id, H5P_DEFAULT,
						    H5P_DEFAULT);
		    const herr_t err = H5Awrite(attr_id, type_id, &v);
		    H5Aclose(attr_id);
		    H5Sclose(space_id);
		    H5Tclose(type_id);
		    return err;
		}
	    };

	    template <>
	    struct CreateAttribute<Double>
	    {
		static inline herr_t write(hid_t id, const String& name, const Double& v)
		{
		    const hsize_t dim = 1;
		    const hid_t type_id =  H5Tarray_create(H5T_NATIVE_DOUBLE, 1, &dim);
		    const hid_t space_id = H5Screate(H5S_SCALAR);
		    const hid_t attr_id = H5Acreate(id, name.c_str(), type_id, space_id, H5P_DEFAULT,
						    H5P_DEFAULT);
		    const herr_t err = H5Awrite(attr_id, type_id, &v);
		    H5Aclose(attr_id);
		    H5Sclose(space_id);
		    H5Tclose(type_id);
		    return err;
		}
	    };

	    template <>
	    struct CreateAttribute<String>
	    {
		static inline herr_t write(hid_t id, const String& name, const String& v)
		{
		    const hsize_t len = v.size();
		    const hid_t type_id =  H5Tcreate(H5T_STRING, len);
		    const hid_t space_id = H5Screate(H5S_SCALAR);
		    const hid_t attr_id = H5Acreate(id, name.c_str(), type_id, space_id, H5P_DEFAULT,
						    H5P_DEFAULT);
		    const herr_t err = H5Awrite(attr_id, type_id, v.c_str());
		    H5Aclose(attr_id);
		    H5Sclose(space_id);
		    H5Tclose(type_id);
		    return err;
		}
	    };


	    template <typename T>
	    inline herr_t writeAttr(hid_t id, const String& name, const T& v)
	    {
		return CreateAttribute<T>::write(id, name, v);
	    }
	}
	
	template <typename T>
	ContentBase& ContentBase::addAttribute(const String& name, const T& value)
	{
	    const herr_t err = writeAttr(mId, name, value);
	    CXZ_ASSERT(err >= 0, "error while writing attribute " << name);
	    return *this;
	}

    }
}

#endif
