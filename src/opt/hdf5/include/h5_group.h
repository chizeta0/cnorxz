
#ifndef __cxz_h5_group_h__
#define __cxz_h5_group_h__

#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	class Group : public ContentBase
	{
	public:
	    DEFAULT_MEMBERS(Group);
	    Group(const String& gname, const ContentBase* _parent);
	    ~Group();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual Group& load() override final;
	    virtual Group& write() override final;
	    virtual Group& close() override final;
	    virtual MArray<Sptr<ContentBase>>* get() override final;
	    virtual const MArray<Sptr<ContentBase>>* get() const override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;

	    bool exists() const;
	    Group& append(const ContentPtr& c);
	    
	private:
	    MArray<ContentPtr> mCont;
	};
	
    }
}

#endif
