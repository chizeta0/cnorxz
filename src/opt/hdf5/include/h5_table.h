// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_table.h
   @brief Table declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_table_h__
#define __cxz_h5_table_h__

#include "h5_types.h"
#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 tables.
	 */
	class Table : public ContentBase
	{
	public:
	    typedef std::pair<SizeT,String> FieldID;
	    
	    DEFAULT_MEMBERS(Table); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param _name Table name.
		@param _parent Parent content object.
	     */
	    Table(const String& name, const ContentBase* _parent);

	    /** Destructor. Release all involved hdf5 ids. */
	    ~Table();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual Table& open() override final;
	    virtual Table& close() override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;

	    /** Ininitialize table field names.
		@param fnames Table field names.
	     */
	    Table& initFieldNames(const Vector<String>& fnames);

	    /** Initialize the table.
		@param n Number of records (rows).
		@param data Table data.
		@param dsize Record type size.
		@param chunk_size Chunk size.
	     */
	    Table& initTable(SizeT n, const void* data, SizeT dsize, SizeT chunk_size);

	    /** Append records to the table.
		@param n Number of records to append.
		@param data Records data.
	     */
	    Table& appendRecords(SizeT n, const void* data);

	    /** Read records.
		@param pos Number of first record to read.
		@param n Number of records to read.
		@param data Target pointer.
	     */
	    Table& readRecords(SizeT pos, SizeT n, char* data);

	    /** Read record.
		@param pos Number of the record to be read.
		@return DType array with containing the record data.
	     */
	    MArray<DType> readRecord(SizeT pos) const;

	    /** Read table.
		@return DType array containing the table data.
	     */
	    MArray<DType> read() const;

	    /** Iterate over table records.
		@param F function object to be executed on each table record.
	     */
	    template <class F>
	    decltype(auto) iterRecords(F&& f) const;

	    /** Get fields range.
		@return Pointer to the range.
	     */
	    const RangePtr& fields() const;

	    /** Get records range.
		@return Pointer to the range.
	     */
	    const RangePtr& records() const;

	protected:
	    RangePtr mRecords;
	    RangePtr mFields; // -> FIndex (position -> offset)
	    MArray<SizeT> mSizes;
	    MArray<SizeT> mOffsets;
	    MArray<hid_t> mTypes;
	    hid_t mType = 0;
	    SizeT mTypesize = 0;
	    MArray<std::function<DType(const char*)>> mInterpret;

	    void mkTypes();
	};

	// caution: the memory ordering is the only thing that counts;
	// std::tuple has REVERSE ordering!
	template <typename... Ts>
	class STable : public Table
	{
	public:
	    DEFAULT_MEMBERS(STable);
	    STable(const String& name, const ContentBase* _parent);
	    STable(const String& name, const ContentBase* _parent,
		   const Arr<String,sizeof...(Ts)>& fnames);

	    STable& initFields(const Arr<String,sizeof...(Ts)>& fnames);

	    STable& appendRecord(const Tuple<Ts...>& t);
	    STable& appendRecord(const MArray<Tuple<Ts...>>& t);

	    MArray<Tuple<Ts...>> read() const;
	    
	    template <class F>
	    decltype(auto) iterRecords(F&& f) const;
	};

    }
}

#endif
