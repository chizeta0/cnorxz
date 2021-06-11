
#ifndef __index_wrapper_h__
#define __index_wrapper_h__

#include "ranges/rbase_def.h"
#include "xfor/xfor.h"
#include "ranges/rheader.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    class IndexWrapperBase
    {
    public:

        IndexWrapperBase() = default;
        IndexWrapperBase(const IndexWrapperBase& in) = default;
        IndexWrapperBase(IndexWrapperBase&& in) = default;
        IndexWrapperBase& operator=(const IndexWrapperBase& in) = default;
        IndexWrapperBase& operator=(IndexWrapperBase&& in) = default;

        virtual IndexType type() const = 0;
	
	virtual IndexWrapperBase& operator=(size_t pos) = 0;
	virtual IndexWrapperBase& operator++() = 0;
	virtual IndexWrapperBase& operator--() = 0;

	virtual int pp(std::intptr_t idxPtrNum) = 0;
	virtual int mm(std::intptr_t idxPtrNum) = 0;

	virtual std::string stringMeta() const = 0;
	
        virtual size_t pos() const = 0;
        virtual size_t max() const = 0;
	virtual size_t dim() const = 0;
	virtual bool last() const = 0;
	virtual bool first() const = 0;

	virtual std::shared_ptr<RangeBase> range() const = 0;
        
        virtual size_t getStepSize(size_t n) const = 0;
        virtual size_t getStepSizeComp(std::intptr_t j) const = 0;
        
        virtual std::intptr_t get() const = 0;
	virtual std::intptr_t ptrNum() const = 0;

        virtual std::shared_ptr<IndexWrapperBase> duplicate() const = 0;

	inline IndexWrapperBase& at(const std::string smeta)
	{
	    // ignore spaces, " and ' (string identifiers)
	    auto rem = [](unsigned char x)
	    {
		bool out = false;
		if(x == ' ' or x == '"' or x == '\'') out = true;
		return out;
	    };
	    std::string redmeta = smeta;
	    redmeta.erase(std::remove_if(redmeta.begin(), redmeta.end(), rem), redmeta.end());
	    for((*this) = 0; this->pos() != this->max(); ++(*this)){
		std::string red = this->stringMeta();
		red.erase(std::remove_if(red.begin(), red.end(), rem), red.end());
		if(red == redmeta){
		    break;
		}
	    }
	    return *this;
	}

        virtual DynamicExpression ifor(size_t step, DynamicExpression ex) const = 0;
        virtual DynamicExpression iforh(size_t step, DynamicExpression ex) const = 0;
        /*
        std::shared_ptr<IndexWrapperBase> duplicateI() const
        { return std::dynamic_pointer_cast<IndexWrapperBase>( this->duplicate() ); }
        */

	virtual std::shared_ptr<ClassicIndex> reduced() const = 0;
    };

    typedef IndexWrapperBase IndexW;
    
    template <class Index>
    class IndexWrapper : public IndexWrapperBase
    {
    public:
        typedef IndexWrapperBase IWB;
	typedef typename Index::MetaType MetaType;

	static constexpr IndexType sType() { return IndexType::SINGLE; }
	
    protected:
        IndexWrapper() = default;

    private:
        std::shared_ptr<Index> mI;
	std::shared_ptr<ClassicIndex> mCI; // reduced;
    public:

        IndexWrapper(const IndexWrapper& in) = default;
        IndexWrapper(IndexWrapper&& in) = default;
        IndexWrapper& operator=(const IndexWrapper& in) = default;
        IndexWrapper& operator=(IndexWrapper&& in) = default;

        IndexWrapper(const std::shared_ptr<Index>& i);
        
        virtual IndexType type() const final;
	
	virtual IndexWrapper& operator=(size_t pos) override final;
	virtual IndexWrapper& operator++() override final;
	virtual IndexWrapper& operator--() override final;

        virtual size_t pos() const override final;
        virtual size_t max() const override final;
        
	virtual int pp(std::intptr_t idxPtrNum) override final;
	virtual int mm(std::intptr_t idxPtrNum) override final;

	virtual std::string stringMeta() const override final;
	//virtual DynamicMetaT meta() const final { return DynamicMetaT(mI->meta()); }
	//virtual const DynamicMetaT* metaPtr() const final { return nullptr; }
	IndexWrapper& at(const typename Index::MetaType& metaPos);
	size_t posAt(const typename Index::MetaType& metaPos);

	//virtual bool isMeta(const U& metaPos) const final { return mI->isMeta(); }
	
	virtual size_t dim() const override final;
	virtual bool last() const override final;
	virtual bool first() const override final;

	virtual std::shared_ptr<RangeBase> range() const override final;
        
        virtual size_t getStepSize(size_t n) const override final;
        virtual size_t getStepSizeComp(std::intptr_t j) const override final;
        
        virtual std::intptr_t get() const override final;
	virtual std::intptr_t ptrNum() const override final;

        virtual DynamicExpression ifor(size_t step, DynamicExpression ex) const override final;
        virtual DynamicExpression iforh(size_t step, DynamicExpression ex) const override final;

        virtual std::shared_ptr<IndexWrapperBase> duplicate() const override final;

	std::shared_ptr<Index> getIndex() const;
	virtual std::shared_ptr<ClassicIndex> reduced() const override final;
	
    };
    /*
    template <class Index>
    std::shared_ptr<IndexWrapperBase> mkIndexWrapper(const Index& i);
    */
}

#endif
