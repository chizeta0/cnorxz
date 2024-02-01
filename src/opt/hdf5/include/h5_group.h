// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_group.h
   @brief Group declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_group_h__
#define __cxz_h5_group_h__

#include "h5_types.h"
#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 groups.
	 */
	class Group : public ContentBase
	{
	public:
	    DEFAULT_MEMBERS(Group); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param _name Group name.
		@param _parent Parent content object.
	     */
	    Group(const String& gname, const ContentBase* _parent);

	    /** Destructor. Release all involved hdf5 ids. */
	    ~Group();

	    virtual ContentType type() const override;
	    virtual bool ro() const override;
	    virtual Group& open() override; // load group if existing, else create new group
	    virtual Group& close() override;
	    virtual String path() const override;
	    virtual String filename() const override;

	    /** Check if group exists in the parent object.
		@return 1 if group exists, 0 if not, -1 if an error occured.
	     */
	    virtual Int exists() const; // TODO: virtual function of base class

	    /** Get object contained by this group.
		@param name Object name.
		@return Pointer to the object.
	     */
	    const ContentPtr& get(const String& name) const;

	    /** Get object contained by this group as group.
		Checks if the object is a group.
		@param name Group name.
		@return Pointer to the group.
	     */
	    Sptr<Group> getGroup(const String& name) const;

	    /** Get object contained by this group as table.
		Checks if the object is a table.
		@param name Table name.
		@return Pointer to the table.
	     */
	    Sptr<Table> getTable(const String& name) const;

	    /** Get object contained by this group as dataset.
		Checks if the object is a table.
		@param name Dataset name.
		@return Pointer to the dataset.
	     */
	    Sptr<Dataset> getDataset(const String& name) const;

	    /** Get object contained by this group as table for given value type.
		Checks if the object is a table.
		@tparam Ts Table entry types.
		@param name Table name.
		@param proto Empty prototype (template argument resolution).
		@return Pointer to the table.
	     */
	    template <typename... Ts>
	    Sptr<STable<Ts...>> getTable(const String& name, Tuple<Ts...> proto);

	    /** Get object contained by this group as dataset for given value type.
		Checks if the object is a dataset.
		@tparam T Dataset value type.
		@param name Dataset name.
		@param proto Empty prototype (template argument resolution).
		@return Pointer to the dataset.
	     */
	    template <typename T>
	    Sptr<SDataset<T>> getDataset(const String& name, T proto);

	    /** Get objects contained by this group.
		@returns MArray of object pointers.
	     */
	    const MArray<ContentPtr>& get() const;

	    /** Add a new group to this group.
		@param name Name of the created group.
	     */
	    Group& addGroup(const String& name);

	    /** Add a new table to this group.
		@tparam Ts Table element types.
		@param name Name of the created table.
		@param data Table data.
		@param fnames Table field names.
	     */
	    template <typename... Ts>
	    Group& addTable(const String& name, const ArrayBase<Tuple<Ts...>>& data,
			    const Arr<String,sizeof...(Ts)>& fnames);

	    /** Add a new dataset to this group.
		@tparam T Value type.
		@param name Name of the created dataset.
		@param data Dataset data.
	     */
	    template <typename T>
	    Group& addDataset(const String& name, const ArrayBase<T>& data);

	    /** Iterate over all group elements (const).
		@param F function object to be executed on each group element.
	     */
	    template <class F>
	    decltype(auto) iter(F&& f) const;

	    /** Iterate recursively over all group elements (const).
		@param F function object to be executed on each group element.
	     */
	    template <class F>
	    decltype(auto) iterRecursive(F&& f) const;

	    /** Iterate over all group elements.
		@param F function object to be executed on each group element.
	     */
	    template <class F>
	    decltype(auto) iter(F&& f);

	    /** Iterate recursively over all group elements.
		@param F function object to be executed on each group element.
	     */
	    template <class F>
	    decltype(auto) iterRecursive(F&& f);

	protected:

	    MArray<ContentPtr> mCont; /**< Group elements. */

	    template <typename C, class F>
	    static void recursion(const C& c, F&& f)
	    {
		if(c->type() == ContentType::GROUP){
		    auto cx = std::dynamic_pointer_cast<Group>(c);
		    cx->open();
		    if(cx->get().range() != nullptr){
			cx->iterRecursive(std::forward<F>(f))();
		    }
		}
	    }
	    
	    void mkCont();
	    AIndex<ContentPtr> getIndexTo(const String& name) const;
	    BIndex<ContentPtr> getIndexTo(const String& name);
	};
    }
}

#endif
