
#ifndef __cxz_h5_file_h__
#define __cxz_h5_file_h__

#include "h5_content_base.h"
#include <hdf5.h>

namespace CNORXZ
{
    namespace hdf5
    {
	// maybe introduce abstraction layer between as base for File and Group
	class File : public ContentBase
	{
	public:
	    typedef URange<String> RangeT;
	    
	    DEFAULT_MEMBERS(File);
	    File(const String& fname, bool _ro = true);
	    ~File();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual File& load() override final;
	    virtual File& write() override final;
	    virtual File& close() override final;
	    virtual MArray<Sptr<ContentBase>>* get() override final;
	    virtual const MArray<Sptr<ContentBase>>* get() const override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;

	    Int exists() const;
	    File& set(const RangePtr& range);
	    File& append(const String& cname);
	    
	private:
	    bool mRo = true;
	    MArray<ContentPtr> mCont;
	};
    }
}

#endif
