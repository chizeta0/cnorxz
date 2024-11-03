// -*- C++ -*-
/**

   @file opt/hdf5-mpi/lib/h5_rdataset.cc
   @brief RDataset implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#include "h5_rdataset.h"
#include "h5_rfile.h"

namespace CNORXZ
{
    namespace hdf5
    {
	RDataset::RDataset(const String& name, const ContentBase* _parent) :
	    Dataset(name, _parent)
	{
	    CXZ_ASSERT( checkHaveParallel(), "tried to open dataset in parallel mode while file"
			<< parent()->filename() << " was opened in serial mode");
	}

	RDataset& RDataset::initbase(const RangePtr& fileRange, hid_t type)
	{
	    RangePtr fr = fileRange;
	    if(fr->stype() == "R"){
		const RangePtr local = fr->sub(1);
		const RangePtr geom = fr->sub(0);
		const SizeT ndims = local->dim();
		//CXZ_ASSERT(ndims == geom->dim(), "")
		Vector<RangePtr> rs(ndims);
		for(SizeT i = 0; i != ndims; ++i){
		    const SizeT ext = local->savesub(i)->size()*geom->savesub(i)->size();
		    rs[i] = CRangeFactory(ext).create();
		}
		fr = yrange(rs);
	    }
	    Dataset::initbase(fr, type);
	    MPI_Barrier(MPI_COMM_WORLD);
	    return *this;
	}
	    
	RDataset& RDataset::writebase(const RangePtr& writeRange, Sptr<YIndex> pos, const void* data)
	{
	    //bool todo = true;
	    RangePtr dr = writeRange;
	    bool parallel = dr->stype() == "R";
	    if(parallel){
		dr = writeRange->sub(1);
	    }
	    CXZ_ASSERT(dr->dim() == mFileRange->dim(), "dimension of data range ("
		       << dr->dim() << ") different from dimension of file range ("
		       << mFileRange->dim() << ")");
	    Vector<hsize_t> offset(mFileRange->dim());
	    if(parallel){
		mpi::RIndex<YIndex,YIndex> idx(writeRange);
		idx.localize();
		const SizeT rat = mpi::getNumRanks() / idx.rankI()->lmax().val();
		assert(rat == 1); // for now...
		assert(mpi::getRankNumber() == idx.rankI()->lex());
		for(SizeT i = 0; i != offset.size(); ++i){
		    offset[i] = idx.rankI()->pack().get(i)->lex() * dr->savesub(i)->size();
		}
	    }
	    if(pos){
		CXZ_ASSERT(pos->range()->dim() == mFileRange->dim(), "dimension of position index ("
			   << pos->range()->dim() << ") different from dimension of file range ("
			   << mFileRange->dim() << ")");
		for(SizeT i = 0; i != offset.size(); ++i){
		    offset[i] += pos->pack().get(i)->lex();
		}
	    }
	    
	    Vector<hsize_t> dims(dr->dim());
	    for(SizeT i = 0; i != dims.size(); ++i){
		dims[i] = dr->sub(i)->size();
	    }
	    H5Sselect_hyperslab(mFilespace, H5S_SELECT_SET, offset.data(), NULL, dims.data(), NULL);
	    const hid_t memspace = H5Screate_simple(dims.size(), dims.data(), NULL);
	    const hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
	    H5Pset_dxpl_mpio(xfer_plist_id, H5FD_MPIO_COLLECTIVE);
	    H5Dwrite(mId, mType, memspace, mFilespace, xfer_plist_id, data);
	    H5Pclose(xfer_plist_id);
	    H5Sclose(memspace);
	    MPI_Barrier(MPI_COMM_WORLD);
	    return *this;
	}

	void RDataset::readbase(void* dest, RangePtr readRange, Sptr<YIndex> beg) const
	{
	    RangePtr dr = readRange;
	    bool parallel = dr->stype() == "R";
	    if(not dr){
		dr = mFileRange;
	    }
	    if(parallel){
		dr = readRange->sub(1);
	    }
	    CXZ_ASSERT(dr->dim() == mFileRange->dim(), "dimension of data range ("
		       << dr->dim() << ") different from dimension of file range ("
		       << mFileRange->dim() << ")");
	    Vector<hsize_t> offset(mFileRange->dim());
	    if(parallel){
		mpi::RIndex<YIndex,YIndex> idx(readRange);
		idx.localize();
		const SizeT rat = mpi::getNumRanks() / idx.rankI()->lmax().val();
		assert(rat == 1); // for now...
		assert(mpi::getRankNumber() == idx.rankI()->lex());
		for(SizeT i = 0; i != offset.size(); ++i){
		    offset[i] = idx.rankI()->pack().get(i)->lex() * dr->savesub(i)->size();
		}
	    }
	    if(beg){
		CXZ_ASSERT(beg->range()->dim() == mFileRange->dim(), "dimension of position index ("
			   << beg->range()->dim() << ") different from dimension of file range ("
			   << mFileRange->dim() << ")");
		for(SizeT i = 0; i != offset.size(); ++i){
		    offset[i] += beg->pack().get(i)->lex();
		}
	    }

	    Vector<hsize_t> dims(mFileRange->dim());
	    for(SizeT i = 0; i != dims.size(); ++i){
		dims[i] = dr->sub(i)->size();
	    }
	    H5Sselect_hyperslab(mFilespace, H5S_SELECT_SET, offset.data(), NULL, dims.data(), NULL);
	    const hid_t mem_space_id = H5Screate_simple(static_cast<hsize_t>(dims.size()),
							dims.data(), nullptr);
	    const hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
	    H5Pset_dxpl_mpio(xfer_plist_id, H5FD_MPIO_COLLECTIVE);
	    //MArray<T> out(readRange);
	    const herr_t err = H5Dread(mId, mType, mem_space_id, mFilespace, xfer_plist_id, dest);
	    CXZ_ASSERT(err >= 0, "error while reading dataset '" << mName
		       << "', errorcode :" << err);
	    H5Pclose(xfer_plist_id);
	    H5Sclose(mem_space_id);
	    MPI_Barrier(MPI_COMM_WORLD);
	}

	bool RDataset::checkHaveParallel() const
	{
	    const ContentBase* p = parent();
	    while(p->type() != ContentType::FILE and p != nullptr){
		p = p->parent();
	    }
	    if(not p){
		return false;
	    }
	    const RFile* fp = dynamic_cast<const RFile*>(p);
	    if(not fp){
		return false;
	    }
	    return true;
	}
    }
}
