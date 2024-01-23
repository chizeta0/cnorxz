
#ifndef __cxz_h5_content_base_h__
#define __cxz_h5_content_base_h__

#include "cnorxz.h"
#include <hdf5.h>
#include <hdf5_hl.h>

namespace CNORXZ
{
    namespace hdf5
    {
	enum class ContentType {
	    ATTR = 1,
	    FILE = 2,
	    GROUP = 3,
	    DSET = 4,
	    TABLE = 5,
	    VALUE = 6,
	};
	    
	class ContentBase
	{
	public:
	    DEFAULT_MEMBERS(ContentBase);
	    ContentBase(const String& _name, const ContentBase* _parent = nullptr);
	    virtual ~ContentBase() = default;

	    virtual ContentType type() const = 0;
	    virtual bool ro() const = 0;
	    virtual ContentBase& open() = 0;
	    virtual ContentBase& close() = 0;
	    virtual String path() const = 0;
	    virtual String filename() const = 0;

	    const String& name() const;
	    const ContentBase* parent() const;
	    RangePtr range() const;
	    hid_t id() const;
	    inline bool isOpen() const { return mId != 0; }

	    template <typename T>
	    ContentBase& addAttribute(const String& name, const T& value);
	    DType getAttribute(const String& name) const;
	    bool isAttribute(const String& name) const;
	    String getAttributesAsString() const;
	    String getRecursiveAttributesAsString() const; // + all parent's attributes
	    
	protected:
	    String mName;
	    const ContentBase* mParent = nullptr;
	    RangePtr mRange;
	    hid_t mId = 0;
	};

	typedef Sptr<ContentBase> ContentPtr;

    }
}

#endif
