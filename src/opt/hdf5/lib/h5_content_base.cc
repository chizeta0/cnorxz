
#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	ContentBase::ContentBase(const String& _name, const ContentBase* _parent) :
	    mName(_name), mParent(_parent)
	{}

	const String& ContentBase::name() const
	{
	    return mName;
	}

	const ContentBase* ContentBase::parent() const
	{
	    return mParent;
	}

	RangePtr ContentBase::range() const
	{
	    return mRange;
	}

	hid_t ContentBase::id() const
	{
	    return mId;
	}
    }
}
