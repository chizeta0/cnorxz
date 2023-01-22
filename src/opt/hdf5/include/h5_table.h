
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

	    Table& openDSet();
	    Table& initFieldNames(const Vector<String>& fnames);
	    
	    template <class F>
	    decltype(auto) iterRecords(F&& f) const;


	private:
	    RangePtr mRecords;
	    RangePtr mFields; // -> FIndex (position -> offset)
	    MArray<SizeT> mSizes;
	    MArray<SizeT> mOffsets;
	    MArray<hid_t> mTypes;
	    hid_t mType;
	    bool mCheckedFile = false;
	};

	template <typename... Ts>
	class STabel : public Table
	{
	public:
	    DEFAULT_MEMBERS(STabel);
	    STabel(const String& name, const ContentBase* _parent);

	    Table& appendRecord(const Tuple<Ts...>& t);
	    Table& appendRecord(const MArray<Tuple<Ts...>>& t);
	    
	};
    }
}

#endif
