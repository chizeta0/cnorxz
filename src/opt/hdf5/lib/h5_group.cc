
#include "h5_group.h"

namespace CNORXZ
{
    namespace hdf5
    {
	Group::Group(const String& gname, const ContentBase* _parent) :
	    ContentBase(gname, _parent)
	{}
	
	Group::~Group()
	{
	    this->close();
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
	
	Group& Group::open()
	{
	    if(this->exists()){
		mId = H5Gopen( mParent->id(), mName.c_str(), H5P_DEFAULT );
	    }
	    else {
		mId = H5Gcreate( mParent->id(), mName.c_str(),
				 H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
	    }
	    this->mkCont();
	    return *this;
	}
	
	Group& Group::write()
	{
	    // ...!!!
	    return *this;
	}

	Group& Group::close()
	{
	    if(mId != 0){
		H5Gclose(mId);
	    }
	    return *this;
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
	
	Int Group::exists() const
	{
	    return H5Lexists(mParent->id(), mName.c_str(), H5P_DEFAULT) != 0 ? 1 : 0;
	}

	const MArray<ContentPtr>& Group::get() const
	{
	    return mCont;
	}

	struct InitContData
	{
	    const ContentBase* parent;
	    BIndex<ContentPtr> index;
	};

	static herr_t addName(hid_t id, const char* name, const H5L_info_t* info, void* x)
	{
	    Vector<String>* names = reinterpret_cast<Vector<String>*>(x);
	    names->push_back(String(name));
	    return 0;
	}
	
	static herr_t initCont(hid_t id, const char* name, const H5L_info_t* info, void* x)
	{
	    const String sname(name);
	    InitContData* icd = reinterpret_cast<InitContData*>(x);
	    BIndex<ContentPtr>& index = icd->index;
	    UIndex<String>& ui = std::dynamic_pointer_cast<XIndex<UIndex<String>,String>>
		(index.pack()[0])->get();
	    ui.at(sname);
	    index();
	    H5O_info_t oinfo;
	    H5Oget_info(id, &oinfo, H5O_INFO_BASIC);
	    switch (oinfo.type) {
	    case H5O_TYPE_GROUP: {
		*index = std::make_shared<Group>(sname, icd->parent);
		break;
	    }/*
	    case H5O_TYPE_DATASET: {
		*index = std::make_shared<DSet>(sname, );
		break;
		}*/
	    default:
		return 1;
	    }
	    return 0;
	}

	void Group::mkCont()
	{
	    Vector<String> names;
	    H5Literate( mId, H5_INDEX_NAME, H5_ITER_NATIVE, NULL,
			addName, reinterpret_cast<void*>(&names) );
	    mCont.init( URangeFactory<String>( names ).create() );

	    InitContData icd;
	    icd.index = mCont.begin();
	    icd.parent = this;
	    H5Literate( mId, H5_INDEX_NAME, H5_ITER_NATIVE, NULL,
			initCont, reinterpret_cast<void*>(&icd) );
	}

    }
}
