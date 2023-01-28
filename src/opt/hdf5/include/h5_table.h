
#ifndef __cxz_h5_table_h__
#define __cxz_h5_table_h__

#include "h5_types.h"
#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	class Table : public ContentBase
	{
	public:
	    typedef std::pair<SizeT,String> FieldID;
	    
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
	    Table& initTable(SizeT n, const void* data, SizeT dsize, SizeT chunk_size);
	    Table& appendRecord(SizeT n, const void* data, SizeT dsize);
	    Table& readRecord(SizeT pos, SizeT n, char* data);
	    const RangePtr& fields() const;
	    const RangePtr& records() const;

	protected:
	    RangePtr mRecords;
	    RangePtr mFields; // -> FIndex (position -> offset)
	    MArray<SizeT> mSizes;
	    MArray<SizeT> mOffsets;
	    MArray<hid_t> mTypes;
	    hid_t mType = 0;
	};

	template <typename... Ts>
	class STable : public Table
	{
	public:
	    DEFAULT_MEMBERS(STable);
	    STable(const String& name, const ContentBase* _parent, const Vector<String>& fnames);

	    STable& appendRecord(const Tuple<Ts...>& t);
	    STable& appendRecord(const MArray<Tuple<Ts...>>& t);
	    
	    template <class F>
	    decltype(auto) iterRecords(F&& f) const;
	};
    }
}

#endif
