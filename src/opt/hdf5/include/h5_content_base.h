// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_content_base.h
   @brief Abstract content base class declaration

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_content_base_h__
#define __cxz_h5_content_base_h__

#include "cnorxz.h"
#include <hdf5.h>
#include <hdf5_hl.h>

namespace CNORXZ
{
    namespace hdf5
    {
	// TODO: IO save error handling !!!

	/** *****
	    Enum indicating the content type.
	    Used by ContainerBase derivates to indicate the derived type.
	 */
	enum class ContentType {
	    NONE = 0,
	    FILE = 1,
	    GROUP = 2,
	    DSET = 3,
	    TABLE = 4,
	};

	/** ****
	    Abstract base class for handling hdf5 objects.
	 */
	class ContentBase
	{
	public:
	    DEFAULT_MEMBERS(ContentBase); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param _name Content name.
		@param _parent Parent content object. Leave null for the root object.
	     */
	    ContentBase(const String& _name, const ContentBase* _parent = nullptr);

	    /** Virtual default destructor. */
	    virtual ~ContentBase() = default;

	    /** Get the content type.
		@return Content type.
	     */
	    virtual ContentType type() const = 0;

	    /** Check if in read-only mode
		@return True if read-only else false.
	     */
	    virtual bool ro() const = 0;

	    /** Open object.
		@return Reference to this object.
	     */
	    virtual ContentBase& open() = 0;

	    /** Close object.
		@return Reference to this object.
	     */
	    virtual ContentBase& close() = 0;

	    /** Get object path.
		@return Absolute hdf5 file internal path of this object.
	     */
	    virtual String path() const = 0;

	    /** Get the file name.
		@return Name of the hdf5 file this object is stored in.
	     */
	    virtual String filename() const = 0;

	    /** Get object name.
		@return The name of this object.
	     */
	    const String& name() const;

	    /** Get parent object.
		@return Pointer to the parent of this object.
	     */
	    const ContentBase* parent() const;

	    /** Get object id.
		@return hdf5 id of the h5 object maintained by this object.
	     */
	    hid_t id() const;

	    /** Check if object is open, i.e. if there is a valid hdf5 id.
		@return True if object is open else false.
	     */
	    bool isOpen() const;

	    /** Add attribute to this object.
		@tparam T Attribute value type.
		@param name Attribute name.
		@param value Attribute value.
	     */
	    template <typename T>
	    ContentBase& addAttribute(const String& name, const T& value);

	    /** Get an attribute of this object.
		@param name Attribute name.
		@return The attribute value as DType.
	     */
	    DType getAttribute(const String& name) const;

	    /** Check if attribute of given name exists in this object.
		@param name Attribute name.
		@return True if attribute exists else false.
	     */
	    bool attributeExists(const String& name) const;

	    /** Get all attributes of this object.
		@return Std map of key-value pairs.  
	     */
	    std::map<String,DType> getAttributes() const;

	    /** Get all attributes of this object and all its parent objects, recursively.
		@return Std map of key-value pairs.  
	     */
	    std::map<String,DType> getRecursiveAttributes() const; // + all parent's attributes
	    
	protected:
	    String mName; /**< Name of this object. */
	    const ContentBase* mParent = nullptr; /**< Pointer to this object's parent. */
	    hid_t mId = 0; /**< hdf5 identifier of the hdf5 object handled by this object. */
	};

	/** Shortcut for a shared pointer to an abstract content object. */
	typedef Sptr<ContentBase> ContentPtr;

    }
}

#endif
