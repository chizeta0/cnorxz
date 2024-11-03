
#ifndef __cxz_hdf5_mpi_diag_utils_h__
#define __cxz_hdf5_mpi_diag_utils_h__

inline String getStringObjType(hid_t obj_id)
{
    const H5I_type_t t = H5Iget_type(obj_id);
    switch(t){
    case H5I_UNINIT:
	return "uninitialized";
    case H5I_BADID:
	return "bad";
    case H5I_FILE:
	return "file";
    case H5I_GROUP:
	return "group";
    case H5I_DATATYPE:
	return "datatype";
    case H5I_DATASPACE:
	return "dataspace";
    case H5I_DATASET:
	return "dataset";
    case H5I_MAP:
	return "map";
    case H5I_ATTR:
	return "attr";
    case H5I_VFL:
	return "vfl";
    case H5I_VOL:
	return "vol";
    case H5I_GENPROP_CLS:
	return "gen prop list class";
    case H5I_GENPROP_LST:
	return "gen prop list";
    case H5I_ERROR_CLASS:
	return "err class";
    case H5I_ERROR_MSG:
	return "err msg";
    case H5I_ERROR_STACK:
	return "err stack";
    case H5I_SPACE_SEL_ITER:
	return "space sel iter";
    case H5I_EVENTSET:
	return "eventset";
    case H5I_NTYPES:
	return "ntypes";
    default:
	return "unknonw";
    }
}
    
void list_objects(hid_t file_id, const String& desc)
{
    constexpr SizeT maxnamesize = 256;
    MPI_Barrier(MPI_COMM_WORLD);
    if(getRankNumber() == 0){
	std::cout << " ======= " << desc << " ======= " << std::endl;
    }
    const ssize_t num = H5Fget_obj_count(file_id, H5F_OBJ_ALL);
    CXZ_ASSERT(num >= 0, "error while counting objects: " << num);
    Vector<hid_t> list(num);
    H5Fget_obj_ids(file_id, H5F_OBJ_ALL, num, list.data());
    const SizeT nranks = getNumRanks();
    for(SizeT n = 0; n != nranks; ++n){
	MPI_Barrier(MPI_COMM_WORLD);
	if(n == getRankNumber()){
	    std::cout << "have " << num << " objects on rank " << n << ":" << std::endl;
	    for(SizeT i = 0; i != static_cast<SizeT>(num); ++i){
		char iname[maxnamesize];
		H5Iget_name(list[i], iname, maxnamesize);
		const String itype = getStringObjType(list[i]);
		std::cout << " - " << list[i] << " [" << itype << "]: " << iname << std::endl;
	    }
	}
	MPI_Barrier(MPI_COMM_WORLD);
    }
    if(getRankNumber() == 0){
	std::cout << " ==================== " << std::endl;
	std::cout << std::endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

#endif