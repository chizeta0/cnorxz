
#include "h5_group.h"

namespace CNORXZ
{
    namespace hdf5
    {
	Group::Group(const String& gname, const ContentBase* _parent) :
	    ContentBase(gname, _parent)
	{
	    if(this->exists()){
		mId = H5Gopen( mParent->id(), mName.c_str(), H5P_DEFAULT );
	    }
	    else {
		mId = H5Gcreate( mParent->id(), mName.c_str(),
				 H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
	    }
	}
	
	Group::~Group()
	{
	    if(mId != 0){
		H5Gclose(mId);
	    }
	}

	ContentType Group::type() const
	{
	    return ContentType::GROUP;
	}
	
	bool Group::ro() const
	{
	    if(mParent){
		return mParent->ro();
	    }
	    return false;
	}
	
	Group& Group::load()
	{
	    // load content!!!
	    return *this;
	}
	
	Group& Group::write()
	{
	    // ...!!!
	    return *this;
	}

	Group& Group::close()
	{
	    return *this;
	}
	
	MArray<Sptr<ContentBase>>* Group::get()
	{
	    return &mCont;
	}
	
	const MArray<Sptr<ContentBase>>* Group::get() const
	{
	    return &mCont;
	}
	
	String Group::path() const
	{
	    if(mParent){
		return mParent->path() + "/" + this->name();
	    }
	    return this->name();
	}
	
	String Group::filename() const
	{
	    if(mParent){
		return mParent->filename();
	    }
	    return String();
	}
	
	bool Group::exists() const
	{
	    return H5Lexists(mParent->id(), mName.c_str(), H5P_DEFAULT) != 0;
	}

	Group& Group::append(const ContentPtr& c)
	{

	    return *this;
	}

    }
}
