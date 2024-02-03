
#include "h5_group.h"
#include "h5_table.h"
#include "h5_dataset.h"

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
	    if(not isOpen()){
		if(this->exists()){
		    mId = H5Gopen( mParent->id(), mName.c_str(), H5P_DEFAULT );
		}
		else {
		    mId = H5Gcreate( mParent->id(), mName.c_str(),
				     H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
		}
		this->mkCont();
	    }
	    return *this;
	}
	
	Group& Group::close()
	{
	    if(mId != 0){
		H5Gclose(mId);
	    }
	    mId = 0;
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
	
	bool Group::exists() const
	{
	    return H5Lexists(mParent->id(), mName.c_str(), H5P_DEFAULT) > 0;
	}

	const ContentPtr& Group::get(const String& name) const
	{
	    const String delim = "/";
	    const SizeT delimpos = name.find(delim);
	    const String thisname = name.substr(0, delimpos);
	    if(delimpos != String::npos and delimpos+1 < name.size()){
		const String next = name.substr(delimpos+1);
		auto g = getGroup(thisname);
		g->open();
		return g->get(next);
	    }
	    auto i = this->getIndexTo(thisname);
	    return *i;
	}

	Sptr<Group> Group::getGroup(const String& name) const
	{
	    auto group = this->get(name);
	    CXZ_ASSERT(group->type() == ContentType::GROUP,
		       "element '" << name << "' is not of type GROUP");
	    return std::dynamic_pointer_cast<Group>( group );
	}

	Sptr<Table> Group::getTable(const String& name) const
	{
	    auto table = this->get(name);
	    CXZ_ASSERT(table->type() == ContentType::TABLE,
		       "element '" << name << "' is not of type TABLE");
	    return std::dynamic_pointer_cast<Table>( table );
	}

	Sptr<Dataset> Group::getDataset(const String& name) const
	{
	    auto dset = this->get(name);
	    CXZ_ASSERT(dset->type() == ContentType::DSET,
		       "element '" << name << "' is not of type DSET");
	    return std::dynamic_pointer_cast<Dataset>( dset );
	}

	const MArray<ContentPtr>& Group::get() const
	{
	    CXZ_ASSERT(this->isOpen(), "tried to get content of closed group");
	    return mCont;
	}

	Group& Group::addGroup(const String& name)
	{
	    CXZ_ASSERT(this->isOpen(), "tried to extend closed group");
	    Vector<String> nvec({name});
	    Vector<DType> dvec({DType(name)});
	    auto extr = URangeFactory<String>( nvec ).create();
	    mCont.extend(extr);
	    auto ii = mCont.begin();
	    ii.at(dvec); // 'at' returns YIndex&, so cannot use it inline...
	    *ii = std::make_shared<Group>(name, this);
	    (*ii)->open(); // create new group
	    return *this;
	}

	/** @cond 0 */
	struct InitContData
	{
	    const ContentBase* parent;
	    BIndex<ContentPtr> index;
	};
	/** @endcond */

	static herr_t addName(hid_t id, const char* name, const H5L_info_t* info, void* x)
	{
	    Vector<String>* names = reinterpret_cast<Vector<String>*>(x);
	    names->push_back(String(name));
	    return 0;
	}

	static bool isTable(hid_t loc_id, const char* name)
	{
	    const hid_t id = H5Dopen(loc_id, name, H5P_DEFAULT);
	    if(not H5Aexists(id, "CLASS")){
		return false;
		H5Dclose(id);
	    }
	    hid_t attrid = H5Aopen(id, "CLASS", H5P_DEFAULT);
	    const hid_t atype = H5Aget_type(attrid);
	    const SizeT asize = H5Tget_size(atype);
	    Vector<char> buff(asize);
	    const herr_t ret = H5Aread(attrid, atype, buff.data());
	    H5Tclose(atype);
	    H5Aclose(attrid);
	    H5Dclose(id);
	    if(ret != 0){
		return false;
	    }
	    else {
		return String(buff.data()) == "TABLE";
	    }
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
#if H5_VERS_MINOR > 10
	    H5Oget_info_by_name(id, name, &oinfo, H5O_INFO_BASIC, H5P_DEFAULT);
#else
	    H5Oget_info_by_name(id, name, &oinfo, H5P_DEFAULT);
#endif
	    switch (oinfo.type) {
	    case H5O_TYPE_GROUP: {
		*index = std::make_shared<Group>(sname, icd->parent);
		break;
	    }
	    case H5O_TYPE_DATASET: {
		if(isTable(id, name)){
		    *index = std::make_shared<Table>(sname, icd->parent);
		}
		else {
		    *index = std::make_shared<Dataset>(sname, icd->parent);
		}
		break;
	    }
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

	AIndex<ContentPtr> Group::getIndexTo(const String& name) const
	{
	    CXZ_ASSERT(this->isOpen(), "tried to get content of closed group");
	    auto dvec = [](const String& n) { return Vector<DType>({DType(n)}); };
	    auto i = mCont.begin();
	    i.at(dvec(name));
	    return i;
	}

	BIndex<ContentPtr> Group::getIndexTo(const String& name)
	{
	    CXZ_ASSERT(this->isOpen(), "tried to get content of closed group");
	    auto dvec = [](const String& n) { return Vector<DType>({DType(n)}); };
	    auto i = mCont.begin();
	    i.at(dvec(name));
	    return i;
	}
	
    }
}
