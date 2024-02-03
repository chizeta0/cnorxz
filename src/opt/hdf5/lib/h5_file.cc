// -*- C++ -*-
/**

   @file opt/hdf5/lib/h5_file.cc
   @brief Group implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#include "h5_file.h"
#include <fstream>

namespace CNORXZ
{
    namespace hdf5
    {
	File::File(const String& fname, bool _ro) :
	    Group(fname, nullptr),
	    mRo(_ro)
	{}

	File::~File()
	{
	    this->close();
	}

	ContentType File::type() const
	{
	    return ContentType::FILE;
	}
	
	bool File::ro() const
	{
	    return mRo;
	}
	
	File& File::open()
	{
	    if(isOpen()){
		return *this;
	    }
	    Int ex = this->exists();
	    const String fn = this->filename();
	    CXZ_ASSERT( ishdf5(), "tried to open non-h5 file '" << fn << "'" );
	    if(mRo){
		CXZ_ASSERT( ex == 1, "could not open file as read-only: '"
			    << fn << "' does not exist'");
		mId = H5Fopen( fn.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT );
	    }
	    else {
		if(ex == 1){
		    mId = H5Fopen( fn.c_str(), H5F_ACC_RDWR, H5P_DEFAULT );
		}
		else {
		    mId = H5Fcreate( fn.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
		}
	    }
	    CXZ_ASSERT( mId > 0, "error while opening file '" << fn << "'" );
	    this->mkCont();
	    return *this;
	}
	
	File& File::close()
	{
	    if(mCont.range() != nullptr){
		for(auto& x: mCont){
		    if(x != nullptr){
			x->close();
		    }
		}
	    }
	    if(mId != 0){
		H5Fclose(mId);
	    }
	    mId = 0;
	    return *this;
	}
	
	String File::path() const
	{
	    return "";
	}
	
	String File::filename() const
	{
	    return name();
	}

	bool File::exists() const
	{
	    bool ex = false;
	    std::ifstream fs(this->filename().c_str(), std::ios_base::binary);
	    if(fs.good()){
		ex = true; // file exists
	    }
	    fs.close();
	    return ex;
	}

	bool File::ishdf5() const
	{
	    if(exists()){
		if(H5Fis_hdf5(this->filename().c_str()) <= 0){
		    return false;
		}
	    }
	    return true; // a non-existing file can be created in hdf5 format
	}
    }
}
