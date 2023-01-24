
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

	    virtual ContentType type() const override;
	    virtual bool ro() const override;
	    virtual Group& open() override; // load group if existing, else create new group
	    virtual Group& close() override;
	    virtual String path() const override;
	    virtual String filename() const override;

	    virtual Int exists() const;

	    const ContentPtr& get(const String& name) const;
	    Sptr<Group> getGroup(const String& name) const;
	    const MArray<ContentPtr>& get() const;
	    Group& addGroup(const String& name);

	    template <typename T>
	    Group& addData(const String& name, const ArrayBase<T>& data);

	    template <typename... Ts>
	    Group& addTable(const String& name, const ArrayBase<Tuple<Ts...>>& data,
			    const RangePtr& fields);
	    
	protected:
	    MArray<ContentPtr> mCont;

	    void mkCont();
	};
    }
}

#endif
