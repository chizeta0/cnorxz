// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_rfile.h
   @brief RFile declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_rfile_h__
#define __cxz_h5_rfile_h__

//#include "h5_file.h"
#include "h5_mpi_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 file objects with parallel I/O.
	    Objects of this type usually serve as root object
	    so they don't have any parent.
	 */
	class RFile : public File
	{
	public:
	    DEFAULT_MEMBERS(RFile);  /**< Default constructors and assignments. */

	    /** Construct the class.
		@param fname Path to the hdf5 file to be handled.
		@param _ro Open in read-only mode if true, otherwise have write access.
	     */
	    RFile(const String& fname, bool _ro = true);

	    /** Destructor. Release all involved hdf5 ids. */
	    ~RFile();
	    
	};
    }
}

#endif
