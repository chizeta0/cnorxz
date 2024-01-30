
#ifndef __cxz_h5_dataset_h__
#define __cxz_h5_dataset_h__

#include "h5_types.h"
#include "h5_content_base.h"

namespace CNORXZ
{
    namespace hdf5
    {
	class Dataset : public ContentBase
	{
	public:
	    DEFAULT_MEMBERS(Dataset);
	    Dataset(const String& name, const ContentBase* _parent);
	    ~Dataset();

	    virtual ContentType type() const override final;
	    virtual bool ro() const override final;
	    virtual Dataset& open() override final;
	    virtual Dataset& close() override final;
	    virtual String path() const override final;
	    virtual String filename() const override final;

	    bool exists() const;

	    Dataset& init(const RangePtr& dataRange, hid_t type);
	    
	    const RangePtr& dataRange() const;
	    
	protected:
	    RangePtr mDataRange;
	    hid_t mType;
	    hid_t mFilespace;
	    
	};

	template <typename T>
	class SDataset : public Dataset
	{
	public:
	    DEFAULT_MEMBERS(SDataset);
	    SDataset(const String& name, const ContentBase* _parent);

	    MArray<T> read() const;
	};
    }
}

#endif
