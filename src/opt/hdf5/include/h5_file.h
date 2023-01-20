
#ifndef __cxz_h5_file_h__
#define __cxz_h5_file_h__

#include "h5_content_base.h"
#include "h5_group.h"
//#include <hdf5.h>

namespace CNORXZ
{
    namespace hdf5
    {
	// maybe introduce abstraction layer between as base for File and Group
	class File : public Group
	{
	public:
	    typedef URange<String> RangeT;
	    
	    DEFAULT_MEMBERS(File);
	    File(const String& fname, bool _ro = true);
	    ~File();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual File& open() override final;
	    virtual File& close() override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;

	    virtual Int exists() const override final;
	    
	private:
	    bool mRo = true;
	};
    }
}

#endif
