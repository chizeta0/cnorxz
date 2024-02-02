// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_file.h
   @brief Group declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_file_h__
#define __cxz_h5_file_h__

#include "h5_types.h"
#include "h5_content_base.h"
#include "h5_group.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 file objects.
	    Objects of this type usually serve as root object
	    so they don't have any parent.
	 */
	class File : public Group
	{
	public:
	    DEFAULT_MEMBERS(File); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param fname Path to the hdf5 file to be handled.
		@param _ro Open in read-only mode if true, otherwise have write access.
	     */
	    File(const String& fname, bool _ro = true);

	    /** Destructor. Release all involved hdf5 ids. */
	    ~File();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual File& open() override final;
	    virtual File& close() override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;
	    virtual bool exists() const override final;

	    /** Check if handled file is in hdf5 format.
		@return True if file is in hdf5 format, else false.
	     */
	    bool ishdf5() const;

	private:
	    bool mRo = true;
	};
    }
}

#endif
