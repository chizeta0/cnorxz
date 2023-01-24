
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

	    Table& initFieldNames(const Vector<String>& fnames);
	    Table& appendRecord(SizeT n, const char* data);
	    Table& readRecord(SizeT pos, SizeT n, char* data);

	protected:
	    RangePtr mRecords;
	    RangePtr mFields; // -> FIndex (position -> offset)
	    MArray<SizeT> mSizes;
	    MArray<SizeT> mOffsets;
	    MArray<hid_t> mTypes;
	    hid_t mType = 0;
	    bool mCheckedFile = false;
	};

	template <typename... Ts>
	class STable : public Table
	{
	public:
	    DEFAULT_MEMBERS(STable);
	    STable(const String& name, const ContentBase* _parent, const RangePtr& fields);

	    STable& appendRecord(const Tuple<Ts...>& t);
	    STable& appendRecord(const MArray<Tuple<Ts...>>& t);
	    
	    template <class F>
	    decltype(auto) iterRecords(F&& f) const;
	};
    }
}

#endif
