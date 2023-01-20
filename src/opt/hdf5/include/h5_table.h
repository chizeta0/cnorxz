
#ifndef __cxz_h5_table_h__
#define __cxz_h5_table_h__

#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	class Table : public ContentBase
	{
	public:
	    DEFAULT_MEMBERS(Table);
	    Table(const String& name, const ContentBase* _parent);
	    ~Table();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual Table& open() override final;
	    virtual Table& close() override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;
	    
	    template <class F>
	    decltype(auto) readRecords(F&& f) const;
	    
	private:
	    RangePtr mRecords;
	    RangePtr mFields; // -> FIndex (position -> offset)
	    MArray mSizes;
	    MArray mOffsets;
	    MArray mTypes;
	};
    }
}

#endif
