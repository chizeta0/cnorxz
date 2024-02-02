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
	    RangePtr mRecords; /**< Records range. */
	    RangePtr mFields; /**< Fields range. */ // -> FIndex (position -> offset)
	    MArray<SizeT> mSizes; /**< Field element type sizes. */
	    MArray<SizeT> mOffsets; /**< Field element offsets. */
	    MArray<hid_t> mTypes; /**< Field element type ids. */
	    hid_t mType = 0; /**< Record type id. */
	    SizeT mTypesize = 0; /**< Record type size. */
	    MArray<std::function<DType(const char*)>> mInterpret; /**< Field element type interpreting functions. */

	    void mkTypes(); /**< Type setup function. */
	};

	/** ****
	    Class to handle hdf5 tables, the record type is known at compile time.
	    The records are accessed by a std tuple.
	    Caution: The ordering of the record entries is fixed by their memory location.
	    The std tuple has a reverse ordering w.r.t. the memory location, i.e. a record
	    with element types T1-T2-T3 (memory ordering) and field names "T1", "T2", "T3",
	    requires template argumens <T3,T2,T1>.
	    @tparam Ts Record element types.
	 */
	template <typename... Ts>
	class STable : public Table
	{
	public:
	    DEFAULT_MEMBERS(STable); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param _name Table name.
		@param _parent Parent content object.
	     */
	    STable(const String& name, const ContentBase* _parent);

	    /** Construct the class.
		@param _name Table name.
		@param _parent Parent content object.
		@param fnames Field names.
	     */
	    STable(const String& name, const ContentBase* _parent,
		   const Arr<String,sizeof...(Ts)>& fnames);

	    /** Ininitialize and setup table fields.
		@param fnames Table field names.
	     */
	    STable& initFields(const Arr<String,sizeof...(Ts)>& fnames);

	    /** Append record to the table.
		@param t Tuple containing the record entries.
	     */
	    STable& appendRecord(const Tuple<Ts...>& t);

	    /** Append records to the table.
		@param t Array of tuples containing the records.
	     */
	    STable& appendRecord(const MArray<Tuple<Ts...>>& t);

	    /** Read the table.
		@return Array of tuples containing the records.
	     */
	    MArray<Tuple<Ts...>> read() const;

	    /** Iterate over all table records.
		@param f Function object to be executed on each record.
	     */
	    template <class F>
	    decltype(auto) iterRecords(F&& f) const;
	};

    }
}

#endif
